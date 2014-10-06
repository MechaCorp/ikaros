
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

float const r = 5000;

float x;
float y;
float z;

float alphaX;
float alphaY;

float h;
float H;
float C;
float A;
float B;

float hy;
float Hy;
float Cy;
float Ay;
float By;

void GazeDirector::Tick() {
  x = HEADS[0][0];
  z = HEADS[0][2];
  alphaX = -HEADS[0][4];
  alphaY = HEADS[0][3];

  B = atan( x / z );
  h = abs(sqrt( x * x + z * z ));
  H = asin( ( h * sin(alphaX) ) / r );
  C = 180 - (H + alphaX);
  A = C + B;

  /* Horizontal */
  By = atan( y / z );
  hy = abs(sqrt( y * y + z * z ));
  Hy = asin( ( hy * sin(alphaY) ) / r );
  Cy = 180 - (Hy + alphaY);
  Ay = Cy + By;

  TARGET_POSITION[0] = A;
  TARGET_POSITION[1] = 90 + Ay;
  TARGET_POSITION[2] = 180.0;



  // lx = x * tan(90.0 - (alphaX + betaX));

  // TARGET_POSITION[0] = ( 180 - (alphaX + betaX) - asin( ( (z - lx) * sin(alphaX + betaX) ) / r ) );

  //  Vertical
  // y = HEADS[0][1];
  // alphaY = -HEADS[0][3];

  // betaY = atan( y / z );
  // ly = x * tan(90.0 - (alphaY + betaY));

  // TARGET_POSITION[1] = 90 + ( 180 - (alphaY + betaY) - asin( ( (z - ly) * sin(alphaY + betaY) ) / r ) );

  // printf("%f ", TARGET_POSITION[0]);
  // printf("(%f) ", alphaX);
  // printf("%f ", TARGET_POSITION[1]);
  // printf("(%f)\n", alphaY);

}

static InitClass init("GazeDirector", &GazeDirector::Create, "Source/UserModules/GazeDirector/");