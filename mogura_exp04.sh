#!/bin/bash
FILE="/home/pi/prog/mog_test/tes"
if [ ! -e $FILE ];then
	g++ -o tes adcread_test.cpp adcread.cpp -lbcm2835
fi

sudo ./tes
