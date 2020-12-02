#include "Examples/Examples.hpp"
#include "Examples/ExamplesFunctions.hpp"

using namespace cfl;

cfl::Slice prb::
couponBond(unsigned iTime, const Data::CashFlow & rBond, 
	   const InterestRateModel & rModel) 
{
  Slice uCashFlow = rModel.cash(iTime, 0.);
  double dTime = rModel.eventTimes()[iTime];
  for (unsigned iI=0; iI<rBond.numberOfPayments; iI++) {
    dTime += rBond.period;
    uCashFlow += rModel.discount(iTime, dTime);
  }
  uCashFlow *= (rBond.rate*rBond.period);
  uCashFlow += rModel.discount(iTime, dTime);
  uCashFlow *= rBond.notional;
  return uCashFlow;
}
	
cfl::Slice prb::
swap(unsigned iTime, const Data::Swap & rSwap, 
     const InterestRateModel & rModel) 
{
  //assume first that we receive fixed and pay float
  Slice uSwap = couponBond(iTime, rSwap, rModel)-rSwap.notional;
  if (!rSwap.payFloat) { //if we pay fixed
    uSwap *= -1;
  }
  return uSwap;
}


cfl::MultiFunction prb::
swaption(const Data::Swap & rSwap, double dMaturity, 
	 InterestRateModel & rModel)
{
  PRECONDITION(rModel.initialTime() < dMaturity);
  
  std::vector<double> uEventTimes(1, rModel.initialTime());
  uEventTimes.push_back(dMaturity);
  rModel.assignEventTimes(uEventTimes);
  
  int iTime = 1;
  Slice uOption = max(swap(iTime, rSwap, rModel),0); 
  uOption.rollback(0);
  return interpolate(uOption);	
}
