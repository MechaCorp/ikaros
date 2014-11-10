//
//	ActionPlanPrioritizer.h		This file is a part of the IKAROS project
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

#ifndef ActionPlanPrioritizer_
#define ActionPlanPrioritizer_

#include "IKAROS.h"

class ActionPlanPrioritizer: public Module
{
public:
    static Module * Create(Parameter * p) { return new ActionPlanPrioritizer(p); }

    ActionPlanPrioritizer(Parameter * p) : Module(p) {}
    virtual ~ActionPlanPrioritizer();

    void 		Init();
    void 		Tick();

    // Inputs

    // float **    STARING;
    // int         STARING_SIZE_X;
    // int         STARING_SIZE_Y;

    // float **    PROXIMITY;
    // int         PROXIMITY_SIZE_X;
    // int         PROXIMITY_SIZE_Y;

    // float *     PERSONOFINTEREST;
    // int         PERSONOFINTEREST_SIZE;

    float **    STIMULI;
    int         STIMULI_SIZE_X;
    int         STIMULI_SIZE_Y;

    // Outputs

    float *     ACTION;
    int         ACTION_SIZE;
};

#endif
