/*
 *  Serial.cpp
 *  sadt
 *
 *  Created by Dave Dribin on 12/16/05.
 *  Copyright 2005 __MyCompanyName__. All rights reserved.
 *
 */

#include "unix.h"

#include <exception>
#include <ios>
#include <sstream>
#include <iostream>

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <errno.h>
#include <IOKit/serial/ioss.h>

using namespace adt;
using std::string;
using std::ios_base;
using std::ostringstream;
using std::cerr;
using std::endl;

#define CLASS UnixSerial

void CLASS::assertSuccess(int rc, string prefix)
{
    if (rc != -1)
		return;

	const char * error = strerror(errno);
	ostringstream message;
    if (!prefix.empty())
    {
        message << "Could not " << prefix << ": ";
    }
    message << "error " << error  << " (" << errno << ")" << " on " << mDeviceName;
    throw ios_base::failure(message.str());
}

void CLASS::assertTrue(bool value, string message)
{
	if (value)
		return;
	
	throw ios_base::failure(message);
}

CLASS::CLASS(string deviceName)
  : mDeviceName(deviceName)
{
	mFileDescriptor = -1;
}

void CLASS::open()
{
    // Open the serial port read/write, with no controlling terminal, and don't wait for a connection.
    // The O_NONBLOCK flag also causes subsequent I/O on the device to be non-blocking.
    // See open(2) ("man 2 open") for details.
    
    mFileDescriptor = ::open(mDeviceName.c_str(), O_RDWR | O_NOCTTY | O_NONBLOCK);
	assertSuccess(mFileDescriptor, "open serial port");

    // Note that open() follows POSIX semantics: multiple open() calls to the same file will succeed
    // unless the TIOCEXCL ioctl is issued. This will prevent additional opens except by root-owned
    // processes.
    // See tty(4) ("man 4 tty") and ioctl(2) ("man 2 ioctl") for details.
    assertSuccess(ioctl(mFileDescriptor, TIOCEXCL), "set TIOCEXCL");
    
    int fileStatus = fcntl(mFileDescriptor, F_GETFL);
    fileStatus &= ~O_NONBLOCK;

    // Now that the device is open, clear the O_NONBLOCK flag so subsequent I/O will block.
    // See fcntl(2) ("man 2 fcntl") for details.
    assertSuccess(fcntl(mFileDescriptor, F_SETFL, fileStatus), "clear O_NONBLOCK");
    fileStatus = fcntl(mFileDescriptor, F_GETFL);
    
    // Get the current options and save them so we can restore the default settings later.
    getAttributes(&mOriginalTTYAttrs, "get TTY attributes in open");

    // The serial port attributes such as timeouts and baud rate are set by modifying the termios
    // structure and then calling tcsetattr() to cause the changes to take effect. Note that the
    // changes will not become effective without the tcsetattr() call.
    // See tcsetattr(4) ("man 4 tcsetattr") for details.
    
    struct termios options = mOriginalTTYAttrs;
    setBaudRate(9600);
    setDataBits(8);
    setStopBits(1);
    setParity(NO_PARITY);
    setHardwareFlowControl(true);
    setSoftwareFlowControl(false);

    getAttributes(&options, "set IFLOW OFLOW");
	cfmakeraw(&options);
	options.c_cflag |= CDTR_IFLOW;
	options.c_cflag |= CDSR_OFLOW;
	options.c_cflag |= CLOCAL;
	setAttributes(&options, "set IFLOW OFLOW");
	
    setMinBytesToRead(1, 1);

	tcflush(mFileDescriptor, TCIOFLUSH);
}

CLASS::~CLASS()
{
	close();
}

void CLASS::close()
{
	if (mFileDescriptor == -1)
		return;

	cerr << "Closing " << mDeviceName << endl;
    // Block until all written output has been sent from the device.
    // Note that this call is simply passed on to the serial device driver. 
	// See tcsendbreak(3) ("man 3 tcsendbreak") for details.
    if (tcdrain(mFileDescriptor) == -1)
    {
        printf("Error waiting for drain - %s(%d).\n",
            strerror(errno), errno);
    }
    
    // Traditionally it is good practice to reset a serial port back to
    // the state in which you found it. This is why the original termios struct
    // was saved.
    if (tcsetattr(mFileDescriptor, TCSANOW, &mOriginalTTYAttrs) == -1)
    {
        printf("Error resetting tty attributes - %s(%d).\n",
            strerror(errno), errno);
    }

	if (::close(mFileDescriptor) == -1)
	{
		cerr << "Error closing file descriptor: " << strerror(errno) << " ("
			<< errno << ")" << endl;
	}

	mFileDescriptor = -1;
}

