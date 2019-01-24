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
#include "Utilities.h"
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
    
    // Serial Number
    string serial = "";
    
    // Current tracking ID (index in the list of tracked/connected devices)
    int id = -1;
    
    // Type of device
    vr::ETrackedDeviceClass type = vr::TrackedDeviceClass_Invalid;
    // POSSIBLE TYPES:
    // TrackedDeviceClass_Invalid           0   invalid id
    // TrackedDeviceClass_HMD               1   head mounted display
    // TrackedDeviceClass_Controller        2   hand controller
    // TrackedDeviceClass_GenericTracker    3   tracker "pucks"
    // TrackedDeviceClass_TrackingReference 4   camera, base stations (lighthouses)
    // TrackedDeviceClass_DisplayRedirect   5   ...
    
    
    
    
    
    
    
    
};

#endif /* Device_hpp */
