//
//	PersonOfInterestDetector.cc		This file is a part of the IKAROS project
//
//    Copyright (C) 2012 <Author Name>
//
//    This program is free software; you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation; either version 2 of the License, or
//    (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with this program; if not, write to the Free Software
//    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
//    See http://www.ikaros-project.org/ for more information.
//

#include "PersonOfInterestDetector.h"
#include "GazeDetection.h"
#include <cmath>

// use the ikaros namespace to access the math library
// this is preferred to using math.h

using namespace ikaros;
float ** targets = create_matrix(2, 3);

void PersonOfInterestDetector::Init() {
    // Inputs

    PEOPLE         = GetInputMatrix("PEOPLE");
    PEOPLE_SIZE_X  = GetInputSizeX("PEOPLE");
    PEOPLE_SIZE_Y  = GetInputSizeY("PEOPLE");

    // Outputs

    PLAN                = GetOutputArray("PLAN");
    PLAN_SIZE           = GetOutputSize("PLAN");

    STRENGTH            = GetOutputArray("STRENGTH");
    STRENGTH_SIZE       = GetOutputSize("STRENGTH");

    // Init

    set_array(PLAN, 180.0, PLAN_SIZE);
    set_matrix(targets, 0.0, 2, 3);

}

PersonOfInterestDetector::~PersonOfInterestDetector() {
    // Destroy data structures that you allocated in Init.
    // destroy_array("starting_position");
    // Do NOT destroy data structures that you got from the
    // kernel with GetInputArray, GetInputMatrix etc.
    // destroy_matrix("PEOPLE");
}
float const sensitivity = 30.0;

bool isClose(float degree1x, float degree1y, float degree2x, float degree2y) {
    if( abs(degree1x - degree2x) < sensitivity && abs(degree1y - degree2y) < sensitivity ) {
        return true;
    }
    return false;
}

float const maxLength = 1.0f;

void PersonOfInterestDetector::Tick() {

    for (int i = 0; i < PEOPLE_SIZE_Y; ++i) {
        if(PEOPLE[i][2] > 0.0) {
          targets[i][0] = CalculateRotationAfterGaze(PEOPLE[i][0], PEOPLE[i][2], PEOPLE[i][4], maxLength, false);
          targets[i][1] = CalculateRotationAfterGaze(PEOPLE[i][1], PEOPLE[i][2], PEOPLE[i][3], maxLength, true);
        }
    }

    if(targets[0][0] > 0.0 && targets[1][0] > 0.0 && targets[2][0] > 0.0) {
        if( isClose(targets[0][0], targets[0][1], targets[1][0], targets[1][1]) ) {
            PLAN[0] = 180.0 - std::atan(PEOPLE[2][0]/PEOPLE[2][2]) * (180/pi);
            PLAN[1] = 270.0 + std::atan(PEOPLE[2][1]/PEOPLE[2][2]) * (180/pi);
            STRENGTH[0] = 1.0;
            printf("\n\nLooking at %i\n\n", 2);
        }

        if( isClose(targets[0][0], targets[0][1], targets[2][0], targets[2][1]) ) {
            PLAN[0] = 180.0 - std::atan(PEOPLE[1][0]/PEOPLE[1][2]) * (180/pi);
            PLAN[1] = 270.0 + std::atan(PEOPLE[1][1]/PEOPLE[1][2]) * (180/pi);
            STRENGTH[0] = 1.0;
            printf("\n\nLooking at %i\n\n", 1);
        }
        if( isClose(targets[1][0], targets[1][1], targets[2][0], targets[2][1]) ) {
            PLAN[0] = 180.0 - std::atan(PEOPLE[0][0]/PEOPLE[0][2]) * (180/pi);
            PLAN[1] = 270.0 + std::atan(PEOPLE[0][1]/PEOPLE[0][2]) * (180/pi);
            STRENGTH[0] = 1.0;
            printf("\n\nLooking at %i\n\n", 0);
        }
    }

}

// Install the module. This code is executed during start-up.

static InitClass init("PersonOfInterestDetector", &PersonOfInterestDetector::Create, "Source/UserModules/PersonOfInterestDetector/");


