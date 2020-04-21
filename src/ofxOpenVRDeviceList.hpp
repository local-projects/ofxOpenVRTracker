//
//  DeviceList.hpp
//  test_101
//
//  Created by Ben Snell on 1/24/19.
//

#ifndef ofxOpenVRDeviceList_hpp
#define ofxOpenVRDeviceList_hpp

#include "ofMain.h"
#include <openvr.h>
#include "ofxOpenVRDevice.hpp"
#include "ofxOpenVRUtilities.hpp"

// This class handles adding new devices into a list and provides helpers for retrieving relevant devices and their parameters.

// ETrackedDeviceProperties that have no relevance include:
// vr::ETrackedDeviceProperty::Prop_NeverTracked_Bool
// vr::ETrackedDeviceProperty::Prop_DeviceIsWireless_Bool

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

	// Get a device with a specific serial number or create it if it doesn't exist
	Device* getDevice(vr::IVRSystem* system, int trackedIndex, string serial);

	// Copies of all generic trackers ever observed are kept in here.
	vector< Device* > trackers;


	// Check if a serial number is new
	bool isNewDevice(string serial);

	// Get a device that already exists (must check to make sure it exists!)
	Device* getExistingDevice(string serial);

	// Add new device (assumes device already has serial number inside
	void addNewDevice(Device* device);
    
    // Holds the poses returned each time we look for tracked devices
    vector< vr::TrackedDevicePose_t > poses;
    
	// Update the general power, wireless, charging, firmware, etc. properties of a device
	void updateGeneralProperties(vr::IVRSystem* system, int trackedIndex, Device* device);
};

#endif /* ofxOpenVRDeviceList_hpp */
