def print_help():
    print("\t\t", "28YJ-48 motor - 2038 steps per revolution")

    print("\t", "FIRST OPERANDS")
    print("\t", "------------------------------")
    print("\t", "[ movc ] : move clockwise")
    print("\t", "[ mova ] : move anticlockwise")
    print("\t", "params : 0 - 9999 (steps)")
    
    print("\t", "------------------------------")
    print("\t", "[ setc ] : set graph color")
    print("\t", "params : R(0-255), G(0-255), B(0-255)")
    
    print("\t", "------------------------------")
    print("\t", "[ clear ] : clear graph")
    print("\t", "no params")

    print("\t", "------------------------------")
    print("\t", "[ stop ] : stop current command")
    print("\t", "no params")
    print("\t\t", "")

    
    print("\t", "SECOND OPERANDS")
    print("\t", "------------------------------")
    print("\t", "[ del ] : delay between steps")
    print("\t", "params: uint (miliseconds)")
    print("\t", "chains with [movc] [mova]")
    
    
        
