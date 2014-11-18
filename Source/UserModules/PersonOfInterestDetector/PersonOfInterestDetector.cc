//
//	PersonOfInterestDetector.cc		This file is a part of the IKAROS project
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

#include "PersonOfInterestDetector.h"
#include "GazeDetection.h"
#include <cmath>
#include <vector>

// use the ikaros namespace to access the math library
// this is preferred to using math.h

using namespace ikaros;

void PersonOfInterestDetector::Init() {
    // Inputs

    PEOPLE         = GetInputMatrix("PEOPLE");
    PEOPLE_SIZE_X  = GetInputSizeX("PEOPLE");
    PEOPLE_SIZE_Y  = GetInputSizeY("PEOPLE");

    // Outputs

    PLAN                = GetOutputArray("PLAN");
    PLAN_SIZE           = GetOutputSize("PLAN");

    STRENGTH            = GetOutputArray("STRENGTH");
    STRENGTH_SIZE       = GetOutputSize("STRENGTH");

    // Init

    set_array(PLAN, 180.0, PLAN_SIZE);
}

PersonOfInterestDetector::~PersonOfInterestDetector() {
    // Destroy data structures that you allocated in Init.
    // destroy_array("starting_position");
    // Do NOT destroy data structures that you got from the
    // kernel with GetInputArray, GetInputMatrix etc.
    // destroy_matrix("PEOPLE");
}

std::vector< int > lefties;
std::vector< int > righties;

float angleLimit = 20.0;
float centerLimit = 150.0;

void PersonOfInterestDetector::Tick() {

    // Check if people are looking left or right
    // Figure out where the people are seated (lrc)
    // Look at the person towards most others are looking
    int toTheRight;
    int toTheLeft;
    int toTheCenter;
    int numberOfPeople = 0;

    for (int i = 0; i < PEOPLE_SIZE_Y; ++i) {
        // Add yaw here to filter out people looking up or down
        if( PEOPLE[i][2] > 0.0 ) {

            numberOfPeople = numberOfPeople + 1;

            bool lookingLeft = false;
            bool lookingRight = false;
            bool lookingForward = false;
            bool seatedRight = false;
            bool seatedLeft = false;
            bool seatedCenter = false;

            if( PEOPLE[i][4] < -angleLimit ) {
                // Looking left
                lookingLeft = true;
            }

            if( PEOPLE[i][4] > angleLimit ) {
                // Looking right
                lookingRight = true;
            }

            if( (PEOPLE[i][4] >= -angleLimit) && (PEOPLE[i][4] <= angleLimit) ) {
                // Looking forward
                lookingForward = true;
            }

            if( PEOPLE[i][0] < -centerLimit ) {
                // Seated to the right
                seatedRight = true;
                toTheRight = i;
            }

            if( PEOPLE[i][0] > centerLimit ) {
                // Seated to the left
                seatedLeft = true;
                toTheLeft = i;
            }

            if( (PEOPLE[i][0] >= -centerLimit) && (PEOPLE[i][0] <= centerLimit) ) {
                // Seated to the center
                seatedCenter = true;
                toTheCenter = i;
            }


            // Looking left and seated right
            // Looking left and seated center
            if( (lookingLeft && seatedRight) || (lookingLeft && seatedCenter) ) {
                lefties.push_back(i);
            }

            // Looking right and seated left
            // Looking right and seated center
            if( (lookingRight && seatedLeft) || (lookingRight && seatedCenter) ) {
                righties.push_back(i);
            }
        }
    }

    // printf("%i\n", numberOfPeople);
    // printf("Righties: ");
    // for (int i = 0; i < righties.size(); ++i) {
    //     printf("%i", righties[i]);
    // }
    // printf("\n");

    // printf("Lefties: ");
    // for (int i = 0; i < lefties.size(); ++i) {
    //     printf("%i", lefties[i]);
    // }
    // printf("\n");

    STRENGTH[0] = 0.0;

    if( numberOfPeople == 3 ) {

        if( lefties.size() == righties.size() && toTheCenter < 4 ) {
            // Look at center
            PLAN[0] = 180.0 - std::atan(PEOPLE[toTheCenter][0]/PEOPLE[toTheCenter][2]) * (180/pi);
            PLAN[1] = 270.0 + std::atan(PEOPLE[toTheCenter][1]/PEOPLE[toTheCenter][2]) * (180/pi);
            PLAN[2] = 180.0;
            STRENGTH[0] = 1.0;
            printf("\n\nLooking forwards at %i\n\n", toTheCenter);
        }

        if( lefties.size() > righties.size() ) {
            // Look at right
            PLAN[0] = 180.0 - std::atan(PEOPLE[toTheRight][0]/PEOPLE[toTheRight][2]) * (180/pi);
            PLAN[1] = 270.0 + std::atan(PEOPLE[toTheRight][1]/PEOPLE[toTheRight][2]) * (180/pi);
            PLAN[2] = 180.0;
            STRENGTH[0] = 1.0;
            printf("\n\nLooking to the right at %i\n\n", toTheRight);
        }

        if( righties.size() > lefties.size() ){
            // Look at left
            PLAN[0] = 180.0 - std::atan(PEOPLE[toTheLeft][0]/PEOPLE[toTheLeft][2]) * (180/pi);
            PLAN[1] = 270.0 + std::atan(PEOPLE[toTheLeft][1]/PEOPLE[toTheLeft][2]) * (180/pi);
            PLAN[2] = 180.0;
            STRENGTH[0] = 1.0;
            printf("\n\nLooking to the left at %i\n\n", toTheLeft);
        }

    }

    lefties.clear();
    righties.clear();

}

// Install the module. This code is executed during start-up.

static InitClass init("PersonOfInterestDetector", &PersonOfInterestDetector::Create, "Source/UserModules/PersonOfInterestDetector/");


