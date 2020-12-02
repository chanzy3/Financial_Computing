#ifndef __testData_hpp__
#define __testData_hpp__

#include "test/Main.hpp"


/////////////////////////////////////////////////////////////////////////////////
//
// FUNCTIONS: useful functions for the testing of Data curves. 
//
//////////////////////////////////////////////////////////////////////////////////

namespace cfl
{
  namespace test
  {
    namespace Data
    {		
      using namespace cfl;
      using namespace std;
		
      //collection of parameters for output
      const unsigned c_iPoints = 10;

      void print(const cfl::Function & rData, double dStartTime, 
		 double dInterval, unsigned iPoints = c_iPoints);

      void project(void (*f)(), const char * pProject, const char * pFile);
      void project(void (*f)(), const char * pProject);
    }
  }
}

#endif // of __testData_hpp__
