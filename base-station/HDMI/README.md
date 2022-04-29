# HDMI

## Diagram

![Top level diagram](../media/top_level_diagram.jpg)

## GUI Mockup

![GUI Mockup](images/GUI_Mockup.svg)

### Responsibilities

- Receive video stream via RTSP/UDP
- Receive bounding box data
- Render bounding boxes and camera streams on HDMI

# Sub Modules Descriptions
**Stream Tx (Camera/HDMI)**  
1. Inputs:  
  ⁃ Formatted images from Camera Driver  
  ⁃ Network information from Device Management  
2. Outputs:  
  ⁃ Packaged stream sent over network to Stream Rx  
3. Functions:  
  ⁃ Establishes network connection using information from Device Management  
  ⁃ Converts received images into buffer to be sent over network to Stream Rx  

**Stream Rx (Camera/HDMI)**  
1. Inputs:  
  ⁃ Packaged stream sent over network from Stream Tx  
  ⁃ Network information from Device Management  
2. Outputs:  
  ⁃ Formatted images for Rendering  
3. Functions:  
  ⁃ Establishes network connection using information from Device Management  
  ⁃ Receives stream over network and converts to formatted images for Rendering  

**Rendering (HDMI)**  
1. Inputs:  
  ⁃ Formatted images from Stream Rx  
  ⁃ Unpacked metadata from Metadata Rx  
  ⁃ Detection information from Zone Detection  
2. Outputs:  
  ⁃ Formatted images for HDMI Driver  
3. Functions  
  ⁃ Compiles received data from all cameras (stream and metadata) and displays it in a GUI  
  ⁃ Displays detection determination also in GUI  
  ⁃ Sends GUI images to HDMI Driver  

**HDMI Driver (HDMI)**  
1. Inputs:  
  ⁃ Formatted images from Rendering  
2. Outputs:  
  ⁃ Formatted images to HDMI image buffer  
3. Functions:  
  ⁃ Converts received images from Rendering into format for the HDMI image buffer  
  ⁃ Pushes buffer to HDMI display  

### Members

*Sub team: Rendering*  
David Boullie (leader) - focused on GUI layout design and placement of various elements on display. Additionally, supporting switching to different views.

Sam Lambrecht - focused on displaying frames from camera stream.

Matt Downing - focused on bounding box drawing and text drawing. Text includes alerts and camera status.

*Sub team: HDMI Driver*  
Joseph Berman

Tom Doyle

*Sub team: Stream Tx/Rx*  

Connor Northway
