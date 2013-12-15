
#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"
#include "printf.h"


/* Declarations */

// Set up nRF24L01 radio on SPI bus plus pins 9 & 10 
RF24 radio(9,10);

// Radio pipe addresses for the 2 nodes to communicate.
const uint64_t tx_addr = 0xF0F0F0F0E3LL;
const uint64_t base_addr = 0xF0F0F0F0D2LL;

const int heat_ctrl = 8;

void setup(void)
{

  Serial.begin(57600);
  printf_begin();

  pinMode(heat_ctrl, OUTPUT);     

  //
  // Setup and configure rf radio
  //

  radio.begin();

  // optionally, increase the delay between retries & # of retries
  radio.setRetries(15,15);

  // optionally, reduce the payload size.  seems to
  // improve reliability
  //radio.setPayloadSize(8);


  radio.openWritingPipe(tx_addr);
  radio.openReadingPipe(1,base_addr);

  // Dump the configuration of the rf unit for debugging
  //radio.printDetails();
  
  // Need to wait a few seconds at startup so that the bootloader doesn't think we are in upload mode
  delay(10000);
  
  radio.startListening();
}

void loop(void)
{

    uint8_t pipe_num;
    // if there is data ready
    if ( radio.available(&pipe_num) )
    {
      // Dump the payloads until we've gotten everything
      unsigned long heater;
      bool done = false;
      while (!done)
      {
        // Fetch the payload, and see if this was the last one.
        done = radio.read( &heater, sizeof(unsigned long) );
        if (pipe_num == 1) {
          if (heater == 1)
            digitalWrite(heat_ctrl, HIGH);
          else
            digitalWrite(heat_ctrl, LOW);
        }
        printf("Received from %i, state %lu\n", pipe_num, heater); 
      }

    }
 
}

