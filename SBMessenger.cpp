/*
 * Copyright (C) 2015 Orange
 *
 * This software is distributed under the terms and conditions of the 'Apache-2.0'
 * license which can be found in the file 'LICENSE.txt' in this package distribution
 * or at 'http://www.apache.org/licenses/LICENSE-2.0'.
 */

/* Device for SimpleBee Protocol
 *
 * Version:     0.1.0
 * Created:     2015-02-24 by Franck Roudet
 */
#include "SBMessenger.h"


// **** Initialization ****

/**
 * CmdMessenger constructor
 */
SBMessenger::SBMessenger(Stream &ccomms, const char cmd_separator, SBDispatcherBase *dispatcher, const SBCheckSumFctType checksum)
{
    init(ccomms,cmd_separator);
    this->checksum = checksum;
    this->dispatcher = dispatcher;
    this->dispatcher->setMessenger(this);
}

/**
 * Enables printing newline after a sent command
 */
void SBMessenger::init(Stream &ccomms,const char cmd_separator) {
    comms            = &ccomms;
    command_separator = cmd_separator;
    reset();

    pauseProcessing   = false;
}

/**
 * Resets the command buffer and message state
 */
void SBMessenger::reset() {
	bufferIndex = 0; // command buffer
}





bool SBMessenger::checkMsgChecksum(const char * msg, int msglen) {
	if (checksum) {
	  char cks[CHECKSUMSIZE];
	  checksum((char * const)msg,msglen-CHECKSUMSIZE,cks);
	  msg+=msglen-CHECKSUMSIZE; // checksum at the end
	  for (int i=0; i<CHECKSUMSIZE; i++) {
		  if (cks[i] != msg[i]) {
			  return false;
		  }
	  }
	}
	return true;
}

/**
 * Reading a command a call dispatcher if exists
 */
void SBMessenger::monitor(void) {
    while ( !pauseProcessing && comms->available() ) {
		// The Stream class has a readBytes() function that reads many bytes at once. On Teensy 2.0 and 3.0, readBytes() is optimized.
		// Benchmarks about the incredible difference it makes: http://www.pjrc.com/teensy/benchmark_usb_serial_receive.html

		size_t bytesAvailable = min(comms->available(),STREAMBUFFERSIZE);
		comms->readBytes(streamBuffer, bytesAvailable);

		// Process the bytes in the stream buffer, and handles dispatches callbacks, if commands are received
		char serialChar;
		for (size_t byteNo = 0; byteNo < bytesAvailable ; byteNo++) {
			serialChar = streamBuffer[byteNo];
			if (serialChar == command_separator) { // Command complete
				commandBuffer[bufferIndex]=0;
				if(bufferIndex > 0) {
					// process the command
					// Verify checksum
					if (checkMsgChecksum(commandBuffer, bufferIndex )) {
						if (dispatcher) {
							dispatcher->dispatch(commandBuffer, bufferIndex);
						}
					}
				}
				reset();
			} else {
				commandBuffer[bufferIndex]=serialChar;
				bufferIndex++;
				if (bufferIndex >= (MESSENGERBUFFERSIZE-1)) reset(); // Problem, ignore command
			}
		}
	}
    // Force unknown device to get an address
	if (dispatcher) {
		dispatcher->heartbeat();
	}
}


/**
 * Send message
 */
void SBMessenger::send(const void * cmd, unsigned int cmdLen) {
	  comms->write((const uint8_t *) cmd, cmdLen);
	  if (checksum) {
		  char cks[CHECKSUMSIZE];
		  checksum((char * const)cmd,cmdLen,cks);
		  comms->write((const uint8_t *)cks,CHECKSUMSIZE);
	  }
	  comms->write(command_separator);
}
