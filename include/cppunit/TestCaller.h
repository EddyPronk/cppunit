#ifndef CPPUNIT_TESTCALLER_H    // -*- C++ -*-
#define CPPUNIT_TESTCALLER_H

#include <cppunit/Exception.h>
#include <cppunit/TestCase.h>
#include <cppunit/extensions/TestFixtureFactory.h>


#if CPPUNIT_USE_TYPEINFO_NAME
#  include <cppunit/extensions/TypeInfoHelper.h>
#endif


CPPUNIT_NS_BEGIN

#if 0
/*! \brief Marker class indicating that no exception is expected by TestCaller.
 * This class is an implementation detail. You should never use this class directly.
 */
class CPPUNIT_API NoExceptionExpected
{
private:
  //! Prevent class instantiation.
  NoExceptionExpected();
};


/*! \brief (Implementation) Traits used by TestCaller to expect an exception.
 *
 * This class is an implementation detail. You should never use this class directly.
 */
template<class ExceptionType>
struct ExpectedExceptionTraits
{
  static void expectedException()
  {
#if CPPUNIT_USE_TYPEINFO_NAME
    throw Exception( Message(
                         "expected exception not thrown",
                         "Expected exception type: " + 
                           TypeInfoHelper::getClassName( typeid( ExceptionType ) ) ) );
#else
    throw Exception( "expected exception not thrown" );
#endif
  }
};


/*! \brief (Implementation) Traits specialization used by TestCaller to 
 * expect no exception.
 *
 * This class is an implementation detail. You should never use this class directly.
 */
template<>
struct ExpectedExceptionTraits<NoExceptionExpected>
{
  static void expectedException()
  {
  }
};


#endif

//*** FIXME: rework this when class Fixture is implemented. ***//


/*! \brief Generate a test case from a fixture method.
 * \ingroup WritingTestFixture
 *
 * A test caller provides access to a test case method 
 * on a test fixture class.  Test callers are useful when 
 * you want to run an individual test or add it to a 
 * suite.
 * Test Callers invoke only one Test (i.e. test method) on one 
 * Fixture of a TestFixture.
 * 
 * Here is an example:
 * \code
 * class MathTest : public CppUnit::TestFixture {
 *         ...
 *     public:
 *         void         setUp();
 *         void         tearDown();
 *
 *         void         testAdd();
 *         void         testSubtract();
 * };
 *
 * CppUnit::Test *MathTest::suite() {
 *     CppUnit::TestSuite *suite = new CppUnit::TestSuite;
 *
 *     suite->addTest( new CppUnit::TestCaller<MathTest>( "testAdd", testAdd ) );
 *     return suite;
 * }
 * \endcode
 *
 * You can use a TestCaller to bind any test method on a TestFixture
 * class, as long as it accepts void and returns void.
 * 
 * \see TestCase
 */

template <class Fixture>
class TestCaller : public TestCase
{ 
  typedef void (Fixture::*TestMethod)();
    
public:
    
  /*!
   * Constructor for TestCaller. 
   * \param name name of this TestCaller
   * \param test the method this TestCaller calls in runTest()
   * \param factory the TestFixtureFactory.
   */
  TestCaller(std::string name, TestMethod test, TestFixtureFactory* factory) :
	    TestCase( name ), 
	    m_fixture( 0 ),
	    m_test( test ),
      m_factory( factory )
  {
  }

  ~TestCaller()
  {
    delete m_factory;
  }
    
  void runTest()
  { 
	    (m_fixture->*m_test)();
  }  

  void setUp()
  { 
	  m_fixture = static_cast<Fixture*>(m_factory->makeFixture());
  	m_fixture->setUp (); 
  }

  void tearDown()
  { 
	  m_fixture->tearDown (); 
	  delete m_fixture;
  }

  std::string toString() const
  { 
  	return "TestCaller " + getName(); 
  }

private: 
  TestCaller( const TestCaller &other ); 
  TestCaller &operator =( const TestCaller &other );

private:
  Fixture *m_fixture;
  TestMethod m_test;
  TestFixtureFactory *m_factory;
};



CPPUNIT_NS_END

#endif // CPPUNIT_TESTCALLER_H
