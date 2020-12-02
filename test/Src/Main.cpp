#include <algorithm>
#include <functional>
#include "cfl/Macros.hpp"
#include "cfl/PathDependent.hpp"
#include "test/Output.hpp"
#include "test/Main.hpp"
#include "test/Black.hpp"
#include "test/HullWhite.hpp"

using namespace std;
using namespace cfl;

using namespace std;
using namespace cfl;

/////////////////////////////////////////////////////////

void cfl::test::print(const cfl::Slice & rSlice)
{
  std::valarray<double> uValues(rSlice.values());
  cfl::test::print(&uValues[0],&uValues[uValues.size()]);
  if (rSlice.timeIndex() == 0) {
    cout << "value at origin = " << atOrigin(rSlice) << endl;
  }
}

void cfl::test::compare(const cfl::Slice & rExact, const cfl::Slice & rApprox)
{
  cout <<"exact result " << endl;
  print(rExact);
  cout <<"numerical result" << endl;
  print(rApprox);
  cout << "the difference" << endl;
  print(rExact-rApprox);
  std::valarray<double> uDifference = rExact.values() - rApprox.values();
  double dMaxElement = 
    *std::max_element(&uDifference[0],&uDifference[uDifference.size()]);
  double dMinElement = 
    *std::min_element(&uDifference[0],&uDifference[uDifference.size()]);
  cout << "maximal error = " << std::max(dMaxElement,-dMinElement) << endl << endl;
  cout << "exact value at origin = " << atOrigin(rExact) << endl;
  cout << "approx value at origin = " << atOrigin(rApprox) << endl;
  cout << "difference = " << atOrigin(rExact-rApprox) << " = " << 
    atOrigin(rExact) - atOrigin(rApprox) << endl;

}

void cfl::test::analyse(const cfl::Slice & rSlice) 
{
  cout << "the pointer to the model = " << rSlice.ptrToModel() << endl;
  cout << " the index of event time = " << rSlice.timeIndex() << endl;	
  cout << "the vector of dependences " << endl;
  print(rSlice.dependence().begin(),rSlice.dependence().end());
  cout << "the values " << endl;
  print(rSlice);
}

void cfl::test::print(const cfl::Function & rF, unsigned iS, double dL, double dR)
{
  PRECONDITION((dL <= dR)&&(iS>0));
  std::vector<double> uValues;
  double dH = (dR-dL)/iS;
  double dX = dL;
  for (unsigned iI=0; iI<iS; iI++) {
    uValues.push_back(rF(dX));
    dX+=dH;
  }
  cfl::test::print(uValues.begin(), uValues.end());
}


std::valarray<double> cfl::test::
getArg(double dL, double dR, unsigned iN)
{
  ASSERT(iN>0);
  std::valarray<double> uResult(iN);
  double dH = (dR-dL)/(iN-1);
  double dX = dL;
  for (unsigned iI=0; iI<iN; iI++) {
    uResult[iI] = dX;
    dX+=dH;
  }
  return uResult;
}

std::valarray<double> cfl::test::
getRandArg(double dL, double dR, unsigned iN)
{
  ASSERT(iN>0);
  std::valarray<double> uResult(iN);
  for (unsigned iI=0; iI<iN; iI++) {
    uResult[iI] = dL + (dR-dL)*rand()/RAND_MAX;
  }
  return uResult;
}

std::valarray<double> cfl::test::
getValues(const Function & rF, const std::valarray<double> & rArg)
{
  std::valarray<double> uResult(rArg.size());
  for (unsigned iI=0;iI<uResult.size();iI++) {
    uResult[iI] = rF(rArg[iI]);
  }
  return uResult;
}

void cfl::test::
checkInterp(const Function & rApprox,	const Function & rExact, 
	    const std::valarray<double> & rX)
{
  unsigned iSize = 11;
  if (iSize > rX.size()) { iSize = rX.size(); }
  int iStart = (rX.size() - iSize)/2;
  int iEnd = iStart + iSize;
  cout << "approximate values " << endl;
  std::valarray<double> uA = getValues(rApprox, rX);
  print(begin(uA)+iStart, begin(uA)+iEnd);
  std::valarray<double> uE = getValues(rExact, rX);
  cout << "exact values " << endl;
  print(begin(uE)+iStart, begin(uE)+iEnd);
  cout << "difference" << endl;
  std::valarray<double> uD = uA - uE;
  print(begin(uD)+iStart, begin(uD)+iEnd);
}

std::vector<double> 
cfl::test::times(double dInitialTime, double dMaturity, unsigned iTimes) 
{
  std::vector<double> uTimes(iTimes);
  double dTimeStep = (dMaturity - dInitialTime)/(iTimes + 1.);
  double dTime = dMaturity;
  std::vector<double>::iterator itTime = uTimes.end();
  while (itTime>uTimes.begin()) {
    itTime--;
    *itTime = dTime;
    dTime-=dTimeStep;
  }
  return uTimes;
}

