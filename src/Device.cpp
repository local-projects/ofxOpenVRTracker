//
//  Device.cpp
//  test_101
//
//  Created by Ben Snell on 1/23/19.
//

#include "Device.hpp"

//--------------------------------------------------------------
Device::Device() {
    
}

//--------------------------------------------------------------
void Device::updateTransformation(vr::HmdMatrix34_t& _mat, uint64_t _timestampMicros) {

    // Set the new values
    mat34 = _mat;
    mat44 = getTransformationMatrix(mat34);
    position = getPosition(mat34);
    quaternion = getQuaternion(mat34);
    timestampMicros = _timestampMicros;
    
    // Should these values be added to a fixed length queue?
    
}

//--------------------------------------------------------------
bool Device::isGenericTracker() {
    return type == vr::TrackedDeviceClass_GenericTracker;
}

//--------------------------------------------------------------
bool Device::isHandheldController() {
    return type == vr::TrackedDeviceClass_Controller;
}

//--------------------------------------------------------------
bool Device::isHeadMountedDisplay() {
    return type == vr::TrackedDeviceClass_HMD;
}

//--------------------------------------------------------------
void Device::updateMotion(vr::HmdVector3_t& _linearVelocity, vr::HmdVector3_t& _angularVelocity) {
    
    linearVelocity[0] = _linearVelocity.v[0];
    linearVelocity[1] = _linearVelocity.v[1];
    linearVelocity[2] = _linearVelocity.v[2];
    
    angularVelocity[0] = _angularVelocity.v[0];
    angularVelocity[1] = _angularVelocity.v[1];
    angularVelocity[2] = _angularVelocity.v[2];
}

//--------------------------------------------------------------
void Device::markNewData() {
    
    // Should we push an update to an ofEvent that new information has been received?
    
    
}

//--------------------------------------------------------------

//--------------------------------------------------------------

//--------------------------------------------------------------



