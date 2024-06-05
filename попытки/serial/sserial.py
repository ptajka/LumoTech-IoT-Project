
import serial, serial.tools.list_ports
import time, random


# подключение к порту Arduino
for COM_port in serial.tools.list_ports.comports():
    # print(COM_port.device)
    COM_port = COM_port.device
    try:
        arduino = serial.Serial(port=COM_port, baudrate=9600)
    except:
        continue

while 1:
  msg = input("Введите 1: ")
  if int(msg)==1:
    tdata = arduino.read()           # Wait forever for anything
    time.sleep(1)              # Sleep (or inWaiting() doesn't give the correct value)
    data_left = arduino.inWaiting()  # Get the number of characters ready to be read
    tdata += arduino.read(data_left) # Do the read and combine it with the first character
    print(tdata)

# while 1:
#     msg = input("Введите: ")
#     if msg!="":
#         arduino.write(bytes(msg, 'utf-8'))
#         time.sleep(1)
#         tdata = arduino.read()
#         time.sleep(1) 
#         print(tdata)