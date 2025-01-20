import socket
import serial
from sys import exit
from threading import Thread

ser_port = input("What is your serial port (COMX): ")
ser = serial.Serial(
    port=ser_port,
    baudrate=9600,
    timeout=1
)

if ser.is_open:
        print("Serial port is open")
else:
    print("Couldnt open serial port")
    exit(1)

host = input("What's your host IP (XXX.XXX.XXX.XXX): ")
sock_port = int(input("What's the port to listen on: "))
server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
server_socket.bind((host, sock_port))
server_socket.listen(5)
print(f"Listening on {host}:{sock_port}")
value = 'L'
capture = True

def capture_serial():
    global value, capture
    print("Starting serial capture")
    try:
        while capture:
            line = ser.readline().decode('utf-8').strip()
            if line:
                print(f"Serial captured: {line}")
                value = line
    except KeyboardInterrupt:
        print("Stopped capture")
    finally:
        ser.close()
        print("Port closed")

try:
    capture_thread = Thread(target=capture_serial)
    capture_thread.start()
    client_socket, addr = server_socket.accept()
    print(f"Connection from {addr}")
    while True:
        client_socket.recv(1024)
        client_socket.send(value.encode('ascii'))
except Exception as e:
    pass
finally:
    server_socket.close()
    client_socket.close()
    capture = False
    capture_thread.join()
    print("Serial capture stopped")