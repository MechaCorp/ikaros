
#include "GazeDirector.h"
#include <string>
#include <algorithm>
#include <iostream>
#include <vector>
#include <stdint.h>
#include <cmath>


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

float const maxLength = 3.0f;

float CalculateRotation(float coord, float z, float alpha, bool vertical) {
  float Y;
  float gamma;
  float beta;

  float distance = ::sqrt(coord * coord + z * z);
  float l2 = maxLength * maxLength;
  float d2 = distance * distance;

  Y = sqrt(l2 + d2 - 2.0 * maxLength * distance * ::cos(alpha));

  float temp1 = (d2 - l2 - (Y * Y));
  float temp2 = (- 2.0 * Y * maxLength);
  float temp3 = temp1 / temp2;
  gamma = ::acos( temp3 );
  //gamma = acos(- (d2 - l2 - Y * Y) / (2.0 * Y * maxLength) );

  if(vertical) {
    beta = ::asin(z / distance);
  }
  else {
    beta = ::asin(coord / distance);
  }

  return (pi - (alpha + gamma + beta))*(180.0/pi);
}

float x;
float y;
float z;

float alphaX;
float alphaY;

void GazeDirector::Tick() {
  // In meters
  x      = HEADS[0][0]/1000.0;
  y      = HEADS[0][1]/1000.0;
  z      = HEADS[0][2]/1000.0;
  
  // In radians
  alphaX = HEADS[0][4] * (pi/180.0);
  alphaY = HEADS[0][3] * (pi/180.0);

  TARGET_POSITION[0] = CalculateRotation(x, z, alphaX, false);
  TARGET_POSITION[1] = CalculateRotation(y, z, alphaY, true);
  TARGET_POSITION[2] = 180.0;

  printf("%f\t", TARGET_POSITION[0]);
  printf("%f\t\n", TARGET_POSITION[1]);

}

static InitClass init("GazeDirector", &GazeDirector::Create, "Source/UserModules/GazeDirector/");