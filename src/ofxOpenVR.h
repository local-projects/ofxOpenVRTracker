#pragma once

// A previous version of this addon made it possible to submit renderings to the head mounted display. This version strips away that functionality, focusing on the ability to track the position and orientation of the hmd, controllers and any trackers.
// ofEvents are registered for each of the types of devices. Add a listener to your application to retrieve these position/orientation updates.

// REQUIREMENTS
// Requires the OpenVR Library (tested with v1.1.3b on Windows 10)
// Requires the ofxRemoteGUI addon

// NOTES ON DESIGN & STRUCTURE
// Extends ofThread so updates from OpenVR are received at highest frequencies possible. The applications that use this addon can choose to make use of these higher framerates, or ignore this functionality and use it at the base app update frequency.

// USEFUL TERMINOLOGY
// HMD			head mounted display
// Lighthouse	base station
// Tracker		active tracker marker (the "puck")
// Controller	hand-held tracker with buttons

#include "ofMain.h"
#include <openvr.h>



//--------------------------------------------------------------
enum class ControllerRole
{
	Left = 0,
	Right = 1,
	Unknown = 3
};

//--------------------------------------------------------------
enum class EventType
{
	ButtonPress = 0,
	ButtonUnpress = 1,
	ButtonTouch = 2,
	ButtonUntouch = 3
};

//--------------------------------------------------------------
enum class ButtonType
{
	ButtonSystem = 0,
	ButtonApplicationMenu = 1,
	ButtonGrip = 2,
	ButtonTouchpad = 3,
	ButtonTrigger = 4
};

//--------------------------------------------------------------
class ofxOpenVRControllerEventArgs : public ofEventArgs
{
public:
	ControllerRole controllerRole;
	ButtonType buttonType;
	EventType eventType;
	float analogInput_xAxis;
	float analogInput_yAxis;
};

// TODO
// Add RemoteUI functionality

enum ofxOpenVRState {
	DISCONNECTED,
	CONNECTING,
	CONNECTED


};

//--------------------------------------------------------------
class ofxOpenVR : public ofThread {
public:

	// Setup OpenVR and connect to a running instance of SteamVR. 
	// This addon requires that an instance of SteamVR is running. 
	void start();

	// Check whether we're connected to SteamVR
	bool isConnected();

	// Exit 
	void stop();





	// Device ID's are integers that can range from 0 to vr::k_unMaxTrackedDeviceCount. Some of the IDs are reserved for certain devices. For example, the head-mounted display (HMD) 




	void update();;

	void drawDebugInfo(float x = 10.0f, float y = 20.0f);

	glm::mat4x4 getHMDMatrixProjectionEye(vr::Hmd_Eye nEye);
	glm::mat4x4 getHMDMatrixPoseEye(vr::Hmd_Eye nEye);
	glm::mat4x4 getCurrentViewProjectionMatrix(vr::Hmd_Eye nEye);
	glm::mat4x4 getCurrentProjectionMatrix(vr::Hmd_Eye nEye);
	glm::mat4x4 getCurrentViewMatrix(vr::Hmd_Eye nEye);

	glm::mat4x4 getControllerPose(vr::ETrackedControllerRole nController);
	bool isControllerConnected(vr::ETrackedControllerRole nController);


	ofEvent<ofxOpenVRControllerEventArgs> ofxOpenVRControllerEvent;

private:

	void threadedFunction();

	// Attempt to connect to an active (running) instance of SteamVR
	bool connectToSteamVR();
	int connectionAttemptCount = 0;
	int maxConnectionAttempts = 5;
	int nSecondsConnectionWaitTime = 5;

	// This pointer references the primary event/data handling abilities of OpenVR
	vr::IVRSystem *system;



	std::string _strTrackingSystemName;
	std::string _strTrackingSystemModelNumber;
	vr::TrackedDevicePose_t _rTrackedDevicePose[vr::k_unMaxTrackedDeviceCount];
	glm::mat4x4 _rmat4DevicePose[vr::k_unMaxTrackedDeviceCount];

	int _iTrackedControllerCount;
	int _iTrackedControllerCount_Last;
	int _iValidPoseCount;
	int _iValidPoseCount_Last;

	std::ostringstream _strPoseClassesOSS;


	glm::mat4x4 _mat4HMDPose;


	int _leftControllerDeviceID;
	int _rightControllerDeviceID;
	glm::mat4x4 _mat4LeftControllerPose;
	glm::mat4x4 _mat4RightControllerPose;

	bool _bDrawControllers;
	ofVboMesh _controllersVbo;
	ofShader _controllersTransformShader;






	void updateDevicesMatrixPose();
	void handleInput();
	void processVREvent(const vr::VREvent_t & event);


	glm::mat4x4 convertSteamVRMatrixToMatrix4(const vr::HmdMatrix34_t &matPose);

};
