#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <time.h>
#include <sys/time.h>
#include <unistd.h>

int main(int argc, char const* argv[]){
	int count = 0;
	int ran=0;

	time_t timer,now_timer;
	timer = time(NULL);
	double dt;
	while(1){
		now_timer = time(NULL);
		dt=difftime(now_timer,timer);
		std::cout<<dt<<std::endl;
		count++;
		if(dt>5){
			timer = time(NULL);
		}
	}
}
