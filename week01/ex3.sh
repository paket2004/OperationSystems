#!/bin/bash

date
sleep 3
mkdir forroot
cd forroot
date
sleep 3
ls / -tr > root.txt
cd ..
date
sleep 3
mkdir forhome
cd forhome
date
sleep 3
ls ~ -tr > home.txt
cat~/forhome/home.txt
cat ~/forroot/root.txt
