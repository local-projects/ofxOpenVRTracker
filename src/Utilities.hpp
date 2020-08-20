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
string getETrackedPropertyErrorString(vr::TrackedPropertyError error);

// Get a string from a tracked device property and turn it into a std::string
string getStringTrackedDeviceProperty(vr::IVRSystem *system, vr::TrackedDeviceIndex_t unDevice, vr::TrackedDeviceProperty prop, vr::TrackedPropertyError *peError = NULL);

// Get various kinds of properties with debug information. Returns true if property was retrieved successfully.
// Optionally provide a list of errors that should be ignored when printing debug information.
bool getStringProperty(vr::IVRSystem* system, vr::ETrackedDeviceProperty prop, int trackingID, string& out, set<vr::ETrackedPropertyError>* ignoreErrors = NULL);
bool getFloatProperty(vr::IVRSystem* system, vr::ETrackedDeviceProperty prop, int trackingID, float& out, set<vr::ETrackedPropertyError>* ignoreErrors = NULL);
bool getInt32Property(vr::IVRSystem* system, vr::ETrackedDeviceProperty prop, int trackingID, int& out, set<vr::ETrackedPropertyError>* ignoreErrors = NULL);
bool getBoolProperty(vr::IVRSystem* system, vr::ETrackedDeviceProperty prop, int trackingID, bool& out, set<vr::ETrackedPropertyError>* ignoreErrors = NULL);
bool getUInt64Property(vr::IVRSystem* system, vr::ETrackedDeviceProperty prop, int trackingID, uint64_t& out, set<vr::ETrackedPropertyError>* ignoreErrors = NULL);

// Get the device type as an english string
string getETrackedDeviceClassString(vr::ETrackedDeviceClass type);


#endif /* Utilities_hpp */
