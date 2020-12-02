#include "Examples/Examples.hpp"

using namespace cfl;

cfl::MultiFunction prb::
barrierUpOrDownAndOut(double dNotional, double dLowerBarrier, 
		      double dUpperBarrier, 
		      const std::vector<double> & rBarrierTimes, 
		      AssetModel & rModel)
{
  PRECONDITION(rModel.initialTime() < rBarrierTimes.front());
  PRECONDITION(dLowerBarrier < dUpperBarrier);

  std::vector<double> uEventTimes(rBarrierTimes);
  uEventTimes.insert(uEventTimes.begin(), rModel.initialTime());
  rModel.assignEventTimes(uEventTimes);
	
  int iTime = uEventTimes.size()-1;
  Slice uOption = rModel.cash(iTime, dNotional); 
  while (iTime > 0) {
    //uOption is the value to continue (the value of the option 
    //if no barriers have been crossed before and now)
    uOption*=indicator(rModel.spot(iTime), dLowerBarrier);
    uOption*=indicator(dUpperBarrier, rModel.spot(iTime));
    iTime--;
    uOption.rollback(iTime);
  }
  return interpolate(uOption);	
}
