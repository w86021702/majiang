#!/bin/bash

g++ -std=c++11 main.cpp GateSvr.cpp Dispather.cpp BgMessageQueue.cpp Worker.cpp -g -L/usr/local/bin -lmuduo -lpthread -I/home/godman/webpoj/majiang/muduo-master/demo -I/home/godman/webpoj/majiang/muduo-master/ -o svr
