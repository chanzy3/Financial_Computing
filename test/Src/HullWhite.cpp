/*-------------------------------------------------------------------------------
  Description	: implementation of functions from test/HullWhite.hpp
  --------------------------------------------------------------------------------*/

#include <functional>
#include <algorithm>
#include "cfl/HullWhiteModel.hpp"
#include "cfl/Data.hpp"
#include "test/HullWhite.hpp"
#include "test/Output.hpp"

using namespace cfl;
using namespace std;
using namespace test;
using namespace cfl::HullWhite;    

cfl::HullWhite::Data 
cfl::test::HullWhite::data(double dYield, double dSigma, 
			   double dLambda, double dInitialTime) 
{
  cout << endl << "PARAMETERS OF HULL-WHITE MODEL:" << endl << endl;
  cout << "interest rate = " << dYield << endl;
  cout << "sigma = " << dSigma << endl;
  cout << "lambda = " << dLambda << endl;
  cout << "initial time = " << dInitialTime << endl << endl;
	
  cfl::test::pause();
	
  cfl::Function uDiscount = cfl::Data::discount(dYield, dInitialTime);
  return cfl::HullWhite::Data(uDiscount, dSigma, dLambda, dInitialTime);
}

//prints values of a derivative security in HullWhite model versus short rate.  
void cfl::test::HullWhite::print(const cfl::Function & rOption, double dShortRate, 
				 double dInterval, unsigned iPoints) 
{

  cout << "OPTION VALUES VERSUS SHORT RATE:" << endl << endl;
	
  PRECONDITION(dInterval >=0.);
  int iI, iSize;
  if (dInterval == 0.) {
    iSize = 1;
  }
  else {
    iSize = 2*(iPoints/2) + 1;
  }
  std::vector<double> uShortRate(iSize);
  std::vector<double> uOption(iSize);
  if (iSize == 1) {
    uShortRate[0] = dShortRate;
    uOption[0] = rOption(0);
  }
  else {
    ASSERT(iSize > 1);
    double dX = -dInterval/2.;
    double dStep = dInterval/(iSize-1.);
    for (iI = 0; iI<iSize; iI++) {
      uShortRate[iI] = dX; 
      uOption[iI] = rOption(dX);
      dX+= dStep;
    }
  }
	
  std::cout << "short rate" << "    " << "option" << endl;
  for (iI=0; iI<iSize; iI++) {
    std::cout << -uShortRate[iI] + dShortRate << "   " << uOption[iI] << endl;
  }
  cfl::test::pause();
}
	
void cfl::test::HullWhite::print(const cfl::MultiFunction & rOption, double dShortRate, 
				 double dInterval, unsigned iPoints) 
{
  PRECONDITION(rOption.dim() == 1);
  cfl::Function uOption= cfl::toFunction(rOption);
  print(uOption, dShortRate, dInterval, iPoints);
}

void  cfl::test::HullWhite::
report(MultiFunction (*f)(InterestRateModel & rModel), 
       double dSpot, double dInterval, unsigned iPoints,
       InterestRateModel & rModel)
{
  Function uOption = toFunction(f(rModel));
  printRisk(uOption);
  print(uOption, dSpot, dInterval, iPoints);
}

void  cfl::test::HullWhite::
report(MultiFunction (*f)(InterestRateModel & rModel, bool bPayFloat), 
       double dSpot, double dInterval, unsigned iPoints, InterestRateModel & rModel) 
{
  for (unsigned i=0; i<2; i++) {
    bool bPayFloat = (i==0) ? true : false;
    Function uOption = toFunction(f(rModel, bPayFloat));
    printRisk(uOption);
    print(uOption, dSpot, dInterval, iPoints);
  }
}

void cfl::test::HullWhite::
project(void (*f)(const cfl::HullWhite::Data & rData), const char * pProject)
{
  const char *pFile = "HullWhiteStandard.txt";
  const char *pHeader =
    "TEST OF STANDARD OPTIONS HULL AND WHITE  MODEL FOR INTEREST RATES";
  std::string sFile = fileName(COURSE_DIR, pProject, pFile);
  project(f,pHeader,sFile.c_str());
}

void cfl::test::HullWhite::
projectPath(void (*f)(const cfl::HullWhite::Data & rData), const char * pProject)
{
  const char *pFile = "HullWhitePathDependent.txt";
  const char *pHeader =
    "TEST OF PATH DEPENDENT OPTIONS IN HULL AND WHITE MODEL FOR INTEREST RATES";
  std::string sFile = fileName(COURSE_DIR, pProject, pFile);
  project(f,pHeader,sFile.c_str());
}
