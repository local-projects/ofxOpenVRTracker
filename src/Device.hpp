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

class Device {
public:
    
    Device();
    
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
    
    /// \brief Check if this device is a generic tracker "puck".
    ///
    bool isGenericTracker();
    
    /// \brief Check if this device is a handheld controller (with buttons).
    ///
    bool isHandheldController();
    
    /// \brief Check if this device is a head-mounted display (HMD).
    ///
    bool isHeadMountedDisplay();
    
    // Serial Model of Device
	string serialNumber = "";
    
    
	/// \brief Update all transformation parameters, including transformation matrix, position, and quaternion.
    ///
    void updateTransformation(vr::HmdMatrix34_t& _mat, uint64_t _timestampMicros);
    
    // Transformation Parameters
    vr::HmdMatrix34_t mat34;    // the original pose (not very useful by itself)
    glm::mat4 mat44;            // transformation matrix
    glm::vec3 position;            // position
    glm::quat quaternion;        // orientation
    uint64_t timestampMicros;   // time at which these parameters were captured (in microseconds)
    
    // Should these be stored as an ofNode?
    
    
    /// \brief Update the motion parameters.
    ///
    /// These parameters were calculated by SteamVR.
    void updateMotion(vr::HmdVector3_t& _linearVelocity, vr::HmdVector3_t& _angularVelocity);
    glm::vec3 linearVelocity;
    glm::vec3 angularVelocity;
    
    
    // Power Parameters
    bool bWireless = false;
    bool bCharging = false;
    float batteryFraction = 0;    // 0 for empty, 1 for full
    
    /// \brief Mark that a new set of data has been set.
    ///
    void markNewData();
    
    
    
    // Create a method for adding a piece of information to a queue (for mats, velocities, etc.)
    
    
    
    
    // Is this device currently connected (Q: to power or bluetooth?)
    bool bConnected = false;
    
    // Is this device currently being tracked? (Q: connected or tracking?)
    bool bTracking = false;
    
    // Not sure what this is
    bool bNeverTracked = true;
    
    // Firmware Parameters
    bool bFirmwareUpdateAvailable = false;
    
    
    
    
};

#endif /* Device_hpp */
