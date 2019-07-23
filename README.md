ofxOpenVRTracker
====================

This addon updates [smallfly's ofxOpenVR addon](https://github.com/smallfly/ofxOpenVR) by making it compatible with [OpenVR v1.1.3b](https://github.com/bensnell/OpenVR/tree/ofxOpenVR_v1.1.3b) and changes the focus of the addon from HMD rendering to headless tracking of generic trackers. It provides a number of methods specific to the tracker's dentification and properties.



### Hardware Needed

This addon's current instantiation is designed to work with Vive Pro (and v2 base stations). Some of the functionality may not be available with previous versions of the HTC Vive. Other VR platforms like the Oculus have not been tested, but should be compatible with some modifications.

The Vive Pro link box is not needed to run the trackers in a headless system. The v2.0 base stations are capable of connecting to either the bluetooth watchman dongles or the link box. Since the link box isn't needed for this application, they will connect to the watchman dongles. Unplug the link box from your computer if it is currently connected.

Make sure one bluetooth dongle is connected to computer for each tracker you plan on using. Furthermore, make sure each dongle is placed at least 18" away from any computer.



### Building this Addon

##### System Requirements

This addon was developed on Windows 10 in Visual Studio 2019 using an x64 architecture.

##### Dependencies

Please install the following pieces of software, if they are not already installed:

- [Steam](https://store.steampowered.com/about/), SteamVR (download from the Steam Store after Steam is installed)

- [Vive Pro Drivers](https://enterprise.vive.com/us/setup/vive-pro/)

##### Build Instructions

1. Create an Openframeworks project using the Project Generator.
2. Open the Visual Studio solution file. If *libs/OpenVR/samples* has been included, remove it from the solution explorer. This "samples" folder should not be included.
3. Program your application, following the usage exemplified in the *example_app* provided.
4. Compile your application.



### Running Your Application

In order for this addon to work, there must be a running instance of SteamVR on your computer. Before starting SteamVR for the first time, follow these steps to setup SteamVR to work in a headless state:

##### Headless Vive Pro SteamVR Setup

Within this addon, there is a folder *steamvr_settings* that contains files with the same relative paths as those in your Steam directory. Your Steam directory is likely located at `C:/Program Files (x86)/Steam`. For each file in the *steamvr_settings* folder, use a text editor to copy all (json formatted) settings into the corresponding files in your native Steam directory. If the same keys exist in the files in your native directory, replace the old value with the new one. Together, changes to these configuration files allow for the Vive Pro to run headless.

Note: These files also increase the standby timeout for controllers and trackers, so they don't fall asleep when they aren't being used. The timeout in these settings is one day, set in the first file under *power > turnOffControllersTimeout*. This value is in seconds.

##### Tracker Connections

Trackers have two non-exclusive states they are in at any point in time. Trackers have a connection status that describe whether they are connected over bluetooth to the computer. This is revealed by the `bConnected` variable in a device. Trackers also have a tracking status that describes whether they are actively being tracked by the base stations. If a device has a true `bTracked` variable, then their positions, orientations, etc. are being updated constantly.

Trackers must be paired with their own bluetooth dongle. Multiple trackers cannot connect to the same dongle.

##### Streamlining Startup

*SteamVR* is a standalone application that can run independent of the *Steam* desktop app. To create a shortcut to this application (to more efficiently connect, open *Steam*, select *Library* > *Tools* from the toolbar. Scroll down to *SteamVR* and right click on it, then select *Create Desktop Shortcut*. A shortcut on your desktop will now run SteamVR, even if Steam isn't open.



### Troubleshooting

SteamVR fails to start and displays the message "Compositor Not Available" 

- The headset needs to be plugged into the main HDMI port of the computer

Trackers won't connect (their lights stay blue). 

- Sometimes, cables come loose within the trackers and controllers. This has been [observed](https://community.viveport.com/t5/Technical-Support/Unpairable-and-untrackable-blue-light-controller/td-p/7834) for people who use them extensively. See [here](https://www.youtube.com/watch?v=0mi3KWG5mic) for instructions on how to resecure the cables.

A mirrored view of the headset appears onscreen.

- If you don't want this view to show, try removing the *default.vrsettings* file from the *null > resources > settings* folder. It's unclear why this works, but sometimes, it does.

I'm uncertain which base stations are connected

- To obtain a (top-view) plan of the play area, the range of connected base stations, and the location of trackers, in the SteamVR small window, from the dropdown menu select *Settings > Developer > View Room Overview*

I can't pair a new tracker. 

- Dongles placed 18" from computer?
- Try to reconnect trackers by selecting from the SteamVR *Menu > Devices > Pair Controller*

SteamVR isn't starting, yet the Steam App shows it as already running

- Close the OF application (which locks onto the SteamVR instance and appears as a SteamVR application), the open SteamVR, then open the OF app again.
- If this still doesn't work, close the OF App and close all Steam or SteamVR tasks from the Windows Task Manager. Then start SteamVR, then start the OF app.

SteamVR welcomes you and asks you to connect your headset.

- The steamvr.vrsettings file was likely corrupted (incorrect JSON) and reset, so re-set its contents using the files provided.

SteamVR popup: "An error occurred while updating SteamVR (app running)" **OR** SteamVR attempts to re-open every time a new device is connected

- Make sure the *steamvr.vrsettings file* has the `power` setting `"autoLaunchSteamVROnButtonPress": false`



### References

This [thread](https://steamcommunity.com/app/358720/discussions/0/485624149150957321/?ctp=2#c2183537632735247703) describes how to run the Vive Pro (and potentially the regular Vive) headless as of July 2019.

More [suppressible warnings](https://github.com/SteamDatabase/GameTracking-SteamVR/blob/636487cb2523be56f5c4c69f976452da7b093e9a/content/vrmonitor/translations/vrmonitor_finnish.txt) can be found here.

##### Useful Terminology

HMD: head mounted display
Tracker: generic tracker ("puck")
Controller: handheld controller stick
Base Station: lighthouse (the black IR boxes)

##### Vive (Regular) Notes

The base stations must be on modes 'b' and 'c' if the sync cable isn't used (this is the default configuration). See [here](https://www.vive.com/us/support/vive/category_howto/installing-the-base-stations.html) for more info.



### Deprecated Documentation

In previous builds, a property sheet file was used to add the necessary dependencies to the Visual Studio project. We no longer need this file, but it is included in *_deprecated*  for reference.