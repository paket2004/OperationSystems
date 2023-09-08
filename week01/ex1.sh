#!/bin/bash

mkdir ~/week01
cd ~/week01
grep -r "gcc" /usr/bin | sort -r | tail -n 5 > ~/week01/ex1.txt

