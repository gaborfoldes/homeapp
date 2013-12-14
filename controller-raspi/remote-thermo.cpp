#define __STDC_FORMAT_MACROS
#include <cstdlib>
#include <iostream>
#include <time.h>
#include <fstream>

#include "../RF24.h"

/* Declarations */

// Set up radio -- SPI device, speed, CE pin (only CE is NEEDED in RPI)
RF24 radio("/dev/spidev0.0", 8000000, 25);

// Radio pipe addresses for the 2 nodes to communicate
const uint64_t pipes[3] = { 0xF0F0F0F0D2LL, 0xF0F0F0F0E1LL, 0xF0F0F0F0E2LL };
const char pipe_name[3][64] = { "Base", "Living room", "Portable" };

// log file
ofstream logfile;
//const char[24] LOG_FILE = "/data/test_temp";

/* Initialize */

void setup(void) {

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
    radio.openReadingPipe(1,pipes[1]);
    radio.openReadingPipe(2,pipes[2]);

  // Start listening
  radio.startListening();
  printf("Listening to radio...\n\r");

  // Dump the configuration of the rf unit for debugging
  radio.printDetails();

  // Open log file
  logfile.open ("/data/test_temp", fstream::out | fstream::app);

}

void loop(void) {
    uint8_t pipe_num;
    // if there is data ready
    if ( radio.available(&pipe_num) )
    {
      // Dump the payloads until we've gotten everything
      unsigned long got_time;
      bool done = false;
      while (!done)
      {
        // Fetch the payload, and see if this was the last one.
        done = radio.read( &got_time, sizeof(unsigned long) );

        // Spew it
 	time_t now;
    	time(&now);
    	char jsontime[sizeof "2011-10-08T07:07:09Z"];
    	strftime(jsontime, sizeof jsontime, "%FT%TZ", gmtime(&now));
	double c_temp = (double)got_time / 100;
	double f_temp = (c_temp * 9 / 5 ) + 32;
	char data[255];
        sprintf(data, "{\"device_id\": \"%" PRIx64 "\", \"device_name\": \"%s\", \"timestamp\": \"%s\", \"celsius\": %.2f, \"fahrenheit\": %.2f}\n",
		pipes[pipe_num], pipe_name[pipe_num], jsontime, c_temp, f_temp);
        //printf(data);
	logfile << data;
	logfile.flush();

	// Delay just a little bit to let the other unit
	// make the transition to receiver
	//delay(20);
      }

      // First, stop listening so we can talk
      //radio.stopListening();

      // Send the final one back.
      //printf("Sent response.\n\r");
      //radio.write( &got_time, sizeof(unsigned long) );

      // Now, resume listening so we catch the next packets.
      //radio.startListening();
    }
}


int main(int argc, char** argv)
{
        setup();
        while(1)
                loop();

        return 0;
}


// vim:cin:ai:sts=2 sw=2 ft=cpp
