
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

float horizontal_viewing_angle = 57.0;
float vertical_viewing_angle = 43.0;

void FaceInfo::Tick(){

  if(FACES[0][0] > 0) {
    // printf("%f ", FACES[0][0]);
    // printf("%f\n", target_degrees);
    TARGET_POSITION[0] = (180.0 + horizontal_viewing_angle/2.0) - (FACES[0][0] * horizontal_viewing_angle);

  }
  if(FACES[0][1] > 0) {
    TARGET_POSITION[1] = (FACES[0][1] * vertical_viewing_angle) - (225.0 + vertical_viewing_angle/2.0) * -1.00f;
  }

}

static InitClass init("FaceInfo", &FaceInfo::Create, "Source/UserModules/FaceInfo/");