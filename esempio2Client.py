#!/usr/bin/env python3
import socket
import sys

#definisco costanti utili nel programma
HOST = sys.argv[1]
PORT = 8080
INPUT_ADDRESS = sys.argv[2]

#apro il socket lato client
with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
    s.connect((HOST, PORT)) # mi connetto al server
    #invio le informazioni al server
    s.sendall(INPUT_ADDRESS.encode('utf-8'))
    #ricevo la risposta dal server
    data = s.recv(1024)
    #chiudo il socket
    s.close()
print('Server\'s response: %s' % data.decode('utf-8'))