#!/usr/bin/env python3
import socket
import sys
import time

#hard-coded port

HOST= '127.0.0.1'
PORT= 2525

with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
    s.bind ((HOST, PORT))
    s.listen(5)
    conn, addr = s.accept()

    #from here ----

    time.sleep(1)
