#!/usr/bin/python3

import socket, sched, time

localIP = "127.0.0.1"
localPort = 4000

UDPSocket = socket.socket(family=socket.AF_INET, type=socket.SOCK_DGRAM)

heartbeat_ownship_altitude = \
bytearray(b'\x7e\x00\x81\x00\x62\x39\x00\x00\xbe\x61\x7e\x7e\x0a\x00\x7c\x20\xe8\xe8\x95\xf7\x53\x41\x6d\x04\x38\x68\x00\x00\x00\x00\x09\x47\x53\x41\x00\x00\x00\x00\x00\x00\x5f\x8c\x7e\x7e\x0b\x00\xc8\x00\x15\xa0\xa8\x7e')
target1 = \
bytearray(b'\x7e\x14\x00\x7c\x80\x0d\xe8\x97\xca\x53\x3f\x9a\x16\xc9\x88\x08\xe0\x00\xf5\x01\x5a\x4b\x56\x20\x20\x20\x20\x20\x06\x05\xb0\x7e')
target2 = \
bytearray(b'\x7e\x14\x00\x7c\x17\x61\xe8\x94\x25\x53\x43\x3f\x4b\x09\x99\x11\x30\x00\x4a\x01\x45\x57\x4a\x20\x20\x20\x20\x20\x06\x29\x5f\x7e')

s = sched.scheduler(time.time, time.sleep)

def send_heartbeat_ownship_altitude(a = 'buffer'):
    UDPSocket.sendto(heartbeat_ownship_altitude, (localIP, localPort))
def send_target1(a = 'buffer'):
    UDPSocket.sendto(target1, (localIP, localPort))
def send_target2(a = 'buffer'):
    UDPSocket.sendto(target2, (localIP, localPort))

def runner():
    t0 = time.time()
    seconds = 0
    while(True):
        seconds += 1
        print("Setting up packet at ", t0 + seconds)
        s.enterabs(t0 + seconds, 1, send_heartbeat_ownship_altitude)
        s.enterabs(t0 + seconds + 0.140, 1, send_target2)
        s.enterabs(t0 + seconds + 0.345, 1, send_target1)
        s.run()

runner()
