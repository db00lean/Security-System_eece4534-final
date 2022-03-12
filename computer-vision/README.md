# Computer Vision


### Responsibilities
- Retrieve images from camera driver via IPC
- Process images, detect humans using CV and generate coordinates for boxes around them
- Transmit coordinates of all bounding boxes to base station via TCP

### Componenets
*Image Processing*
- Generate bounding boxes around people/objects detected on each camera
- Receives images from the Camera
- Sends images to the Metadata TX component

*Metadata TX*
- Transmit bounding box metadata from camera to base station

*Metadata RX*
- Receive bounding box metadata from cameras on the base station

### Inputs
- Images from camera

### Outputs
- Coordinates of bounding boxes of detected people/objects

### Members
John Privitera (leader) - Initial focus on bounding boxes

Jack Horton - Initial focus on base station's metadata receiver

Nicole Johnson - Initial focus on camera's metadata transmitter
