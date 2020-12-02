#include "Examples/Examples.hpp" 
#include "Examples/ExamplesFunctions.hpp"

using namespace cfl;

class IndUpDownOut: public IResetValues
{
public:
  IndUpDownOut(double dLowerBarrier, double dUpperBarrier, 
	       const AssetModel & rModel)
    :m_dLowerBarrier(dLowerBarrier), m_dUpperBarrier(dUpperBarrier), 
     m_rModel(rModel) 
  {
    ASSERT(dLowerBarrier < dUpperBarrier);
  }
		
  Slice resetValues(unsigned iTime, double dBeforeReset) const 
  {
    return dBeforeReset*indicator(m_rModel.spot(iTime), m_dLowerBarrier)*
      indicator(m_dUpperBarrier, m_rModel.spot(iTime));
  }
private:
  double m_dLowerBarrier; 
  double m_dUpperBarrier;
  const AssetModel & m_rModel;
};

cfl::PathDependent prb::
indUpDownOut(double dLowerBarrier, double dUpperBarrier, 
	     const std::vector<unsigned> & rBarrierIndexes, 
	     const AssetModel & rModel) 
{
  double dInitialValue = 1.;
  return PathDependent(new IndUpDownOut(dLowerBarrier, dUpperBarrier, rModel), 
		       rBarrierIndexes, dInitialValue);
}  

cfl::MultiFunction prb::
barrierUpOrDownAndOut_path(double dNotional, double dLowerBarrier, 
			   double dUpperBarrier, 
			   const std::vector<double> & rBarrierTimes, 
			   AssetModel & rModel)
{
  PRECONDITION(rModel.initialTime() < rBarrierTimes.front());
  PRECONDITION(dLowerBarrier < dUpperBarrier);

  std::vector<double> uEventTimes(rBarrierTimes);
  uEventTimes.insert(uEventTimes.begin(), rModel.initialTime());
  rModel.assignEventTimes(uEventTimes);
  
  std::vector<unsigned> uResetIndexes(rBarrierTimes.size(), 1);
  std::transform(uResetIndexes.begin(), uResetIndexes.end()-1, 
		 uResetIndexes.begin()+1, 
		 [](unsigned iX){ return iX+1; }); 
  int iInd = rModel.addState(indUpDownOut(dLowerBarrier, dUpperBarrier, 
					  uResetIndexes, rModel));

  int iTime = rModel.eventTimes().size()-1;
  Slice uOption = dNotional*rModel.state(iTime, iInd);
  uOption.rollback(0);
  return interpolate(uOption, iInd);	
}
