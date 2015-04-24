========================================================================
    CONSOLE APPLICATION : FaceDetection Project Overview
========================================================================

This app uses CascadeClassifier to identify face, and Local Binary Patterns Histogram 
algorithm to predict the face, captured from VideoCapture device, from one sample for each face type.
Sample training images can be from CSV file or just an array of images.



FaceDetection.vcxproj
    This is the main project file for VC++ projects generated using an Application Wizard.
    It contains information about the version of Visual C++ that generated the file, and
    information about the platforms, configurations, and project features selected with the
    Application Wizard.

FaceDetection.vcxproj.filters
    This is the filters file for VC++ projects generated using an Application Wizard. 
    It contains information about the association between the files in your project 
    and the filters. This association is used in the IDE to show grouping of files with
    similar extensions under a specific node (for e.g. ".cpp" files are associated with the
    "Source Files" filter).

Detector.cpp
    Contains the VideoCapture object to capture frame. Also detects faces of the captured frame.

Recognizer.cpp
    Contains the face recognizer object. Train and predict images from a vector of images.
    Also reads CSV file into images and labels vectors.

main.cpp
    Some hardcoded test of face detecting and face recognizing. Also capture and save image or face
    to use for training.

/////////////////////////////////////////////////////////////////////////////
Other standard files:

StdAfx.h, StdAfx.cpp
    These files are used to build a precompiled header (PCH) file
    named FaceDetection.pch and a precompiled types file named StdAfx.obj.

/////////////////////////////////////////////////////////////////////////////
Other notes:

Configuration file may be needed to compile and run.
Follow http://opencv-srf.blogspot.com/2013/05/installing-configuring-opencv-with-vs.html for configuration.

CSV file reading or images files are hard-coded. Need to change the path of image file for it to work.
/////////////////////////////////////////////////////////////////////////////
