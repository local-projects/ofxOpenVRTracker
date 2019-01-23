#include "ofxOpenVR.h"

//--------------------------------------------------------------
// Purpose: Helper to get a string from a tracked device 
//          property and turn it into a std::string
//--------------------------------------------------------------
std::string getTrackedDeviceString(vr::IVRSystem *pHmd, vr::TrackedDeviceIndex_t unDevice, vr::TrackedDeviceProperty prop, vr::TrackedPropertyError *peError = NULL)
{
	uint32_t unRequiredBufferLen = pHmd->GetStringTrackedDeviceProperty(unDevice, prop, NULL, 0, peError);
	if (unRequiredBufferLen == 0)
		return "";

	char *pchBuffer = new char[unRequiredBufferLen];
	unRequiredBufferLen = pHmd->GetStringTrackedDeviceProperty(unDevice, prop, pchBuffer, unRequiredBufferLen, peError);
	std::string sResult = pchBuffer;
	delete[] pchBuffer;
	return sResult;
}

//--------------------------------------------------------------
ofxOpenVR::ofxOpenVR() {
    
    setState(DISCONNECTED);
}

//--------------------------------------------------------------
void ofxOpenVR::connect(int maxAttempts, int waitTimeMS) {
    
    // Store these parameters
    connectionAttemptsMax = maxAttempts;
    connectionAttemptWaitTime = waitTimeMS;
    lastConnectionAttemptTimeMS = -connectionAttemptWaitTime;
    
    // Reset the counter
    // Should this be here or below this "if" block?
    connectionAttemptsCounter = 0;
    
    // Check to see if we really need to connect.
    if (isConnected()) {
        
        ofLogNotice() << "OpenVR is already connected.";
        return;
        
    } else if (isThreadRunning()) {
        
        ofLogNotice() << "OpenVR is still trying to connect. There's no need to call connect() again.";
        return;
	}

    // change the state of this addon
    setState(TRY_CONNECT);
    
	// Begin the thread and begin trying to connect
    startThread();
}

//--------------------------------------------------------------
void ofxOpenVR::threadedFunction() {

	while (isThreadRunning()) {

        switch (state) {
                
            case DISCONNECTED: {
                
                // If the addon is disconnected from the SteamVR server, then this thread need not be running
                stopThread();
                
            } break;
                
            case TRY_CONNECT: {
                
                // If the app is actively trying to connect...
                // If we haven't connected too many times...
                if (connectionAttemptsCounter <= connectionAttemptsMax) {
                    
                    // Check if we're ready to try connecting again
                    int64_t thisTime = ofGetElapsedTimeMillis();
                    if (thisTime - lastConnectionAttemptTimeMS >= connectionAttemptWaitTime) {
                        
                        // increment the counter
                        connectionAttemptsCounter++;
                        
                        // Attempt to Connect
                        bool bConnected = connectToSteamVR();
                        
                        // Save this connection time
                        lastConnectionAttemptTimeMS = thisTime;
                        
                        // If we connected, then switch the state
                        if (bConnected) setState(CONNECTED);
                    }
                } else {
                    // We've surpassed our allotted number of attempts. Stop trying to connect.
                    setState(DISCONNECTED);
                }
            } break;
                
            case CONNECTED: {
                
                // Check to make sure we're still connected
                // Should this also ping the SteamVR Server?
                if (system == NULL) {
                    
                    // change the state
                    setState(DISCONNECTED);
                    // try to reconnect
                    connect();
                    
                } else {
                    
                    // ---- THIS IS, ESSENTIALLY, THE UPDATE LOOP ----
                    
                    // Get all connected devices and their properties (like position, orientation, battery status, connected, tracking)
                    getDeviceInfo();
                    
                    
                    // Check for new events (like button presses, status changes in the system, etc.)
                    
                    
                    
                    
                    
                    
                }
                
            } break;
                
            default:
                break;
        }
        
        


	}
}

