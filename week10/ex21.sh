#!/bin/bash
#file /dev/*
#file /etc/*
# in dev directory we have directory, character special, symbolic link to /proc/self/fd, block special, sticky, directory.
# in etc directory: directory, ASCII text, Unicode text, UTF-8 text, data, magic text file for file(1) cmd, ASCII text, a /usr/sbin/nft -f script, ASCII text executable, empty, symbolic link to /etc/alternatives/vtrgb
# ls -l / etc | grep ^d | wc -l ANS: 83.
#before compilation: ex.c: C source, ASCII text
#After compilation:  ex.c: C source, ASCII text (for ex.c, for ex different. (for ex it is different)
# After changing for non-english words: ex.c: C source, Unicode text, UTF-8 text
#Now this is another type of symbols and system defines in differently
