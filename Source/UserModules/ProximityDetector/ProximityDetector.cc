//
//	ProximityDetector.cc		This file is a part of the IKAROS project
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

#include "ProximityDetector.h"
#include <cmath>
#include <algorithm>

// use the ikaros namespace to access the math library
// this is preferred to using math.h

using namespace ikaros;

void ProximityDetector::Init() {
    // Inputs

    DEPTH         = GetInputMatrix("DEPTH");
    DEPTH_SIZE_X  = GetInputSizeX("DEPTH");
    DEPTH_SIZE_Y  = GetInputSizeY("DEPTH");

    // Outputs

    PROXIMITY           = GetOutputMatrix("PROXIMITY");
    PROXIMITY_SIZE_X    = GetOutputSizeX("PROXIMITY");
    PROXIMITY_SIZE_Y    = GetOutputSizeY("PROXIMITY");

    // Init

    set_matrix(PROXIMITY, 0.0, PROXIMITY_SIZE_Y, PROXIMITY_SIZE_X);
}

ProximityDetector::~ProximityDetector() {
    // Destroy data structures that you allocated in Init.
    // destroy_array("starting_position");
    // Do NOT destroy data structures that you got from the
    // kernel with GetInputArray, GetInputMatrix etc.
    // destroy_matrix("PEOPLE");
}

void ProximityDetector::Tick() {
    float minimum = 1000.0;

    for(int x = 0; x < DEPTH_SIZE_X; x++) {
        for(int y = 0; y < DEPTH_SIZE_Y; y++) {
            minimum = std::min(DEPTH[x][y], minimum);
            printf("%f\n", minimum);
        }
    }

    if(minimum < 500.0) {
        printf("To close!\n");
        PROXIMITY[0][0] = 1.0f;
        PROXIMITY[0][1] = 90.0f;
        PROXIMITY[0][2] = 180.0f;
        PROXIMITY[0][3] = 90.0f;
    }
    else {
        PROXIMITY[0][0] = 0.0f;
    }

}

// Install the module. This code is executed during start-up.

static InitClass init("ProximityDetector", &ProximityDetector::Create, "Source/UserModules/ProximityDetector/");


