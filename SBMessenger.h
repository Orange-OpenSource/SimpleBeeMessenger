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
#ifndef SBMESSENGER_H_
#define SBMESSENGER_H_

#include <inttypes.h>
#if ARDUINO >= 100
#include <Arduino.h> 
#else
#include <WProgram.h> 
#endif


#ifndef STREAMBUFFERSIZE
#	define STREAMBUFFERSIZE 24   // The length of the streambuffer  (default: 24) must be > MESSENGERBUFFERSIZE
#endif

#ifndef MESSENGERBUFFERSIZE
#	define MESSENGERBUFFERSIZE 10   // The length of the commandbuffer  (default: 10)
#endif
#ifndef CHECKSUMSIZE // for overidding
#	define CHECKSUMSIZE         2   //
#endif

#include <SBDispatcherBase.h>
#include <sbdevice.h>

// Message States
enum {
  kProccesingMessage,            // Message is being received, not reached command separator
  kEndOfMessage,				 // Message is fully received, reached command separator
};

class SBDevice;
/*
 * CallBack checksum function type
 */
typedef void (*SBCheckSumFctType)(char * const start, int length, char * dest);


class SBMessenger {
private:

  // **** Private variables *** 
  
  bool    startCommand;            // Indicates if sending of a command is underway
  bool pauseProcessing;             // pauses processing of new commands, during sending
  char commandBuffer[MESSENGERBUFFERSIZE]; // Buffer command that holds the data
  uint8_t bufferIndex;              // Index where to write data in buffer
  char streamBuffer[STREAMBUFFERSIZE]; // Buffer command that holds the data
  Stream *comms;                    // Serial data stream
  SBCheckSumFctType checksum;       // checksum function
  char command_separator;           // Character indicating end of command (default: '\r')
  SBDispatcherBase * dispatcher;    // Message Dispatcher
  // **** Initialize ****
  
  void init (Stream & comms, const char cmd_separator);
  void reset ();
  
  bool checkMsgChecksum(const char * msg, int msglen);

public:

  // ****** Public functions ******

  // **** Initialization ****
  
  SBMessenger (Stream & comms,
				const char cmd_separator = '\r',
				SBDispatcherBase *dispatcher=NULL,
				const SBCheckSumFctType checksum=NULL);

  /**
   * Setting devices receive message
   */
  void setDeviceList(SBDevice *devList);

  // **** Command processing ****
  
  void monitor (void);
  bool available ();
  bool isArgOk ();
  uint8_t CommandID ();
  
  /**
   * Send message
   */
  void send(const void * cmd, unsigned int cmdLen) ;

};
#endif // SBMESSENGER_H_
