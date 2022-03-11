# System Management

## High level functions to the system as a whole

- self discovery of camera modules over the network
- assert network config info so camera and CV TX/RX know where to send/get data
- provide camera module status back to base station
- provide "user configurable" things such as location of the forbidden zone

## Diagram

![Top level diagram](../media/top_level_diagram.jpg)

## Submodule definitions
crossover to other teams in **bold**

- system management client
  - input identification info for discovery via TX/RX module
  - output identification info for discovery via TX/RX module
  - output network information to **stream TX** and to **metadata TX** (port number, etc)
  - output status info back to TX module
- client side TX/RX
  - input data from base station over network
  - output data from base station to system management client 
  - input data from system management client
  - output data from system management client to base station over network
- base station TX/RX
  - input data from client over network
  - output data from client to system management server
  - input data from system management server
  - output data from system management server to client over network
- system management server
  - input data from base station TX/RX
  - output network information to **stream RX** and to **metadata RX** (port number, etc)
  - output configuration info to data aggregator (coordinate of the forbidden zone, later we will determine how the user will set this, hard code for early integration)
- data aggregator/zone detection
  - input metadata from **metadata RX**
  - input configuration info from system management server
  - output decision of alert status to **rendering**

## Members
John Craffey (leader)

Sid Chenrayan

Joshua Erickson

Samir Elhelw
