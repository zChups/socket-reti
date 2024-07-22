#!/usr/bin/env python3
import socket
import sys

#only host as input from command line
HOST = sys.argv[1]
PORT = 2525

with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
    s.connect((HOST, PORT))

    #from here ---

    s.close
