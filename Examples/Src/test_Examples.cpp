#include <iostream>
#include <fstream>
#include "test/Main.hpp"
#include "test/Black.hpp"
#include "test/HullWhite.hpp"
#include "test/Data.hpp"
#include "test/Output.hpp"
#include "Examples/test_Examples.hpp"
#include "Examples/Examples.hpp"

using namespace std;
using namespace prb;
using namespace cfl;
using namespace cfl::test;
using namespace cfl::test::Black;
using namespace cfl::test::HullWhite;

namespace test_prb
{
  void data();
  void standard(const cfl::Black::Data & rData);
  void pathDependent(const cfl::Black::Data & rData);
  void standard(const cfl::HullWhite::Data & rData);
  void pathDependent(const cfl::HullWhite::Data & rData);
}

int main()
{
  static const char * c_pProject = "Examples";
  using namespace cfl::test;
  using namespace std;
  try{
    cfl::test::Data::project(test_prb::data, c_pProject); 
    cfl::test::pause();
    cfl::test::Black::project(test_prb::standard, c_pProject);
    cfl::test::pause();
    cfl::test::HullWhite::project(test_prb::standard, c_pProject);
    cfl::test::pause();
    cfl::test::Black::projectPath(test_prb::pathDependent, c_pProject);
    cfl::test::pause();
    cfl::test::HullWhite::projectPath(test_prb::pathDependent, c_pProject);
    cfl::test::pause();
  }
  catch(const std::exception & rError) {
    std::cerr<< rError.what() << endl;
    cfl::test::pause();
  }
}

void test_prb::data()
{
  prb::test::yieldShapeHullWhite();
  prb::test::discountLogLinearInterp(); 
  prb::test::discountFitHullWhite();
}

void test_prb::standard(const cfl::Black::Data & rData)
{
  double dQuality = cfl::test::Black::c_dQuality;
  cout << "quality = " << dQuality << endl;
  double dSpot = rData.forward()(rData.initialTime());
  double dInterval = cfl::test::Black::c_dInterval;
  unsigned iPoints = cfl::test::Black::c_iPoints;

  AssetModel uModel = cfl::Black::model(rData, dInterval, dQuality);

  using namespace prb::test; 

  report(put, dSpot, dInterval, iPoints, uModel);
  report(americanPut, dSpot, dInterval, iPoints, uModel);
  report(barrierUpOrDownAndOut, dSpot, dInterval, iPoints, uModel);
  report(downAndOutAmericanCall, dSpot, dInterval, iPoints, uModel);
  report(swing, dSpot, dInterval, iPoints, uModel);
}


void test_prb::pathDependent(const cfl::Black::Data & rData)
{
  double dQuality = cfl::test::Black::c_dQuality;
  cout << "quality = " << dQuality << endl;
  double dPathDependQuality = cfl::test::Black::c_dPathDependQuality;
  cout << "quality for additional state process = " 
       << dPathDependQuality << endl;
  double dSpot = rData.forward()(rData.initialTime());
  double dInterval = cfl::test::Black::c_dInterval;
  unsigned iPoints = cfl::test::Black::c_iPoints;

  AssetModel uModel = cfl::Black::model(rData, dInterval, dQuality, 
					dPathDependQuality);

  using namespace prb::test;

  report(asianCall, dSpot, dInterval, iPoints, uModel);
  report(barrierUpOrDownAndOut_path, dSpot, dInterval, iPoints, uModel);
}

void test_prb::standard(const cfl::HullWhite::Data & rData)
{
  double dQuality = cfl::test::HullWhite::c_dQuality;
  cout << "quality = " << dQuality << endl;
  const double c_dDay = 1./365.;
  double dRate = - 
    std::log(rData.discount()(rData.initialTime()+c_dDay))/c_dDay;
  double dInterval = cfl::test::HullWhite::c_dInterval;
  unsigned iPoints = cfl::test::HullWhite::c_iPoints;

  InterestRateModel uModel = cfl::HullWhite::model(rData, dInterval, dQuality);

  using namespace prb::test;

  report(cap, dRate, dInterval, iPoints, uModel);
  report(swaption, dRate, dInterval, iPoints, uModel);
  report(cancellableCollar, dRate, dInterval, iPoints, uModel);
  report(downAndOutCap, dRate, dInterval, iPoints, uModel);
  report(futureOnLibor, dRate, dInterval, iPoints, uModel);
}

