#ifndef ADT_UNIX_H
#define ADT_UNIX_H

#include <termios.h>
#include "serial.h"

namespace adt {

class UnixSerial : public Serial
{
  public:
  	UnixSerial(std::string deviceName);

  	~UnixSerial();

  	void open();

  	void close();

  	void setBaudRate(int baudRate);

  	void setDataBits(int dataBits);

  	void setStopBits(int stopBits);

  	enum Parity {
  		NO_PARITY,
  		EVEN_PARITY,
  		ODD_PARITY
  	};

  	void setParity(Parity parity);

  	void setHardwareFlowControl(bool hardwareFlowControl);

  	void setSoftwareFlowControl(bool softwareFlowControl);

  	void setMinBytesToRead(unsigned char minBytes, unsigned char timeout);

  	int receiveByte();

  	void sendByte(Uint8 byte);

  private:
  	std::string mDeviceName;
  	int mFileDescriptor;
  	struct termios mOriginalTTYAttrs;

  	void assertSuccess(int rc, std::string prefix = "");

  	void assertTrue(bool value, std::string message = "");

  	void getAttributes(struct termios * options, std::string prefix = "");
  	void setAttributes(struct termios * options, std::string prefix = "");
};

};

#endif