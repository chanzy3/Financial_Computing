#include "Examples/Examples.hpp"

using namespace cfl;

cfl::MultiFunction prb::
downAndOutCap(const Data::CashFlow & rCap, double dLiborBarrier, 
	      InterestRateModel & rModel)
{
  std::vector<double> uEventTimes(rCap.numberOfPayments);
  uEventTimes.front() = rModel.initialTime();
  std::transform(uEventTimes.begin(), uEventTimes.end()-1, 
		 uEventTimes.begin()+1, 
		 [&rCap](double dX){ return dX+rCap.period; }); 
  rModel.assignEventTimes(uEventTimes);

  //last minus one payment time  
  int iTime = uEventTimes.size()-1; 
  //express barrier event in terms of discount factors
  double dUpperDiscount = 1./(1. + dLiborBarrier*rCap.period);
  //fixed payment plus notional (as percentage of notional)
  double dFixedFactor = 1. + rCap.period*rCap.rate;
  double dPaymentTime = rModel.eventTimes()[iTime] + rCap.period;
  Slice uDiscount = rModel.discount(iTime, dPaymentTime);
  //value of next payment (we multiply on notional at the end)
  Slice uOption = max(1.-uDiscount*dFixedFactor,0.);
  
  while (iTime > 0) {
    //uOption is the value to continue (the value of future payments
    //if no barriers have been crossed before and today). 
    uOption *= indicator(dUpperDiscount, uDiscount);
    iTime--;
    uOption.rollback(iTime);
    dPaymentTime = rModel.eventTimes()[iTime] + rCap.period;
    uDiscount = rModel.discount(iTime, dPaymentTime);
    //add value of next payment
    uOption += max(1.-uDiscount*dFixedFactor,0.); 
  }
  uOption *= rCap.notional;

  return interpolate(uOption);	
}
