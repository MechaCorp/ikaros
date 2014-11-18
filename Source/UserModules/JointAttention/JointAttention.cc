
#include "JointAttention.h"
#include <stdint.h>
#include "GazeDetection.h"


using namespace ikaros;
using namespace std;

float ** attendCeiling;
float ** attendTable;

float const ceilingDistance = 2.0f;
float const tableDistance = 0.5f;

float const ceilingFactor = 0.015f;
float const tableFactor = 0.005f;

float peopleAttendingCeiling;
float peopleAttendingTable;

float ceilingSaliency;
float tableSaliency;

void JointAttention::Init() {
  HEADS               = GetInputMatrix("HEADS");
  HEADS_SIZE_X        = GetInputSizeX("HEADS");
  HEADS_SIZE_Y        = GetInputSizeY("HEADS");

  PLAN                = GetOutputArray("PLAN");
  PLAN_SIZE           = GetOutputSize("PLAN");

  STRENGTH            = GetOutputArray("STRENGTH");
  STRENGTH_SIZE       = GetOutputSize("STRENGTH");

  set_array(PLAN, 180.0, PLAN_SIZE);

  attendCeiling           = create_matrix(3, HEADS_SIZE_Y);
  attendTable             = create_matrix(3, HEADS_SIZE_Y);

  peopleAttendingCeiling  = 0.0;
  peopleAttendingTable    = 0.0;

  ceilingSaliency         = 0.0;
  tableSaliency           = 0.0;

  set_matrix(attendCeiling, 0.0, 3, HEADS_SIZE_Y);
  set_matrix(attendTable, 0.0, 3, HEADS_SIZE_Y);
}

JointAttention::~JointAttention() {
    // Destroy data structures that you allocated in Init.
    // Do NOT destroy data structures that you got from the
    // kernel with GetInputArray, GetInputMatrix etc.
}

void JointAttention::Tick() {
  // Reset number of people looking att ceiling/table
  peopleAttendingCeiling  = 0.0;
  peopleAttendingTable    = 0.0;

  for (int i = 0; i < HEADS_SIZE_Y; ++i) {
    // Reset all instructions
    attendCeiling[i][0] = 0.0;
    attendCeiling[i][1] = 0.0;
    attendCeiling[i][2] = 0.0;

    attendTable[i][0] = 0.0;
    attendTable[i][1] = 0.0;
    attendTable[i][2] = 0.0;

    // If someone looks up
    if(HEADS[i][3] < -30.0) {
      // Increase counter for people looking up
      peopleAttendingCeiling = peopleAttendingCeiling + 1.0;
      // Calculate instructions for following gaze
      attendCeiling[i][0] = CalculateRotationAfterGaze(HEADS[i][0], HEADS[i][2], HEADS[i][4], ceilingDistance, false);
      attendCeiling[i][1] = 180.0 + CalculateRotationAfterGaze(HEADS[i][1], HEADS[i][2], HEADS[i][3], ceilingDistance, true);
      attendCeiling[i][2] = 180.0;
    }

    // If someone looks down
    if(HEADS[i][3] > 30.0) {
      // Increase counter for people looking at table
      peopleAttendingTable = peopleAttendingTable + 1.0;
      // Calculate instructions for following gaze
      attendTable[i][0] = CalculateRotationAfterGaze(HEADS[i][0], HEADS[i][2], HEADS[i][4], tableDistance, false);
      attendTable[i][1] = CalculateRotationAfterGaze(HEADS[i][1], HEADS[i][2], HEADS[i][3], tableDistance, true);
      attendTable[i][2] = 200.0;
    }

  }

  // If people are attending the ceiling
  if(peopleAttendingCeiling > 0.0) {
    // Calculate this saliency
    ceilingSaliency = (ceilingSaliency >= 1.0) ? 1.0 : ceilingSaliency + peopleAttendingCeiling * ceilingFactor;
  }
  else {
    // Else, there is no saliency
    ceilingSaliency = 0.0;
  }
  // If peorple are attending the table
  if(peopleAttendingTable > 0.0) {
    // Calculate this saliencey
    tableSaliency = (tableSaliency >= 1.0) ? 1.0 : tableSaliency + peopleAttendingTable * tableFactor;
  }
  else {
    // Else there is no saliency
    tableSaliency = 0.0;
  }

  // If more people are looking at the table, send these instructions
  if(tableSaliency > ceilingSaliency) {
    PLAN[0] = attendTable[0][0];
    PLAN[1] = attendTable[0][1];
    PLAN[2] = attendTable[0][2];
    STRENGTH[0] = tableSaliency;
  }
  else {
    PLAN[0] = attendCeiling[0][0];
    PLAN[1] = attendCeiling[0][1];
    PLAN[2] = attendCeiling[0][2];
    STRENGTH[0] = ceilingSaliency;
  }

}

static InitClass init("JointAttention", &JointAttention::Create, "Source/UserModules/JointAttention/");