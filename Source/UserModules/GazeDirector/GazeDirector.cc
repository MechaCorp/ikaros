
#include "GazeDirector.h"
#include <string>
#include <algorithm>
#include <iostream>
#include <vector>
#include <stdint.h>
#include "GazeDetection.h"


using namespace ikaros;
using namespace std;

void GazeDirector::Init() {
  HEADS         = GetInputMatrix("HEADS");
  HEADS_SIZE_X  = GetInputSizeX("HEADS");
  HEADS_SIZE_Y  = GetInputSizeY("HEADS");

  TARGET_POSITION        = GetOutputArray("TARGET_POSITION");
  TARGET_POSITION_SIZE   = GetOutputSize("TARGET_POSITION");

  set_array(TARGET_POSITION, 180.0, TARGET_POSITION_SIZE);
}

GazeDirector::~GazeDirector() {
    // Destroy data structures that you allocated in Init.
    // Do NOT destroy data structures that you got from the
    // kernel with GetInputArray, GetInputMatrix etc.
}

float const maxLength = 2.0f;

float x;
float y;
float z;

float alphaX;
float alphaY;

void GazeDirector::Tick() {
  x      = HEADS[0][0];
  y      = HEADS[0][1];
  z      = HEADS[0][2];

  alphaX = HEADS[0][4];
  alphaY = HEADS[0][3];

  TARGET_POSITION[0] = CalculateRotationAfterGaze(x, z, alphaX, maxLength, false);
  TARGET_POSITION[1] = 180.0 + CalculateRotationAfterGaze(y, z, alphaY, maxLength, true);
  TARGET_POSITION[2] = 180.0;
}

static InitClass init("GazeDirector", &GazeDirector::Create, "Source/UserModules/GazeDirector/");