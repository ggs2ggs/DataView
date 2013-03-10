#include <iostream>

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/CompilerOutputter.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestResult.h>
#include <cppunit/TestResultCollector.h>
#include <cppunit/TestRunner.h>
#include <cppunit/BriefTestProgressListener.h>

#include "pandora/BaseContainer.hpp"

#include <H5Cpp.h>
#include <H5File.h>

using namespace std;
using namespace pandora;


class TestBaseContainer : public CPPUNIT_NS::TestFixture, BaseContainer
{

private:
  CPPUNIT_TEST_SUITE(TestBaseContainer);
  CPPUNIT_TEST(testOpen);
  CPPUNIT_TEST(testAttrBasic);
  CPPUNIT_TEST_SUITE_END ();

  H5::H5File h5file;

public:
  void setUp(void) {
    h5file = H5::H5File("test_bc.h5", H5F_ACC_TRUNC);
    h5group = h5file.createGroup("BC-Test", 0);
  }

  void tearDown(void) {
    h5group.close();
    h5file.close();
  }

  void testOpen(void) {
    CPPUNIT_ASSERT(h5group.getId() > 0);
  }

  void testAttrBasic(void) {
    bool res;

    res = attrExists("nonexistant");
    CPPUNIT_ASSERT_EQUAL(false, res);

    string testStr = "I saw the best minds of my generation destroyed by madness";
    setAttr("testStrAttr", testStr);

    res = attrExists("testStrAttr");
    CPPUNIT_ASSERT_EQUAL(true, res);

    string retString;
    getAttr("testStrAttr", retString);
    CPPUNIT_ASSERT_EQUAL(testStr, retString);

    delAttr("testStrAttr");
    res = attrExists("testStrAttr");
    CPPUNIT_ASSERT_EQUAL(false, res);

    //resetting the attr so one can eyeball it in the .h5 file
    setAttr("testStrAttr", testStr);

    // non-string attr test
    setAttr<int>("testIntAttr", 23);
    
    int testIntVal = 0;
    getAttr<int>("testIntAttr", testIntVal);
    CPPUNIT_ASSERT_EQUAL(23, testIntVal);
    setAttr<double>("testDoubleAttr", 3.14159);
    
    double testDoubleVal = 0.0;
    getAttr<double>("testDoubleAttr", testDoubleVal);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(testDoubleVal, 3.14159, 0.0001); //comparing doubles by ==, heh ;-)
    
    testStr = "Hab' nun, ach, Philosophie";
    setAttr<std::string>("testTStringAttr", testStr);
    getAttr("testTStringAttr", retString);
    CPPUNIT_ASSERT_EQUAL(testStr, retString);
    

  }

};


int main (int argc, char* argv[])
{
    CPPUNIT_TEST_SUITE_REGISTRATION(TestBaseContainer);

    CPPUNIT_NS::TestResult testresult;

    CPPUNIT_NS::TestResultCollector collectedresults;
    testresult.addListener (&collectedresults);

    CPPUNIT_NS::BriefTestProgressListener progress;
    testresult.addListener (&progress);

    CPPUNIT_NS::TestRunner testrunner;
    testrunner.addTest (CPPUNIT_NS::TestFactoryRegistry::getRegistry().makeTest());
    testrunner.run (testresult);

    CPPUNIT_NS::CompilerOutputter compileroutputter(&collectedresults, std::cerr);
    compileroutputter.write ();

    return ! collectedresults.wasSuccessful();
}
