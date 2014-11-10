//
//	SmoothMovement.cc		This file is a part of the IKAROS project
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

#include "SmoothMovement.h"
#include "CalcDampedSimpleHarmonicMotion.cpp"
#include <cmath>

// use the ikaros namespace to access the math library
// this is preferred to using math.h

using namespace ikaros;


void SmoothMovement::Init() {
    // To get the parameters from the IKC file, use the Bind
    // function for each parameter. The parameters are initialized
    // from the IKC and can optionally be changed from the
    // user interface while Ikaros is running. If the parameter is not
    // set, the default value will be used instead.

    Bind(debug, "debug");

    // We should bind parameters for Angular Frequency and Dampening

    // Inputs

    GOAL_POSITION              = GetInputArray("GOAL_POSITION");
    GOAL_POSITION_SIZE         = GetInputSize("GOAL_POSITION");

    CURRENT_POSITION           = GetInputArray("CURRENT_POSITION");
    CURRENT_POSITION_SIZE      = GetInputSize("CURRENT_POSITION");

    // Outputs

    POSITION_OUT        = GetOutputArray("POSITION_OUT");
    POSITION_OUT_SIZE   = GetOutputSize("POSITION_OUT");

    VELOCITY            = GetOutputArray("VELOCITY");
    VELOCITY_SIZE       = GetOutputSize("VELOCITY");

    // Init

    set_array(POSITION_OUT, 180.0, POSITION_OUT_SIZE);
    set_array(VELOCITY, 0.0, VELOCITY_SIZE);
    set_array(GOAL_POSITION, 180.0, GOAL_POSITION_SIZE);
}

SmoothMovement::~SmoothMovement() {
    // Destroy data structures that you allocated in Init.
    // destroy_array("starting_position");
    // Do NOT destroy data structures that you got from the
    // kernel with GetInputArray, GetInputMatrix etc.
}

// float percent = 0.0;
float * percent = create_array(3);
// float percent2 = 0.0;

void SmoothMovement::Tick() {
    // Calculate how much we have to move between two positions from 0 to 1
    // where 1 means that we are there and 0 means that we have 300 (max) degrees to move

    for (int i = 0; i < 3; ++i) {
        if(GOAL_POSITION[i] > 0.0) {
            percent[i] = 1 - fabs(CURRENT_POSITION[i] - GOAL_POSITION[i]) / (300);
            ::CalcDampedSimpleHarmonicMotion(&POSITION_OUT[i], &VELOCITY[i], GOAL_POSITION[i], percent[i], 1.0, 1.0);
        }
    }

    // if(GOAL_POSITION[0] > 0.0 && GOAL_POSITION[1] > 0.0) {
    //     percent = 1 - fabs(CURRENT_POSITION[0] - GOAL_POSITION[0]) / (300);
    //     percent2 = 1 - fabs(CURRENT_POSITION[1] - GOAL_POSITION[1]) / (300);

    //     // Change position and speed
    //     ::CalcDampedSimpleHarmonicMotion(&POSITION_OUT[0], &VELOCITY[0], GOAL_POSITION[0], percent, 1.0, 1.0);
    //     ::CalcDampedSimpleHarmonicMotion(&POSITION_OUT[1], &VELOCITY[1], GOAL_POSITION[1], percent2, 1.0, 1.0);
    // }

    // Debug log
    if(debug) {
        printf("\n");

        printf("%f\t", CURRENT_POSITION[0]);
        printf("%f\t", GOAL_POSITION[0]);
        printf("%f\t", percent[0]);
        printf("%f\t", POSITION_OUT[0]);
        printf("%f\n", VELOCITY[0]);

        printf("\n");
    }
}

// Install the module. This code is executed during start-up.

static InitClass init("SmoothMovement", &SmoothMovement::Create, "Source/UserModules/SmoothMovement/");


