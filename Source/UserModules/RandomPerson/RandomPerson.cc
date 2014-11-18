//
//	RandomPerson.cc		This file is a part of the IKAROS project
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

#include "RandomPerson.h"
#include <cmath>
#include <cstdlib>
#include <ctime>

// use the ikaros namespace to access the math library
// this is preferred to using math.h

using namespace ikaros;

void RandomPerson::Init() {
    // Inputs

    PEOPLE              = GetInputMatrix("PEOPLE");
    PEOPLE_SIZE_X       = GetInputSizeX("PEOPLE");
    PEOPLE_SIZE_Y       = GetInputSizeY("PEOPLE");

    // Outputs

    PLAN                = GetOutputArray("PLAN");
    PLAN_SIZE           = GetOutputSize("PLAN");

    STRENGTH            = GetOutputArray("STRENGTH");
    STRENGTH_SIZE       = GetOutputSize("STRENGTH");

    // Init

    set_array(PLAN, 180.0, PLAN_SIZE);

    srand (static_cast <unsigned> (90212039912));
}

RandomPerson::~RandomPerson() {
    // Destroy data structures that you allocated in Init.
    // destroy_array("starting_position");
    // Do NOT destroy data structures that you got from the
    // kernel with GetInputArray, GetInputMatrix etc.
    // destroy_matrix("PEOPLE");
}

int forTicks = 50;
int i = 0;

void RandomPerson::Tick() {
    forTicks = forTicks - 1;

    printf("Randomly looking at %i\n", i);

    if( PEOPLE[i][2] > 10.0 ) {
        PLAN[0] = 180.0 - std::atan(PEOPLE[i][0]/PEOPLE[i][2]) * (180/pi);
        PLAN[1] = 270.0 + std::atan(PEOPLE[i][1]/PEOPLE[i][2]) * (180/pi);
        PLAN[2] = 180.0;
        STRENGTH[0] = 0.3;
    }

    if(PEOPLE[i][2] < 10.0 || forTicks <= 0) {
        forTicks = (rand() % 20 + 5);
        i = (rand() % 3);
    }

}

// Install the module. This code is executed during start-up.

static InitClass init("RandomPerson", &RandomPerson::Create, "Source/UserModules/RandomPerson/");


