#define __STDC_FORMAT_MACROS
#include <cstdlib>
#include <iostream>

#include "./RF24/librf24-rpi/librf24/RF24.h"


/* Declarations */

const unsigned int TEMPERATURE = 1;
const unsigned int MOTION = 2;
struct Signals {
  unsigned long sensor_type;
  unsigned long data;
};

// Set up radio -- SPI device, speed, CE pin (only CE is NEEDED in RPI)
RF24 radio("/dev/spidev0.0", 8000000, 24);

// Radio pipe addresses for the 2 nodes to communicate
const uint64_t pipes[4] = { 0xF0F0F0F0D2LL, 0xF0F0F0F0E1LL, 0xF0F0F0F0E2LL, 0xF0F0F0F0E3LL };
const char pipe_name[4][64] = { "Base", "Kikis room", "Entrance", "Living room" };


void setup(void) {

  radio.begin();
  radio.setRetries(15,15);
  radio.setChannel(0x4c);
  radio.setPALevel(RF24_PA_HIGH);
  radio.openWritingPipe(pipes[0]);
  radio.openReadingPipe(1,pipes[1]);
  radio.openReadingPipe(2,pipes[2]);
  radio.openReadingPipe(3,pipes[3]);
  radio.startListening();
  printf("Listening to radio...\n\r");
  radio.printDetails();

}


void loop(void) {
    uint8_t pipe_num;
    // if there is data ready
    if ( radio.available(&pipe_num) )
    {
      bool done = false;
      while (!done)
      {

	struct Signals signal;
        done = radio.read( &signal, sizeof(signal) );
	printf( "type: %lu, data: %lu\n", signal.sensor_type, signal.data);

      }

    }
}


int main(int argc, char** argv)
{
  setup();
  while(1)
    loop();
  return 0;
}

