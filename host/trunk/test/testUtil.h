#ifndef ADT_TEST_UTIL_H
#define ADT_TEST_UTIL_H

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <cppunit/extensions/HelperMacros.h>
#include "util.h"

namespace adt {

void checkStringEquals(std::string expected, std::string actual,
                       CPPUNIT_NS::SourceLine sourceLine);

#define ASSERT_STRING_EQUAL(expected, actual) \
    checkStringEquals(expected, actual, CPPUNIT_SOURCELINE())
    
void checkDataEquals(std::string expected, std::string actual,
                     CPPUNIT_NS::SourceLine sourceLine,
                     const std::string & prefix = "");
                     
#define ASSERT_DATA_EQUAL(expected, actual) \
    checkDataEquals(expected, actual, CPPUNIT_SOURCELINE())

#define ASSERT_DATA_EQUAL_MESSAGE(message, expected, actual) \
    checkDataEquals(expected, actual, CPPUNIT_SOURCELINE(), message)

void checkMemEquals(void * expected, void * actual, size_t length,
                     CPPUNIT_NS::SourceLine sourceLine,
                     const std::string & prefix = "");

#define ASSERT_MEM_EQUAL(expected, actual, length) \
    checkMemEquals(expected, actual, length, CPPUNIT_SOURCELINE())

#define ASSERT_MEM_EQUAL_MESSAGE(message, expected, actual, length) \
    checkMemEquals(expected, actual, length, CPPUNIT_SOURCELINE(), message)

class DosDisk;

void checkDiskEquals(const DosDisk & expected, const DosDisk & actual,
                       CPPUNIT_NS::SourceLine sourceLine);

#define ASSERT_DISK_EQUAL(expected, actual) \
    checkDiskEquals(expected, actual, CPPUNIT_SOURCELINE())

template<class T>
void checkNotEquals(const T& expected, const T& actual,
                    CPPUNIT_NS::SourceLine sourceLine)
{
    bool assertion = (expected != actual);
    if (!assertion)
    {
        CPPUNIT_NS::Asserter::failNotEqual(
            CPPUNIT_NS::assertion_traits<T>::toString(expected),
            CPPUNIT_NS::assertion_traits<T>::toString(actual),
            sourceLine);
    }
}

#define ASSERT_NOT_EQUAL(expected, actual) \
    checkNotEquals(expected, actual, CPPUNIT_SOURCELINE())


#define ASSERT_EQUAL CPPUNIT_ASSERT_EQUAL

#if 0
template<class T>
void checkVectorEquals(const std::vector<T> & expected,
                       const std::vector<T> & actual,
                       CPPUNIT_NS::SourceLine sourceLine)
{
    if (!adt::util::VectorEquals(expected, actual))
    {
        std::ostringstream message;
        message << "Comparison failure: " << std::endl;
        message << "Expected: " << adt::util::Output(expected)
                << std::endl;
        message << "Actual  : " << adt::util::Output(actual);
        CPPUNIT_NS::Asserter::fail(
            CPPUNIT_NS::Message(message.str()), sourceLine);
    }
}

void checkVectorEquals(const std::vector<std::string> & expected,
                       const std::vector<std::string> & actual,
                       CPPUNIT_NS::SourceLine sourceLine);


void checkVectorEquals(const std::vector<int> & expected,
                       const std::vector<int> & actual,
                       CPPUNIT_NS::SourceLine sourceLine);

#define ASSERT_VECTOR_EQUAL(expected, actual) \
    checkVectorEquals(expected, actual, CPPUNIT_SOURCELINE());

#endif

void setResourceRoot(std::string resourceRoot);

istreamAPtr getResource(std::string resourceName,
		       std::ios_base::openmode mode = std::ios_base::out);

};

#endif

/* Local Variables: */
/* mode: c++ */
/* End: */
