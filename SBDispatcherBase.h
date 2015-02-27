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

#ifndef SBDISPATCHERBASE_H_
#define SBDISPATCHERBASE_H_

class SBMessenger; // forward declaration

/**
 * Simple Dispatcher Base.
 * Interface for distribute Simple message incoming form messenger to devices.
 */
class SBDispatcherBase {
protected:
	SBMessenger * sbmessenger=0L;    // Message manager

public:

	void setMessenger(SBMessenger * sbmessenger) {this->sbmessenger = sbmessenger;};

	/**
	 * Dispatcher message incoming form SBMessenger
	 */
	virtual void dispatch(char * msg, int len)=0;
  
	/**
	 * Check if Heart Beat message must be sent and if there is awaiting ident/address
	 */
	virtual void heartbeat(void) =0;
};


#endif // SBDISPATCHERBASE_H_
