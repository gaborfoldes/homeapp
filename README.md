homeapp
=======

Home automation system using simple/cheap components.

  * Arduino boards for sensors and actuators
  * Raspberry Pi for control board and web server
  * nRF24L01+ 2.4GHz wireless transceiver chips for remote communication
  * DS18B20 1-wire digital thermometers
  * Traditional or solid state 5v relays


## Devices (Arduino)

To begin with, you will need the RF24 library: https://github.com/maniacbug/RF24

To drive the thermometers, you also need to download the OneWire library for Arduino: http://www.pjrc.com/teensy/td_libs_OneWire.html


## Controller (Raspberry Pi)

The controller is used to:
  * Identify the sensors
  * Recieve data
  * Send control commands

First, get the Pi version of the RF24 library:

    cd controller-pi
    git clone https://github.com/stanleyseow/RF24.git
    cd RF24
    cd librf24-rpi/librf24
    make
    sudo make install

Then compile the controller:

    g++ -Wall -Ofast -mfpu=vfp -mfloat-abi=hard -march=armv6zk -mtune=arm1176jzf-s -LRF24/librf24-rpi/librf24/  -lrf24 controller.cpp -o controller

Before starting the controller, you need to enable access to the SPI device.  The easiest way to do this is through wiringPi's GPIO utility (http://wiringpi.com/):

    gpio load spi

Now you can start the controller. To run it in the background:

    nohup ./controller &