//--------------------------------------------------------------
bool ofxOpenVR::connectToSteamVR()
{
	// Loading the SteamVR Runtime
	ofLogNotice() << "Attempting to connect to SteamVR...";

	// Set the error to be returned to the no error value
	vr::EVRInitError initError = vr::VRInitError_None;

	// The IVRSystem is the primary handler for our interaction with openVR. Here, we will initialize it. We need to tell it what kind of applicatio this is, using an EVRApplicationType. Types are documented in openvr.h. Here, we will use the type:
	vr::EVRApplicationType applicationType = vr::EVRApplicationType::VRApplication_Other;

	// Initialize the system with the error reference and the type of application.
	system = vr::VR_Init(&initError, applicationType);

	// Check to see if the init was successful
	if (initError != vr::VRInitError_None) {
        
		// We didn't connect properly
		system = NULL;
		string error = vr::VR_GetVRInitErrorAsEnglishDescription(initError);
		ofLogError() << "Couldn't connect to SteamVR: " + error;
		return false;
        
    } else if (system == NULL) {
        
        // What happens when SteamVR stops before this app does? Does system remain NULL?
        
        // If for some strange reason system is still null, init was not successful
        ofLogError() << "IVRSystem should not be NULL here! Init was not successful even though there were no errors returned.";
        return false;
        
    } else {
        
		// We connected!
        ofLogNotice() << "Successfully connected to SteamVR.";
        
	}
	return true;
}

//--------------------------------------------------------------
bool ofxOpenVR::isConnected() {
    
    return state == CONNECTED;
}

//--------------------------------------------------------------
void ofxOpenVR::disconnect()
{
	if (system || isConnected()) {
        
        // Set the new state
        setState(DISCONNECTED);
        
        // Shutdown the connection
		vr::VR_Shutdown();
        
        // Reset the system pointer
		system = NULL;
	}
}

//--------------------------------------------------------------
void ofxOpenVR::getDeviceInfo() {

    // Question: Should I just iterate through all possible device ID's? There are only 16 possible ones. Then, we would know when devices are tracked (but might still be connected), and we could monitor all battery info, etc.
    
    // First, iterate through all tracked devices
    system->GetDeviceToAbsoluteTrackingPose(vr::ETrackingUniverseOrigin::TrackingUniverseStanding, 0, devicePoses, vr::k_unMaxTrackedDeviceCount);
    
    
    
}

//--------------------------------------------------------------





//--------------------------------------------------------------
void ofxOpenVR::update()
{
	// for now as fast as possible
	if (system)
	{
		handleInput();
	}

	// Spew out the controller and pose count whenever they change.
	if (_iTrackedControllerCount != _iTrackedControllerCount_Last || _iValidPoseCount != _iValidPoseCount_Last)
	{
		_iValidPoseCount_Last = _iValidPoseCount;
		_iTrackedControllerCount_Last = _iTrackedControllerCount;

		//printf("PoseCount:%d(%s) Controllers:%d\n", _iValidPoseCount, _strPoseClasses.c_str(), _iTrackedControllerCount);
		printf("PoseCount:%d Controllers:%d\n", _iValidPoseCount, _iTrackedControllerCount);
	}

	updateDevicesMatrixPose();
}

//--------------------------------------------------------------
glm::mat4x4 ofxOpenVR::getHMDMatrixProjectionEye(vr::Hmd_Eye nEye)
{
	if (!system)
		return glm::mat4x4();

	vr::HmdMatrix44_t mat = system->GetProjectionMatrix(nEye, 0.1f, 30.0f);

	return glm::mat4x4(
		mat.m[0][0], mat.m[1][0], mat.m[2][0], mat.m[3][0],
		mat.m[0][1], mat.m[1][1], mat.m[2][1], mat.m[3][1],
		mat.m[0][2], mat.m[1][2], mat.m[2][2], mat.m[3][2],
		mat.m[0][3], mat.m[1][3], mat.m[2][3], mat.m[3][3]
	);
}

//--------------------------------------------------------------
glm::mat4x4 ofxOpenVR::getHMDMatrixPoseEye(vr::Hmd_Eye nEye)
{
	if (!system)
		return glm::mat4x4();

	vr::HmdMatrix34_t matEyeRight = system->GetEyeToHeadTransform(nEye);
	glm::mat4x4 matrixObj(
		matEyeRight.m[0][0], matEyeRight.m[1][0], matEyeRight.m[2][0], 0.0,
		matEyeRight.m[0][1], matEyeRight.m[1][1], matEyeRight.m[2][1], 0.0,
		matEyeRight.m[0][2], matEyeRight.m[1][2], matEyeRight.m[2][2], 0.0,
		matEyeRight.m[0][3], matEyeRight.m[1][3], matEyeRight.m[2][3], 1.0f
	);

	return glm::inverse(matrixObj);
}

