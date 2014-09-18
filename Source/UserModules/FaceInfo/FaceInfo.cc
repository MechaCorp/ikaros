
#include "FaceInfo.h"

using namespace ikaros;

void FaceInfo::Init() {
	INPUT1_size_x = GetInputSizeX("INPUT1");
	INPUT1_size_y = GetInputSizeY("INPUT1");
	INPUT1 = GetInputMatrix("INPUT1");
}

FaceInfo::~FaceInfo() {
    // Destroy data structures that you allocated in Init.
    // Do NOT destroy data structures that you got from the
    // kernel with GetInputArray, GetInputMatrix etc.
}

void FaceInfo::Tick(){
	for(int i=0; i<INPUT1_size_x; i++){
		for(int j=0;j<INPUT1_size_y; j++){
				printf("%f\t", INPUT1[i][j]);
		}
		printf("\n");
	}
	printf("\n");
}

static InitClass init("FaceInfo", &FaceInfo::Create, "Source/UserModules/FaceInfo/");