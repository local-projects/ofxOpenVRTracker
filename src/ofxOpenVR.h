#pragma once

// A previous version of this addon made it possible to submit renderings to the head mounted display. This version strips away that functionality, focusing on the ability to track the position and orientation of the hmd, controllers and any trackers.
// ofEvents are registered for each of the types of devices. Add a listener to your application to retrieve these position/orientation updates.

// REQUIREMENTS
// Requires the OpenVR Library (tested with v1.1.3b on Windows 10)
// Requires the ofxRemoteGUI addon.

// NOTES ON DESIGN & STRUCTURE
// Extends ofThread so updates from OpenVR are received at highest frequencies possible. The applications that use this addon can choose to make use of these higher framerates, or ignore this functionality and use it at the base app update frequency.

// USEFUL TERMINOLOGY
// HMD			head mounted display
// Lighthouse	base station
// Tracker		active tracker marker (the "puck")
// Controller	hand-held tracker with buttons

#include "ofMain.h"
#include <openvr.h>
#include "Device.hpp"
#include "Utilities.h"

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



// State of this addon
enum ofxOpenVRState {
	DISCONNECTED,
	TRY_CONNECT, // actively trying to connect
	CONNECTED
};


//--------------------------------------------------------------
class ofxOpenVR : public ofThread {
public:
    
    /// \brief Create an instance of ofxOpenVR. There should only be one per program.
    ///
    ofxOpenVR();

    /// \brief Attempt to connect to a running instance of SteamVR.
    ///
    /// A running instance of SteamVR is required for this addon to work properly. Call this function once at the statrt of your program. It will attempt to connect continuously for a maxAttempts number of times, waiting waitTime milliseconds between each until connected or unsuccessful.
    ///
	void connect(int maxAttempts = 5, int waitTimeMS = 5000);

	/// \brief Check whether we're connected to SteamVR
    ///
	bool isConnected();
    
    /// \brief Disconnect from SteamVR. (Call this on exiting your application.
    ///
	void disconnect();





	// Device ID's are integers that can range from 0 to vr::k_unMaxTrackedDeviceCount. Some of the IDs are reserved for certain devices. For example, the head-mounted display (HMD) 




	void update();

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
    
    // State of the app
    ofxOpenVRState state;
    // set the state of this app
    void setState(ofxOpenVRState _state);

    // (function run in the thread)
	void threadedFunction();

	// Attempt to connect once to an active (running) instance of SteamVR
	bool connectToSteamVR();
    // Connection parameters
	int connectionAttemptsCounter = 0;
	int connectionAttemptsMax;
	int connectionAttemptWaitTime; // milliseconds
    int64_t lastConnectionAttemptTimeMS;

	// This pointer references the primary event/data handling abilities of OpenVR
    vr::IVRSystem *system = NULL;

    // Get all information associated with connected devices
    void getDeviceInfo();
    // Pose array into which device information is initially loaded
    void getAllPoses(vr::TrackedDevicePose_t* _poses, int arrayLength);
    
    vr::TrackedDevicePose_t devicePoses[vr::k_unMaxTrackedDeviceCount];
    
    
    
    


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
