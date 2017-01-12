#!/usr/bin/python

import socket
import struct

HOST="127.0.0.1"
PORT=9093
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect((HOST,PORT))
while 1:
	cmd = raw_input("input msg: ")
	if cmd == "exit":
		break
	i = 0
	mss = ''
	while i < 1000:
		client_type = int(1)
		check_sum = int(4)
		#length = int(len(cmd))
		ts = long(5)
		i = i + 1
		length = i
		print "i:",i
		msg = struct.pack('iiiiq', client_type, i,check_sum,length,ts)
		s.send(msg)
		mss = mss + 'a'
		s.send(bytes(mss))

#	while i > 0:
#		data = s.recv(1024)
#		print "resp data:", data
#		--i
s.close()