void CLASS::getAttributes(struct termios * options, std::string prefix)
{
	assertSuccess(tcgetattr(mFileDescriptor, options), prefix);
}

void CLASS::setAttributes(struct termios * options, std::string prefix)
{
	assertSuccess(tcsetattr(mFileDescriptor, TCSANOW, options), prefix);
}

void CLASS::setBaudRate(int baudRate)
{
	struct termios options;
	getAttributes(&options, "set baud rate");
	assertSuccess(cfsetspeed(&options, baudRate), "set baud rate");
	setAttributes(&options, "set baud rate");
    assertSuccess(ioctl(mFileDescriptor, IOSSIOSPEED, &baudRate), "ioctl baud rate");
}

	
void CLASS::setDataBits(int dataBits)
{
	static const int BITS_TO_SIZE [4] = { CS5, CS6, CS7, CS8 };	// Use (dataBits - 5) as index
	assertTrue((dataBits >= 5) && (dataBits <= 8), "set data bits");
	struct termios options;
	getAttributes(&options, "set data bits");
	options.c_cflag = (options.c_cflag & ~CSIZE) | BITS_TO_SIZE[dataBits - 5];
	setAttributes(&options, "set data bits");
}
	
void CLASS::setStopBits(int stopBits)
{
	assertTrue((stopBits == 1) || (stopBits == 2), "set stop bits");
	struct termios options;
	getAttributes(&options, "set stop bits");
	if (stopBits == 1)
		options.c_cflag &= ~CSTOPB;
	else
		options.c_cflag |= CSTOPB;
	setAttributes(&options);
}
	
void CLASS::setParity(Parity parity)
{
	static const int PARITY_MASK = PARENB | PARODD;
	int parityValue;
	switch(parity)
	{
		case NO_PARITY:
			parityValue = 0;
			break;
			
		case EVEN_PARITY:
			parityValue = PARENB;
			break;
			
		case ODD_PARITY:
			parityValue = PARENB | PARODD;
			break;
	}
	struct termios options;
	getAttributes(&options, "set parity");
	options.c_cflag = (options.c_cflag & ~PARITY_MASK) | parityValue;
	setAttributes(&options, "set parity");
}
	
void CLASS::setHardwareFlowControl(bool hardwareFlowControl)
{
	struct termios options;
	getAttributes(&options, "set hw flow control");
	if (hardwareFlowControl)
		options.c_cflag |= CRTSCTS;
	else
		options.c_cflag &= ~CRTSCTS;
	setAttributes(&options, "set hw flow control");
}
	
void CLASS::setSoftwareFlowControl(bool softwareFlowControl)
{
	struct termios options;
	static const int cXOnXOff = IXON | IXOFF;
	getAttributes(&options, "set sw flow control");
	if (softwareFlowControl)
		options.c_iflag |= cXOnXOff;
	else
		options.c_iflag &= ~cXOnXOff;
	setAttributes(&options, "set sw flow control");
}

void CLASS::setMinBytesToRead(unsigned char minBytes, unsigned char timeout)
{
	struct termios options;
	getAttributes(&options, "set min bytes to read");
	options.c_cc[VMIN] = minBytes;
    options.c_cc[VTIME] = timeout;
	setAttributes(&options);
}
	
int CLASS::receiveByte()
{
	Uint8 byte;
	int bytesRead;
	assertSuccess(bytesRead = read(mFileDescriptor, &byte, 1), "read byte");
	if (bytesRead == 0)
	{
		return -1;
	}
	return byte;
}
	
void CLASS::sendByte(Uint8 byte)
{
	assertSuccess(write(mFileDescriptor, &byte, 1), "write byte");
}
