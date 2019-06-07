//
//  Device.cpp
//  test_101
//
//  Created by Ben Snell on 1/23/19.
//

#include "Device.hpp"

//--------------------------------------------------------------
Device::Device() {
	firstObservationTimeMicros = ofGetElapsedTimeMicros();
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
    
    // Push an update to all listeners that new data has been received for this one device
	bool tmp = true;
	ofNotifyEvent(newDataReceived, tmp);
}

//--------------------------------------------------------------
string Device::getDebugString() {

	stringstream ss;
	ss << "serial number\t" << serialNumber << "\n";
	ss << "device type  \t" << getETrackedDeviceClassString(type) << "\n";
	ss << "connected?   \t" << bConnected << "\n";
	ss << "tracking?    \t" << bTracking << "\n";
	ss << "tracked index\t" << trackedIndex << "\n";
	ss << "timestamp    \t" << timestampMicros << "\n";
	ss << setprecision(3) << std::fixed << std::showpos;
	ss << "position     \t" << position.x << "\t" << position.y << "\t" << position.z << "\n";
	ss << "quaternion   \t" << quaternion[0] << "\t" << quaternion[1] << "\t" << quaternion[2] << "\t" << quaternion[3] << "\n";
	ss << "velocity     \t" << linearVelocity << "\n";
	ss << "ang velocity \t" << angularVelocity << "\n";
	ss << std::noshowpos << setprecision(2);
	ss << "charging?    \t" << bCharging << "\n";
	ss << "battery %    \t" << batteryFraction << "\n";
	ss << "firmwr avail?\t" << bFirmwareUpdateAvailable;

	return ss.str();
}

//--------------------------------------------------------------


//--------------------------------------------------------------



