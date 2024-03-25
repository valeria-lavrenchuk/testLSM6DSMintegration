# testLSM6DSMintegration

## Overview
This test app integrates an LSM6DSM sensor driver with an inclinometer application. The inclinometer calculates the angle based on accelerometer and gyroscope data from the sensor. 

Test aplication consist from several components: 

Main Program: This is the entry point of your firmware framework.  In real project it will be part of main Loundpeacker app

Inclinometer App: This module contains the application logic related to the inclinometer functionality and Complementary Filter to periodically assess the inclination value in an RTOS environment.It interacts with the middleware layer to access sensor data and perform calculations.
Platform: This layer provides an abstraction over low-level hardware interfaces and facilitates communication between the application and the driver, abstracts platform-specific functionality such as I2C communication. It includes the platform.c and platform.h files.
Driver: This layer interfaces directly with the hardware, providing functions to communicate with the LSM6DSM sensor. It includes the lsm6dsm_reg.c and lsm6dsm_reg.h files.

## Build Instructions
To build the project, simply run the following command in your terminal:

make

This command will compile the source code and generate an app.

## Clean Up
To clean up the project, use the following command:

make clean

## Running the Test App
Once the project is built, run app with a command:

./inclinometer_app

This will run app, but proper setup is needed to work.

## Running the Test App
Once the project is built, run comand to run unit test:

make test_inclinometer

then: 

./test_inclinometer

This will run tests, present in /test folder
