
#include "FaceInfo.h"

using namespace ikaros;

void FaceInfo::Init() {
  FACES_SIZE_X  = GetInputSizeX("FACES");
  FACES_SIZE_Y  = GetInputSizeY("FACES");
  FACES         = GetInputMatrix("FACES");

  TARGET_POSITION        = GetOutputArray("TARGET_POSITION");
  TARGET_POSITION_SIZE   = GetOutputSize("TARGET_POSITION");
}

FaceInfo::~FaceInfo() {
    // Destroy data structures that you allocated in Init.
    // Do NOT destroy data structures that you got from the
    // kernel with GetInputArray, GetInputMatrix etc.
}

void FaceInfo::Tick(){
  for(int i = 0; i < INPUT1_SIZE_X; i++){

    for(int j=0; j < INPUT1_SIZE_Y; j++){
        printf("%f\t", INPUT1[i][j]);
    }

    printf("\n");
  }

  printf("\n");

}

static InitClass init("FaceInfo", &FaceInfo::Create, "Source/UserModules/FaceInfo/");