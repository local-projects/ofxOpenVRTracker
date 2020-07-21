#pragma once

#include "ofMain.h"

// This class is useful for downsampling something which runs too
// fast, or upsampling something which runs too slow. It is 
// intended to make use of a thread's sleep function to adjust 
// the time of a cycle. It is loosely based on the functioning
// of Kalman filters. It has been designed to sleep for a discrete
// number of milliseconds, instead of a continuous number.
class TemporalResampler {
public:

    TemporalResampler();
    ~TemporalResampler();

    // Set the desired FPS we want to sample at
    void setDesiredFPS(float _fps);
    
    // Mark a point in the cycle we monitoring
    void update();

    // Get the duration of time we should sleep for. 
    // (User sleeps on their own)
    int getSleepDurationMS() { return roundedWaitMS; }

    // Reset the resampler
    void reset();

private:

    float desiredFPS = 60.0;

    int lastRoundedWaitMS = -1; // NEEDED?
    uint64_t lastCycleStartTimeUS = 0;
    double avgProcessDurationMS = 0.0;
    double avgProcessDurationMSEasing = 0.95;
    double accumulatedError = 0.0;

    // Calculated wait time 
    int roundedWaitMS = 0;

    //queue<double> processDurationQueue;
    //double processDurationQueueSum = 0.0;


};

