/*-------------------------------------------------------------------------------
  Description	: implementaion of functions from test/Black.h
  --------------------------------------------------------------------------------*/
#include <iostream>
#include <fstream>
#include <functional>
#include <algorithm>
#include "cfl/BlackModel.hpp"
#include "cfl/Data.hpp"
#include "test/Black.hpp"
#include "test/Output.hpp"

using namespace cfl;
using namespace std;
using namespace test;
using namespace cfl::Black;

cfl::Black::Data 
cfl::test::Black::data(double dYield, double dSpot, 
		       double dDividendYield, double dSigma, 
		       double dLambda, double dInitialTime) 
{
  cout << endl; 
  cout << "PARAMETERS OF BLACK MODEL:" << endl << endl;
  cout << "interest rate = " << dYield << endl;
  cout << "spot price = " << dSpot << endl;
  cout << "convenience yield = " << dDividendYield << endl;
  cout << "sigma = " << dSigma << endl;
  cout << "lambda = " << dLambda << endl;
  cout << "initial time = " << dInitialTime << endl << endl;
	
  cfl::test::pause();
	
  cfl::Function uDiscount = cfl::Data::discount(dYield, dInitialTime);
  cfl::Function uForward = cfl::Data::forward(dSpot, dDividendYield, uDiscount,
					      dInitialTime);
  return cfl::Black::Data(uDiscount, uForward, dSigma, dLambda, dInitialTime);
}

//prints values of a derivative security in Black model versus spot price.  
void cfl::test::Black::print(const cfl::Function & rOption, double dSpot, 
			     double dInterval, unsigned iPoints) 
{
  cout << "OPTION VALUES VERSUS SPOT:" << endl << endl;
	
  PRECONDITION(dInterval >=0.);
  int iI, iSize;
  if (dInterval == 0.) {
    iSize = 1;
  }
  else {
    iSize = 2*(iPoints/2) + 1;
  }
  std::vector<double> uSpot(iSize);
  std::vector<double> uOption(iSize);
  if (iSize == 1) {
    uSpot[0] = dSpot;
    uOption[0] = rOption(0);
  }
  else {
    ASSERT(iSize > 1);
    double dX = -dInterval/2.;
    double dStep = dInterval/(iSize-1.);
    for (iI = 0; iI<iSize; iI++) {
      uSpot[iI] = ::exp(dX)*dSpot;
      uOption[iI] = rOption(dX);
      dX+= dStep;
    }
  }
	
  std::cout << "spot" << "    " << "option" << endl;
  for (iI=0; iI<iSize; iI++) {
    std::cout << uSpot[iI] << "   " << uOption[iI] << endl;
  }
  cfl::test::pause();
}
	
void cfl::test::Black::print(const cfl::MultiFunction & rOption, double dSpot, 
			     double dInterval, unsigned iPoints)
{
  PRECONDITION(rOption.dim() == 1);
  cfl::Function uOption= cfl::toFunction(rOption);
  print(uOption, dSpot, dInterval, iPoints);
}
 
void  cfl::test::Black::
report(MultiFunction (*f)(AssetModel & rModel), 
       double dSpot, double dInterval, unsigned iPoints, AssetModel & rModel)
{
  Function uOption = toFunction(f(rModel));
  printRisk(uOption);
  print(uOption, dSpot, dInterval, iPoints);
}

void  cfl::test::Black::
report(MultiFunction (*f)(AssetModel & rModel, bool bPayFloat), 
       double dSpot, double dInterval, unsigned iPoints, AssetModel & rModel) 
{
  for (unsigned i=0; i<2; i++) {
    bool bPayFloat = (i==0) ? true : false;
    Function uOption = toFunction(f(rModel, bPayFloat));
    printRisk(uOption);
    print(uOption, dSpot, dInterval, iPoints);
  }
}

void cfl::test::Black::
project(void (*f)(const cfl::Black::Data & rData), const char * pProject)
{
  const char *pFile = "BlackStandard.txt";
  const char *pHeader =
    "TEST OF STANDARD OPTIONS IN BLACK MODEL FOR SINGLE ASSET";
  std::string sFile = fileName(COURSE_DIR, pProject, pFile);
  project(f,pHeader,sFile.c_str());
}

void cfl::test::Black::
projectPath(void (*f)(const cfl::Black::Data & rData), const char * pProject)
{
  const char *pFile = "BlackPathDependent.txt";
  const char *pHeader =
    "TEST OF PATH DEPENDENT OPTIONS IN BLACK MODEL FOR SINGLE ASSET";
  std::string sFile = fileName(COURSE_DIR, pProject, pFile);
  project(f,pHeader,sFile.c_str());
}

  
