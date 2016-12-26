#!/usr/bin/python

import socket

HOST="127.0.0.1"
PORT=2000
PORT = int(raw_input("input port: "))
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect((HOST,PORT))
while 1:
    cmd = raw_input("input msg: ")
    if cmd == "exit":
        break
    s.send(cmd)
    data = s.recv(1024)
    print "resp data:", data
s.close()
