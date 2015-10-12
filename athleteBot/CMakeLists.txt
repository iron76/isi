# Copyright: (C) 2009 RobotCub Consortium
# Author: Paul Fitzpatrick
# CopyPolicy: Released under the terms of the LGPLv2.1 or later, see LGPL.TXT

# Make sure that YARP_DIR is set in your environment to wherever you 
# compiled YARP.

cmake_minimum_required(VERSION 2.8.9)

# Start a project.
PROJECT(run_athletebot)

FIND_PACKAGE(YARP REQUIRED)
INCLUDE_DIRECTORIES(${YARP_INCLUDE_DIRS})

SET(ENABLE_athletebot TRUE CACHE BOOLEAN "we want to use athletebot")

# Search for source code.
SET(folder_source athletebot/AthleteBot.cpp adder.cpp add_athletebot.cpp yarpdev.cpp)
SET(folder_header athletebot/AthleteBot.h adder.h)
SOURCE_GROUP("Source Files" FILES ${folder_source})
SOURCE_GROUP("Header Files" FILES ${folder_header})

INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/athletebot)

# Set up our main executable.
IF (folder_source)
  ADD_EXECUTABLE(run_athletebot ${folder_source} ${folder_header})
  TARGET_LINK_LIBRARIES(run_athletebot ${YARP_LIBRARIES})
  INSTALL_TARGETS(/bin run_athletebot)
ELSE (folder_source)
  MESSAGE(FATAL_ERROR "No source code files found. Please add something")
ENDIF (folder_source)
