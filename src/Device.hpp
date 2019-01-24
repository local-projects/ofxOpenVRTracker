//
//  Device.hpp
//  test_101
//
//  Created by Ben Snell on 1/23/19.
//

#ifndef Device_hpp
#define Device_hpp

#include "ofMain.h"
#include <openvr.h>
#include "Utilities.hpp"
//
//// These types are the same as ETrackedDeviceClass
//enum DeviceType {
//    INVALID,            // device doesn't exist
//    HMD,                // head mounted display
//    CONTROLLER,         // hand controller
//    GENERIC_TRACKER,    // tracker puck
//    TRACKING_REFERENCE, // base station (lighthouse)
//    DISPLAY_REDIRECT    //
//}

class Device {
public:
    
    // Current tracking ID (index in the list of tracked/connected devices)
    int trackedIndex = -1;
    
    // Type of device
    vr::ETrackedDeviceClass type = vr::TrackedDeviceClass_Invalid;
    // POSSIBLE TYPES:
    // TrackedDeviceClass_Invalid           0   invalid id
    // TrackedDeviceClass_HMD               1   head mounted display
    // TrackedDeviceClass_Controller        2   hand controller
    // TrackedDeviceClass_GenericTracker    3   tracker "pucks"
    // TrackedDeviceClass_TrackingReference 4   camera, base stations (lighthouses)
    // TrackedDeviceClass_DisplayRedirect   5   ...
    
    // Serial Model of Device
	string serialNumber = "";
    
    // Power Parameters
	bool bWireless = false;
	bool bCharging = false;
	float batteryFraction = 0;	// 0 for empty, 1 for full

	// Firmware Parameters
	bool bFirmwareAvailable = false;

	// Transformation Parameters
	vr::HmdMatrix34_t mat34;	// the original pose (not very useful by itself)
	glm::mat4 mat44;			// transformation matrix
	glm::vec3 position;			// position
	glm::quat quaternion;		// orientation


    
    
    
    
};

#endif /* Device_hpp */
