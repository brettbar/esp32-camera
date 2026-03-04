
Video Link: https://www.youtube.com/watch?v=J8zc8mMNKtc&t=15s

### What is ESP-IDF

- Espressif IoT Development Framework

- SDK for working on ESP projects

- Hosted on Github and Open Source

- Includes various tool and Libraries

### Why use ESP-IDF SDK
- Written in C/C++
- Lots of components
- Configurable at component level
- Constantly maintained
- Easy to use, can be used from CLI
- Has GDB support

---
### IDF installation

- Rembemer there is a ESP-IDF Vscode extension 

1. Install dependieces
	- Python 3
	- Git
	- CMake
	- Ninja
	
2. Clone ESP-IDF repo
	- Components
	- Examples
	- Tools
3.  Run esp-idf/install.sh
	- Installs tool chain
	- Installs Python packages inside py enviorment
4. Create a project dir
5. Run esp-idf/export.sh
	- Exposes the tools just installed
	
---
### IDF workflow

- Four main steps
1. Generate Configuration file
2. Compile into binary
3. Flash to ESP-32
4. Reset ESP into execute mode and monitor


###### Configure

- `idf.py set-target esp32`
- `idf.py menuconfig`
- Now use menu gui to configure
- Should now see build/ and sdkconfig/ in the project

###### Build

`idf.py all`
- First build it will compile source code for all components
- Subsequent builds will only compile modified files

###### Flash

- Find serial port in /dev/
- Run `idf.py -p /dev/<serial port> flash `


###### Monitor
- Returns to execute mode after flashing automatically
- Run `idf.py monitor`

- You will see a log
	- Reset to exectue
	- first stage bootloader
	- second stage bootloader
		- reads the partition table to find the app binary
		- Load app binary
	- Enter app binary
		- Calls app_main()

[Read more about the boot process ](https://docs.espressif.com/projects/esp-idf/en/v5.1/esp32/api-guides/startup.html)

---

### How to program an ESP-IDF project

- We want to multithread our program (Tell Brett)
- There are so many peripherals that it would be very inefficient to run a single process
- Multithreading requires an OS for thread scheduling and management

We use FreeRTOS
- Real time operating system
- Small and simple
- Integrated into ESP-IDF
- Allows multi-threading (known as tasks)

Overview of FreeRTOS Task
- Each task is a C function
- Implement each as an infinite loop
- Tasks can be blocked by other events
- Task can emit events to block or unblock other tasks
- Functions never return, they have to delete themselves

FreeRTOS Task States
- Running -- Currently being executed
- Ready -- The task is able to execute but isn't currently doing so
- Blocked -- Task is waiting for a temporal or external event, cannot be scheduled until unblocked or timed out
- Suspended -- Task will not be scheduled until explicitly unsuspended


ESP-32 Memory

- Different and non uniform pyhiscal memory
- DRAM, ROM, IRAM, external flash, external SPI flash
- Multiple tasks mean multiple stacks exist



---

### Build System & Components

- Components are like libraries with extra features
- Build system based on CMake
- CMake command is integrated into idf.py command


Components consist of:
- Source files, headers, and include directories (.c and .h)
- Linker Fragments (.lf)
- Embedded binary
- pre-compiled libraries
- Kconfig configuration?
- Unit tests

- When you are constructing a project separate the components almost in an OOP fashion (i.e. Camera, Button, LCD screen)
- Either find or create a library for each component
- Connect the components together with glue logic for desired functionality 

- Find components at:
	- ESP-IDF Component Reference (ESP maintained)
	- IDP Component  Registry (Non ESP)
	- Write you're own, or modify a third party piece


Fatal error screen
- Provides CPU register values at crash
- EXCVADDR provides address access at fail
- Back trace offers last function addresses
- Monitor comand should decode these


For debugging recourses go 23:00 in the video linked above