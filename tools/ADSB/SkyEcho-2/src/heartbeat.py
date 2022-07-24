#!/usr/bin/python3

import socket, sched, time

localIP = "127.0.0.1"
localPort = 4000

UDPSocket = socket.socket(family=socket.AF_INET, type=socket.SOCK_DGRAM)

buffer = bytearray(b'\x7e\x00\x81\x00\x31\x38\x00\x00\x73\x2a\x7e')

s = sched.scheduler(time.time, time.sleep)

def send_packet(a = 'default'):
    UDPSocket.sendto(buffer, (localIP, localPort))

def runner():
    t0 = time.time()
    seconds = 0
    while(True):
        seconds += 1
        print("Setting up packet at ", t0 + seconds)
        s.enterabs(t0 + seconds, 1, send_packet)
        s.run()

runner()
