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
            
            // Update transformation, battery %, etc.
            
            // First, get its identifiable information (serial number)
            string serial = "";
            if (!getStringProperty(system, vr::ETrackedDeviceProperty::Prop_SerialNumber_String, i, serial)) continue;
            
            // Save this serial number
            connectedDevices.insert(serial);
            
            // Get the device with this serial number
            Device* device;
            if (getDevice(serial, device)) {
                
                // New device, so add its class
                device->type = system->GetTrackedDeviceClass(i);
                
                // If it's a tracker, add it to the tracker list
                if (device->isGenericTracker()) {
                    trackers.push_back(device);
                }
            }
            
            // Set the current tracking index
            device->trackedIndex = i;
            
            // Mark that this device is connected (to what?, how?)
            device->bConnected = true;
            
            
            // Only continue if this device type is generic controller?
            
            
            // Update the battery info if we can get it
            getFloatProperty(system, vr::ETrackedDeviceProperty::Prop_DeviceBatteryPercentage_Float, i, device->batteryFraction);
            
            // Update the wireless info if we can get it
            getBoolProperty(system, vr::ETrackedDeviceProperty::Prop_DeviceIsWireless_Bool, i, device->bWireless);
            
            // Get the charging info
            getBoolProperty(system, vr::ETrackedDeviceProperty::Prop_DeviceIsCharging_Bool, i, device->bCharging);
            
            // Get "never tracked" property -- not sure what this means. Perhaps it is connected (charging or on bluetooth) but it has never been seen by the base stations?
            getBoolProperty(system, vr::ETrackedDeviceProperty::Prop_NeverTracked_Bool, i, device->bNeverTracked);
            
            getBoolProperty(system, vr::ETrackedDeviceProperty::Prop_Firmware_UpdateAvailable_Bool, i, device->bFirmwareUpdateAvailable);
            
            if (pose->bPoseIsValid) {
                
                // Mark that this device is tracking (what kind of tracking?)
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
        }
    }
    // Should there be some debouncing of when we can't see the tracker momentarily?
    
    
    // Should we also look at the activity on the device using GetTrackedDeviceActivityLevel() ?
    
    
    // We've successfully updated all devices
    return true;
}

//--------------------------------------------------------------
bool DeviceList::getDevice(string serial, Device* deviceOut) {
    
    if (serial2index.find(serial) == serial2index.end()) {
        
        // Serial number wasn't found, so this a new device.
        deviceOut = new Device();
        deviceOut->serialNumber = serial;
        
        // Add it to the vector.
        allDevices.push_back(deviceOut);
        
        // Add it to the map
        serial2index[serial] = allDevices.size()-1;
        
        // New device; return true
        return true;
        
    } else {
        
        // This is not a new device. We've seen it before. Return a pointer to it.
        deviceOut = allDevices[ serial2index[serial] ];
        
        // Old device; return false. (Note: false only means it isn't new in this context).
        return false;
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

//--------------------------------------------------------------

//--------------------------------------------------------------

//--------------------------------------------------------------

