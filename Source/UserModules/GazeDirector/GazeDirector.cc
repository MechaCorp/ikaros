
#include "GazeDirector.h"
#include <string>
#include <algorithm>
#include <iostream>
#include <vector>
#include <stdint.h>


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

float beta;
float alpha;
float r = 3000;
float l;
float x;
float z;

void GazeDirector::Tick() {
  x = HEADS[0][0];
  z = HEADS[0][2];
  alpha = HEADS[0][4]; // Maybe 3 instead

  beta = atan( x / z );
  l = x * tan(90.0 - (alpha + beta));

  TARGET_POSITION[0] = ( 180 - (alpha + beta) - asin( ( (z-l) * sin(alpha + beta) ) / r ) );

  printf("%f\n", TARGET_POSITION[0]);

  // printf("%f\t", HEADS[0][0]);
  // printf("%f\t", HEADS[0][1]);
  // printf("%f\t", HEADS[0][2]);
  // printf("%f\t", HEADS[0][3]);
  // printf("%f\t", HEADS[0][4]);
  // printf("%f\n", HEADS[0][5]);
}

static InitClass init("GazeDirector", &GazeDirector::Create, "Source/UserModules/GazeDirector/");