homeapp
=======

## Controller (Raspberry Pi)

The controller is used to:
1. Identify the sensors
2. Recieve data
3. Send control commands

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
