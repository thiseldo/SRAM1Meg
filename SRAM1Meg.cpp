/*
writestream: Setup the SRAM in sequential write mode starting from the passed address.
             Data can then be written one byte at a time using RWdata(byte data).
			 Each byte is stored in the next location and it wraps around 32767.

readstream:  Setup the SRAM in sequential read mode starting from the passed address.
             Data can then be read one byte at a time using  byte RWdata(0).The passed data is irrelavent.
			 Each byte is read from the next location and it wraps around 32767.

RWdata:      Write or read the data from the SRAM.
             If the writesteam is open the passed data will be written to the current address.
			 If the readstream is open the data from the current address will be returned.

closeRWstream: Use to close the  open read or write stream.
               Dont need when changing between read/write.
               Close before using SPI somewhere else.

digital pin 13    SCK
digital pin 12    MISO
digital pin 11    MOSI
digital pin  9    SS
*/

#include "SRAM1Meg.h"

SRAMclass::SRAMclass()  //constructor
{
	setupDDRB;
	setupSPI;
	pinMode(SS9, OUTPUT);
	selectSS;
	RWdata(0x05);//read status register
	int Sreg = RWdata(0xff);//get status value
	deselectSS;

	if(Sreg != 0x41) //are we in sequential mode
	{
		selectSS;
		RWdata(0x01); //write to status reg
		RWdata(0x41);  //set sequencial  mode
		deselectSS;
	}//end of set sequential mode
}//end of constructor

void SRAMclass::writestream(long address)
{
	deselectSS;  //deselect if still selected
	digitalWrite(10,HIGH);
	selectSS; //select now
	RWdata(0x02);//write to address
	RWdata((address >> 16) & 0xFF);//read from address
	RWdata((address >> 8) & 0xFF);//read from address
	RWdata(address);//lsb address 
}//end of write stream

void SRAMclass::readstream(long address)
{
	deselectSS;  //deselect if still selected
	selectSS; //select now
	RWdata(0x03);//read from address
	RWdata((address >> 16) & 0xFF);//read from address
	RWdata((address >> 8) & 0xFF);//read from address
	RWdata(address); 
}//end of read stream

void SRAMclass::closeRWstream(void)
{
	deselectSS;  //deselect
}//end of close RW stream

byte SRAMclass::RWdata(byte data)
{
 	SPDR = data;
  	while (!(SPSR & _BV(SPIF)))
    		;
  	return SPDR;
}//end of RWdata

SRAMclass SRAM;
