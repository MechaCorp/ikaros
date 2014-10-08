//
//	PeopleTracker.cc		This file is a part of the IKAROS project
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

#include "PeopleTracker.h"
#include <cmath>

// use the ikaros namespace to access the math library
// this is preferred to using math.h

using namespace ikaros;

void PeopleTracker::Init() {
    // Inputs

    HEADS         = GetInputMatrix("HEADS");
    HEADS_SIZE_X  = GetInputSizeX("HEADS");
    HEADS_SIZE_Y  = GetInputSizeY("HEADS");

    // Outputs

    PEOPLE         = GetOutputMatrix("PEOPLE");
    PEOPLE_SIZE_X  = GetOutputSizeX("PEOPLE");
    PEOPLE_SIZE_Y  = GetOutputSizeY("PEOPLE");

    // Init

    set_matrix(PEOPLE, 0.0, PEOPLE_SIZE_X, PEOPLE_SIZE_Y);
}

PeopleTracker::~PeopleTracker() {
    // Destroy data structures that you allocated in Init.
    // destroy_array("starting_position");
    // Do NOT destroy data structures that you got from the
    // kernel with GetInputArray, GetInputMatrix etc.
    // destroy_matrix("PEOPLE");
}

void updatePerson(float * oldPerson, float * newPerson) {
    // XYZ
    oldPerson[0] = newPerson[0];
    oldPerson[1] = newPerson[1];
    oldPerson[2] = newPerson[2];
    // PJR
    oldPerson[3] = newPerson[3];
    oldPerson[4] = newPerson[4];
    oldPerson[5] = newPerson[5];
    // Distance
    oldPerson[6] = newPerson[6];
    // Angle from person to Kinect
    oldPerson[7] = asin( newPerson[0] / newPerson[6] );
}

void PeopleTracker::Tick() {
    // Angle of person to the kinect
    float angle;
    // Loop through input HEADS
    for (int i = 0; i < HEADS_SIZE_Y; ++i) {
        // Bool to see if we have updated this head or not
        bool updated = false;

        // If we have a head to track
        if(HEADS[i][2] > 10.0) {
            // Calculate angle to kinect
            angle = asin( HEADS[i][0] / HEADS[i][6] );

            // Loop through internal people matrix PEOPLE
            for (int j = 0; j < HEADS_SIZE_Y; ++j) {

                // Update person if within boundaries
                if( PEOPLE[j][2] > 10.0 && PEOPLE[j][7] >= (angle - 0.14) && PEOPLE[j][7] <= (angle + 0.14) ) {
                    printf("Update person %i\n", j);
                    updatePerson(PEOPLE[j], HEADS[i]);
                    updated = true;
                    break;
                }
            }

            // New person detected
            if(updated == false) {
                // Loop through internal people
                for (int k = 0; k < HEADS_SIZE_Y; ++k) {
                    // Find empty spot
                    if(PEOPLE[k][2] < 10.0) {
                        // Add new head to PEOPLE
                        updatePerson(PEOPLE[k], HEADS[i]);
                        printf("New person %i!\n", k);
                        break;
                    }

                }
            }
        }
    }

    for (int i = 0; i < PEOPLE_SIZE_Y; ++i) {
        printf("%i\t", i);
        printf("%.0lf\t", floor(PEOPLE[i][0]));
        printf("%.0lf\t", floor(PEOPLE[i][1]));
        printf("%.0lf\t\t", floor(PEOPLE[i][2]));
        printf("%.0lf\t", floor(PEOPLE[i][3]));
        printf("%.0lf\t", floor(PEOPLE[i][4]));
        printf("%.0lf\t\t", floor(PEOPLE[i][5]));
        printf("%.0lf\t", floor(PEOPLE[i][6]));
        printf("%f\n", PEOPLE[i][7]);
    }
}

// Install the module. This code is executed during start-up.

static InitClass init("PeopleTracker", &PeopleTracker::Create, "Source/UserModules/PeopleTracker/");


