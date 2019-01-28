//
//  DeviceList.cpp
//  test_101
//
//  Created by Ben Snell on 1/24/19.
//

#include "DeviceList.hpp"

//--------------------------------------------------------------
DeviceList::DeviceList() {
    
    // Create enough space for all devices to be tracked
    poses.resize(vr::k_unMaxTrackedDeviceCount);
}

//--------------------------------------------------------------
bool DeviceList::update(vr::IVRSystem* system) {

    if (system == NULL) return false;
    
    // First, get all devices that are being tracked
    system->GetDeviceToAbsoluteTrackingPose(
            vr::ETrackingUniverseOrigin::TrackingUniverseStanding,
            0,
            &(poses[0]),
            vr::k_unMaxTrackedDeviceCount);
    uint64_t thisTimeMicros = ofGetElapsedTimeMicros();
    
    // Set of all connected (tracking or not tracking) devices stores their string for reference later.
    set<string> connectedDevices;
    
    // Iterate through all possible devices to get their info.
    for (int i = 0; i < poses.size(); i++) {
        
        // Save this pose for easier access
        vr::TrackedDevicePose_t* pose = &(poses[i]);
        
        // If a device is connected to power, update its information
        if (pose->bDeviceIsConnected) {
                        
            // First, get its identifiable information (serial number)
            string serial = "";
            if (!getStringProperty(system, vr::ETrackedDeviceProperty::Prop_SerialNumber_String, i, serial)) continue;
            
            // Save this serial number
            connectedDevices.insert(serial);
            
            // Get the device with this serial number
			Device* device = getDevice(system, i, serial);
            
            // Set the current tracking index
            device->trackedIndex = i;
            
            // Mark that this device is connected to bluetooth
            device->bConnected = true;
                        
            // Update the power, firmware and misc properties of this device
			updateGeneralProperties(system, i, device);
            
            if (pose->bPoseIsValid) {
                
                // Mark that this device is actively tracking (location is being updated)
                device->bTracking = true;
                
                // Pose is valid. Device is still being tracked. Update the transformation information.
                device->updateTransformation(pose->mDeviceToAbsoluteTracking, thisTimeMicros);
                
                // Should we check if this pose is new (i.e. it contains new information?) Or does each retrieval of information provide new information?
                
                // Update motion parameters
                device->updateMotion(pose->vVelocity, pose->vAngularVelocity);
                
                // Mark that we have received new data
                device->markNewData();
                
            } else {
                
                // Pose isn't valid. Device is not being tracked.
                
                if (device->bTracking) {
                    // Device has just switched from tracking to not tracking
                }
                
                // Save that this isn't tracking
                device->bTracking = false;
				device->trackedIndex = -1;
            }
            
        } else {
            
            // Device is no longer connected to power. (We aren't able to query its information anymore?) (Do we know its serial number?)
            
        }
    }
    
    // For all devices that aren't "connected", update their tracking / connection bools
    for (int i = 0; i < allDevices.size(); i++) {
        
        if (connectedDevices.find(allDevices[i]->serialNumber) == connectedDevices.end()) {
            
            // We can't find the serial number in the last list of devices, so this device is not connected. Update its parameters.
            
            if (allDevices[i]->bConnected) {
                // Device has just switched from connected to not connected.
            }
            
            if (allDevices[i]->bTracking) {
                // Device has just switched from tracking to not tracking.
            }
            
            // Update the parameters
            allDevices[i]->bConnected = false;
            allDevices[i]->bTracking = false;
			allDevices[i]->trackedIndex = -1;
        }
    }
    // Should there be some debouncing of when we can't see the tracker momentarily?
    
    
    // Should we also look at the activity on the device using GetTrackedDeviceActivityLevel() ?
    
    
    // We've successfully updated all devices
    return true;
}

//--------------------------------------------------------------
Device* DeviceList::getDevice(vr::IVRSystem* system, int trackedIndex, string serial) {

	Device* device;
	if (isNewDevice(serial)) {
		// allocate space
		device = new Device();
		// Set its serial #
		device->serialNumber = serial;
		// set its type
		device->type = system->GetTrackedDeviceClass(trackedIndex);
		// Add this device to the list
		addNewDevice(device);
	}
	else {
		// Retrive the device with this serial number
		device = getExistingDevice(serial);
	}
	return device;
}

//--------------------------------------------------------------
bool DeviceList::isNewDevice(string serial) {

	return (serial2index.find(serial) == serial2index.end());
}

//--------------------------------------------------------------
Device* DeviceList::getExistingDevice(string serial) {

	// Return a pointer to the device
	return allDevices[serial2index[serial]];
}

//--------------------------------------------------------------
void DeviceList::addNewDevice(Device* device) {

	// Add this new device to the vector
	allDevices.push_back(device);

	// Add this device to the map
	serial2index[device->serialNumber] = allDevices.size() - 1;

	// add the device to the list of tracker if it is a tracker
	if (device->isGenericTracker()) {
		trackers.push_back(device);
	}
}
                
//--------------------------------------------------------------
vector< Device* >* DeviceList::getDevices() {
    
    return &allDevices;
}

//--------------------------------------------------------------
vector< Device* >* DeviceList::getTrackers() {
    
    return &trackers;
}

//--------------------------------------------------------------
void DeviceList::updateGeneralProperties(vr::IVRSystem* system, int trackedIndex, Device* device) {

	// Different properties apply to different kinds of devices.
	// Minimize the amount of errors by only querying the correct properties for each device type.
	switch (device->type) {
	case 0: break;
	case 1: {		// hmd


	}; break;
	case 2: {		// controller

		getFloatProperty(system, vr::ETrackedDeviceProperty::Prop_DeviceBatteryPercentage_Float, trackedIndex, device->batteryFraction);

		getBoolProperty(system, vr::ETrackedDeviceProperty::Prop_DeviceIsCharging_Bool, trackedIndex, device->bCharging);

		getBoolProperty(system, vr::ETrackedDeviceProperty::Prop_Firmware_UpdateAvailable_Bool, trackedIndex, device->bFirmwareUpdateAvailable);


	}; break;
	case 3: {		// tracker

		// Update the battery info if we can get it
		getFloatProperty(system, vr::ETrackedDeviceProperty::Prop_DeviceBatteryPercentage_Float, trackedIndex, device->batteryFraction);

		// Get the charging info
		getBoolProperty(system, vr::ETrackedDeviceProperty::Prop_DeviceIsCharging_Bool, trackedIndex, device->bCharging);

		// Check if a firmware update is available
		getBoolProperty(system, vr::ETrackedDeviceProperty::Prop_Firmware_UpdateAvailable_Bool, trackedIndex, device->bFirmwareUpdateAvailable);

	}; break;
	case 4: {		// base station

		getBoolProperty(system, vr::ETrackedDeviceProperty::Prop_Firmware_UpdateAvailable_Bool, trackedIndex, device->bFirmwareUpdateAvailable);

	};  break;
	case 5: break;	// display redirect
	default: break;
	}
}

//--------------------------------------------------------------

//--------------------------------------------------------------

//--------------------------------------------------------------

