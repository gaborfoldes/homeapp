
#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"
#include "printf.h"



/* Declarations */

const unsigned int TEMPERATURE = 1;
const unsigned int MOTION = 2;
struct Signals {
  unsigned long sensor_type;
  unsigned long data;
};

int PIR_sensor = 2;

// Set up nRF24L01 radio on SPI bus plus pins 9 & 10 
RF24 radio(9,10);

// Radio pipe addresses for the 2 nodes to communicate.
const uint64_t tx_addr = 0xF0F0F0F0E2LL;
const uint64_t base_addr = 0xF0F0F0F0D2LL;

void setup(void)
{

  pinMode(PIR_sensor, INPUT);
  
  // Serial.begin(57600);
  // printf_begin();

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
  // radio.printDetails();
  
  // Need to wait a few seconds at startup so that the bootloader doesn't think we are in upload mode
  delay(10000);
}

void loop(void)
{
  // Repeatedly send the current temperature

    radio.stopListening();

    while (!digitalRead(PIR_sensor)) {}
    
    struct Signals signal;
    signal.sensor_type = MOTION;
    signal.data = 1;

    // Take the time, and send it.  This will block until complete
    unsigned long time = millis();
//    printf("Now sending %lu...",Tf);
    bool ok = radio.write( &signal, sizeof(signal) );
    
/*
    if (ok)
      printf("ok...\n\r");
    else
      printf("failed.\n\r");
*/

    // Now, continue listening
    radio.startListening();


    // Wait 10 sec until next read
    delay(10000);
 
}



