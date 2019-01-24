//
//  Utilities.hpp
//  test_101
//
//  Created by Ben Snell on 1/24/19.
//

#ifndef Utilities_hpp
#define Utilities_hpp

#include "ofMain.h"
#include <openvr.h>

// Math utilities for converting values back and forth between openvr's matrices and GLM's

glm::vec3 GetPosition(vr::HmdMatrix34_t matrix);

glm::quat GetQuaternion(vr::HmdMatrix34_t matrix);

glm::mat4 getMatrix(vr::HmdMatrix34_t matrix);


#endif /* Utilities_hpp */
