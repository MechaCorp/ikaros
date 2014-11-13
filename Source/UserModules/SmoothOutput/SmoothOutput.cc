//
//	SmoothOutput.cc		This file is a part of the IKAROS project
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

#include "SmoothOutput.h"
#include <cmath>

// use the ikaros namespace to access the math library
// this is preferred to using math.h

using namespace ikaros;


void SmoothOutput::Init() {
    // To get the parameters from the IKC file, use the Bind
    // function for each parameter. The parameters are initialized
    // from the IKC and can optionally be changed from the
    // user interface while Ikaros is running. If the parameter is not
    // set, the default value will be used instead.

    // Parameter

    Bind(alpha, "alpha");

    // Inputs

    INPUT              = GetInputArray("INPUT");
    INPUT_SIZE         = GetInputSize("INPUT");

    // Outputs

    OUTPUT             = GetOutputArray("OUTPUT");
    OUTPUT_SIZE        = GetOutputSize("OUTPUT");

    // Init

    set_array(OUTPUT, 0.0, OUTPUT_SIZE);
}

SmoothOutput::~SmoothOutput() {
    // Destroy data structures that you allocated in Init.
    // destroy_array("starting_position");
    // Do NOT destroy data structures that you got from the
    // kernel with GetInputArray, GetInputMatrix etc.
}

void SmoothOutput::Tick() {
    OUTPUT[0] = (alpha * INPUT[0]) + (1.0 - alpha) * OUTPUT[0];
}

// Install the module. This code is executed during start-up.

static InitClass init("SmoothOutput", &SmoothOutput::Create, "Source/UserModules/SmoothOutput/");


