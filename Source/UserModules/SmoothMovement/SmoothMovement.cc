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
//  This example is intended as a starting point for writing new Ikaros modules
//  The example includes most of the calls that you may want to use in a module.
//  If you prefer to start with a clean example, use he module MinimalModule instead.
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

    Bind(overall_speed, "overall_speed");

    // Inputs

    goal_position              = GetInputArray("goal_position");
    goal_position_size         = GetInputSize("goal_position");

    current_position           = GetInputArray("current_position");
    current_position_size      = GetInputSize("current_position");

    // Outputs

    POSITION_OUT        = GetOutputArray("POSITION_OUT");
    POSITION_OUT_SIZE   = GetOutputSize("POSITION_OUT");

    VELOCITY            = GetOutputArray("VELOCITY");
    VELOCITY_SIZE       = GetOutputSize("VELOCITY");


    // Init

    starting_position          = create_array(3);
    set_array(POSITION_OUT, 45.0, POSITION_OUT_SIZE);
    set_array(goal_position, 45.0, goal_position_size);

}



SmoothMovement::~SmoothMovement() {
    // Destroy data structures that you allocated in Init.
    // destroy_array("starting_position");
    // Do NOT destroy data structures that you got from the
    // kernel with GetInputArray, GetInputMatrix etc.
}

float percent = 0.0;

void SmoothMovement::Tick() {
    // This should be better
    percent = 1-fabs(current_position[0] - goal_position[0]) / (300);
    //percent = 1 - ( current_position[0] / fabs(current_position[0] - goal_position[0]) );
    printf("%f\t", current_position[0]);
    printf("%f\t", goal_position[0]);
    printf("%f\t", percent);

    //Reached the goal position, reset the speed
    if(percent > 0.99) {
        printf("Reached goal position\n");
        VELOCITY[0] = 0.01;
    }
    else {
        if(VELOCITY[0] <= 0.99) {
            // Increase the speed for each tick
            ::CalcDampedSimpleHarmonicMotion(&POSITION_OUT[0], &VELOCITY[0], goal_position[0], percent, 1.0, 1.0);
            printf("%f\t", POSITION_OUT[0]);
            printf("%f\n", VELOCITY[0]);
        }
    }
}



// Install the module. This code is executed during start-up.

static InitClass init("SmoothMovement", &SmoothMovement::Create, "Source/UserModules/SmoothMovement/");