//--------------------------------------------------------------
glm::mat4x4 ofxOpenVR::getCurrentViewProjectionMatrix(vr::Hmd_Eye nEye)
{
	glm::mat4x4 matMVP;
	if (nEye == vr::Eye_Left)
	{
		matMVP = getHMDMatrixProjectionEye(vr::Eye_Left) * getHMDMatrixPoseEye(vr::Eye_Left) * _mat4HMDPose;
	}
	else if (nEye == vr::Eye_Right)
	{
		matMVP = getHMDMatrixProjectionEye(vr::Eye_Right) * getHMDMatrixPoseEye(vr::Eye_Right) *  _mat4HMDPose;
	}

	return matMVP;
}

//--------------------------------------------------------------
glm::mat4x4 ofxOpenVR::getCurrentProjectionMatrix(vr::Hmd_Eye nEye)
{
	glm::mat4x4 matP;
	if (nEye == vr::Eye_Left)
	{
		matP = getHMDMatrixProjectionEye(vr::Eye_Left);
	}
	else if (nEye == vr::Eye_Right)
	{
		matP = getHMDMatrixProjectionEye(vr::Eye_Right);
	}

	return matP;
}

//--------------------------------------------------------------
glm::mat4x4 ofxOpenVR::getCurrentViewMatrix(vr::Hmd_Eye nEye)
{
	glm::mat4x4 matV;
	if (nEye == vr::Eye_Left)
	{
		matV = getHMDMatrixPoseEye(vr::Eye_Left) * _mat4HMDPose;
	}
	else if (nEye == vr::Eye_Right)
	{
		matV = getHMDMatrixPoseEye(vr::Eye_Right) *  _mat4HMDPose;
	}

	return matV;
}

//--------------------------------------------------------------
glm::mat4x4 ofxOpenVR::getControllerPose(vr::ETrackedControllerRole nController)
{
	glm::mat4x4 matrix;

	if (nController == vr::TrackedControllerRole_LeftHand) {
		matrix = _mat4LeftControllerPose;
	}
	else if (nController == vr::TrackedControllerRole_RightHand) {
		matrix = _mat4RightControllerPose;
	}

	return matrix;
}

//--------------------------------------------------------------
bool ofxOpenVR::isControllerConnected(vr::ETrackedControllerRole nController)
{
	if (system) {
		if (_iTrackedControllerCount > 0) {
			if (nController == vr::TrackedControllerRole_LeftHand) {
				return system->IsTrackedDeviceConnected(_leftControllerDeviceID);
			}
			else if (nController == vr::TrackedControllerRole_RightHand) {
				return system->IsTrackedDeviceConnected(_rightControllerDeviceID);
			}
		}
	}

	return false;
}

