#!/usr/bin/env python3
import socket
import sys
import time
import ipaddress

#definizione di costanti/parametri fissi
HOST = '127.0.0.1'
PORT = 8080

#DEFINIZIONE DI FUNZIONI
def elaborate_ip(ip_addr):
    try:
        ipaddress.ip_address(ip_addr)
    except ValueError:
        return 'ERROR'
    ip_class = check_class(int(ip_addr.split('.')[0]))
    net_addr = calculate_net_addr(ip_addr.split('.'), ip_class)
    broadcast = calculate_broadcast(ip_addr.split('.'), ip_class)
    return f'{ip_class} {net_addr} {broadcast}'
    


def check_class(class_part):
    if class_part in range(0, 128):
        return 'A'
    if class_part in range(128, 192):
        return 'B'
    if class_part in range(192, 224):
        return 'C'
    if class_part in range(224, 240):
        return 'D'
    if class_part in range(240, 255):
        return 'E'
    
def calculate_net_addr(ip, addr_class):
    if addr_class == 'A':
        return f'{ip[0]}.0.0.0'
    if addr_class == 'B':
        net_id = '.'.join(ip[0:2])
        return f'{net_id}.0.0'
    if addr_class == 'C':
        net_id = '.'.join(ip[0:3])
        return f'{net_id}.0'
    return 'No Net ID in this class'

def calculate_broadcast(ip, addr_class):
    if addr_class == 'A':
        return f'{ip[0]}.255.255.255'
    if addr_class == 'B':
        net_id = '.'.join(ip[0:2])
        return f'{net_id}.255.255'
    if addr_class == 'C':
        net_id = '.'.join(ip[0:3])
        return f'{net_id}.255'
    return 'No Broadcast in this class'                


with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
    s.bind((HOST, PORT))
    s.listen()
    conn, addr = s.accept()
    #ricevo le informazioni dal client
    data = conn.recv(1024)
    process_result = elaborate_ip(data.decode('utf-8'))
    #rispondo al client (DEVO fare encode)
    conn.sendall(process_result.encode('utf-8'))
    time.sleep(1) #il socket deve essere chiuso dal client