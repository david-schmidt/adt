/*
 * Copyright (C) 2005 National Association of REALTORS(R)
 *
 * All rights reserved.
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use, copy,
 * modify, merge, publish, distribute, and/or sell copies of the
 * Software, and to permit persons to whom the Software is furnished
 * to do so, provided that the above copyright notice(s) and this
 * permission notice appear in all copies of the Software and that
 * both the above copyright notice(s) and this permission notice
 * appear in supporting documentation.
 */
 
#include <fstream>
#include <iomanip>
#include "test/testUtil.h"

using namespace adt;
// using namespace adt::util;
using std::string;
using std::ifstream;
using std::vector;
using std::endl;
using std::ostream;
using std::ostringstream;
using std::ios;
using std::ios_base;
using std::setw;
using std::setfill;
using std::hex;
using std::dec;

#define NS adt

void NS::checkStringEquals(string expected, string actual,
                           CPPUNIT_NS::SourceLine sourceLine)
{
    if (expected == actual)
        return;

    CPPUNIT_NS::Asserter::failNotEqual(expected,
                                       actual,
                                       sourceLine);
}

static void hexDump(ostream & stream, string data)
{
    string lineSeparator = "";
    stream << hex << setfill('0');
    unsigned i;
    for (i = 0; i < data.length(); i++)
    {
        if ((i % 16) == 0)
        {
            stream << lineSeparator << setw(4) << i << ":";
            lineSeparator = "\n";
        }
        char ch = data.at(i);
        stream << " " << setw(2) << (int(ch) & 0xFF) << setw(0);
    }
    stream << "\n";
    stream << dec << setfill(' ');
}

void NS::checkDataEquals(string expected, string actual,
                         CPPUNIT_NS::SourceLine sourceLine,
                         const string & prefix)
{
    if (expected != actual)
    {
        ostringstream message;
        if (!prefix.empty())
            message << prefix << ": ";
        message << "Comparison failure: " << endl;
        message << "Expected: " << endl;
        hexDump(message, expected);
        message << "Actual  : " << endl;
        hexDump(message, actual);
        CPPUNIT_NS::Asserter::fail(
            CPPUNIT_NS::Message(message.str()), sourceLine);
    }
}

void NS::checkMemEquals(void * expected, void * actual, size_t length,
                        CPPUNIT_NS::SourceLine sourceLine,
                        const std::string & prefix)
{
    if (memcmp(expected, actual, length))
    {
        ostringstream message;
        if (!prefix.empty())
            message << prefix << ": ";
        message << "Comparison failure: " << endl;
        message << "Expected: " << endl;
        hexDump(message, string((char *) expected, length));
        message << "Actual  : " << endl;
        hexDump(message, string((char *) actual, length));
        CPPUNIT_NS::Asserter::fail(
            CPPUNIT_NS::Message(message.str()), sourceLine);
    }
}

#if 0
static string sResourceRoot(".");

void NS::setResourceRoot(string resourceRoot)
{
    sResourceRoot = resourceRoot;
}

istreamPtr NS::getResource(string resourceName, ios_base::openmode mode)
{
    string fileName = sResourceRoot + "/project/librets/test/src/resources/" +
        resourceName;
    ifstreamPtr inputStream(new ifstream(fileName.c_str(), mode));
    if (!(*inputStream))
    {
        throw failure("Could not open file: " + fileName);
    }
    return inputStream;
}

void NS::checkVectorEquals(const vector<string> & expected,
                           const vector<string> & actual,
                           CPPUNIT_NS::SourceLine sourceLine)
{
    if (expected != actual)
    {
        ostringstream message;
        message << "Comparison failure: " << endl;
        message << "Expected: " << Output(expected) << endl;
        message << "Actual  : " << Output(actual);
        CPPUNIT_NS::Asserter::fail(
            CPPUNIT_NS::Message(message.str()), sourceLine);
    }
}

void NS::checkVectorEquals(const vector<int> & expected,
                           const vector<int> & actual,
                           CPPUNIT_NS::SourceLine sourceLine)
{
    if (expected != actual)
    {
        ostringstream message;
        message << "Comparison failure: " << endl;
        message << "Expected: " << Output(expected) << endl;
        message << "Actual  : " << Output(actual);
        CPPUNIT_NS::Asserter::fail(
                                   CPPUNIT_NS::Message(message.str()), sourceLine);
    }
}
#endif