#include <iostream>
#include <fstream>
#include <sstream>
#include "unix.h"
#include "dosxfer.h"

using namespace adt;
using std::string;
using std::cout;
using std::cerr;
using std::endl;
using std::ofstream;
using std::stringstream;

void sigint_handler(int i)
{
}

class CliPlatform : public AdtPlatform
{
    
};

int main(int argc, char ** argv)
{
    try
    {
        // Allow SIGINT to cause EINTR on read(2)
        struct sigaction new_action;
        new_action.sa_handler = sigint_handler;
        sigemptyset(&new_action.sa_mask);
        new_action.sa_flags = 0;
        sigaction(SIGINT, &new_action, NULL);
        
        if (argc != 3)
        {
            cerr << "Usage: adt <device> <baud>" << endl;
            return 1;
        }
        string device = argv[1];
        stringstream baudString(argv[2]);
        int baud;
        baudString >> baud;
        
        cout << "Using device " << device << " at " << baud << " bps" << endl;

        UnixSerial serialPort(device);
        serialPort.open();
        serialPort.setBaudRate(baud);
        serialPort.setDataBits(8);
        serialPort.setStopBits(1);
        serialPort.setParity(UnixSerial::NO_PARITY);
        serialPort.setHardwareFlowControl(false);
        serialPort.setSoftwareFlowControl(false);
        serialPort.setMinBytesToRead(1, 1);
        
        int command = serialPort.receiveByte() & 0x7F;
        if (command != 'S')
        {
            cerr << "Unknown command: " << command << endl;
            return 1;
        }
        cout << "Receiving disk" << endl;

        CliPlatform platform;
        DosReceiver receiver;
        receiver.execute(serialPort, platform);
        ofstream diskFile(receiver.getFileName().c_str());
        diskFile << receiver.getDisk();
        diskFile.close();
    }
    catch (std::exception & e)
    {
        cerr << e.what() << endl;
        return 2;
    }
    
    return 0;
}