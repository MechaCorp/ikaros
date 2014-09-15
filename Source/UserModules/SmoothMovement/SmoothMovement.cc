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

    active                     = GetInputArray("active");
    active_size                = GetInputSize("active");

    // Outputs

    POSITION_OUT        = GetOutputArray("POSITION_OUT");
    POSITION_OUT_SIZE   = GetOutputSize("POSITION_OUT");

    VELOCITY            = GetOutputArray("VELOCITY");
    VELOCITY_SIZE       = GetOutputSize("VELOCITY");


    // Init

    starting_position          = create_array(3);
    set_array(POSITION_OUT, 0.0, POSITION_OUT_SIZE);

}



SmoothMovement::~SmoothMovement() {
    // Destroy data structures that you allocated in Init.
    // destroy_array("starting_position");
    // Do NOT destroy data structures that you got from the
    // kernel with GetInputArray, GetInputMatrix etc.
}



// float CosineInterpolate(float start, float stop, float mu) {
//    float mu2;
//    mu2 = (1 - cos(mu*3.14) ) / 2;
//    return (start * (1-mu2) + stop * mu2);
// }

// double SPRING_CONSTANT = 2.0;
// float VELOCITY = 2.0;

// float CriticallyDampedSpring( float a_Target, float a_Current, float & a_Velocity, float a_TimeStep ) {
//     float currentToTarget = a_Target - a_Current;
//     float springForce = currentToTarget * SPRING_CONSTANT;
//     float dampingForce = -a_Velocity * 2 * sqrt( SPRING_CONSTANT );
//     float force = springForce + dampingForce;
//     a_Velocity += force * a_TimeStep;
//     float displacement = a_Velocity * a_TimeStep;
//     return a_Current + displacement;
// }

int tick = 0;
int duration = 10;
float percent = 0.0;

void SmoothMovement::Tick() {

    if(active[0] == 1) {
        tick = tick + 1;

        // Initialize movement
        if(tick == 1) {
            copy_array(starting_position, current_position, current_position_size);
            copy_array(POSITION_OUT, current_position, current_position_size);

            duration = floor(labs( (goal_position[0] - starting_position[0]) / 7.5));

            printf("From %f", starting_position[0]);
            printf(" to %f", goal_position[0]);
            printf(" in %i ticks\n", duration);
        }

        // Calculate movement
        if(tick < duration) {
            percent = (float)tick / (float)duration;

            // Reached the goal position, reset the speed
            if(percent > 0.9) {
                printf("Reached goal position\n");
                VELOCITY[0] = 0.01;
            }

            else {
                if(VELOCITY[0] <= 0.95) {
                    // Increase the speed for each tick
                    printf("%i", tick);
                    printf(" (%f)", percent);
                    ::CalcDampedSimpleHarmonicMotion(&POSITION_OUT[0], &VELOCITY[0], goal_position[0], percent, 1.0, 1.0);
                    printf("\tp %f", POSITION_OUT[0]);
                    printf("\tv %f\n", VELOCITY[0]);
                    // printf("%f\n", ::CosineInterpolate(starting_position[0], final_goal_position[0], percent ) / (starting_position[0]+final_goal_position[0]) );
                    // movement_speed[0] = ::CosineInterpolate(starting_position[0], final_goal_position[0], percent ) / (starting_position[0]+final_goal_position[0]);

                    // printf("%f\n", ::CriticallyDampedSpring(starting_position[0], final_goal_position[0], VELOCITY, percent ) / (starting_position[0]+final_goal_position[0]) );
                    // movement_speed[0] = ::CriticallyDampedSpring(starting_position[0], final_goal_position[0], VELOCITY, percent ) / (starting_position[0]+final_goal_position[0]);
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


