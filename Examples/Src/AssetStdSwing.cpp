#include "Examples/Examples.hpp"

using namespace cfl;

cfl::MultiFunction 
prb::swing(double dStrike, 
	   const std::vector<double> & rExerciseTimes, 
	   unsigned iNumberOfExercises, AssetModel & rModel)
{
  PRECONDITION(rModel.initialTime() < rExerciseTimes.front());

  std::vector<double> uEventTimes(rExerciseTimes);
  uEventTimes.insert(uEventTimes.begin(), rModel.initialTime()); 
  rModel.assignEventTimes(uEventTimes);

  int iTime = rModel.eventTimes().size()-1;
  std::vector<Slice> uOption(iNumberOfExercises, rModel.cash(iTime,0.));
  while (iTime > 0) {
    //uOption[i] is the value to continue under the condition that 
    //i exercises have taken place before and at current time
    for (unsigned int iI=0; iI<uOption.size()-1; iI++) {
      uOption[iI] = 
	max(uOption[iI], uOption[iI+1] + rModel.spot(iTime) - dStrike); 
    }
    uOption.back() = max(uOption.back(), rModel.spot(iTime) - dStrike);
    iTime--;
    for (unsigned int iI=0; iI<uOption.size(); iI++) {
      uOption[iI].rollback(iTime);
    }
  }
  return interpolate(uOption.front());	
}
