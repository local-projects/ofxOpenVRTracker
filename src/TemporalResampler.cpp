#include "TemporalResampler.hpp"

//--------------------------------------------------------------
TemporalResampler::TemporalResampler() {
    
}

//--------------------------------------------------------------
TemporalResampler::~TemporalResampler() {

}

//--------------------------------------------------------------
void TemporalResampler::setDesiredFPS(float _fps) {

    if (_fps != desiredFPS) {
        desiredFPS = _fps;
        avgProcessDurationMSEasing = exp(log(0.9) * 60.0 / desiredFPS);
        // reset?
    }
}

//--------------------------------------------------------------
void TemporalResampler::update() {

    // The last sleep time was likely not exact, so accumulate its error
    double sleepErrorMS = lastObservedSleepDurationMS - double(roundedWaitMS);
    // Update the average sleep time
    avgObservedSleepDurationErrorMS = glm::mix(sleepErrorMS, avgObservedSleepDurationErrorMS, avgProcessDurationMSEasing);

    // How long did it take to complete the last cycle?
    uint64_t thisTimeUS = ofGetElapsedTimeMicros();
    long cycleDurationUS = 0;
    if (lastCycleStartTimeUS > 0) {
        cycleDurationUS = thisTimeUS - lastCycleStartTimeUS;
        // Compare this to the ideal duration
        double idealCycleDurationMS = 1000.0 / desiredFPS;
        // Accumulate error
        accumulatedError += double(cycleDurationUS)/1000.0 - idealCycleDurationMS;
    }
    lastCycleStartTimeUS = thisTimeUS;
                        
    // How long did it actually take to process the data? (subtract last wait time)
    double lastProcessDurationMS = 0;
    //if (lastRoundedWaitMS >= 0) lastProcessDurationMS = double(cycleDurationUS) / 1000.0 - double(lastRoundedWaitMS);
    if (lastRoundedWaitMS >= 0) lastProcessDurationMS = double(cycleDurationUS) / 1000.0 - lastObservedSleepDurationMS;
    
    // Calc the average process time
    // === METHOD 1 ===
    avgProcessDurationMS = glm::mix(lastProcessDurationMS, avgProcessDurationMS, avgProcessDurationMSEasing);
    // === METHOD 2 ===
    //processDurationQueue.push(lastProcessDurationMS);
    //processDurationQueueSum += lastProcessDurationMS;
    //while (processDurationQueue.size() > int(desiredFPS)) {
    //    processDurationQueueSum -= processDurationQueue.front();
    //    processDurationQueue.pop();
    //}
    //avgProcessDurationMS = 0.0;
    //if (!processDurationQueue.empty()) avgProcessDurationMS = processDurationQueueSum / double(processDurationQueue.size());

    // The average process duration will be our expected process duration
    double expectedProcessDurationMS = avgProcessDurationMS;
    // The average sleep duration error will be our expected sleep error
    double expectedSleepAdjustmentMS = -avgObservedSleepDurationErrorMS;

    // Calculate the ideal cycle duration
    double idealCycleDurationMS = 1000.0 / desiredFPS;
    // Calculate the ideal sleep duration, given average process time
    double idealWaitDurationMS = idealCycleDurationMS - expectedProcessDurationMS;
    // Calculated expected sleep duration, given sleep errors
    double expectedWaitDurationMS = idealWaitDurationMS + expectedSleepAdjustmentMS;
    // Adjust the sleep time given the accumulated error
	double adjustedWaitDurationMS = max(expectedWaitDurationMS - accumulatedError, 0.0);
    // Round the sleep time
    roundedWaitMS = round(adjustedWaitDurationMS);
    // Accumulate error
    accumulatedError -= (expectedWaitDurationMS - roundedWaitMS);

    // Save this wait time
    lastRoundedWaitMS = roundedWaitMS;
}

//--------------------------------------------------------------
void TemporalResampler::reset() {

    lastRoundedWaitMS = -1;
    lastCycleStartTimeUS = 0;
    avgProcessDurationMS = 0.0;
    accumulatedError = 0.0;
    roundedWaitMS = 0;

    startSleepTimeUS = 0;
    stopSleepTimeUS = 0;
    lastObservedSleepDurationMS = 0.0;
    avgObservedSleepDurationErrorMS = 0.0;

    //while (!processDurationQueue.empty()) processDurationQueue.pop();
}

//--------------------------------------------------------------
void TemporalResampler::flagStartSleep() {

    startSleepTimeUS = ofGetElapsedTimeMicros();
}

//--------------------------------------------------------------
void TemporalResampler::flagStopSleep() {

    stopSleepTimeUS = ofGetElapsedTimeMicros();
    lastObservedSleepDurationMS = double(stopSleepTimeUS - startSleepTimeUS) / 1000.0;
}

//--------------------------------------------------------------

//--------------------------------------------------------------
