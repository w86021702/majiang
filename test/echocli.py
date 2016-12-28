#!/usr/bin/python

import socket
import struct

HOST="127.0.0.1"
PORT=2000
PORT = int(raw_input("input port: "))
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect((HOST,PORT))
while 1:
    cmd = raw_input("input msg: ")
    if cmd == "exit":
        break
    uid = int(1)
    client_id = int(2)
    cmd = int(3)
    check_sum = int(4)
    #length = int(len(cmd))
    length = 0
    ts = long(5)
    msg = struct.pack('iiiiiq',uid,client_id,cmd,check_sum,length,ts)
    s.send(msg)
    data = s.recv(1024)
    print "resp data:", data
s.close()
