#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofSetVerticalSync(false);
	ofSetLogLevel(OF_LOG_VERBOSE);

	// Setup OpenVR and connect to the SteamVR server.
    openvr.connect();

    // Add a listener to receive new data
    ofAddListener(openvr.newDataReceived, this, &ofApp::newDeviceData);
}

//--------------------------------------------------------------
void ofApp::exit() {
    
    // Remove listener for new device data
    ofRemoveListener(openvr.newDataReceived, this, &ofApp::newDeviceData);
    
    // disconnect from the server
    openvr.disconnect();
}

//--------------------------------------------------------------
void ofApp::update(){
	
}

//--------------------------------------------------------------
void ofApp::draw(){

	// Draw debug info to screen
	ofDrawBitmapStringHighlight(out, 10, 20);
}

//--------------------------------------------------------------
void ofApp::newDeviceData(ofxOpenVREventArgs& args) {

	// Save debug info
	string tmp = "";
	for (int i = 0; i < (*args.devices->getTrackers()).size(); i++) {
		tmp += (*args.devices->getTrackers())[i]->getDebugString() + "\n";
    }
	out = tmp;
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}

