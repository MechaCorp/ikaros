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
        if(PEOPLE[i][2] > 100.0)  {

            float alphaX = abs(PEOPLE[i][4]);
            float alphaY = abs(PEOPLE[i][3]);

            if(alphaX <= 15.0 && alphaY <= 15.0) {
                STRENGTH[0] = 1.0 - ( ( (alphaY + alphaX) / 2.0) / 15.0 );
                PLAN[0] = 180.0 - std::atan(PEOPLE[i][0]/PEOPLE[i][2]) * (180/pi);
                PLAN[1] = 270.0 + std::atan(PEOPLE[i][1]/PEOPLE[i][2]) * (180/pi);
                PLAN[2] = 180.0;
                // printf("Staring %i \t", i);
                // printf("%f\t", STRENGTH[0]);
                // printf("%f\t", alphaX);
                // printf("%f\n\n", alphaY);
                return;
            }
            else {
                STRENGTH[0] = 0.0;
            }
        }
    }
}

// Install the module. This code is executed during start-up.

static InitClass init("StaringDetector", &StaringDetector::Create, "Source/UserModules/StaringDetector/");


