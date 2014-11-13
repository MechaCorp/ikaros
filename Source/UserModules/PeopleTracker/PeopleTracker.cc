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
#include <iostream>
#include <iomanip>

// use the ikaros namespace to access the math library
// this is preferred to using math.h

using namespace ikaros;

float ** oldPeople;
float * peopleInactivity;

float tick = 0.0;

float maxInactivityCounter = 2.0;

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

    oldPeople = create_matrix(PEOPLE_SIZE_X, PEOPLE_SIZE_Y);
    peopleInactivity = create_array(PEOPLE_SIZE_Y);

    set_matrix(PEOPLE, 0.0, PEOPLE_SIZE_X, PEOPLE_SIZE_Y);
    set_matrix(oldPeople, 0.0, PEOPLE_SIZE_X, PEOPLE_SIZE_Y);
    set_array(peopleInactivity, 0.0, PEOPLE_SIZE_Y);
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
    // Increase tick counter
    tick = tick + 1.0;
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
                    //printf("Update person %i\n", j);
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
                        //printf("New person %i!\n", k);
                        break;
                    }

                }
            }
        }

        // Save values
        oldPeople[i][0] = oldPeople[i][0] == 0.0 ? PEOPLE[i][0] : oldPeople[i][0];
        oldPeople[i][1] = oldPeople[i][1] == 0.0 ? PEOPLE[i][1] : oldPeople[i][1];
        oldPeople[i][2] = oldPeople[i][2] == 0.0 ? PEOPLE[i][2] : oldPeople[i][2];
        oldPeople[i][3] = oldPeople[i][3] == 0.0 ? PEOPLE[i][3] : oldPeople[i][3];
        oldPeople[i][4] = oldPeople[i][4] == 0.0 ? PEOPLE[i][4] : oldPeople[i][4];
        oldPeople[i][5] = oldPeople[i][5] == 0.0 ? PEOPLE[i][5] : oldPeople[i][5];
        oldPeople[i][6] = oldPeople[i][6] == 0.0 ? PEOPLE[i][6] : oldPeople[i][6];
        oldPeople[i][7] = oldPeople[i][7] == 0.0 ? PEOPLE[i][7] : oldPeople[i][7];
    }


    for (int i = 0; i < HEADS_SIZE_Y; ++i) {
        // Calculate inter-tick delta
        float diff = PEOPLE[i][3] - oldPeople[i][3];

        // Check if coordinates have not changed
        if(diff == 0.0) {
            // If so increase inactivity counter
            peopleInactivity[i] = peopleInactivity[i] + 1.0;
        }
        else {
            // If not, zero it
            peopleInactivity[i] = 0.0;
        }

        // If inactivity counter is over 20, remove person
        if(peopleInactivity[i] > maxInactivityCounter) {
            PEOPLE[i][0] = 0.0;
            PEOPLE[i][1] = 0.0;
            PEOPLE[i][2] = 0.0;
            PEOPLE[i][3] = 0.0;
            PEOPLE[i][4] = 0.0;
            PEOPLE[i][5] = 0.0;
            PEOPLE[i][6] = 0.0;
            PEOPLE[i][7] = 0.0;

            oldPeople[i][0] = 0.0;
            oldPeople[i][1] = 0.0;
            oldPeople[i][2] = 0.0;
            oldPeople[i][3] = 0.0;
            oldPeople[i][4] = 0.0;
            oldPeople[i][5] = 0.0;
            oldPeople[i][6] = 0.0;
            oldPeople[i][7] = 0.0;

            peopleInactivity[i] = 0.0;
        }

        // Clear all oldPeople values every nth-tick
        if(tick >= maxInactivityCounter) {
            oldPeople[i][0] = 0.0;
            oldPeople[i][1] = 0.0;
            oldPeople[i][2] = 0.0;
            oldPeople[i][3] = 0.0;
            oldPeople[i][4] = 0.0;
            oldPeople[i][5] = 0.0;
            oldPeople[i][6] = 0.0;
            oldPeople[i][7] = 0.0;
        }
    }

    // Reset tick counter ecery nth-tick
    if(tick >= maxInactivityCounter) {
        tick = 0.0;
    }


    // for (int i = 0; i < PEOPLE_SIZE_Y; ++i) {
    //     printf("%i\t", i);
    //     printf("%f\t", PEOPLE[i][0]);
    //     printf("%f\t", PEOPLE[i][1]);
    //     printf("%f\t|\t", PEOPLE[i][2]);
    //     printf("%f\t", PEOPLE[i][3]);
    //     printf("%f\t", PEOPLE[i][4]);
    //     printf("%f\n", peopleInactivity[i]);
    // }

    // printf("\n");
}

// Install the module. This code is executed during start-up.

static InitClass init("PeopleTracker", &PeopleTracker::Create, "Source/UserModules/PeopleTracker/");


