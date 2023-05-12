#!/bin/bash

make clean
make
rmmod book_module
insmod book_module.ko
