# chapter 28 in python

# sudo apt-get install python3-pip
# python3 -m pip install pyserial
# sudo apt-get install python3-matplotlib


import numpy as np
import matplotlib.pyplot as plt 
from statistics import mean 
def read_plot_matrix():
    n_str = ser.read_until(b'\n');  # get the number of data points to receive
    n_int = int(n_str) # turn it into an int
    print('Data length = ' + str(n_int))
    ref = []
    data = []
    data_received = 0
    while data_received < n_int:
        dat_str = ser.read_until(b'\n');  # get the data as a string, ints seperated by spaces
        dat_int = list(map(int,dat_str.split())) # now the data is a list
        ref.append(dat_int[0])
        data.append(dat_int[1])
        data_received = data_received + 1
    meanzip = zip(ref,data)
    meanlist = []
    for i,j in meanzip:
        meanlist.append(abs(i-j))
    score = mean(meanlist)
    t=np.arange(0.0,20,0.2)    #Source: https://stackoverflow.com/questions/477486/how-to-use-a-decimal-range-step-value
    plt.plot(t,ref,'r*-',t,data,'b*-') 
    plt.title('Kp = 50 PWM %/mA, Ki = 2 PWM %/mA unitsScore in mA = ' + str(score))
    plt.ylabel('Current (mA)')
    plt.xlabel('Time (ms)')
    plt.legend(["Reference","Output"]) #Source: https://www.geeksforgeeks.org/matplotlib-pyplot-legend-in-python/
    plt.show()

def genRef(method):
    if method == 'step':
        print('Step!')
    elif method == 'cubic':
        print('Cubic!')
    else:
        print('Not a valid method')
        return [-1]

    reflist = [0,0,1,90,2,45,3,45] # defualt refs
    #refs_str = input('Enter times and angles, starting at t=0 (ex: 0 0 1 90 2 90 3 0): ')
    #refs_str=' 0 0 1 90 3 -45 4 0  5 0' #this is for step
    
    refs_str = ' 0 0 1 180 3 -90 4 0  5 0' #This is for cubic
    reflist = list(map(float,refs_str.split()))

    # check to see if the reflist is even and the odd values (times) are increasing
    if (len(reflist)%2!=0 or len(reflist) < 2 or reflist[0]!=0):
        print('Not a valid input: odd number of inputs or too short')
        return [-1]
    dataok = 1
    for i in range(2, len(reflist), 2):
        if reflist[i] <= reflist[i-2]:
            dataok = 0
    if dataok == 0:
        print('Not a valid input: time must increase')
        return [-1]

    MOTOR_SERVO_RATE = 200 # the position control ISR is 200Hz
    dt = 1/MOTOR_SERVO_RATE # time per control cycle

    numpos = int(len(reflist)/2)

    if method == 'step':
        sample_list = [] # time
        angle_list = [] # position
        for i in range(0, len(reflist), 2):
            sample_list.append(reflist[i]*MOTOR_SERVO_RATE)
        for i in range(1, len(reflist), 2):
            angle_list.append(reflist[i])
        ref = [0]*int(sample_list[-1])
        last_sample = 0
        for i in range(1, numpos):
            for samp in range(int(last_sample), int(sample_list[i])):
                ref[samp] = angle_list[i-1]
            last_sample = sample_list[i]
        ref[samp] = angle_list[-1]
    
    if method == 'cubic':
        ref = [] # store the output trajectory
        time_list = [] # time
        pos_list = [] # position
        for i in range(0, len(reflist), 2):
            time_list.append(reflist[i])
        for i in range(1, len(reflist), 2):
            pos_list.append(reflist[i])
        vel_list=[0]*numpos
        if numpos >= 3:
            for i in range(1, numpos-1):
                vel_list[i] = (pos_list[i+1]-pos_list[i-1])/(time_list[i+1]-time_list[i-1])
        print(vel_list)
        refCtr = 0
        for i in range(0,numpos-1):
            timestart = time_list[i]
            timeend = time_list[i+1]
            deltaT = timeend-timestart
            posstart = pos_list[i]
            posend = pos_list[i+1]
            velstart = vel_list[i]
            velend = vel_list[i+1]
            a0 = posstart # calculate coeffs of traj pos = a0+a1*t+a2*t^2+a3*t^3
            a1 = velstart
            a2 = (3*posend - 3*posstart - 2*velstart*deltaT - velend*deltaT)/(deltaT*deltaT)
            a3 = (2*posstart + (velstart+velend)*deltaT - 2*posend)/(deltaT*deltaT*deltaT)
            while (refCtr)*dt < time_list[i+1]:
                tseg = (refCtr)*dt -time_list[i]
                ref.append(a0 + a1*tseg + a2*tseg*tseg + a3*tseg*tseg*tseg)
                refCtr = refCtr + 1

    return ref



import serial
ser = serial.Serial('/dev/tty.usbserial-DB00MA06',230400,rtscts=1)
print('Opening port: ')
print(ser.name)

