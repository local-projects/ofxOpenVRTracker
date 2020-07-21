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
#include "ofxOpenVRDeviceList.hpp"
#include "ofxOpenVRDevice.hpp"
#include "ofxOpenVRUtilities.hpp"
#include "TemporalResampler.hpp"

// Add RemoteUI functionality if available
#if defined(__has_include)
#if __has_include("ofxRemoteUIServer.h")
#include "ofxRemoteUIServer.h"
#define OFXOPENVRTRACKER_USE_RUI
#endif
#endif

// Event arguments for new data
class ofxOpenVRTrackerEventArgs : public ofEventArgs {
public:
    DeviceList* devices;
};

// State of this addon
enum ofxOpenVRTrackerState {
	DISCONNECTED,
	TRY_CONNECT, // actively trying to connect
	CONNECTED
};

class ofxOpenVRTracker : public ofThread {
public:
    
    /// \brief Create an instance of ofxOpenVRTracker. There should only be one per program.
    ///
    ofxOpenVRTracker();

    // Setup parameters (useful if ofxRemoteUI is available)
    void setup();

    /// \brief Attempt to connect to a running instance of SteamVR.
    ///
    /// A running instance of SteamVR is required for this addon to work properly. Call this function once at the statrt of your program. It will attempt to connect continuously for a maxAttempts number of times, waiting waitTime milliseconds between each until connected or unsuccessful.
    ///
	void connect(int maxAttempts = -1, int waitTimeMS = -1);

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
    ofEvent< ofxOpenVRTrackerEventArgs > newDataReceived;

	/// \brief Ends the thread
	void exit();

    // Get the FPS (depends on FPS mode)
    float getFPS() { return isFPSOverriden() ? getDesiredFPS() : float(frameTimesMS.size()); }

    // FPS Override Controls
    bool isFPSOverriden() { return bOverrideFPS; }
    float getDesiredFPS() { return desiredFPS; }
    void setOverrideFPS(bool _bOverrideFPS, float _desiredFPS) {
        if (bOverrideFPS == _bOverrideFPS && desiredFPS == _desiredFPS) return;
        bOverrideFPS = _bOverrideFPS;
        desiredFPS = _desiredFPS;
#ifdef OFXOPENVRTRACKER_USE_RUI
        RUI_PUSH_TO_CLIENT();
#endif
    }

private:
    
    // State of the app
    ofxOpenVRTrackerState state;
    // set the state of this app
    void setState(ofxOpenVRTrackerState _state);

    // (function run in the thread)
	void threadedFunction();

	// Attempt to connect once to an active (running) instance of SteamVR
	bool connectToSteamVR();
    // Connection parameters
	int connectionAttemptsCounter = 0;
	int connectionAttemptsMax = 5;
	int connectionAttemptWaitTime = 5000; // milliseconds
    int64_t lastConnectionAttemptTimeMS;

	// This pointer references the primary event/data handling abilities of OpenVR
    vr::IVRSystem *system = NULL;

    // Framerate settings
    // Should we override with a custom frame rate?
    bool bOverrideFPS = false;
    bool bLastOverrideFPS = false;
    float desiredFPS = 240.0;
    TemporalResampler resampler;
    
    // Non-resampled framerate calculations
    queue<uint64_t> frameTimesMS;
    
};

