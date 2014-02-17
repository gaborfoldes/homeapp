
#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"
#include "printf.h"

#include <OneWire.h>


/* Declarations */

OneWire ds(8);

// Set up nRF24L01 radio on SPI bus plus pins 9 & 10 
RF24 radio(9,10);

// Radio pipe addresses for the 2 nodes to communicate.
const uint64_t tx_addr = 0xF0F0F0F0E3LL;
const uint64_t base_addr = 0xF0F0F0F0D2LL;

unsigned long Tf;

void setup(void)
{

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

    Tf = getTemperature();

    // Take the time, and send it.  This will block until complete
    unsigned long time = millis();
//    printf("Now sending %lu...",Tf);
    bool ok = radio.write( &Tf, sizeof(unsigned long) );
    
/*
    if (ok)
      printf("ok...\n\r");
    else
      printf("failed.\n\r");
*/

    // Now, continue listening
    radio.startListening();


    // Wait 1 min until next read
    delay(59000);
 
}

unsigned long getTemperature(void) {
  
  byte i;
  byte present = 0;
  byte data[12];
  byte addr[8];
  
  int HighByte, LowByte, TReading, SignBit, Tc_100, Tf_100;
  
  byte debug;
  debug = 0;


  ds.reset_search();
  ds.search(addr);

  ds.reset();
  ds.select(addr);
  ds.write(0x44,1);         // start conversion, with parasite power on at the end

  delay(1000);     // maybe 750ms is enough, maybe not
  // we might do a ds.depower() here, but the reset will take care of it.

  present = ds.reset();
  ds.select(addr);    
  ds.write(0xBE);         // Read Scratchpad

  for ( i = 0; i < 9; i++) {           // we need 9 bytes
    data[i] = ds.read();
  }

  LowByte = data[0];
  HighByte = data[1];
  TReading = (HighByte << 8) + LowByte;
  SignBit = TReading & 0x8000;  // test most sig bit
  if (SignBit) // negative
  {
    TReading = (TReading ^ 0xffff) + 1; // 2's comp
  }
  Tc_100 = (6 * TReading) + TReading / 4;    // multiply by (100 * 0.0625) or 6.25
  //Tf_100 = 3200 + Tc_100 * 9 / 5; 

  return Tc_100;

}

// vim:cin:ai:sts=2 sw=2 ft=cpp
