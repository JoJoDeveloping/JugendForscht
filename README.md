Code to access the MPU9250 inertial reference sensor with an Arduino, storing data on a SD card.
The code is published here for Jugend Forscht.

The whole thing is written to consume as little space as possible. Therefore, there are no support libaries to access the MPU9250, everything is directly done via register writes and reads.
I don't think you will find this particularly useful, but it may help implementing a driver for the MPU9250 yourself.
