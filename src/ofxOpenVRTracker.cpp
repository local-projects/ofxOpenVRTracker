#include "ofxOpenVRTracker.h"

//--------------------------------------------------------------
ofxOpenVRTracker::ofxOpenVRTracker() {
    
    setState(DISCONNECTED);
}

//--------------------------------------------------------------
void ofxOpenVRTracker::connect(int maxAttempts, int waitTimeMS) {
    
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
void ofxOpenVRTracker::threadedFunction() {

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

						ofLogNotice() << "bConnected? " + ofToString(bConnected);
                        
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
                    devices.update(system);
                    
                    // Check for new events (like button presses, status changes in the system, etc.)
                    // Integrate this information with that already received. Q: Should this focus on info relating to the system's state since device info has already been received?
                    
                    
                    
                    // Flag that we have received new information using an ofEvent
                    ofxOpenVRTrackerEventArgs _args;
                    _args.devices = &devices;
                    ofNotifyEvent(newDataReceived, _args);
                }
            } break;
                
            default:
                break;
        }
	}
}

//--------------------------------------------------------------
bool ofxOpenVRTracker::connectToSteamVR()
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
bool ofxOpenVRTracker::isConnected() {
    
    return state == CONNECTED;
}

//--------------------------------------------------------------
void ofxOpenVRTracker::disconnect()
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
void ofxOpenVRTracker::setState(ofxOpenVRTrackerState _state) {

	state = _state;
}

//--------------------------------------------------------------

//--------------------------------------------------------------

//--------------------------------------------------------------

//--------------------------------------------------------------

//--------------------------------------------------------------

//--------------------------------------------------------------









// Example for processing events
//void ofxOpenVRTracker::handleInput()
//{
//    // Process SteamVR events
//    vr::VREvent_t event;
//    while (system->PollNextEvent(&event, sizeof(event)))
//    {
//        ofLogVerbose() << "Even has been received";
//        switch (event.eventType) {
//            case vr::VREvent_TrackedDeviceActivated:
//            {
//                printf("Device %u attached. Setting up render model.\n", event.trackedDeviceIndex);
//            }
//                break;
//
//            case vr::VREvent_TrackedDeviceDeactivated:
//            {
//                printf("Device %u detached.\n", event.trackedDeviceIndex);
//            }
//                break;
//
//            case vr::VREvent_TrackedDeviceUpdated:
//            {
//                printf("Device %u updated.\n", event.trackedDeviceIndex);
//            }
//                break;
//        }
//    }
//}

