ofxOpenVRTracker
====================

This addon updates [smallfly's ofxOpenVR addon](https://github.com/smallfly/ofxOpenVR) by making it compatible with [OpenVR v1.1.3b](https://github.com/bensnell/OpenVR/tree/ofxOpenVR_v1.1.3b) and changes the focus of the addon from HMD rendering to headless tracking of generic trackers. It provides a number of methods specific to the tracker's dentification and properties.



### Hardware Needed

This addon's current instantiation is designed to work with Vive Pro (and v2 base stations). Some of the functionality may not be available with previous versions of the HTC Vive. Other VR platforms like the Oculus have not been tested, but should be compatible with some modifications.

The Vive Pro link box is not needed to run the trackers in a headless system. The v2.0 base stations are capable of connecting to either the bluetooth watchman dongles or the link box. Since the link box isn't needed for this application, they will connect to the watchman dongles. Unplug the link box from your computer if it is currently connected.

Make sure one bluetooth dongle is connected to computer for each tracker you plan on using. Furthermore, make sure each dongle is placed at least 18" away from any computer. This one tracker to one dongle relationship has been confirmed as required as of 11/21/2019.



### Building this Addon

##### System Requirements

This addon was developed on Windows 10 in Visual Studio 2019 using an x64 architecture.

##### Dependencies

Please install the following pieces of software, if they are not already installed:

- [Steam](https://store.steampowered.com/about/), SteamVR (download from the Steam Store after Steam is installed)

Note: Do *NOT* install [Vive Pro Drivers](https://enterprise.vive.com/us/setup/vive-pro/), as they will cause problems with SteamVR later on.

##### Build Instructions

1. Create an Openframeworks project using the Project Generator.
2. Open the Visual Studio solution file. Under the *Property Manager*, add the property sheet `ofxOpenVRTracker.props` to all configurations.
3. Program your application, following the usage exemplified in the *example* provided.
4. Compile your application.



### Running Your Application

In order for this addon to work, there must be a running instance of SteamVR on your computer. Before starting SteamVR for the first time, follow these steps to setup SteamVR to work in a headless state:

##### Headless Vive Pro SteamVR Setup

Within this addon, there is a folder *steamvr_settings* that contains files with the same relative paths as those in your Steam directory. Your Steam directory is likely located at `C:/Program Files (x86)/Steam`. For each file in the *steamvr_settings* folder, use a text editor to copy all (json formatted) settings into the corresponding files in your native Steam directory. If the same keys exist in the files in your native directory, replace the old value with the new one. Together, changes to these configuration files allow for the Vive Pro to run headless.

Additionally, it is recommended to delete all contents of the folder `C:\Program Files (x86)\Steam\config\lighthouse\`. Do this once before you run SteamVR, before running the application which uses this addon. This will clear all existing `lighthousedb.json` files and associated configurations. This, in combination with the replacement of the `chaperone_info.json` in the previous step, will ensure you have only one universe (coordinate space), so that SteamVR does not get confused about which universe to choose before or during program execution.

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
- Try restarting your computer.

SteamVR won't start and/or connect the trackers

- The **VIVEPORT Desktop Service** Application prevents this. Search Windows for "Services." Find this application in the list, right click on it and select *Properties.* Under *Startup Type,* select *Disabled*. Apply these changes and restart the machine.

SteamVR prompts you to complete *Room Setup*

- The `chaperone_info.json` file is either nonexistent or invalid. Make sure that your file is replaced with the file provided here in *steamvr_settings*. 

System calibration changes mid-program execution

- This is observable through a sudden change in coordinate information coming from OpenVR (or Vive, SteamVR, etc.). The data will likely have a new origin and some translation, since a new base station has been automatically set as the "reference" base station. It is unclear *why* this happens. Looking into the SteamVR log files shows that a new universe is selected due to a change in the devices present. One reason this may happen is that SteamVR automatically choses the best universe (coordinate system + calibration) given the set of devices present. Thus, if a new device (base station, tracker, etc.) comes online and a different universe more closely matches this device set, SteamVR automatically switches the universe. (It's unclear whether this can be explicitly switched off with a toggle). All aavailable universes are listed in the `chaperone_info.json` file. Configuration files in the `C:\Program Files (x86)\Steam\config\lighthouse` directory appear also to have some effect on this choice. Following the relevant steps above in the [section above](#headless-vive-pro-steamvr-setup) should resolve this problem, by providing only one universe to choose from.
- References:
  - [Automatic Universe Selection](https://github.com/alsoknownasfoo/ViveArcadeTool/issues/2#issuecomment-494911749)
  - [Commented Chaperone Reference File](https://developer.valvesoftware.com/wiki/SteamVR/chaperone_info.vrchap)


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