//--------------------------------------------------------------
void ofxOpenVR::updateDevicesMatrixPose()
{
	if (!system) {
		ofLogNotice() << "There is no pHMD, so we cannot update device poses";
		return;
	}
	
	// Reset some vars.
	_iValidPoseCount = 0;
	_iTrackedControllerCount = 0;
	_leftControllerDeviceID = -1;
	_rightControllerDeviceID = -1;

	_strPoseClassesOSS.str("");
	_strPoseClassesOSS.clear();
	_strPoseClassesOSS << "FPS " << ofToString(ofGetFrameRate()) << endl;
	_strPoseClassesOSS << "Frame #" << ofToString(ofGetFrameNum()) << endl;
	_strPoseClassesOSS << endl;
	_strPoseClassesOSS << "Connected Device(s): " << endl;

	// Retrieve all tracked devices' matrix/pose.
	// This cannot be called here.... another function must be called 
	vr::VRCompositor()->WaitGetPoses(_rTrackedDevicePose, vr::k_unMaxTrackedDeviceCount, NULL, 0);


	// Go through all the tracked devices.
	for (int nDevice = 0; nDevice < vr::k_unMaxTrackedDeviceCount; ++nDevice)
	{
		if (_rTrackedDevicePose[nDevice].bPoseIsValid)
		{
			_iValidPoseCount++;

			// Keep all valid matrices.
			_rmat4DevicePose[nDevice] = convertSteamVRMatrixToMatrix4(_rTrackedDevicePose[nDevice].mDeviceToAbsoluteTracking);

			// Add info to the debug panel.
			switch (system->GetTrackedDeviceClass(nDevice))
			{
				case vr::TrackedDeviceClass_Controller:
					if (system->GetControllerRoleForTrackedDeviceIndex(nDevice) == vr::TrackedControllerRole_LeftHand) {
						_strPoseClassesOSS << "Controller Left" << endl;
					}
					else if (system->GetControllerRoleForTrackedDeviceIndex(nDevice) == vr::TrackedControllerRole_RightHand) {
						_strPoseClassesOSS << "Controller Right" << endl;
					}
					else {
						_strPoseClassesOSS << "Controller" << endl;
					}
					break;

				case vr::TrackedDeviceClass_HMD:
					_strPoseClassesOSS << "HMD" << endl;
					break;

				case vr::TrackedDeviceClass_Invalid:
					_strPoseClassesOSS << "Invalid Device Class" << endl;
					break;

				case vr::TrackedDeviceClass_GenericTracker:
					_strPoseClassesOSS << "Generic trackers, similar to controllers" << endl;
					break;

				case vr::TrackedDeviceClass_TrackingReference:
					_strPoseClassesOSS << "Tracking Reference - Camera" << endl;
					break;

				default:
					_strPoseClassesOSS << "Unknown Device Class" << endl;
					break;
			}

			// Store controllers' ID and matrix. 
			if (system->GetTrackedDeviceClass(nDevice) == vr::TrackedDeviceClass_Controller) {
				_iTrackedControllerCount += 1;

				if (system->GetControllerRoleForTrackedDeviceIndex(nDevice) == vr::TrackedControllerRole_LeftHand) {
					_leftControllerDeviceID = nDevice;
					_mat4LeftControllerPose = _rmat4DevicePose[nDevice];
				}
				else if (system->GetControllerRoleForTrackedDeviceIndex(nDevice) == vr::TrackedControllerRole_RightHand) {
					_rightControllerDeviceID = nDevice;
					_mat4RightControllerPose = _rmat4DevicePose[nDevice];
				}
			}
		}
	}

	// Store HDM's matrix
	if (_rTrackedDevicePose[vr::k_unTrackedDeviceIndex_Hmd].bPoseIsValid)
	{
		_mat4HMDPose = glm::inverse(_rmat4DevicePose[vr::k_unTrackedDeviceIndex_Hmd]);
	}

	// Aad more info to the debug panel.
	_strPoseClassesOSS << endl;
	_strPoseClassesOSS << "Pose Count: " << _iValidPoseCount << endl;
    _strPoseClassesOSS << "Controller Count: " << _iTrackedControllerCount << endl;

}

//--------------------------------------------------------------
void ofxOpenVR::handleInput()
{
	// Process SteamVR events
	vr::VREvent_t event;
	while (system->PollNextEvent(&event, sizeof(event)))
	{
		ofLogVerbose() << "Even has been received";
		processVREvent(event);
	}
}


//--------------------------------------------------------------
// Purpose: Processes a single VR event
//--------------------------------------------------------------
void ofxOpenVR::processVREvent(const vr::VREvent_t & event)
{	
	// Check device's class.
	switch (system->GetTrackedDeviceClass(event.trackedDeviceIndex))
	{
		case vr::TrackedDeviceClass_Controller:
			ofxOpenVRControllerEventArgs _args;

			// Controller's role.
			if (system->GetControllerRoleForTrackedDeviceIndex(event.trackedDeviceIndex) == vr::TrackedControllerRole_LeftHand) {
				_args.controllerRole = ControllerRole::Left;
			}
			else if (system->GetControllerRoleForTrackedDeviceIndex(event.trackedDeviceIndex) == vr::TrackedControllerRole_RightHand) {
				_args.controllerRole = ControllerRole::Right;
			}
			else {
				_args.controllerRole = ControllerRole::Unknown;
			}

			// Get extra data about the controller.
			vr::VRControllerState_t pControllerState;
			vr::VRSystem()->GetControllerState(event.trackedDeviceIndex, &pControllerState, sizeof(pControllerState));

			_args.analogInput_xAxis = -1;
			_args.analogInput_yAxis = -1;

			// Button type
			switch (event.data.controller.button) {
				case vr::k_EButton_System:
				{
					_args.buttonType = ButtonType::ButtonSystem;
				}
				break;

				case vr::k_EButton_ApplicationMenu:
				{
					_args.buttonType = ButtonType::ButtonApplicationMenu;
				}
				break;

				case vr::k_EButton_Grip:
				{
					_args.buttonType = ButtonType::ButtonGrip;
				}
				break;

				case vr::k_EButton_SteamVR_Touchpad:
				{
					_args.buttonType = ButtonType::ButtonTouchpad;

					_args.analogInput_xAxis = pControllerState.rAxis->x;
					_args.analogInput_yAxis = pControllerState.rAxis->y;
				}
				break;

				case vr::k_EButton_SteamVR_Trigger:
				{
					_args.buttonType = ButtonType::ButtonTrigger;
				}
				break;
			}

			// Check event's type.
			switch (event.eventType) {
				case vr::VREvent_ButtonPress:
				{
					_args.eventType = EventType::ButtonPress;
				}
				break;

				case vr::VREvent_ButtonUnpress:
				{
					_args.eventType = EventType::ButtonUnpress;
				}
				break;

				case vr::VREvent_ButtonTouch:
				{
					_args.eventType = EventType::ButtonTouch;
				}
				break;

				case vr::VREvent_ButtonUntouch:
				{
					_args.eventType = EventType::ButtonUntouch;
				}
				break;
			}

			ofNotifyEvent(ofxOpenVRControllerEvent, _args);
			break;

		case vr::TrackedDeviceClass_HMD:
			break;

		case vr::TrackedDeviceClass_Invalid:
			break;

		case vr::TrackedDeviceClass_GenericTracker:
			break;

		case vr::TrackedDeviceClass_TrackingReference:
			break;

		default:
			break;
	}
	
	// Check event's type.
	switch (event.eventType) {
		case vr::VREvent_TrackedDeviceActivated:
		{
			printf("Device %u attached. Setting up render model.\n", event.trackedDeviceIndex);
		}
		break;

		case vr::VREvent_TrackedDeviceDeactivated:
		{
			printf("Device %u detached.\n", event.trackedDeviceIndex);
		}
		break;	

		case vr::VREvent_TrackedDeviceUpdated:
		{
			printf("Device %u updated.\n", event.trackedDeviceIndex);
		}
		break;
	}			
}



