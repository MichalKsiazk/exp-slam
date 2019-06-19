import serial as sp
import time
import io
import keyboard
import helper
from renderer import Renderer
from os import system
from enum import Enum

tp = sp.Serial('/dev/ttyACM0', 9600, timeout = 0.1)

valid_c1 = ['movc', 'stop', 'mova']
valid_c2 = ['del']

r = Renderer(500, 500) 

class Mode(Enum):
    CMD_MODE = 1;
    FREE_MODE = 2;

PROGRAM_MODE = Mode.FREE_MODE

def is_int(s):
    try:
        int(s)
        return True
    except ValueError:
        return False

def get_input():
    print("enter radar command:")
    user_input = input()
    user_input = user_input.replace("\x1bf", "")
    input_arr = user_input.split()
    return input_arr

def print_help():
    print("first operands:")
    for op in valid_c1:
        print("\t", op)

def validate_cmd(cmd):

    # empty cmd
    if not cmd:
        print("empty msg")
        return False

    # quick stop
    if(len(cmd) == 1 and (cmd[0] == "stop" or cmd[0] == "clear")):
        return True

    #set color
    if(cmd[0] == "setc"):
        r.set_color(int(cmd[1]), int(cmd[2]), int(cmd[3]))
        print("color changed")
        return False

    # valid first cmd
    if not(cmd[0] in valid_c1):
        print("invalid cmd")
        return False

    # valid second cmd
    if not(cmd[2] in valid_c2):
        print("invalid cmd")
        return False

    #valid values
    if not(is_int(cmd[1]) and is_int(cmd[3])):
        print("invalid value")
        return False
    
    #values in range
    if len(cmd[1]) > 4 or len(cmd[3] ) > 5:
        print("value too large")
        return False
    
    print("valid command!")
    return True


def send_cmd(cmd):
    is_valid = validate_cmd(cmd)
    if is_valid:
        output_cmd = '.'.join(cmd)
        print(output_cmd)
        print("cmd sent!")
        tp.write(str.encode(output_cmd + "\n"))
        return
  
def set_cmd_mode():
    global PROGRAM_MODE
    if PROGRAM_MODE == Mode.FREE_MODE:
        PROGRAM_MODE = Mode.CMD_MODE
    
keyboard.add_hotkey('alt+f', set_cmd_mode)


#for i in range(0, 2038):
#    r.draw_point(10, i, 4)

#r.flip()
while 1:
    if PROGRAM_MODE == Mode.CMD_MODE:
        print(" ")
        command = get_input()

        if(command[0] == "help"):
            helper.print_help()
        if(command[0] == "clear"):
            r.clear()

        if(command[0] == "free"):
            PROGRAM_MODE = Mode.FREE_MODE
        else:
            send_cmd(command)
            time.sleep(0.1)
            print(" ")
            PROGRAM_MODE = Mode.FREE_MODE
    elif PROGRAM_MODE == Mode.FREE_MODE:
        serial_input = tp.readline().decode("utf-8")
        if serial_input:
            splited_input = serial_input.split()
            if splited_input[0] == 's':
                r.draw_point(float(splited_input[3]), float(splited_input[1]), 8)
            print(serial_input)
            if splited_input[0] == 'e':
                send_cmd(["movc", "10", "del", "100"])

      

