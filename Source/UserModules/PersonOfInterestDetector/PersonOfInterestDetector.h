//
//	PersonOfInterestDetector.h		This file is a part of the IKAROS project
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

#ifndef PersonOfInterestDetector_
#define PersonOfInterestDetector_

#include "IKAROS.h"

class PersonOfInterestDetector: public Module
{
public:
    static Module * Create(Parameter * p) { return new PersonOfInterestDetector(p); }

    PersonOfInterestDetector(Parameter * p) : Module(p) {}
    virtual ~PersonOfInterestDetector();

    void 		Init();
    void 		Tick();

    // Inputs

    float **    PEOPLE;
    int         PEOPLE_SIZE_X;
    int         PEOPLE_SIZE_Y;

    // Outputs

    float *     ACTION;
    int         ACTION_SIZE;

};

#endif