void cfl::test::printRisk(const cfl::Function & rOption, double dShift) 
{
  cout << endl << "RISK REPORT: " << endl << endl;
  double dCenter = 0.; 
  double dLeft = -dShift;
  double dRight  = dShift;
  double dPrice = rOption(dCenter);
  cout << "price = "  << dPrice << endl;
  if (rOption.belongs(dRight) && rOption.belongs(dLeft)) {
    double dValueLeft = rOption(dLeft);
    double dValueRight = rOption(dRight);
    cout << "delta = " << (dValueRight - dValueLeft)/(2.*dShift) << 
      endl;
    cout << "one percent gamma = " << c_dOnePercent*(dValueRight- 2.*dPrice + dValueLeft)/(dShift*dShift) << endl << endl;
  }
  pause();
}

void cfl::test::pause()
{
  //std::cout << endl;
}

void cfl::test::print(const char * pM) 
{
  std::cerr<<endl << pM << endl;
  std::cout<< endl << pM << endl;
}

void cfl::test::printAtStart(const char * pMessage) 
{
  std::string sOut(pMessage);
  sOut.append(" by ");
  sOut.append(STUDENT_ID);
  cfl::test::print(sOut.c_str()); 
}

void cfl::test::printAtEnd(const char * pFileName) 
{
  cout << endl << "The output is written to the file " << pFileName;
  cout << endl << endl;
}

namespace testMain
{
  template <class Data> 
  inline void testProject(const Data & rData, void (*f)(const Data & rData), 
			  const char * pProject, const char * pFile) {
    std::ofstream fOut(pFile);
    std::streambuf * strmBuffer = std::cout.rdbuf();
		
    std::cout.rdbuf (fOut.rdbuf());
		
    cfl::test::printAtStart(pProject);
		
    f(rData);
		
    std::cout.rdbuf (strmBuffer);
		
    cfl::test::printAtEnd(pFile);
  }

  template <class Data> 
  inline void testProject2(Data (*data)(), void (*f)(const Data & rData), 
			   const char * pProject, const char * pFile) {
    std::ofstream fOut(pFile);
    std::streambuf * strmBuffer = std::cout.rdbuf();
		
    std::cout.rdbuf (fOut.rdbuf());
		
    cfl::test::printAtStart(pProject);
		
    f(data());
    std::cout.rdbuf (strmBuffer);
    cfl::test::printAtEnd(pFile);
  }
}


void cfl::test::Black::project(const cfl::Black::Data & rData, void (*f)(const cfl::Black::Data & rData), 
			       const char * pProject, const char * pFile) {
  testMain::testProject<cfl::Black::Data>(rData, f, pProject, pFile); 
}

void cfl::test::HullWhite::project(void (*f)(const cfl::HullWhite::Data & rData), 
				   const char * pProject, const char * pFile) {
  testMain::testProject2<cfl::HullWhite::Data>(cfl::test::HullWhite::data, f, pProject, pFile); 
}

void cfl::test::Black::project(void (*f)(const cfl::Black::Data & rData), 
			       const char * pProject, const char * pFile) {
  testMain::testProject2<cfl::Black::Data>(cfl::test::Black::data, f, pProject, pFile); 
}

void cfl::test::HullWhite::project(const cfl::HullWhite::Data & rData, 
				   void (*f)(const cfl::HullWhite::Data & rData), 
				   const char * pProject, const char * pFile) {
  testMain::testProject<cfl::HullWhite::Data>(rData, f, pProject, pFile); 
}

void cfl::test::project(void (*f)(const cfl::Black::Data & , 
				  const cfl::HullWhite::Data & ), 
			const char * pProject, const char * pFile)
{
  std::ofstream fOut(pFile);
  std::streambuf * strmBuffer = std::cout.rdbuf();
		
  std::cout.rdbuf (fOut.rdbuf());
		
  cfl::test::printAtStart(pProject);
		
  f(Black::data(), HullWhite::data());
		
  std::cout.rdbuf (strmBuffer);
		
  cfl::test::printAtEnd(pFile);
}

void cfl::test::print(const cfl::Data::CashFlow & rCashFlow) {
  cout << "notional = " << rCashFlow.notional << endl;
  cout << "period between payments = " << rCashFlow.period << endl;
  cout << "number of payments = " << rCashFlow.numberOfPayments << endl;
  cout << "rate = " << rCashFlow.rate << endl;
}

void cfl::test::print(const cfl::Data::Swap & rSwap) {
  print(cfl::Data::CashFlow(rSwap));
  if (rSwap.payFloat) {
    cout << "we are paying float and receiving fixed" << endl;
  }
  else {
    cout << "we are paying fixed and receiving float " << endl;
  }
}

std::string cfl::test::fileName(const char *pDir, const char *pProject, const char *pFile)
{
  std::string a(pDir);
  a+= std::string("/");
  a += std::string(pProject);
  a+= std::string("/Output/");
  a += std::string(pProject);
  a += std::string(pFile);
  return a;
}
