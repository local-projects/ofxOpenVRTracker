#pragma once

// A previous version of this addon made it possible to submit renderings to the head mounted display. This version strips away that functionality, focusing on the ability to track the position and orientation of the hmd, controllers and any trackers.
// ofEvents are registered for each of the types of devices. Add a listener to your application to retrieve these position/orientation updates.

#include "ofMain.h"
#include <openvr.h>

//--------------------------------------------------------------
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

//--------------------------------------------------------------
//--------------------------------------------------------------
class ofxOpenVR {

public:
	void setup();
	void exit();

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

	// The primary openVR system
	vr::IVRSystem *system;

	// Attempt to connect to an active (running) instance of SteamVR
	bool connectToSteamVR();


	void updateDevicesMatrixPose();
	void handleInput();
	void processVREvent(const vr::VREvent_t & event);


	glm::mat4x4 convertSteamVRMatrixToMatrix4(const vr::HmdMatrix34_t &matPose);

};
