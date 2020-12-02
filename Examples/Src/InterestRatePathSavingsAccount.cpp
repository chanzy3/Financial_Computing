#include "Examples/Examples.hpp"
#include "Examples/ExamplesFunctions.hpp"

using namespace cfl;

class SavingsAccountNextTime: public IResetValues
{
public:
  SavingsAccountNextTime(double dPeriod, const InterestRateModel & rModel) 
    :m_dPeriod(dPeriod), m_rModel(rModel)
  {};
	  
  Slice resetValues(unsigned iTime, double dBeforeReset) const 
  {
    return dBeforeReset/ 
      m_rModel.discount(iTime, m_rModel.eventTimes()[iTime]+m_dPeriod);
  }
private:
  double m_dPeriod;
  const InterestRateModel & m_rModel;
};

cfl::PathDependent prb::
savingsAccountNextTime(double dPeriod, double dNotional, 
		       const std::vector<unsigned> & rResetTimes, 
		       const cfl::InterestRateModel & rModel)
{
  return PathDependent(new SavingsAccountNextTime(dPeriod, rModel),
		       rResetTimes, dNotional);
}


cfl::MultiFunction prb::
savingsAccount(double dPeriod, unsigned iNumberOfPeriods, 
	       double dNotional, InterestRateModel & rModel)
{
  std::vector<double> uEventTimes(iNumberOfPeriods+1, rModel.initialTime());
  std::transform(uEventTimes.begin(), uEventTimes.end()-1, 
		 uEventTimes.begin()+1, 
		 [dPeriod](double dX){ return dX+dPeriod; }); 
  rModel.assignEventTimes(uEventTimes);
  
  std::vector<unsigned> uResetIndexes(uEventTimes.size(), 0);
  std::transform(uResetIndexes.begin(), uResetIndexes.end()-1, 
		 uResetIndexes.begin()+1, 
		 [](unsigned iX){ return iX+1; }); 
  int iState = rModel.addState(savingsAccountNextTime(dPeriod, dNotional, 
						      uResetIndexes, rModel));
  int iTime = uEventTimes.size()-1;
  //the value of the savings account at maturity
  Slice uSavingsAccount = rModel.state(iTime, iState)
    * rModel.discount(iTime, rModel.eventTimes()[iTime]+dPeriod);
  uSavingsAccount.rollback(0);

  return interpolate(uSavingsAccount, iState);	
}
