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

	struct timeval myTime,nowTime;
	struct tm *time_st;
	time_t diffsec;
	suseconds_t diffsub;
	double realsec;

	gettimeofday(&myTime, NULL);
	while(1){
		gettimeofday(&nowTime,NULL);

		diffsec = difftime(nowTime.tv_sec,myTime.tv_sec);
		diffsub = nowTime.tv_usec - myTime.tv_usec;
		realsec = diffsec + diffsub*1e-6;
		std::cout<<realsec<<std::endl;

		now_timer = time(NULL);
		dt=difftime(now_timer,timer);
		//std::cout<<dt<<std::endl;
		//std::cout<<myTime<<std::endl;
		count++;
		//if(dt>5){
		//	timer = time(NULL);
		//}
	}
}
