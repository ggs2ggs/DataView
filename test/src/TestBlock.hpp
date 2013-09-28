#include <iostream>
#include <sstream>
#include <iterator>
#include <stdexcept>
#include <limits>

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/CompilerOutputter.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestResult.h>
#include <cppunit/TestResultCollector.h>
#include <cppunit/TestRunner.h>
#include <cppunit/BriefTestProgressListener.h>

#include <pandora.hpp>

using namespace pandora;

class TestBlock : public CPPUNIT_NS::TestFixture {

private:

  CPPUNIT_TEST_SUITE(TestBlock);
  CPPUNIT_TEST(testAddRemoveDataArray);
  CPPUNIT_TEST_SUITE_END ();

  File *f1;
public:

  void setUp() {
    f1 = new File("test_block.h5", "org.g-node", FileMode::ReadWrite);
  }

  void tearDown() {
    delete f1;
  }


  void testAddRemoveDataArray() {
    // TODO implement testAddRemoveDataArray()
  }

};