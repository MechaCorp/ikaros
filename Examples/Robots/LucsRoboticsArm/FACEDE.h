#ifndef Arm 
#define Arm
#include "IKAROS.h"

class FACEDE: public Module  	
{


public:
static Module * Create(char * name, Parameter * p) { 
	return new Arm(p); }

FACEDE(char * name, Parameter * p) : Module(p) {}; 	
virtual -Arm() {};

void Init();
void Tick();

int size;
	
float * input1; 
float * output1; 
float * output2;
float * output3;

	};

#endif
