#!/bin/bash

make clean
make
rmmod counter 
insmod counter.ko
