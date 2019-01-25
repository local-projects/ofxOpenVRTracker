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
#include "DeviceList.hpp"
#include "Device.hpp"
#include "Utilities.hpp"


// TODO
// Add RemoteUI functionality


// Event arguments for new data
class ofxOpenVREventArgs : public ofEventArgs {
public:
    DeviceList* devices;
};

// State of this addon
enum ofxOpenVRState {
	DISCONNECTED,
	TRY_CONNECT, // actively trying to connect
	CONNECTED
};

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

    /// \brief List of all generic tracker devices that have ever connected. Query for more specific information.
    ///
    DeviceList devices;

    /// \brief Listen to this event to know when data is received
    ofEvent< ofxOpenVREventArgs > newDataReceived;
    

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
    
};

