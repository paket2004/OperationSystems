#!/bin/bash
#stat ex1.c
#  Blocks: 8, size: 98, therefore size of each block is size/Blocks = 12.25
# ls -ilhs ex.c
# permissions:  -rw-r--r-- 
#(-) indicates that it is a regular file. (rw-) represent read, write, and no execute permissions for the file owner. (r--) represent read-only permissions for the group, and (r--) represent read-only permissions for 
#others.
# stat -c "%h" ex2.c ANS: 1
# ls -ilhs ex2.c No, they have different I-node numbers, because we just copy the info of the files.
# stat -c "%h - %n" / etc /* | grep ^3
#3 - /etc/apparmor
#3 - /etc/apport
#3 - /etc/ca-certificates
#3 - /etc/dconf
#3 - /etc/emacs
#3 - /etc/gdb
#3 - /etc/glvnd
#3 - /etc/gss
#3 - /etc/logcheck
#3 - /etc/perl
#3 - /etc/pm
#3 - /etc/pulse
#3 - /etc/sgml
#3 - /etc/ssh
#3 - /etc/ufw
#3 - /etc/update-manager
#This is  filenames that all map to the same i-node number
