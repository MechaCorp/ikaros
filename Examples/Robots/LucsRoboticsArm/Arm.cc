#include Arm.h





void Sum::Init()
{
	//läs in värdena för höger- respektive vänster-öga 

	
	input1 = GetInput("LEFT_EYE_POS");
	input2 = GetInput("RIGHT_EYE_POS");
	
	}

// skriv ut värdena för ögonen i kommandotolken
void Sum::Tick()
{ 	
	printf("%s\n", input1 );
	printf("%s\n", input2 );
	

	}
	 static InitClass init("Arm", &Arm::Create, "Source/UserModules/Arm/");