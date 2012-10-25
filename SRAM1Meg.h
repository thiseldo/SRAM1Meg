/*
SRAM lib written by by Carl Morey. Adaptations: Marcus Hund aka Marko Hiero
Modifications for 23LC1024 using 24 bit addressing by Andrew Lindsay andrew@thiseldo.co.uk

writestream: Setup the SRAM in sequential write mode starting from the passed address.
             Bytes can then be written one byte at a time using RWdata(byte data).
			 Each byte is stored in the next location and it wraps around 131071.

readstream:  Setup the SRAM in sequential read mode starting from the passed address.
             Bytes can then be read one byte at a time using  byte RWdata(0).The passed data is irrelavent.
			 Each byte is read from the next location and it wraps around 131071.

RWdata:      Write or read a byte at any time from the SRAM.
             If the writesteam is open the passed byte will be written to the current address.
			 If the readstream is open the byte from the current address will be returned.

closeRWstream: Use to close the  open read or write stream.
               Dont need when changing between read/write.
			   Close before using SPI somewhere else.

digital pin 13    SCK
digital pin 12    MISO
digital pin 11    MOSI
digital pin 9     SS
*/
#ifndef SRAM_h
#define SRAM_h

#if (ARDUINO >= 100)
#include <Arduino.h>
#else
#include <WProgram.h>
#endif


#define setupSPI SPCR = 0x50      //Master mode, MSB first, SCK phase low, SCK idle low, clock/4
#define setupDDRB DDRB |= 0x2c    //set  SCK(13) MOSI(11) and SS as output 
#define selectSS PORTB &= ~0x02   //set the SS to 0 to select MSH:use pin 9 for sram instead of 10
#define deselectSS PORTB |= 0x02  //set the SS to 1 to deselect 
#define SS9 9

class SRAMclass
{
public:

SRAMclass();  //the constructor
void writestream(long address);
void readstream(long address);
void closeRWstream(void);
byte RWdata(byte data);

};//end of class SRAMclass

 extern SRAMclass SRAM;

#endif
