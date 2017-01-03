#!/bin/bash

g++ -std=c++11 main.cpp GateSvr.cpp Dispather.cpp BgMessageQueue.cpp -g -L/usr/local/bin -lmuduo -lpthread -I/home/godman/webpoj/majiang/muduo-master/ -o svr
