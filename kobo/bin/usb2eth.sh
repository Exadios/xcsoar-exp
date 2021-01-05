#!/bin/sh

/sbin/insmod /drivers/mx6sll-ntx/usb/gadget/configfs.ko
/sbin/insmod /drivers/mx6sll-ntx/usb/gadget/libcomposite.ko
/sbin/insmod /drivers/mx6sll-ntx/usb/gadget/u_ether.ko
/sbin/insmod /drivers/mx6sll-ntx/usb/gadget/usb_f_ecm.ko
/sbin/insmod /drivers/mx6sll-ntx/usb/gadget/usb_f_eem.ko
/sbin/insmod /drivers/mx6sll-ntx/usb/gadget/usb_f_ecm_subset.ko
/sbin/insmod /drivers/mx6sll-ntx/usb/gadget/usb_f_rndis.ko
/sbin/insmod /drivers/mx6sll-ntx/usb/gadget/g_ether.ko
/sbin/ifconfig usb0 192.168.2.2

