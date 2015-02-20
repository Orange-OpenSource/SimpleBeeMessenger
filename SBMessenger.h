
#ifndef SBMessenger_h
#define SBMessenger_h

#include <inttypes.h>
#if ARDUINO >= 100
#include <Arduino.h> 
#else
#include <WProgram.h> 
#endif

//#include "Stream.h"

#ifndef MESSENGERBUFFERSIZE
#	define MESSENGERBUFFERSIZE 24   // The length of the commandbuffer  (default: 24)
#endif
#ifndef CHECKSUMSIZE // for overidding
#	define CHECKSUMSIZE         2   //
#endif

// Message States
enum {
  kProccesingMessage,            // Message is being received, not reached command separator
  kEndOfMessage,				 // Message is fully received, reached command separator
};

/*
 * CallBack checksum function type
 */
typedef void (*SBCheckSumFctType)(char * const start, int length, char * dest);


class SBMessenger {
private:

  // **** Private variables *** 
  
  bool    startCommand;            // Indicates if sending of a command is underway
  uint8_t bufferIndex;              // Index where to write data in buffer
  uint8_t bufferLength;             // Is set to MESSENGERBUFFERSIZE
  uint8_t bufferLastIndex;          // The last index of the buffer
  char CmdlastChar;                 // Bookkeeping of command escape char 
  bool pauseProcessing;             // pauses processing of new commands, during sending
  char commandBuffer[MESSENGERBUFFERSIZE]; // Buffer that holds the data
  uint8_t messageState;             // Current state of message processing
  char *current;                    // Pointer to current buffer position
  char *last;                       // Pointer to previous buffer position
  Stream *comms;                    // Serial data stream
  SBCheckSumFctType checksum;       //
  char command_separator;           // Character indicating end of command (default: '\r')

  // **** Initialize ****
  
  void init (Stream & comms, const char cmd_separator);
  void reset ();
  
  // **** Command processing ****
  
  //inline uint8_t processLine (char serialChar) __attribute__((always_inline));
  //inline void handleMessage() __attribute__((always_inline));
  //inline bool blockedTillReply (unsigned long timeout = DEFAULT_TIMEOUT, int ackCmdId = 1) __attribute__((always_inline));
  //inline bool CheckForAck (int AckCommand) __attribute__((always_inline));

    
  // **** Command receiving ****
  
  //int findNext (char *str, char delim);

  
public:

  // ****** Public functions ******

  // **** Initialization ****
  
  SBMessenger (Stream & comms,
				const char cmd_separator = '\r',
				const SBCheckSumFctType checksum= NULL);

  // **** Command processing ****
  
  void feedinSerialData ();
  bool available ();
  bool isArgOk ();
  uint8_t CommandID ();
  
  // ****  Command sending ****
  
  void send(const void * cmd, unsigned int cmdLen) {
	  comms->write((const uint8_t *) cmd, cmdLen);
	  if (checksum) {
		  char cks[CHECKSUMSIZE];
		  checksum((char * const)cmd,cmdLen,cks);
		  comms->write((const uint8_t *)cks,CHECKSUMSIZE);
	  }
	  comms->write(command_separator);
  }

};
#endif
