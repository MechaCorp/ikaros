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

// use the ikaros namespace to access the math library
// this is preferred to using math.h

using namespace ikaros;

float ** people;

void PeopleTracker::Init() {
    // Inputs

    HEADS         = GetOutputMatrix("HEADS");
    HEADS_SIZE_X  = GetOutputSizeX("HEADS");
    HEADS_SIZE_Y  = GetOutputSizeY("HEADS");

    // Outputs

    PEOPLE         = GetInputMatrix("PEOPLE");
    PEOPLE_SIZE_X  = GetInputSizeX("PEOPLE");
    PEOPLE_SIZE_Y  = GetInputSizeY("PEOPLE");

    // Init

    set_matrix(PEOPLE, 0.0, PEOPLE_SIZE_X, PEOPLE_SIZE_Y);

    // Save people as a hash with key sqrt(x*x + y*y + z*z)?

    // arcsin(x/d) = vinkeln (d = avståndet till personen)
    people = create_matrix(4, 3);
}

PeopleTracker::~PeopleTracker() {
    // Destroy data structures that you allocated in Init.
    // destroy_array("starting_position");
    // Do NOT destroy data structures that you got from the
    // kernel with GetInputArray, GetInputMatrix etc.
}

void PeopleTracker::Tick() {
    for (int i = 0; i < HEADS_SIZE_Y; ++i) {
        // HEADS
        PEOPLE[i] = HEADS[i];
        PEOPLE[i][7] = asin( PEOPLE[i][6] / PEOPLE[i][0] );
        // Compare all xyz and update corresponding perons gaze direction and position
    }

    for (int i = 0; i < PEOPLE_SIZE_Y; ++i) {
        printf("%i: ", i);
        printf("%f\n", PEOPLE[i][7]);
    }
}

// Install the module. This code is executed during start-up.

static InitClass init("PeopleTracker", &PeopleTracker::Create, "Source/UserModules/PeopleTracker/");


