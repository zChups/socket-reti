#!/usr/bin/env python3

# functionality: basic Web server, two modalities to uncomment: DUMMY + FORK

import socket
import os
import sys
import time

HOST='127.0.0.1'
PORT=8080

def send_dummy_response(conn, url):
    """ [OPZIONALE] invia una risposta http al client """

    body='<html><body><h1>Requested: %s</h1></body></html>' % url
    resp='HTTP/1.0 200 OK\r\n' + \
			'Connection: close\r\n' + \
			'Content-Type: text/html\r\n' + \
		  'Content-Length: %d\r\n\r\n' % len(body) + body
    conn.sendall(resp.encode('utf-8')) #ricordarsi di encodare il messaggio prima di spedirlo
    time.sleep(1)

def parse_request(conn):
    """individua url, request line e header della richiesta http"""

    request=''
    while True:
        request += conn.recv(1024).decode('utf-8')
        if request.find('\r\n\r\n') > 0: break
        reqline, headers = request.split('\r\n', 1)
        method, url, version = reqline.split(' ', 3)
        return url

def serve_request(conn):
    """ gestisce le richieste http """

    print('Connected by, ', addr)
    url=parse_request(conn)
    print(url)
    send_dummy_response(conn, url)
    conn.close()

# MAIN
with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
    s.bind((HOST, PORT))
    s.listen()

""" DUMMY/syncronised version

conn, addr = s.accept()
serve_request(conn)
"""



#ESEMPIO DI GESTIONE CONNESSIONI CONCORRENTI
#while True:
#    conn, addr = s.accept()
#    child_pid = os.fork()
#    if child_pid == 0:
#        #se la creazione è andata a buon fine servi la richiesta
#        serve_request(conn)
#        sys.exit()
#    else:
#        conn.close()