# -*- coding: utf-8 -*-

import socket
SERVER_IP = '192.168.111.197'
SERVER_PORT = 1000

conn = socket.socket()
conn.connect((SERVER_IP, SERVER_PORT))
conn.send(b"Hello!\r\n")
data = b""
tmp = conn.recv(1024)
while tmp:
    data += tmp
    tmp = conn.recv(1024)
print( data.decode("utf-8") )
conn.close()
