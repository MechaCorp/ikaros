#include <cmath>

float CalculateRotationAfterGaze(float coord, float z, float alpha, float maxLength, bool vertical) {
  float Y;
  float gamma;
  float beta;
  bool flip = false;

  // In meters
  coord = coord/1000.0;
  z = z/1000.0;

  // In radians
  alpha = alpha * (3.1416/180.0);

  float distance = ::sqrt(coord * coord + z * z);
  float l2 = maxLength * maxLength;
  float d2 = distance * distance;

   if(alpha > 0.0 && !vertical) {
     flip = true;
     alpha = -alpha;
   }

  Y = sqrt(l2 + d2 - 2.0 * maxLength * distance * ::cos(alpha));

  gamma = ::acos( (d2 - l2 - Y * Y) / (- 2.0 * Y * maxLength) );

  if(vertical) {
    beta = ::asin(z / distance);
  }
  else {
    beta = ::asin(coord / distance);
  }

  // printf("%f\t", alpha);
  // printf("%f\t", beta);
  // printf("%f\t\t", gamma);

  // printf("%f\t", distance);
  // printf("%f\t\n", Y);
  // printf("%f\n", (3.1416 - (-alpha + gamma + beta))*(180.0/3.1416));

   if(flip && !vertical) {
     return 360 - (3.1416 - (-alpha + gamma + beta))*(180.0/3.1416);
   }
   else {
    return (3.1416 - (-alpha + gamma + beta))*(180.0/3.1416);
  }
}