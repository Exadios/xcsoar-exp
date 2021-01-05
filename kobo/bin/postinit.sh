#!/bin/sh

mkdir -p /dev/pts
mount -t devpts devpts /dev/pts

cp /opt/XCSoar/udev/ac   /usr/local/Kobo/udev/ac
cp /opt/XCSoar/udev/plug /usr/local/Kobo/udev/plug
