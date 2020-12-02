#include "Examples/Examples.hpp"
#include "cfl/Interp.hpp"

using namespace cfl;

cfl::Function prb::
discountLogLinearInterp(const std::vector<double> & rTimes, 
			const std::vector<double> & rDiscount, 
			double dInitialTime)
{
  PRECONDITION(rTimes.size() == rDiscount.size()); 
  PRECONDITION(rTimes.size()>0);
  PRECONDITION(rTimes.front()>dInitialTime);
  PRECONDITION(std::equal(rTimes.begin()+1, rTimes.end(), rTimes.begin(), 
			  std::greater<double>())); 

  //times for interpolation = initial time + discount times
  std::vector<double> uTimes(rTimes);
  uTimes.insert(uTimes.begin(),dInitialTime);

  //logarithm of discount factors
  std::vector<double> uLogDiscount(uTimes.size());
  uLogDiscount.front() = 0.;
  std::transform(rDiscount.begin(), rDiscount.end(), uLogDiscount.begin()+1, 
		 [](double dX){ return std::log(dX); }); 

  //linear interpolation of the logarithm of discount factors
  cfl::Interp uLinear = NInterp::linear(); 
  Function uLogDiscountFunction = 
    uLinear.interpolate(uTimes.begin(), uTimes.end(), uLogDiscount.begin()); 

  return cfl::exp(uLogDiscountFunction);
}