has_quit = False
# menu loop
while not has_quit:
    print('PIC32 MOTOR DRIVER INTERFACE')
    # display the menu options; this list will grow
    print('\ta: Read current sensor (ADC counts) \tb: Read current sensor (mA)')
    print('\tc: Read encoder (counts) \td: Read encoder (counts) \n\td: Encoder angle \te: Reset encoder') # '\t' is a tab
    print('\tf: Set PWM (-100 to 100) \tg: Set current gains')
    print('\th: Get current gains \ti: Set position gains')
    print('\tj: Get position gains \tk: Test current gains')
    print('\tl: Go to angle (deg) \tm: Load step trajectory')
    print('\tn: Load cubic tracjectory h\to: Execute trajectory')
    print('\tq: Quit client \tr: Get mode')
    print('\tu: Unpower the motor')
    # read the user's choice
    selection = input('\nENTER COMMAND: ')
    selection_endline = selection+'\n'
     
    # send the command to the PIC32
    ser.write(selection_endline.encode()); # .encode() turns the string into a char array
    
    # take the appropriate action
    # there is no switch() in python, using if elif instead
    if (selection =='a'):
        print('Reading current sensor (counts)')
        n_str = ser.read_until(b'\n');
        n_int = int(n_str)
        print('Got back: ' + str(n_int) + ' counts\n')
    elif (selection =='b'):
        print('Reading current sensor (mA)')
        n_str = ser.read_until(b'\n')
        print('Read!')
        n_int = int(n_str)
        print('Got back: ' +str(n_int) + ' mA\n')
    elif (selection == 'c'): 
        n_str = ser.read_until(b'\n');  # get the incremented number back
        n_int = int(n_str) # turn it into an int
        print('Got back: ' + str(n_int) + ' counts\n') # print it to the screen
   
    elif (selection == 'd'):
        n_str = ser.read_until(b'\n');  # get the incremented number back
        n_int = int(n_str) # turn it into an int
        print('Got back: ' + str(n_int) + ' degrees\n') # print it to the screen           
    
    elif (selection =='e'):
        print('Resetting encoder...')
    elif (selection =='f'):
        print('Setting the PWM duty cycle...')
        n_str = input('Enter a duty cycle in % ')
        n_int = int(n_str) 
        ser.write((str(n_int)+'\n').encode())
    elif (selection =='g'):
        print('Setting current gains')
        n_str_Kp = input('Enter Kp')
        n_str_Ki = input('Enter Ki')
        ser.write((n_str_Kp + ' ' + n_str_Ki + '\n').encode())
    elif (selection =='h'):
         print('The gains are ...\n')
         gain_str = ser.read_until(b'\n');
         gain_float = list(map(float,gain_str.split()))
         Kp_float = gain_float[0]
         Ki_float = gain_float[1]
         print('Kp and Ki are '+str(Kp_float)+' and '+str(Ki_float)+'\n')
    elif (selection == 'i'):
         print('Setting position gains')
         n_str_Kp_pos = input('Enter Kp_pos')
         n_str_Ki_pos = input('Enter Ki_pos')
         n_str_Kd_pos = input('Enter Kd_pos')  
         ser.write((n_str_Kp_pos+' '+n_str_Ki_pos+' '+n_str_Kd_pos+'\n').encode())
    elif (selection == 'j'):
        print('The positional gains are ...\n')
        gain_str = ser.read_until(b'\n');
        gain_float = list(map(float,gain_str.split()))
        Kp_pos_float = gain_float[0]
        Ki_pos_float = gain_float[1]
        Kd_pos_float = gain_float[2]
        print('Kp, Ki and Kd are '+str(Kp_pos_float)+' , '+str(Ki_pos_float)+' and '+str(Kd_pos_float)+'\n')                          
    elif (selection =='k'):
         read_plot_matrix()
    elif (selection =='l'):
        n_str_target = input('Enter a target position')
        ser.write((n_str_target+'\n').encode())
    elif (selection == 'm'):
        ref = genRef('step')
        print(len(ref))
        t = range(len(ref))
        plt.plot(t,ref,'r*-')
        plt.ylabel('Angle (Deg)')
        plt.xlabel('index (200 Hz)')
        plt.legend(['Reference']) ##Source: https://www.geeksforgeeks.org/matplotlib-pyplot-legend-in-python/
        plt.show()
        for i in range(0,len(ref)):
            ref_str=str(ref[i])
            ser.write((ref_str+'\n').encode())
    elif (selection =='n'):
        ref = genRef('cubic') #switch in accordance with test. Gains for cubic are Kp 10, Kd 500, for step are Kp 1, Kd 1000
        print(len(ref))
        t = range(len(ref))
        plt.plot(t,ref,'r*-')
        plt.ylabel('Angle (deg)')
        plt.xlabel('index (200 Hz)')
        plt.legend(['Reference']) #Source: https://www.geeksforgeeks.org/matplotlib-pyplot-legend-in-python/
        plt.show() 
        for i in range(0,len(ref)):
            ref_str=str(ref[i])
            ser.write((ref_str+'\n').encode())
    elif (selection =='o'):
        print('Executing trajectory \n')
        data_received=0
        posn=[]
        ref = genRef('cubic') #switch in accordance with test. Gains for cubic are Kp 10, Kd 500, for step are Kp 1, Kd 1000
        t = range(len(ref))
        while data_received < 1000:
            dat_str = ser.read_until(b'\n');  # get the data as a string, ints seperated by spaces
            dat_int = list(map(int,dat_str.split())) # now the data is a list
            posn.append(dat_int)
            data_received = data_received + 1
        plt.plot(t,ref,'r*-',t,posn,'b*-')
        plt.legend(["Reference","Output"]) #Source: https://www.geeksforgeeks.org/matplotlib-pyplot-legend-in-python/
        plt.title('Kp = 10mA/deg, Ki = 0mA/deg, Kd = 500dmA/deg')
        plt.ylabel('Angle (deg)')
        plt.xlabel('index (200 Hz)')
        plt.show()
    elif (selection == 'u'):
        print('Unpowering the motor')
    elif (selection == 'q'):
        print('Exiting client')
        has_quit = True; # exit client
        # be sure to close the port
        ser.close()
    elif (selection == 'r'):
        print('Getting mode...')
        n_str = ser.read_until(b'\n');
        n_int = int(n_str) # turn it into an int
        print('Got back mode ' + str(n_int)) # print it to the screen
    else:
        print('Invalid Selection ' + selection_endline)



 