void test_prb::pathDependent(const cfl::HullWhite::Data & rData)
{
  double dQuality = cfl::test::HullWhite::c_dQuality;
  double dPathDependQuality =
    cfl::test::HullWhite::c_dPathDependQuality;
  
  cout << "quality = " << dQuality << endl;
  cout << "quality for additional state processes = " <<
    dPathDependQuality << endl;
  const double c_dDay = 1./365.;
  double dRate = 
    - std::log(rData.discount()(rData.initialTime()+c_dDay))/c_dDay;
  double dInterval = cfl::test::HullWhite::c_dInterval;
  unsigned iPoints = cfl::test::HullWhite::c_iPoints;

  InterestRateModel uModel =
    cfl::HullWhite::model(rData, dInterval, dQuality, dPathDependQuality);

  using namespace prb::test;

  report(savingsAccount, dRate, dInterval, iPoints, uModel);
  report(putOnSavingsAccount, dRate, dInterval, iPoints, uModel);
}

void prb::test::yieldShapeHullWhite() 
{
  cfl::test::print("STATIONARY FORM FOR CHANGES IN YIELD CURVE IN HULL AND WHITE MODEL");
  cout << endl;
  double dLambda = 0.05; 
  double dInitialTime = 109.;
  cout << "initial time = " << dInitialTime << endl;
  cout << "mean-reversion rate =" << dLambda << endl<<endl;
  Function uYieldShapeHW = 
    prb::yieldShapeHullWhite(dLambda, dInitialTime);
  double dInterval = 4.75;
  cfl::test::Data::print(uYieldShapeHW, dInitialTime+0.001, dInterval);	
}

void prb::test::discountLogLinearInterp() 
{
  cfl::test::print("LOG LINEAR INTERPOLATION OF DISCOUNT CURVE");
  cout << endl;
  double dRate = 0.12;
  double dInitialTime = 1.;
  unsigned iTimes = 10;
  std::vector<double> uTimes(iTimes);
  double dPeriod = 0.5;
  uTimes.front() = dInitialTime + dPeriod;
  std::transform(uTimes.begin(), uTimes.end()-1, uTimes.begin()+1, 
		 [dPeriod](double dX){ return dX+dPeriod; });
  std::vector<double> uDiscountFactors(iTimes);
  std::transform(uTimes.begin(), uTimes.end(), uDiscountFactors.begin(), 
		 cfl::Data::discount(dRate, dInitialTime)); 
  cout << "initial time = " << dInitialTime << endl;
  cout << "input times: " << endl;
  cfl::test::print(uTimes.begin(), uTimes.end());
  cout << "input discount factors: " << endl;
  cfl::test::print(uDiscountFactors.begin(), uDiscountFactors.end());
  Function uDiscount = 
    prb::discountLogLinearInterp(uTimes,uDiscountFactors,dInitialTime); 
  double dInterval = 4.25;
  cout << "the result of log-linear interpolation:" << endl;
  cfl::test::Data::print(uDiscount, dInitialTime, dInterval); 
}

void prb::test::discountFitHullWhite() 
{
  cfl::test::print("LEAST SQUIRE FIT OF DISCOUNT CURVE FOR HULL AND WHITE MODEL");
  cout << endl;
  double dLambda = 0.05; 
  double dInitialTime = 1.;
  unsigned iTimes = 10;
  std::vector<double> uTimes(iTimes);
  double dPeriod = 0.5;
  uTimes.front() = dInitialTime + dPeriod;
  std::transform(uTimes.begin(), uTimes.end()-1, uTimes.begin()+1, 
		 [dPeriod](double dX){ return dX+dPeriod; });
  std::vector<double> uDiscountFactors(iTimes);
  double dRate = 0.07;
  std::transform(uTimes.begin(), uTimes.end(), uDiscountFactors.begin(), 
		 cfl::Data::discount(dRate, dInitialTime)); 
  cout << "lambda = " << dLambda << endl;
  cout << "initial time = " << dInitialTime << endl;
  cout << "input times: " << endl;
  cfl::test::print(uTimes.begin(), uTimes.end());
  cout << "input discount factors: " << endl;
  cfl::test::print(uDiscountFactors.begin(), uDiscountFactors.end());
  Function uDiscount = 
    prb::discountFitHullWhite(uTimes, uDiscountFactors, 
			      dLambda, dInitialTime); 
  double dInterval = 5;
  cout << "output discount factors" << endl;
  cfl::test::Data::print(uDiscount, dInitialTime, dInterval); 	
}


MultiFunction prb::test::americanPut(AssetModel & rModel) 
{
  cfl::test::print("AMERICAN PUT OPTION IN ASSET MODEL");
  cout << endl;
  double dStrike = cfl::test::Black::c_dStrike;
  const std::vector<double> uExerciseTimes = 
    cfl::test::Black::c_uExerciseTimes;
  cout << "strike = " << dStrike << endl;
  cout << "exericse times = "<< endl;
  cfl::test::print(uExerciseTimes.begin(), uExerciseTimes.end());
  return prb::americanPut(dStrike, uExerciseTimes, rModel);

}

