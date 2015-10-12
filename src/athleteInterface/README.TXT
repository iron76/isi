
# Copyright: (C) 2006 RobotCub Consortium
# Author: Paul Fitzpatrick
# CopyPolicy: Released under the terms of the LGPLv2.1 or later, see LGPL.TXT

This program simulates a robot head looking out to sea at a boat.

On Linux type: 

   cmake .
   make
   # Edit athletebot.ini and change USERNAME to something you like
   ./run_athletebot --file athletebot.ini

On Windows with MSVC

   Use cmake and your compiler.
   Copy run_athletebot.exe from the Debug or Release subdirectory to the
   current directory (with athletebot.ini, back.ppm, and fore.ppm in it).
   Edit athletebot.ini and change USERNAME to something you like
   Run the program as "run_athletebot --file athletebot.ini"

Now you can run a copy of yarpview and connect it to /USERNAME/athletebot/camera

The simulated camera can be moved around by sending commands to 
/USERNAME/athletebot/motor/rpc:i
