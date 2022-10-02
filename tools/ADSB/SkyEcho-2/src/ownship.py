#!/usr/bin/python3

import socket, sched, time

localIP = "127.0.0.1"
localPort = 4000

UDPSocket = socket.socket(family=socket.AF_INET, type=socket.SOCK_DGRAM)

heartbeat_ownship_altitude = \
bytearray(b'\x7e\x00\x81\x00\x62\x39\x00\x00\xbe\x61\x7e\x7e\x0a\x00\x7c\x20\xe8\xe8\x95\xf7\x53\x41\x6d\x04\x38\x68\x00\x00\x00\x00\x09\x47\x53\x41\x00\x00\x00\x00\x00\x00\x5f\x8c\x7e\x7e\x0b\x00\xc8\x00\x15\xa0\xa8\x7e')

s = sched.scheduler(time.time, time.sleep)

def send_target_heartbeat_ownship_altitude(a = 'buffer'):
    UDPSocket.sendto(target_heartbeat_ownship_altitude, (localIP, localPort))
def send_target_target_ahrs(a = 'buffer'):
    UDPSocket.sendto(target_target_ahrs, (localIP, localPort))
def send_heartbeat_ownship_altitude(a = 'buffer'):
    UDPSocket.sendto(heartbeat_ownship_altitude, (localIP, localPort))
def send_target_ahrs(a = 'buffer'):
    UDPSocket.sendto(target_ahrs, (localIP, localPort))
def send_target(a = 'buffer'):
    UDPSocket.sendto(target, (localIP, localPort))

def runner():
    t0 = time.time()
    seconds = 0
    while(True):
        seconds += 1
        print("Setting up packet at ", t0 + seconds)
        s.enterabs(t0 + seconds, 1, send_heartbeat_ownship_altitude)
        s.run()

runner()
