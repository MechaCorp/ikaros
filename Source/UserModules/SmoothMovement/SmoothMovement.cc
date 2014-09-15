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

    active                     = GetInputArray("active");
    active_size                = GetInputSize("active");

    // Outputs

    final_goal_position        = GetOutputArray("final_goal_position");
    final_goal_position_size   = GetOutputSize("final_goal_position");

    movement_speed             = GetOutputArray("movement_speed");
    movement_speed_size        = GetOutputSize("movement_speed");

    starting_position          = create_array(3);

    set_array(final_goal_position, 0.0, final_goal_position_size);

}



SmoothMovement::~SmoothMovement() {
    // Destroy data structures that you allocated in Init.

    // Do NOT destroy data structures that you got from the
    // kernel with GetInputArray, GetInputMatrix etc.
}



float CosineInterpolate(float start, float stop, float mu) {
   float mu2;
   mu2 = (1 - cos(mu*3.14) ) / 2;
   return (start * (1-mu2) + stop * mu2);
}

int tick = 0;
int duration = 10;
float percent = 0.0;

void SmoothMovement::Tick() {

    if(active[0] == 1) {
        tick = tick + 1;

        if(tick == 1) {
            copy_array(starting_position, current_position, current_position_size);
            copy_array(final_goal_position, goal_position, goal_position_size);

            duration = floor(labs( (final_goal_position[0] - starting_position[0]) / 0.75));

            printf("From %f", starting_position[0]);
            printf(" to %f", final_goal_position[0]);
            printf(" in %i ticks\n", duration);
        }

        if(tick < duration) {
            percent = (float)tick / (float)duration;

            // Reached the goal position, reset the speed
            if(percent > 0.9) {
                printf("Reached goal position\n");
                movement_speed[0] = 0.01;
            }

            else {
                if(movement_speed[0] <= 0.95) {
                    // Increase the speed for each tick
                    printf("%i: ", tick);
                    printf("%f = ", percent);
                    printf("%f\n", ::CosineInterpolate(starting_position[0], final_goal_position[0], percent ) / (starting_position[0]+final_goal_position[0]) );
                    movement_speed[0] = ::CosineInterpolate(starting_position[0], final_goal_position[0], percent ) / (starting_position[0]+final_goal_position[0]);
                }

            }
        }

    }
    else {
        tick = 0;
        duration = 10;
    }
}



// Install the module. This code is executed during start-up.

static InitClass init("SmoothMovement", &SmoothMovement::Create, "Source/UserModules/SmoothMovement/");


