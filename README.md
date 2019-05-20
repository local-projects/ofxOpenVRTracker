ofxOpenVRTracker
====================

This addon updates [smallfly's ofxOpenVR addon](https://github.com/smallfly/ofxOpenVR) by making it compatible with [OpenVR v1.1.3b](https://github.com/bensnell/OpenVR/tree/ofxOpenVR_v1.1.3b) and changes the focus of the addon from HMD rendering to tracking of generic trackers.

By following the build instructions below, you should be able to track generic trackers using OF and SteamVR, without the HMD.



## Dependencies

Steam, SteamVR

Install the Link Box Bluetooth Driver (will be prompted after room boundaries defined)



## Build Instructions

1. Create an openframeworks' project using the Project Generator, or add the addon's source files and the OpenVR headers to your existing project.
2. In `Property Manager` (open it from `View -> Other Windows -> Property Manager`), right click on your solution to select `Add Existing Property Sheet...` and select the `ofxOpenVRTracker.props` file.
3. Open the Visual Studio solution file. If libs/OpenVR/samples has been included, remove it from the solution explorer. This "samples" folder should not be included.
4. Program your application, following the usage exemplified in [test_101](https://github.com/bensnell/ofxOpenVR/tree/master/test_101/src).
5. Compile your application.

## Running SteamVR

In order for this addon to work, there must be on your computer a running instance of SteamVR. 



SteamVR requires that a user perform room-setup. Download the files [here](https://www.reddit.com/r/Vive/comments/4d3c6h/i_made_some_small_batch_files_for_switching/) and place them in the correct location within the Steam directory.



In order to run the Vive without a headset, two settings files must be changed before SteamVR runs. The first file



also change render info of the null driver to 0, 0, 0, 0 for x, y, w, h



These two files are provided here in their corollary file structure in *steamvr_settings*. Replace the respective files in your computer with those provided here:

`C:/Program Files (x86)/Steam/steamapps/common/SteamVR/resources/settings/default.vrsettings`

`C:/Program Files (x86)/Steam/steamapps/common/SteamVR/drivers/null/resources/settings/default.vrsettings`

Note: These files also increase the standby timeout for controllers and trackers, so they don't fall asleep when they aren't being used. The timeout in these settings is one day, set in the first file under *power > turnOffControllersTimeout*. This value is in seconds.

Open the Steam application and launch SteamVR. (Note: a desktop shortcut can be created for SteamVR by navigating to *Steam > Library > Tools > SteamVR* and right clicking on *SteamVR* and selecting *Create Desktop Shortcut*.)

## Using the Addon

Run the app containing this addon only after SteamVR has begun. The app by default attempts to connect to SteamVR 5 times in 10 second intervals before timing out.

The app is designed specifically for generic tracker pucks and provides a number of methods specific to their identification and properties.

## Tracker Connections

Trackers have two non-exclusive states they are in at any point in time. Trackers have a connection status that describe whether they are connected over bluetooth to the computer. This is revealed by the `bConnected` variable in a device. Trackers also have a tracking status that describes whether they are actively being tracked by the base stations. If a device has a true `bTracked` variable, then their positions, orientations, etc. are being updated constantly.

Trackers must be paired with their own bluetooth dongle. Multiple trackers cannot connect to the same dongle.



## Base Stations Setup

The base stations must be on modes 'b' and 'c' if the sync cable isn't used (this is the default configuration). See [here](https://www.vive.com/us/support/vive/category_howto/installing-the-base-stations.html) for more info.

## Troubleshooting

SteamVR fails to start and displays the message "Compositor Not Available" 

- The headset needs to be plugged into the main HDMI port of the computer

Trackers won't connect (their lights stay blue). 

- Sometimes, cables come loose within the trackers and controllers. This has been [observed](https://community.viveport.com/t5/Technical-Support/Unpairable-and-untrackable-blue-light-controller/td-p/7834) for people who use them extensively. See [here](https://www.youtube.com/watch?v=0mi3KWG5mic) for instructions on how to resecure the cables.

## Notes

Tested with the HTC Vive on Windows 10 (32- and 64-bit systems).

## Useful Terminology

HMD: head mounted display
Tracker: generic tracker ("puck")
Controller: handheld controller stick
Base Station: lighthouse (the black IR boxes)
