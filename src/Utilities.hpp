//
//  Utilities.hpp
//  test_101
//
//  Created by Ben Snell on 1/24/19.
//

#ifndef Utilities_hpp
#define Utilities_hpp

#include "ofMain.h"
#include <openvr.h>

// Math utilities for converting values back and forth between openvr's matrices and GLM's

// Using the 3x4 matrix provided in the poses returned from openvr, determine the transforms in GLM
glm::vec3 getPosition(vr::HmdMatrix34_t matrix);
glm::quat getQuaternion(vr::HmdMatrix34_t matrix);
glm::mat4 getTransformationMatrix(vr::HmdMatrix34_t matrix);

// Get the English string description of the error returned from reading a device property
string getETrackedPropertyErrorString(int error_code);

// Get a string from a tracked device property and turn it into a std::string
string getStringTrackedDeviceProperty(vr::IVRSystem *pHmd, vr::TrackedDeviceIndex_t unDevice, vr::TrackedDeviceProperty prop, vr::TrackedPropertyError *peError = NULL)


#endif /* Utilities_hpp */
