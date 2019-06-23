# Select Default Sound Render Device (using undocumented COM-interface IPolicyConfig).

**System Requirements**: Microsoft Windows Vista and later
Visual C++ Redistributable for Visual Studio 2013: [http://www.microsoft.com/en-us/download/details.aspx?id=40784](http://www.microsoft.com/en-us/download/details.aspx?id=40784)

## Run without parameters for show system tray icon
* right click: opens a context menu quick-change sound render device (audio endpoint)
* left double click: switch to next sound render device (audio endpoint) <br/>
  ![](https://user-images.githubusercontent.com/1287295/59975595-a4508080-95b1-11e9-8803-9b18a46a6818.png)

* GDI+ hint on device switch <br/>
  ![](https://user-images.githubusercontent.com/1287295/59975598-ae727f00-95b1-11e9-83af-644f84f33247.png)

## Commad line parameters:
```DefSound.exe <DEVICE>[,DEVICE...] [ROLE]```
* DEVICE - device description or zero-based index
* ROLE -  role that the system has assigned to an audio endpoint device ([http://msdn.microsoft.com/en-us/library/windows/desktop/dd370842(v=vs.85).aspx](http://msdn.microsoft.com/en-us/library/windows/desktop/dd370842(v=vs.85).aspx)). Variants:
    * ALL - all roles, default value
    * CON - system notification sounds and voice commands
    * MMEDIA - multimedia
    * VOICE - voice communications
    
If you supply -1, it will switch to next sound render device (audio endpoint) <br/>
If you specify more than one device, then the list will be rotated so that the default device is the latest. Example: <br/>

| Default device before | Command line | Default device after |
| --------------------- | ------------ | -------------------- |
| 2 | DefSound.exe 0,1,2 | 0 |
| 0 | DefSound.exe 0,1,2 | 1 |
| 1 | DefSound.exe 0,1,2 | 2 |
