#!/bin/bash

DEV=$(ip link | grep 'enp[^ ]*u' | awk '{print $2}' | tr -d ':')
sudo ip addr add 192.168.2.1 broadcast + dev $DEV
sudo ip route add 192.168.2.0/24 dev $DEV

