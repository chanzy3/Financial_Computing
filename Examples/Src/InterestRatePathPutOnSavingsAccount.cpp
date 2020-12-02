#include "Examples/Examples.hpp"
#include "Examples/ExamplesFunctions.hpp"

using namespace cfl;


cfl::MultiFunction prb::
putOnSavingsAccount(const Data::CashFlow & rAccount, 
		    InterestRateModel & rModel)
{
  std::vector<double> 
    uEventTimes(rAccount.numberOfPayments, rModel.initialTime());
  std::transform(uEventTimes.begin(), uEventTimes.end()-1, 
		 uEventTimes.begin()+1, 
		 [&rAccount](double dX){ return dX+rAccount.period; }); 
  rModel.assignEventTimes(uEventTimes);
  
  std::vector<unsigned> uResetIndexes(uEventTimes.size(), 0);
  std::transform(uResetIndexes.begin(), uResetIndexes.end()-1, 
		 uResetIndexes.begin()+1, 
		 [](unsigned iX){ return iX+1; }); 
  int iState = 
    rModel.addState(savingsAccountNextTime(rAccount.period, rAccount.notional, 
					   uResetIndexes, rModel));
  //last payment time before maturity
  int iTime = uEventTimes.size()-1;
  //value of savings account at maturity
  Slice uSavingsAccount = rModel.state(iTime, iState);
  //value of fixed payment at maturity
  double dFixedPayment = rAccount.notional;
  for (unsigned iI=0; iI<rAccount.numberOfPayments; iI++) {
    dFixedPayment += (dFixedPayment*rAccount.rate*rAccount.period);
  }
  double dTime = rModel.eventTimes()[iTime];
  Slice uOption = max(dFixedPayment - uSavingsAccount, 0.)
    * rModel.discount(iTime, dTime + rAccount.period);
  uOption.rollback(0);
  return interpolate(uOption, iState);	
}
