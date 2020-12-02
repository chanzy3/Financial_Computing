#include "Examples/Examples.hpp"
#include "Examples/ExamplesFunctions.hpp"

using namespace cfl;

cfl::Slice prb::rate(unsigned iTime, double dPeriod, 
		     const cfl::InterestRateModel & rModel)
{
  PRECONDITION(iTime < rModel.eventTimes().size());
  PRECONDITION(dPeriod>cfl::c_dEps);
  
  double dTime = rModel.eventTimes()[iTime] + dPeriod;
  Slice uDiscount = rModel.discount(iTime, dTime);
  return (1./uDiscount - 1.)/dPeriod;
}

cfl::MultiFunction prb::
futureOnLibor(double dLiborPeriod, 
	      unsigned iFutureTimes, double dMaturity, 
	      InterestRateModel & rModel)
{
  PRECONDITION(rModel.initialTime() < dMaturity); 
  PRECONDITION(iFutureTimes > 0);

  double dPeriod = (dMaturity - rModel.initialTime())/(iFutureTimes);
  std::vector<double> uEventTimes(iFutureTimes + 1);
  uEventTimes.front() = rModel.initialTime();
  std::transform(uEventTimes.begin(), uEventTimes.end()-1, 
		 uEventTimes.begin()+1,
		 [dPeriod](double dX){ return dX+dPeriod; }); 
  ASSERT(std::abs(uEventTimes.back()- dMaturity) < c_dEps);
  rModel.assignEventTimes(uEventTimes);
	
  int iTime = rModel.eventTimes().size()-1;
  Slice uFuture = 1.-rate(iTime, dLiborPeriod, rModel); 
  while (iTime > 0) {
    //uFuture is the future price today
    iTime--;
    uFuture.rollback(iTime);
    uFuture/=rModel.discount(iTime, rModel.eventTimes()[iTime] + dPeriod);
  }
	
  return interpolate(uFuture);
}
