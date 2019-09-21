#@Makefile
TARGET=main

CC=g++
OS=$(shell uname | cut -d_ -f1)

PATH_LIB = /usr/local/lib
PATH_INC = /usr/local/include

LDFLAGS = -L$(PATH_LIB) -L. -lbcm2835 -lwiringPi -lpthread
CFLAGS = -I$(PATH_INC) -I.

PROGRAM=$(addsuffix .cpp, $(TARGET))
OBJS = adcread.o servo_c.o mpu9250.o

#$(TARGET): $(PROGRAM)
$(TARGET): $(OBJS)
	$(CC) -std=c++11 -o $(TARGET) $(OBJS) $(PROGRAM) $(CFLAGS) $(LDFLAGS)

clean:
	rm $(TARGET)
	rm *.o

#adcread: adcread.c
#    g++ -L/usr/local/lib adcread.cpp -lbcm2835
#    g++ -I/usr/local/include -L/usr/local/lib servo.cpp -lwiringPi
