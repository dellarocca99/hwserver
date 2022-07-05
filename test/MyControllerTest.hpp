#ifndef __MYCONTROLLERTEST_H__
#define __MYCONTROLLERTEST_H__

#include <oatpp-test/UnitTest.hpp>

class MyControllerTest : public oatpp::test::UnitTest {
public:

  MyControllerTest() : UnitTest("TEST[MyControllerTest]"){}
  void onRun() override;

};

#endif // __MYCONTROLLERTEST_H__