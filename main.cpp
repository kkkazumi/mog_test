#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "servo.h"

int main(int argc, char const* argv[]){
  mog_servo mogura;
	mogura.setup();
	mogura.move(80);
	mogura.move(120);
}
