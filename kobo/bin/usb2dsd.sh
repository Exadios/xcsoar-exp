#!/bin/sh

ifconfig usb0 down
rmmod g_ether usb_f_rndis usb_f_ecm_subset usb_f_eem usb_f_ecm u_ether libcomposite configfs

