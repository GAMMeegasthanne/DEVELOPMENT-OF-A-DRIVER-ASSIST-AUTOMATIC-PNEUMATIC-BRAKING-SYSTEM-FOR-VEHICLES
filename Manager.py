import serial

# Establish the serial connection
ser = serial.Serial('/dev/ttyUSB0', 9600)  # Replace '/dev/ttyUSB0' with your Arduino's serial port
ser.flushInput()

while True:
    if ser.in_waiting > 0:
        # Read the serial data line by line
        line = ser.readline().decode().rstrip()
        
        # Split the line into the parameter name and value
        param_name, param_value = line.split(': ')
        
        # Process the received parameter values
        if param_name == 'RPM':
            rpm = int(param_value)
            # Do something with the RPM value in Python
        elif param_name == 'Braking Distance':
            braking_distance = float(param_value)
            # Do something with the braking distance value in Python
        elif param_name == 'Distance':
            distance = int(param_value)
            # Do something with the distance value in Python
        elif param_name == 'Raining':
            raining = bool(param_value)
            # Do something with the raining condition value in Python
