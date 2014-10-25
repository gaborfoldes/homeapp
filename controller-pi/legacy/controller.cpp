#define __STDC_FORMAT_MACROS
#include <cstdlib>
#include <iostream>
#include <time.h>
#include <fstream>

#include "./RF24/librf24-rpi/librf24/RF24.h"

#include <curl/curl.h>


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
const char pipe_name[4][64] = { "Base", "Kiki's room", "Entrance", "Living room" };

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
    radio.openReadingPipe(3,pipes[3]);

  // Start listening
  radio.startListening();
  printf("Listening to radio...\n\r");

  // Dump the configuration of the rf unit for debugging
  radio.printDetails();

  // Open log file
  logfile.open ("/data/test_temp", fstream::out | fstream::app);

}

int store_signal(char signal[])
{
  CURL *curl;
  CURLcode res;
 
  curl = curl_easy_init();
  if(curl) {

    struct curl_slist *headers = NULL;
    headers = curl_slist_append(headers, "Accept: application/json");
    headers = curl_slist_append(headers, "Content-Type: application/json");
    headers = curl_slist_append(headers, "charsets: utf-8");

    curl_easy_setopt(curl, CURLOPT_URL, "http://localhost/signal");
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers); 
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, signal);

    res = curl_easy_perform(curl);
    /* Check for errors */ 
    if(res != CURLE_OK)
      fprintf(stderr, "curl_easy_perform() failed: %s\n",
              curl_easy_strerror(res));

    /* always cleanup */ 
    curl_easy_cleanup(curl);
  }
  return 0;
}


void loop(void) {
    uint8_t pipe_num;
    // if there is data ready
    if ( radio.available(&pipe_num) )
    {
      // Dump the payloads until we've gotten everything

      time_t now;
      time(&now);
      char jsontime[sizeof "2011-10-08T07:07:09Z"];
      strftime(jsontime, sizeof jsontime, "%FT%TZ", gmtime(&now));

      bool done = false;
      while (!done)
      {

        struct Signals signal;
        char json[255];

        // Fetch the payload, and see if this was the last one.
        done = radio.read( &signal, sizeof(signal) );

	switch (signal.sensor_type) {

        case MOTION:
          sprintf(json, "{\"device_id\": \"%" PRIx64 "\", \"device_name\": \"%s\", \"timestamp\": \"%s\", \"motion_detected\": %lu}\n",
	    pipes[pipe_num], pipe_name[pipe_num], jsontime, signal.data);
          break;

        case TEMPERATURE:
	  double c_temp = (double)signal.data / 100;
	  double f_temp = (c_temp * 9 / 5 ) + 32;
          sprintf(json, "{\"device_id\": \"%" PRIx64 "\", \"device_name\": \"%s\", \"timestamp\": \"%s\", \"celsius\": %.2f, \"fahrenheit\": %.2f}\n",
	    pipes[pipe_num], pipe_name[pipe_num], jsontime, c_temp, f_temp);
          break;

        }

        if (strlen(json) > 0) {

          store_signal(json);
	  logfile << json;
	  logfile.flush();

        }

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
