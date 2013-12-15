#define __STDC_FORMAT_MACROS
#include <cstdlib>
#include <iostream>
#include <time.h>
#include <fstream>

#include "./RF24/librf24-rpi/librf24/RF24.h"

/* Declarations */

// Set up radio -- SPI device, speed, CE pin (only CE is NEEDED in RPI)
RF24 radio("/dev/spidev0.0", 8000000, 25);

// Radio pipe addresses for the 2 nodes to communicate
const uint64_t pipes[4] = { 0xF0F0F0F0D2LL, 0xF0F0F0F0E1LL, 0xF0F0F0F0E2LL, 0xF0F0F0F0E3LL };
const char pipe_name[4][64] = { "Base", "Living room", "Portable", "Heat" };

// log file
ofstream logfile;
//const char[24] LOG_FILE = "/data/test_temp";

/* Initialize */

void setup(char* heater_state) {

  unsigned long heater;

  if (strcmp(heater_state, "on") == 0)
    heater = 1;
  else
    heater = 0;

  // Start rf radio configuration
  radio.begin();

  // Increase the delay between retries & # of retries (optional)
  radio.setRetries(15,15);

  // Reduce the payload size -- might improve reliability (optional)
  // radio.setPayloadSize(8);
  radio.setChannel(0x4c);
  radio.setPALevel(RF24_PA_LOW);

  // Open pipes for communication
  // Open 'our' pipe for writing
  // Open the 'other' pipe for reading, in position #1 (we can have up to 5 pipes open for reading)

    radio.openWritingPipe(pipes[0]);
    //radio.openReadingPipe(1,pipes[1]);
    //radio.openReadingPipe(2,pipes[2]);
    //radio.openReadingPipe(3,pipes[3]);

  // Stop listening
  radio.stopListening();
  //heater = 0;
  printf("Sending to heater: %lu...", heater);
  bool ok = radio.write( &heater, sizeof(unsigned long) );

  if (ok)
    printf("ok.\n");
  else
    printf("failed.\n");

}

int main(int argc, char** argv)
{
        setup(argv[1]);
        return 0;
}


// vim:cin:ai:sts=2 sw=2 ft=cpp
