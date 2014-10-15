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

// use the ikaros namespace to access the math library
// this is preferred to using math.h

using namespace ikaros;

void Brain::Init() {
    // Inputs

    STARING         = GetInputMatrix("STARING");
    STARING_SIZE_X  = GetInputSizeX("STARING");
    STARING_SIZE_Y  = GetInputSizeY("STARING");

    // Outputs

    ACTION           = GetOutputArray("ACTION");
    ACTION_SIZE      = GetOutputSize("ACTION");

    // Init

    set_array(ACTION, 180.0, ACTION_SIZE);
}

Brain::~Brain() {
    // Destroy data structures that you allocated in Init.
    // destroy_array("starting_position");
    // Do NOT destroy data structures that you got from the
    // kernel with GetInputArray, GetInputMatrix etc.
    // destroy_matrix("PEOPLE");
}

void Brain::Tick() {

    for (int i = 0; i < STARING_SIZE_Y; ++i) {
        if(STARING[i][0] > 0.0 && STARING[i][2] > 0.6) {
            ACTION[0] = STARING[i][0];
            ACTION[1] = STARING[i][1];
        }
//        printf("Person %i ", i);
//        printf("%f\t", ACTION[0]);
//        printf("%f\t", ACTION[1]);
//        printf("%f\n", STARING[i][2]);
    }

}

// Install the module. This code is executed during start-up.

static InitClass init("Brain", &Brain::Create, "Source/UserModules/Brain/");