MultiFunction prb::test::barrierUpOrDownAndOut(AssetModel & rModel) 
{
  cfl::test::print("BARRIER UP-OR-DOWN-AND-OUT OPTION IN ASSET MODEL");
  cout << endl;
  double dLowerBarrier = cfl::test::Black::c_dLowerBarrier;
  double dUpperBarrier = cfl::test::Black::c_dUpperBarrier;
  double dNotional = cfl::test::Black::c_dNotional;
  const std::vector<double> uBarrierTimes = cfl::test::Black::c_uBarrierTimes;
  cout << "lower barrier = " << dLowerBarrier << endl;
  cout << "upper barrier = " << dUpperBarrier << endl;
  cout << "notional = " << dNotional << endl;
  cout << "barrier times = "<< endl;
  cfl::test::print(uBarrierTimes.begin(), uBarrierTimes.end());
  return prb::barrierUpOrDownAndOut(dNotional, dLowerBarrier, 
				    dUpperBarrier, uBarrierTimes, rModel);
}

MultiFunction prb::test::downAndOutAmericanCall(AssetModel & rModel) 
{
  cfl::test::print("DOWN-AND-OUT AMERICAN CALL OPTION IN ASSET MODEL");
  cout << endl;
  double dStrike = cfl::test::Black::c_dStrike;
  const std::vector<double> uExerciseTimes = 
    cfl::test::Black::c_uExerciseTimes;
  double dLowerBarrier = cfl::test::Black::c_dLowerBarrier;
  const std::vector<double> uBarrierTimes = cfl::test::Black::c_uBarrierTimes;
  cout << "strike = " << dStrike << endl;
  cout << "exericse times = "<< endl;
  cfl::test::print(uExerciseTimes.begin(), uExerciseTimes.end());
  cout << "lower barrier = " << dLowerBarrier << endl;
  cout << "barrier times = "<< endl;
  cfl::test::print(uBarrierTimes.begin(), uBarrierTimes.end());
  return prb::downAndOutAmericanCall(dLowerBarrier, uBarrierTimes, 
				     dStrike, uExerciseTimes, rModel);

}

MultiFunction prb::test::put(AssetModel & rModel) 
{
  cfl::test::print("EUROPEAN PUT OPTION IN ASSET MODEL");
  cout << endl;
  double dStrike = cfl::test::Black::c_dStrike;
  double dMaturity = cfl::test::Black::c_dMaturity;
  cout << "strike = " << dStrike << endl;
  cout << "maturity = " << dMaturity << endl;
  return prb::put(dStrike, dMaturity, rModel);
}

cfl::MultiFunction prb::test::swing(AssetModel & rModel) 
{
  cfl::test::print("SWING OPTION IN ASSET MODEL");
  cout << endl;
  double dStrike = cfl::test::Black::c_dStrike;
  const std::vector<double> uExerciseTimes = 
    cfl::test::Black::c_uExerciseTimes;
  cout << "strike = " << dStrike << endl;
  cout << "exercise times = "<< endl;
  cfl::test::print(uExerciseTimes.begin(), uExerciseTimes.end());
  unsigned iNumberOfExercises = uExerciseTimes.size()/3;
  cout << "maximal number of exercises = " << iNumberOfExercises << endl;
  return prb::swing(dStrike, uExerciseTimes, iNumberOfExercises, rModel);
}

cfl::MultiFunction prb::test::
cancellableCollar(InterestRateModel & rModel)
{
  cfl::test::print("CANCELLABLE INTEREST RATE COLLAR IN INTEREST RATE MODEL");
  cout << endl;
  cfl::Data::CashFlow uCapParameters = cfl::test::HullWhite::swapParameters();
  uCapParameters.rate = cfl::test::HullWhite::c_dCapRate;
  double dFloorRate = cfl::test::HullWhite::c_dFloorRate;
  cout << "cap parameters:" << endl;
  cfl::test::print(uCapParameters);
  cout << "floor rate = " << dFloorRate << endl;
  return prb::cancellableCollar(uCapParameters, dFloorRate, rModel);
}

cfl::MultiFunction prb::test::cap(InterestRateModel & rModel)
{
  cfl::test::print("INTEREST RATE CAP IN INTEREST RATE MODEL");
  cout << endl;
  cfl::Data::CashFlow uCapParameters = cfl::test::HullWhite::swapParameters();
  uCapParameters.rate = cfl::test::HullWhite::c_dCapRate;
  cout << "cap parameters:" << endl;
  cfl::test::print(uCapParameters);
  return prb::cap(uCapParameters, rModel);
}

