//
//	StaringDetector.cc		This file is a part of the IKAROS project
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

#include "StaringDetector.h"
#include <cmath>

// use the ikaros namespace to access the math library
// this is preferred to using math.h

using namespace ikaros;

void StaringDetector::Init() {
    // Inputs

    PEOPLE         = GetInputMatrix("PEOPLE");
    PEOPLE_SIZE_X  = GetInputSizeX("PEOPLE");
    PEOPLE_SIZE_Y  = GetInputSizeY("PEOPLE");

    // Outputs

    STARING           = GetOutputMatrix("STARING");
    STARING_SIZE_X    = GetOutputSizeX("STARING");
    STARING_SIZE_Y    = GetOutputSizeY("STARING");

    // Init

    set_matrix(STARING, 0.0, PEOPLE_SIZE_Y, STARING_SIZE_X);
}

StaringDetector::~StaringDetector() {
    // Destroy data structures that you allocated in Init.
    // destroy_array("starting_position");
    // Do NOT destroy data structures that you got from the
    // kernel with GetInputArray, GetInputMatrix etc.
    // destroy_matrix("PEOPLE");
}

void StaringDetector::Tick() {

    for (int i = 0; i < PEOPLE_SIZE_Y; ++i) {
        float alphaX = abs(PEOPLE[i][4]);
        float alphaY = abs(PEOPLE[i][3]);

        // STARING[i][0] = 180 + alphaX;
        // STARING[i][1] = 270 + alphaY;

        STARING[i][0] = 180.0 - std::atan(PEOPLE[i][0]/PEOPLE[i][2]) * (180/pi);
        STARING[i][1] = 270.0 + std::atan(PEOPLE[i][1]/PEOPLE[i][2]) * (180/pi);

        if(alphaX <= 15.0 && alphaY <= 15.0) {
            STARING[i][2] = 1.0 - ( ( (alphaY + alphaX) / 2.0) / 15.0 );
        }
        else {
            STARING[i][2] = 0.0;
        }

        printf("Person %i ", i);
        printf("%f\n", STARING[i][2]);
    }

    printf("\n");

}

// Install the module. This code is executed during start-up.

static InitClass init("StaringDetector", &StaringDetector::Create, "Source/UserModules/StaringDetector/");


