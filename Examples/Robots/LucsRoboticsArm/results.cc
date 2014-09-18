#include results.h





void Sum::Init()
{
	//läs in värdena för höger- respektive vänster-öga 

	size = GetInputSize("INPUT1");
	input = GetInput("INPUT");
	output1 = GetOutput("FACES");
	output2 = GetOutput("EYE_LEFT_POSITION");
	output3 = GetOutput("EYE_RIGHT_POSITION");


	
	}

// skriv ut värdena för ögonen i kommandotolken
void Sum::Tick()
{ 	


	}
	 static InitClass init("FACEDE", &FACEDE::Create, 
	 	"Source/UserModules/FACEDE/");