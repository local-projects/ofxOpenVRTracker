//
//  DeviceList.hpp
//  test_101
//
//  Created by Ben Snell on 1/24/19.
//

#ifndef DeviceList_hpp
#define DeviceList_hpp

#include "ofMain.h"
#include <openvr.h>
#include "Device.hpp"
#include "Utilities.hpp"

// This class handles adding new devices into a list and provides helpers for retrieving relevant devices and their parameters.

class DeviceList {
public:
    
    DeviceList();
    
    /// \brief Update all devices' information. Pass the IVRSystem pointer here.
    ///
    bool update(vr::IVRSystem *system);
    
    /// \brief Get a pointer to a vector with pointers to all observed devices.
    ///
    vector< Device* >* getDevices();
    
    /// \brief Returns a vector with pointers to all observed generic tracker devices.
    ///
    vector< Device* >* getTrackers();
    
    
private:
    
    // List of all devices which have ever been observed
    vector< Device* > allDevices;
    // mapping from a device's serial number to its index in this vector
    map< string, int > serial2index;
    
    // Get the device with the corresponding serial number, or create one if it doesn't exist. Returns true if it is a new device and false if it is one we've seen before.
    bool getDevice(string serial, Device* deviceOut);
    
    // Copies of all generic trackers ever observed are kept in here.
    vector< Device* > trackers;
    
    // Holds the poses returned each time we look for tracked devices
    vector< vr::TrackedDevicePose_t > poses;
    
};

#endif /* DeviceList_hpp */
