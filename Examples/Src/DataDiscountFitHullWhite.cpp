#include <numeric>
#include "Examples/Examples.hpp"

using namespace cfl;

cfl::Function prb::
discountFitHullWhite(const std::vector<double> & rTimes, 
		     const std::vector<double> & rDiscount, 	
		     double dLambda, double dInitialTime)
{
  PRECONDITION(rTimes.size() == rDiscount.size()); 
  PRECONDITION(rTimes.size()>0);
  PRECONDITION(rTimes.front()>dInitialTime+cfl::c_dEps);
  PRECONDITION(std::equal(rTimes.begin()+1, rTimes.end(), rTimes.begin(), 
			  std::greater<double>())); 

  std::vector<double> uYield(rTimes.size());
  std::transform(rTimes.begin(), rTimes.end(), rDiscount.begin(), 
		 uYield.begin(), [dInitialTime](double dT, double dD) {
		   return - std::log(dD)/(dT - dInitialTime);
		 }); 

  Function uYieldShape = yieldShapeHullWhite(dLambda, dInitialTime);

  std::vector<double> uA(rTimes.size());
  std::transform(rTimes.begin(), rTimes.end(), uA.begin(),uYieldShape);
  
  double dCov = 
    std::inner_product(uYield.begin(), uYield.end(), uA.begin(), 0.);
  double dVar = std::inner_product(uA.begin(), uA.end(), uA.begin(),0.);

  double dA = dCov/dVar;

  return cfl::Data::discount(dA*uYieldShape,dInitialTime);
}
