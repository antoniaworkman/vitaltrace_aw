Implement Bluetooth LE Peripheral role functionality with standard battery service and custom fetal heart rate service.

Overview
********

The application implements basic BLE functionality such as advertising and callbacks for connecting and disconnecting
(Zephyr sample "Peripheral" used to start this implementation). A battery module has been implemented to 
simulate discharge of the battery (at a rate of 1% per 5 seconds) and send an update of the battery level via BLE notification
to a connected central device at the same frequency the level changes. A (fetal) heart rate module has been implemented
to send dummy heart rate data to a central device once notifications are activated by writing to the control characteristic
in the application layer. In addition to the data buffer, an index value has been added to the BLE packet in case 
the order of received data is important for the central device.

Assumptions
************

Device should be in peripheral BLE role, connecting to a central.
Fetal heart rate data can fit into a byte array of size 10 and should be streamed every 100ms.

Requirements
************

* Hardware: A board with Bluetooth LE support

Testing
*********

Unit tests are written using the Unity test framework and run on the host machine without hardware.
Unity tests were written leveraging Claude as an example for this assessment.
Prerequisites

CMake (≥ 3.20)
GCC or AppleClang
Unity (cloned into tests/unity/)
nanopb headers (via nRF Connect SDK at /opt/nordic/ncs/v3.2.4)

Run the following to test:
cd peripheral/tests
mkdir -p build && cd build
cmake ..
make
ctest

Example Results
Start 1: battery
1/2 Test #1: battery ..........................   Passed    0.29 sec
    Start 2: heart_rate
2/2 Test #2: heart_rate .......................   Passed    0.22 sec


Building and Running
********************

nrf Connect SDK: v3.2.4
nrf Connect Toolchain: v3.2.4
Board target: nrf5340dk/nrf5340/cpuapp (or any board with BLE support)
Base configuration file: prj.conf

In VSCode, once the SDK and Toolchain have been installed and nrf Connect extension added.
1. Create the build configuration with above details
2. Build firmware
3. Output firmware file will be generated under peripheral/build/peripheral/zephyr/zephyr.hex
   zephyr.hex can be flashed onto nordic board using VSCode, nRF Connect Programmer or JFlashLite
