#include "Examples/Examples.hpp"

using namespace cfl;

cfl::MultiFunction 
prb::americanPut(double dStrike, 
		 const std::vector<double> & rExerciseTimes, 
		 AssetModel & rModel) 
{
  PRECONDITION(rModel.initialTime() < rExerciseTimes.front());

  std::vector<double> uEventTimes(rExerciseTimes);
  uEventTimes.insert(uEventTimes.begin(), rModel.initialTime());
  rModel.assignEventTimes(uEventTimes);
  
  int iTime = uEventTimes.size()-1;
  Slice uOption = rModel.cash(iTime, 0.);
  while (iTime > 0) {
    //uOption is the value to continue
    uOption = max(uOption, dStrike - rModel.spot(iTime));
    iTime--;
    uOption.rollback(iTime);
  }
  return interpolate(uOption);	
}
