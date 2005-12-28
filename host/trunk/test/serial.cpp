#include <cppunit/extensions/HelperMacros.h>
#include <sstream>
#include "testUtil.h"
#include "TestSerial.h"
#include "serial.h"
#include "crc.h"
#include "util.h"

using namespace adt;
using namespace std;

#define CLASS SerialTest

class CLASS : public CPPUNIT_NS::TestFixture
{
    CPPUNIT_TEST_SUITE(CLASS);
    CPPUNIT_TEST(testSendDataPacketOfAscending);
    CPPUNIT_TEST(testSendDataPacketOfZeros);
    CPPUNIT_TEST(testSendDataPacketTooLong);
    CPPUNIT_TEST(testSendDataPacketTooShort);
    CPPUNIT_TEST(testReceiveDataPacketOfAscending);
    CPPUNIT_TEST(testReceiveDataPacketOfZeros);
    CPPUNIT_TEST(testReceiveDataPacketBadCrc);
    CPPUNIT_TEST(testReceiveString);
    CPPUNIT_TEST_SUITE_END();

  protected:
    void testSendDataPacketOfAscending();
    void testSendDataPacketOfZeros();
    void testSendDataPacketTooLong();
    void testSendDataPacketTooShort();
    void testReceiveDataPacketOfAscending();
    void testReceiveDataPacketOfZeros();
    void testReceiveDataPacketBadCrc();
    void testReceiveString();
};

CPPUNIT_TEST_SUITE_REGISTRATION(CLASS);

void CLASS::testSendDataPacketOfAscending()
{
    istringstream input;
    ostringstream output;
    TestSerial serial(input, output);
    string packet;
    string::size_type i;
    for (i = 0; i < Serial::PACKET_SIZE; i ++)
    {
        packet.append(1, i & 0xFF);
    }

    string expected;
    expected.append(1, 0x00).append(0x100, 0x01);
    // CRC is 0x7E55
    expected.append(1, 0x55).append(1, 0x7E);

    serial.sendDataPacket(packet);
    ASSERT_DATA_EQUAL(expected, output.str());
}

void CLASS::testSendDataPacketOfZeros()
{
    istringstream input;
    ostringstream output;
    TestSerial serial(input, output);
    string packet(Serial::PACKET_SIZE, 0x00);
    serial.sendDataPacket(packet);

    string expected;
    expected.append(2, '\0');
    Uint16 crc = Crc16::calculateCrc(packet);
    expected.append(1, crc & 0xFF);
    expected.append(1, crc >> 8);

    ASSERT_DATA_EQUAL(expected, output.str());
}

void CLASS::testSendDataPacketTooLong()
{
    istringstream input;
    ostringstream output;
    TestSerial serial(input, output);
    string packet(Serial::PACKET_SIZE + 1, 0x00);
    try
    {
        serial.sendDataPacket(packet);
        CPPUNIT_FAIL("Should have thrown exception");
    }
    catch (runtime_error &)
    {
        // Expected
    }
}

void CLASS::testSendDataPacketTooShort()
{
    istringstream input;
    ostringstream output;
    TestSerial serial(input, output);
    string packet(Serial::PACKET_SIZE - 1, 0x00);
    try
    {
        serial.sendDataPacket(packet);
        CPPUNIT_FAIL("Should have thrown exception");
    }
    catch (runtime_error &)
    {
        // Expected
    }
}

void CLASS::testReceiveDataPacketOfAscending()
{
    string inputData;
    inputData.append(1, 0x00).append(0x100, 0x01);
    // CRC is 0x7E55
    inputData.append(1, 0x55).append(1, 0x7E);
    istringstream input(inputData);
    ostringstream output;
    TestSerial serial(input, output);
    
    string expected;
    string::size_type i;
    for (i = 0; i < Serial::PACKET_SIZE; i++)
    {
        expected.append(1, i & 0xFF);
    }

    string packet;
    CPPUNIT_ASSERT(serial.receiveDataPacket(packet));
    ASSERT_DATA_EQUAL(expected, packet);
}

void CLASS::testReceiveDataPacketOfZeros()
{
    string inputData;
    inputData.append(2, 0x00);
    // CRC is 0x0000
    inputData.append(2, 0x00);
    istringstream input(inputData);
    ostringstream output;
    TestSerial serial(input, output);
    
    string expected(Serial::PACKET_SIZE, 0x00);

    string packet;
    CPPUNIT_ASSERT(serial.receiveDataPacket(packet));
    ASSERT_DATA_EQUAL(expected, packet);
}


void CLASS::testReceiveDataPacketBadCrc()
{
    string inputData;
    inputData.append(2, 0x00);
    // CRC should be 0x0000, set it to 0x0101
    inputData.append(2, 0x01);
    istringstream input(inputData);
    ostringstream output;
    TestSerial serial(input, output);
    
    string packet;
    CPPUNIT_ASSERT(!serial.receiveDataPacket(packet));
    string expected(Serial::PACKET_SIZE, 0x00);
    ASSERT_DATA_EQUAL(expected, packet);
}

void CLASS::testReceiveString()
{
    string inputData;
    // Set high bit on each character
    inputData.append(1, 'h' | 0x80).append(1, 'i' | 0x80).append(1, '\0');
    istringstream input(inputData);
    ostringstream output;
    TestSerial serial(input, output);
    
    ASSERT_DATA_EQUAL("hi", serial.receiveString());
}
