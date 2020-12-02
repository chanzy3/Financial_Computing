/*-------------------------------------------------------------------------------
  Description	: implementaion of functions from test/Data.hpp
  --------------------------------------------------------------------------------*/

#include <functional>
#include <algorithm>
#include "cfl/Data.hpp"
#include "test/Data.hpp"
#include "test/Output.hpp"

using namespace cfl;
using namespace std;
using namespace cfl::test;

void cfl::test::Data::print(const cfl::Function & rData, double dStartTime, 
			    double dInterval, unsigned iPoints) 
{

  cout << "VALUES VERSUS TIME:" << endl << endl;
	
  PRECONDITION(dInterval >=0.);
  int iI, iSize;
  if (dInterval == 0.) {
    iSize = 1;
  }
  else {
    iSize = iPoints;
  }
  double dPeriod = dInterval/iSize;
  std::cout << "time" << "    " << "value" << endl;

  for (iI=0; iI<iSize; iI++) {
    double dTime = dStartTime + iI*dPeriod;
    std::cout << dTime << "   " << rData(dTime) << endl;
  }
  pause();
}

void cfl::test::Data::project(void (*f)(), const char * pProject, const char * pFile) 
{
  std::ofstream fOut(pFile);
  std::streambuf * strmBuffer = std::cout.rdbuf();
  std::cout.rdbuf (fOut.rdbuf());
  cfl::test::printAtStart(pProject);
  f();
  std::cout.rdbuf (strmBuffer);
  cfl::test::printAtEnd(pFile);
}

void cfl::test::Data::project(void (*f)(), const char * pProject) 
{
  const char *pFile = "Data.txt";
  const char *pHeader =  "TEST OF DATA CURVES";
  std::string sFile = fileName(COURSE_DIR, pProject, pFile);
  project(f,pHeader,sFile.c_str());
}