cfl::MultiFunction
prb::test::downAndOutCap(InterestRateModel & rModel)
{
  cfl::test::print("DOWN-AND-OUT INTEREST RATE CAP IN INTEREST RATE MODEL");
  cout << endl;
  cfl::Data::CashFlow uCapParameters = cfl::test::HullWhite::swapParameters();
  uCapParameters.rate = cfl::test::HullWhite::c_dCapRate;
  double dLowerBarrier = cfl::test::HullWhite::c_dLowerBarrier;
  cout << "cap parameters:" << endl;
  cfl::test::print(uCapParameters);
  cout << "lower barrier = " << dLowerBarrier << endl;
  return prb::downAndOutCap(uCapParameters, dLowerBarrier, 
			    rModel);
}

MultiFunction prb::test::asianCall(cfl::AssetModel & rModel)
{
  cfl::test::print("ASIAN CALL OPTION IN ASSET MODEL");
  cout << endl;
  double dStrike = cfl::test::Black::c_dStrike;
  double dMaturity = cfl::test::Black::c_dMaturity;
  const std::vector<double> uResetTimes = cfl::test::Black::c_uBarrierTimes;

  cout << "strike = " << dStrike << endl;
  cout << "maturity = " << dMaturity << endl;
  cout << "reset times = "<< endl;
  cfl::test::print(uResetTimes.begin(), uResetTimes.end());

  return prb::asianCall(uResetTimes, dStrike, dMaturity, rModel);
}

MultiFunction prb::test::barrierUpOrDownAndOut_path(cfl::AssetModel & rModel)
{
  cfl::test::print("BARRIER UP-OR-DOWN-AND-OUT OPTION IN ASSET MODEL");
  cout << endl;
  double dLowerBarrier = cfl::test::Black::c_dLowerBarrier;
  double dUpperBarrier = cfl::test::Black::c_dUpperBarrier;
  double dNotional = cfl::test::Black::c_dNotional;
  const std::vector<double> uBarrierTimes = cfl::test::Black::c_uBarrierTimes;

  cout << "lower barrier = " << dLowerBarrier << endl;
  cout << "upper barrier = " << dUpperBarrier << endl;
  cout << "notional = " << dNotional << endl;
  cout << "barrier times = "<< endl;
  cfl::test::print(uBarrierTimes.begin(), uBarrierTimes.end());

  return prb::barrierUpOrDownAndOut_path(dNotional, dLowerBarrier, 
					 dUpperBarrier, uBarrierTimes, rModel);
}

cfl::MultiFunction prb::test::
putOnSavingsAccount(InterestRateModel & rModel) 
{
  cfl::test::print("PUT ON SAVINGS ACCOUNT IN INTEREST RATE MODEL");
  cout << endl;
  cfl::Data::CashFlow uAccount = cfl::test::HullWhite::swapParameters();
  uAccount.rate = cfl::test::HullWhite::c_dYield;
  cout << "account parameters: " << endl;
  cfl::test::print(uAccount);
  return  prb::putOnSavingsAccount(uAccount, rModel);
}

cfl::MultiFunction prb::test::
savingsAccount(InterestRateModel & rModel) 
{
  cfl::test::print("VALUE OF SAVINGS ACCOUNT IN INTEREST RATE MODEL");
  cout << endl;

  double dPeriod = cfl::test::HullWhite::c_dPeriod;
  double dNotional = cfl::test::HullWhite::c_dNotional;
  unsigned iNumberOfPayments = cfl::test::HullWhite::c_iNumberOfPeriods;
  
  cout << "period = " << dPeriod << endl;
  cout << "notional = " << dNotional << endl;
  cout << "number of payments = " << iNumberOfPayments << endl;

  return  prb::savingsAccount(dPeriod, iNumberOfPayments, dNotional, rModel);
}

cfl::MultiFunction prb::test::swaption(InterestRateModel & rModel, 
				       bool bPayFloat)
{
  cfl::test::print("SWAPTION IN INTEREST RATE MODEL");
  cout << endl;
  cfl::Data::Swap uSwapParameters = cfl::test::HullWhite::swapParameters();
  uSwapParameters.payFloat = bPayFloat;
  double dMaturity = cfl::test::HullWhite::c_dMaturity;
  cout << "maturity = " << dMaturity << endl;
  cout << "swap parameters:" << endl;
  cfl::test::print(uSwapParameters);
  return prb::swaption(uSwapParameters, dMaturity, rModel);
}

cfl::MultiFunction prb::test::futureOnLibor(InterestRateModel & rModel)
{
  cfl::test::print("LIBOR FUTURE IN INTEREST RATE MODEL");
  cout << endl;
  double dLiborPeriod = cfl::test::HullWhite::c_dPeriod;
  unsigned iFutureTimes = 20; 
  double dMaturity = 0.25;
  cout << "period for LIBOR = " << dLiborPeriod << endl;
  cout << "number of future times = " << iFutureTimes << endl;
  cout << "maturity = " << dMaturity << endl;
  return prb::
    futureOnLibor(dLiborPeriod, iFutureTimes, dMaturity, rModel);
}
