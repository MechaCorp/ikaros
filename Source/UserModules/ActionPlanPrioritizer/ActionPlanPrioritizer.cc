//
//	ActionPlanPrioritizer.cc		This file is a part of the IKAROS project
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

#include "ActionPlanPrioritizer.h"
#include <cmath>

// use the ikaros namespace to access the math library
// this is preferred to using math.h

using namespace ikaros;

void ActionPlanPrioritizer::Init() {
    // Inputs

    STARING         = GetInputArray("STARING");
    STARING_SIZE    = GetInputSize("STARING");

    PROXIMITY         = GetInputArray("PROXIMITY");
    PROXIMITY_SIZE    = GetInputSize("PROXIMITY");

    PERSONOFINTEREST         = GetInputArray("PERSONOFINTEREST");
    PERSONOFINTEREST_SIZE    = GetInputSize("PERSONOFINTEREST");

    // Outputs

    PLANS           = GetOutputMatrix("ACTION");
    PLANS_SIZE_X      = GetOutputSizeX("ACTION");
    PLANS_SIZE_Y      = GetOutputSizeY("ACTION");

    // Init

    set_matrix(PLANS, 0.0, PLANS_SIZE_X, PLANS_SIZE_Y);
}

ActionPlanPrioritizer::~ActionPlanPrioritizer() {
    // Destroy data structures that you allocated in Init.
    // destroy_array("starting_position");
    // Do NOT destroy data structures that you got from the
    // kernel with GetInputArray, GetInputMatrix etc.
    // destroy_matrix("PEOPLE");
}

int highestStimuliIndex;
float highestStimuli = 0.0;

void sendAction( int stimuliIndex ) {
    ACTION[0] = STIMULI[i][1];
    ACTION[1] = STIMULI[i][2];
    ACTION[2] = STIMULI[i][3];
}

void ActionPlanPrioritizer::Tick() {
    for (int i = STIMULI_SIZE_Y; i == 0; i--) {

        if(STIMULI[i][0] > highestStimuli) {
            highestStimuliIndex = i;
        }

    }

    sendAction( STIMULI[highestStimuliIndex] );
    // for (int i = 0; i < STARING_SIZE_Y; ++i) {
    //     if(STARING[i][0] > 0.0 && STARING[i][2] > 0.6) {
    //         ACTION[0] = STARING[i][0];
    //         ACTION[1] = STARING[i][1];
    //         printf("\n\nStaring at %i\n", i);
    //     }
    // }

    // if(PERSONOFINTEREST[2] > 0.5) {
    //     ACTION[0] = PERSONOFINTEREST[0];
    //     ACTION[1] = PERSONOFINTEREST[1];
    // }

    // if(PROXIMITY[0][0] > 0.5) {
    //     ACTION[0] = PROXIMITY[0][1];
    //     ACTION[1] = PROXIMITY[0][2];
    //     ACTION[2] = PROXIMITY[0][2];
    // }

}

// Install the module. This code is executed during start-up.

static InitClass init("ActionPlanPrioritizer", &ActionPlanPrioritizer::Create, "Source/UserModules/ActionPlanPrioritizer/");


