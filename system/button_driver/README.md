# Zed Button Driver

## Overview

`zed_btns.c` defines a kernel module that catches button presses on the ZedBoard and stores the button presses in a FIFO. In addition to the GPIO interface, `zed_btns.c` also initializes a character device interface (currently, `/dev/zedbtns0`) from which these button presses can be read.

## Description

- GPIO interface
  - Utilizes GPIO chip kernel module and devices to retrieve information about the buttons
  - The GPIO chips are not actually used for reading the buttons (only retrieving irq and memory resources)
  - Whenever buton state changes, interrupt is triggered. 
    - The button values are read (32 bit register, truncated to lower 8 bits).
    - Raw data is pushed into the FIFO as a `u8`. 
  - The FIFO has a hardcoded maximum size.
    - Any button presses that are recorded once the FIFO reaches capacity will be dropped.
    - Old data is not pushed out in favor of new data (this might change).
    - Currently, the hardcoded max size is 32 bytes. This might become configurable. 
- Character device `/dev/zedbtns0`
  - Exposes two overloaded file operations: `poll` and `read`
  - `poll` will block until the kfifo is non-empty. 
    - When the kfifo becomes non-empty, the return value of `poll` will indicate that there is data to be read.
  - `read` will read all available data in the kfifo (the data will be cleared after the read)
  - The button buffer is protected by a mutex. Any read/write operations occur behind the mutex lock. 
