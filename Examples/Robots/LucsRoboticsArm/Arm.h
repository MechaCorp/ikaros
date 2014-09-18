#ifndef Arm 
#define Arm
#include "IKAROS.h"
class Arm: public Module  	{
public:
static Module * Create(char * name, Parameter * p) { 
	return new Arm(p); }

Arm(char * name, Parameter * p) : Module(p) {}; 	
virtual -Arm() {};

void Init();
void Tick();

int size;
	float * input1; 
	float * input2; 

	};

#endif
