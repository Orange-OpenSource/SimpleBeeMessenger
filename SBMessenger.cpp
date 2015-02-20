#include "SBMessenger.h"


// **** Initialization ****

/**
 * CmdMessenger constructor
 */
SBMessenger::SBMessenger(Stream &ccomms, const char cmd_separator, const SBCheckSumFctType checksum)
{
    init(ccomms,cmd_separator);
    this->checksum = checksum;
}

/**
 * Enables printing newline after a sent command
 */
void SBMessenger::init(Stream &ccomms,const char cmd_separator)
{
    comms            = &ccomms;
    command_separator = cmd_separator;
    bufferLength      = MESSENGERBUFFERSIZE;
    bufferLastIndex   = MESSENGERBUFFERSIZE -1;
    reset();

    pauseProcessing   = false;
}

/**
 * Resets the command buffer and message state
 */
void SBMessenger::reset()
{
    bufferIndex = 0;
    current     = NULL;
    last        = NULL;
}