//--------------------------------------------------------------
void ofxOpenVR::drawDebugInfo(float x, float y)
{
	_strPoseClassesOSS << endl;
	_strPoseClassesOSS << "System Name: " << _strTrackingSystemName << endl;
	_strPoseClassesOSS << "System S/N: " << _strTrackingSystemModelNumber << endl;

	ofDrawBitmapStringHighlight(_strPoseClassesOSS.str(), ofPoint(x, y), ofColor(ofColor::black, 100.0f));
}

//--------------------------------------------------------------
glm::mat4x4 ofxOpenVR::convertSteamVRMatrixToMatrix4(const vr::HmdMatrix34_t &matPose)
{
	glm::mat4x4 matrixObj(
		matPose.m[0][0], matPose.m[1][0], matPose.m[2][0], 0.0,
		matPose.m[0][1], matPose.m[1][1], matPose.m[2][1], 0.0,
		matPose.m[0][2], matPose.m[1][2], matPose.m[2][2], 0.0,
		matPose.m[0][3], matPose.m[1][3], matPose.m[2][3], 1.0f
	);
	return matrixObj;
}



// Example Code to draw controllers 
// Useful for understanding matrix conversions, property extraction
//if (system->IsTrackedDeviceConnected(_leftControllerDeviceID)) {
//	glm::vec4 center = _mat4LeftControllerPose * glm::vec4(0, 0, 0, 1);
//
//	for (int i = 0; i < 3; ++i)
//	{
//		glm::vec3 color(0, 0, 0);
//		glm::vec4 point(0, 0, 0, 1);
//		point[i] += 0.05f;  // offset in X, Y, Z
//		color[i] = 1.0;  // R, G, B
//		point = _mat4LeftControllerPose * point;
//
//		_controllersVbo.addVertex(ofVec3f(center.x, center.y, center.z));
//		_controllersVbo.addColor(ofFloatColor(color.x, color.y, color.z));
//
//		_controllersVbo.addVertex(ofVec3f(point.x, point.y, point.z));
//		_controllersVbo.addColor(ofFloatColor(color.x, color.y, color.z));
//	}
//}


// Rendering Example Code
//for (uint32_t unTrackedDevice = 0; unTrackedDevice < vr::k_unMaxTrackedDeviceCount; unTrackedDevice++) {
//	if (!_rTrackedDeviceToRenderModel[unTrackedDevice])
//		continue;
//
//	const vr::TrackedDevicePose_t & pose = _rTrackedDevicePose[unTrackedDevice];
//	if (!pose.bPoseIsValid) {
//		continue;
//	}
//
//
//	glm::mat4x4 matMVP = getCurrentViewProjectionMatrix(nEye) * _rmat4DevicePose[unTrackedDevice];
//
//	_renderModelsShader.setUniformMatrix4f("matrix", matMVP, 1);
//	_rTrackedDeviceToRenderModel[unTrackedDevice]->Draw();
//}

