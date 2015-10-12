// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

/*
 * Copyright: (C) 2010 RobotCub Consortium
 * Author: Paul Fitzpatrick
 * CopyPolicy: Released under the terms of the LGPLv2.1 or later, see LGPL.TXT
 */

#include <yarp/os/Network.h>
#include <yarp/dev/Drivers.h>

#include <AthleteBot.h>

using namespace yarp::os;
using namespace yarp::dev;

#define ENABLE_athletebot (1)

void add_athletebot() {
#if ENABLE_athletebot
    DriverCreator *factory = 
        new DriverCreatorOf<AthleteBot>("athletebot",
                                           "",
                                           "AthleteBot");
    Drivers::factory().add(factory); // hand factory over to YARP
#endif
}

