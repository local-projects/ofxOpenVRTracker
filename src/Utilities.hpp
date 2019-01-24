//
//  Utilities.h
//  test_101
//
//  Created by Ben Snell on 1/24/19.
//

// Math utilities for converting values back and forth between openvr's matrices and GLM's

#ifndef Utilities_hpp
#define Utilities_hpp

#include "ofMain.h"
#include <openvr.h>

//--------------------------------------------------------------
glm::vec3 GetPosition(vr::HmdMatrix34_t matrix) {
    
    // Reference: https://github.com/osudrl/OpenVR-Tracking-Example/blob/master/LighthouseTracking.cpp
    
    glm::vec3 position;
    
    position.x = matrix.m[0][3];
    position.y = matrix.m[1][3];
    position.z = matrix.m[2][3];
    
    return position;
}

//--------------------------------------------------------------
glm::quat GetQuaternion(vr::HmdMatrix34_t matrix) {
    
    // Reference: https://github.com/osudrl/OpenVR-Tracking-Example/blob/master/LighthouseTracking.cpp
    
    glm::quat quaternion;
    
    // Should this abs() instead of fmax ()?
    quaternion.w = sqrt(fmax(0, 1 + matrix.m[0][0] + matrix.m[1][1] + matrix.m[2][2])) / 2;
    quaternion.x = sqrt(fmax(0, 1 + matrix.m[0][0] - matrix.m[1][1] - matrix.m[2][2])) / 2;
    quaternion.y = sqrt(fmax(0, 1 - matrix.m[0][0] + matrix.m[1][1] - matrix.m[2][2])) / 2;
    quaternion.z = sqrt(fmax(0, 1 - matrix.m[0][0] - matrix.m[1][1] + matrix.m[2][2])) / 2;
    quaternion.x = copysign(quaternion.x, matrix.m[2][1] - matrix.m[1][2]);
    quaternion.y = copysign(quaternion.y, matrix.m[0][2] - matrix.m[2][0]);
    quaternion.z = copysign(quaternion.z, matrix.m[1][0] - matrix.m[0][1]);
    
    return quaternion;
}

//--------------------------------------------------------------
glm::mat4 getMatrix(vr::HmdMatrix34_t matrix) {
    
    // The HmdMatrix34_t matrix returned from IVRsystem has the form m[3][4], where the 4x4 matrix would look like:
    //
    // in[0][0]     in[0][1]    in[0][2]    in[0][3]
    // in[1][0]     in[1][1]    in[1][2]    in[1][3]
    // in[2][0]     in[2][1]    in[2][2]    in[2][3]
    // 0            0           0           1
    //
    // where the position (x,y,z) is (in[0][3], in[1][3], in[2][3]) according to this resource:
    // https://github.com/osudrl/CassieVrControls/wiki/OpenVR-Quick-Start#the-position-vector
    // Thus, this matrix would be considered "row-major."
    
    
    // GLM uses column-major vectors, so some shifting around is required to convert HmdMatrix34_t
    // The last of 4 columns in glm::mat4 is the position vector (truncated to 3 values).
    // glm::vec3 position = glm::vec3(out[3]);
    //
    //  out[0][0]       out[1][0]       out[2][0]       out[3][0]
    //  out[0][1]       out[1][1]       out[2][1]       out[3][1]
    //  out[0][2]       out[1][2]       out[2][2]       out[3][2]
    //  out[0][3]       out[1][3]       out[2][3]       out[3][3]
    //
    // A good reference is here:
    // https://gist.github.com/kylemcdonald/411914969b25ce47786fd411f0ae6210
    
    glm::mat4 out;
    
    out[3][0] = matrix.m[0][3];
    out[3][1] = matrix.m[1][3];
    out[3][2] = matrix.m[2][3];
    
    out[2][0] = matrix.m[0][2];
    out[2][1] = matrix.m[1][2];
    out[2][2] = matrix.m[2][2];
    
    out[1][0] = matrix.m[0][1];
    out[1][1] = matrix.m[1][1];
    out[1][2] = matrix.m[2][1];
    
    out[0][0] = matrix.m[0][0];
    out[0][1] = matrix.m[1][0];
    out[0][2] = matrix.m[2][0];
    
    // Normalize?
    return out;
}

#endif /* Utilities_h */
