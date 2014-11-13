//
//	Brain.cc		This file is a part of the IKAROS project
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

#include "Brain.h"
#include <cmath>
#include <cstdlib>

// use the ikaros namespace to access the math library
// this is preferred to using math.h

using namespace ikaros;

Brain::Brain(Parameter * p) : Module(p) {

    no_of_inputs    = GetIntValue("no_of_inputs");

    PLAN_INDEX      = new char * [no_of_inputs];
    STRENGTH_INDEX  = new char * [no_of_inputs];

    PLAN            = new float * [no_of_inputs];
    STRENGTH        = new float * [no_of_inputs];

    for( int i = 0; i < no_of_inputs; i++ ) {
        AddInput( PLAN_INDEX[i] = create_formatted_string("PLAN_%d", i+1) );
        AddInput( STRENGTH_INDEX[i] = create_formatted_string("STRENGTH_%d", i+1) );
    }

    AddOutput("ACTION");
}

void Brain::Init() {
    // Inputs

    for(int i = 0; i < no_of_inputs; i++) {
        PLAN[i]     = GetInputArray(PLAN_INDEX[i]);
        STRENGTH[i] = GetInputArray(STRENGTH_INDEX[i], false);
    }

    // Outputs

    ACTION           = GetOutputArray("ACTION");
    ACTION_SIZE      = GetOutputSize("ACTION");

    // Init

    set_array(ACTION, 180.0, ACTION_SIZE);
    srand (static_cast <unsigned> (123012301));
}

Brain::~Brain() {
    // Destroy data structures that you allocated in Init.
    // destroy_array("starting_position");
    // Do NOT destroy data structures that you got from the
    // kernel with GetInputArray, GetInputMatrix etc.
    // destroy_matrix("PEOPLE");

    for (int i = 0; i < no_of_inputs; i++) {
        destroy_string(PLAN_INDEX[i]);
        destroy_string(STRENGTH_INDEX[i]);
    }

    delete [] PLAN_INDEX;
    delete [] STRENGTH_INDEX;

    delete [] PLAN;
    delete [] STRENGTH;
}

void Brain::SetSizes() {
    SetOutputSize("ACTION", 3);
}

int idleTicks = 0;

void Brain::Tick() {
    int highestStimuliIndex = 100;
    float highestStimuli = 0.0;

    for (int i = 0; i < no_of_inputs; ++i) {

        if(*STRENGTH[i] > highestStimuli) {
            highestStimuli = *STRENGTH[i];
            highestStimuliIndex = i;
        }

        // printf("%i\t", i);
        // printf("%f\t", *STRENGTH[i]);
        // printf("%f\t", PLAN[i][0]);
        // printf("%f\t", PLAN[i][1]);
        // printf("%f\n", PLAN[i][2]);
    }

    if( highestStimuliIndex != 100 && highestStimuli > 0.1 ) {
        //printf("Action %i\n\n", highestStimuliIndex);
        ACTION[0] = PLAN[highestStimuliIndex][0];
        ACTION[1] = PLAN[highestStimuliIndex][1];
        ACTION[2] = PLAN[highestStimuliIndex][2];

        printf("%f\t", ACTION[0]);
        printf("%f\t", ACTION[1]);
        printf("%f\n", ACTION[2]);

        idleTicks = 0;
    }

    if(  highestStimuliIndex != 100 || highestStimuli < 0.1 ) {
        idleTicks = idleTicks + 1;
        printf("Idle %i\n", idleTicks);
    }

    if(idleTicks >= 50) {
        printf("idle\n");
        ACTION[0] = 180.0 + static_cast <float> (rand() % 100);
        ACTION[1] = 180.0 + static_cast <float> (rand() % 100);
        ACTION[2] = 180.0 + static_cast <float> (rand() % 100);
    }
}

// Install the module. This code is executed during start-up.

static InitClass init("Brain", &Brain::Create, "Source/UserModules/Brain/");


