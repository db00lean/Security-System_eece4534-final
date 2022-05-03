# Zed Button Driver and Client

- Author: Siddharth Chenrayan (chenrayan.s@northeastern.edu)
- Description: Documentation for the ZedBoard button driver and client. 
- Version: 0.1
- Date: 3/31/2022

## Button driver

### Overview

`zed_btns.c` defines a kernel module that catches button presses on the ZedBoard and stores the button presses in a FIFO. In addition to the GPIO interface, `zed_btns.c` also initializes a character device interface (currently, `/dev/zedbtns0`) from which these button presses can be read.

### Description

- GPIO interface
  - Utilizes GPIO chip kernel module and devices to retrieve information about the buttons
  - The GPIO chips are not actually used for reading the buttons (only retrieving irq and memory resources)
  - Whenever buton state changes, interrupt is triggered. 
    - The button values are read (32 bit register, truncated to lower 8 bits).
    - Raw data is pushed into the FIFO as a `u8`. 
  - The FIFO has a hardcoded maximum size
    - This puts a limit on the amount of button presses that can be stored before the FIFO must be emptied. 
    - Any button presses that are recorded once the FIFO reaches capacity will be dropped.
    - Old data is not pushed out in favor of new data (this might change).
    - Currently, the hardcoded max size is 32 bytes (i.e. 32 button presses can be stored). This might become configurable in future versions). 
- Character device `/dev/zedbtns0`
  - Exposes two overloaded file operations: `poll` and `read`
  - `poll` will put calling processes to sleep until the kfifo is non-empty. 
    - When the kfifo becomes non-empty, the return value of `poll` will indicate that there is data to be read, and any processes waiting on button presses will be woken up. 
  - `read` will read all available data in the kfifo (the data will be cleared after the read)
  - The button buffer is protected by a mutex. Any read/write operations occur behind the mutex lock. 

### Limitations

Since the kfifo gets emptied on-read, realistically, only a single process can be listening to `/dev/zedbtns0` to receive a complete log of button presses. Unsure if I'm gonna change this. To me it really doesn't make sense for a single button press to have multiple possible side effects. 

### Build/Usage instructions

Hopefully pretty straightforward! 

- Build: 
  - Run `make` from the top of the `button_driver` directory.
    - This will create the `zed_btns.ko` file (as well as a bunch of other files, but really you only need `zed_btns.ko`)
- Inserting the module: 
  - `scp` the `zed_btns.ko` file to the ZedBoard. In our case, `scp zed_btns.ko root@10.42.1.1:`
  - On the ZedBoard (you can just `ssh` to the same address you `scp`'ed to, e.g. `ssh root@10.42.1.1`), run `insmod zed_btns.ko`
  - In the kernel logs (you can access them via `tail -f /var/logs/kern.log`), you should see log messages to let you know that the module has been inserted properly. 
    - When you press buttons, you should see the corresponding output in the kernel logs. 
      - NOTE: this is only available if you compiled the module with the `-DDEBUG` flag. 

## Button client/button listener

### Overview

`button_client.c` implements a set of functions declared and documented in `button_client.h` which can be used by any userspace program to respond to button presses from the ZedBoard's 5 GPIO buttons. This system was designed to use as little system resources as possible, while still remaining responsive to button presses. 

### Description

- The most important function provided by `button_client.h` is `void* run_button_client(void* args)` 
  - Comprehensive function that loops infinitely listening to button press data from the `/dev/zedbtns0` character device.
  - Meant to be run in a thread separate from the main rendering operations. 
    - See the `main` in `button_client.c` for an example of how to launch a separate thread that listens to button presses
  - Currently, the `args` is unused, but this is definitely going to change in later versions. 
    - For now, `run_button_client` just logs button presses to standard-out.
    - These actions are captured in the `debug_actions` struct defined in `button_client.c`
  - While other utility functions are exposed (such as `exec_action` or `init_zedbtn_pollfd`), these are not necessary for use by client programs who can leverage the out of the box functionality of `run_button_client`. 
    - `run_button_client` uses these functions internally.
  - Uses the `poll` syscall to wait for data from `/dev/zedbtns0`.
    - This allows for minimal system usage, since the thread will be put to sleep if `/dev/zedbtns0` has no data to report.
    - The `zed_btns.ko` kernel module takes care of waking up the listener thread when a button press is recorded. 

### Supported menu operations and button usage

- Menu operations: users can change the following properties
  - x-position of orbidden zone
  - y-position of forbidden zone
  - x-length of forbidden zone
  - y-length of forbidden zone
  - camera brightness
  - camera contrast

- Button actions
  - Center button: cycles active camera
  - Left/Right buttons: cycles through menu modes
  - Up/down: increments/decrements value

### Building and executing the binaryUsage
  - Building the overall sysman client also builds the button driver/client dependencies.
  - Of course, make sure to insert the kernel module for the button driver before launching the system management server
