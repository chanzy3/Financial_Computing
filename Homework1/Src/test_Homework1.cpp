#include <iostream>
#include <fstream>
#include "test/Main.hpp"
#include "test/Black.hpp"
#include "test/HullWhite.hpp"
#include "test/Data.hpp"
#include "test/Output.hpp"
#include "Homework1/test_Homework1.hpp"
#include "Homework1/Homework1.hpp"

using namespace std;
using namespace prb;
using namespace cfl;
using namespace cfl::test;
using namespace cfl::test::Black;
using namespace cfl::test::HullWhite;

namespace test_prb
{
  void data();
}

int main()
{
  const char * c_pProject = "Homework1";
  using namespace cfl::test;
  using namespace std;
  try{
    cfl::test::Data::project(test_prb::data, c_pProject);
    cfl::test::pause();
  }
  catch(const std::exception & rError) {
    std::cerr<< rError.what() << endl;
    cfl::test::pause();
  }
}

void test_prb::data()
{
  prb::test::forwardFX();
  prb::test::forwardCouponBond();
  prb::test::forwardLogLinearInterp();
  prb::test::volatilityFitBlack();
}

void prb::test::forwardFX() 
{
  cfl::test::print("FORWARD PRICES FOR EXCHANGE RATES");
  cout << endl;
  double dSpotFX = 100;
  double dDom = 0.12;
  double dFor = 0.05;
  double dInitialTime = 1.;

  cout << "initial time = " << dInitialTime << endl;
  cout << "spot FX rate = " << dSpotFX << endl;
  cout << "domestic interest rate = " << dDom << endl;
  cout << "foreign interest rate = " << dFor << endl << endl;


  Function uDomestic = cfl::Data::discount(dDom, dInitialTime);
  Function uForeign = cfl::Data::discount(dFor, dInitialTime);
  Function uForwardFX = prb::forwardFX(dSpotFX, uDomestic, uForeign);
  double dInterval = 0.5;
  cout << "output forward FX rates: " << endl;
  cfl::test::Data::print(uForwardFX, dInitialTime, dInterval); 	
}

void prb::test::forwardCouponBond() 
{
  cfl::test::print("FORWARD PRICES FOR COUPON BOND");
  cout << endl;
  cfl::Data::CashFlow uBondParameters = cfl::test::HullWhite::swapParameters();
  double dRate = uBondParameters.rate;
  double dInitialTime = 1.;
  Function uDiscount = cfl::Data::discount(dRate, dInitialTime);
  cout << "initial time = " << dInitialTime << endl;
  cout << "interest rate = " << dRate << endl << endl;
  cout << "parameters of the bond:" << endl;
  cfl::test::print(uBondParameters);
  cout << endl;

  for (int iI=0; iI<2; iI++) {
    bool bClean = (iI==0) ? true : false;
    if (bClean) {
      cout << "clean prices:" << endl;
      cout << endl;
    }
    else {
      cout << "dirty prices:" << endl;
      cout << endl;
    }
    Function uForwardCouponBond = 
      prb::forwardCouponBond(uBondParameters, uDiscount, 
			     dInitialTime, bClean); 
    int iPoints = cfl::test::Data::c_iPoints;
    double dInterval = 
      uBondParameters.period*uBondParameters.numberOfPayments/1.1;
    cfl::test::Data::print(uForwardCouponBond, dInitialTime, 
			   dInterval, iPoints); 
  }
}

void prb::test::forwardLogLinearInterp() 
{
  cfl::test::print("LOG LINEAR INTERPOLATION OF FORWARD CURVE");
  cout << endl;
  double dSpot = 100.;
  double dRate = 0.3;
  double dInitialTime = 1.;
  unsigned iTimes = 10;
  std::vector<double> uTimes(iTimes);
  double dPeriod = 0.5;
  uTimes.front() = dInitialTime + dPeriod;
  std::transform(uTimes.begin(), uTimes.end()-1, uTimes.begin()+1, 
		 [dPeriod](double dX){ return dX+dPeriod; });
  std::vector<double> uForwardPrices(iTimes);
  cfl::Function uForwardCurve = cfl::Data::forward(dSpot, dRate, dInitialTime);
  std::transform(uTimes.begin(), uTimes.end(), uForwardPrices.begin(), 
		 uForwardCurve); 
  cout << "initial time = " << dInitialTime << endl;
  cout << "spot = " << dSpot << endl;
  cout << "input maturities of forwards: " << endl;
  cfl::test::print(uTimes.begin(), uTimes.end());
  cout << "input forward prices: " << endl;
  cfl::test::print(uForwardPrices.begin(), uForwardPrices.end());

  Function uForward = 
    prb::forwardLogLinearInterp(dSpot, uTimes, 
				uForwardPrices, dInitialTime); 
  double dInterval = 4.25;
  cout << "output forward prices: " << endl;
  cfl::test::Data::print(uForward, dInitialTime, dInterval); 	 
}

void prb::test::volatilityFitBlack() 
{
  cfl::test::print("LEAST SQUARES FIT OF VOLATILITY CURVE FOR BLACK MODEL");
  cout << endl;
  double dSigma = 0.3;
  double dLambda = 0.05; 
  double dInitialTime = 1.;
  unsigned iTimes = 10;
  std::vector<double> uTimes(iTimes);
  double dPeriod = 0.5;
  uTimes.front() = dInitialTime + dPeriod;
  std::transform(uTimes.begin(), uTimes.end()-1, uTimes.begin()+1, 
		 [dPeriod](double dX){ return dX+dPeriod; });
  std::vector<double> uVolatilities(iTimes);
  std::transform(uTimes.begin(), uTimes.end(), uVolatilities.begin(), 
		 cfl::Data::volatility(dSigma, dLambda, dInitialTime)); 
  cout << "lambda = " << dLambda << endl;
  cout << "initial time = " << dInitialTime << endl;
  cout << "input times: " << endl;
  cfl::test::print(uTimes.begin(), uTimes.end());
  cout << "input volatilities: " << endl;
  cfl::test::print(uVolatilities.begin(), uVolatilities.end());
  Function uVolatility = 
    prb::volatilityFitBlack(uTimes, uVolatilities, 
			    dLambda, dInitialTime); 
  double dInterval = 4.75;
  cout << "output volatilities" << endl;
  cfl::test::Data::print(uVolatility, dInitialTime+0.001, dInterval);	
}
