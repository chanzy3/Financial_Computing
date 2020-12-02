#include "Examples/Examples.hpp"
#include "Examples/ExamplesFunctions.hpp"

using namespace cfl;

class HistAverage: public IResetValues
{
public:
  HistAverage(const std::vector<unsigned> & rAverIndexes, 
	      const AssetModel & rModel) 
    :m_uAverIndexes(rAverIndexes), m_rModel(rModel)
  {}
		
  Slice resetValues(unsigned iTime, double dBeforeReset) const 
  {
    PRECONDITION(std::binary_search(m_uAverIndexes.begin(), 
				    m_uAverIndexes.end(), iTime));
    //number of averaging times including the current one
    int iTimes = std::lower_bound(m_uAverIndexes.begin(), 
				  m_uAverIndexes.end(), iTime) - 
      m_uAverIndexes.begin() + 1;
    return ((iTimes-1)*dBeforeReset  + m_rModel.spot(iTime))/iTimes;
  }

private:
  std::vector<unsigned> m_uAverIndexes;
  const AssetModel & m_rModel;
};

cfl::PathDependent prb::
histAverage(const std::vector<unsigned> & rAverIndexes, 
	    const AssetModel & rModel) 
{
  double dInitialValue = 0.;
  return PathDependent(new HistAverage(rAverIndexes, rModel), 
		       rAverIndexes, dInitialValue);
}

cfl::MultiFunction prb::
asianCall(const std::vector<double> & rAverTimes, double dStrike, 
	  double dMaturity, AssetModel & rModel)
{
  PRECONDITION(rModel.initialTime() < rAverTimes.front());
  PRECONDITION(rAverTimes.back() < dMaturity);

  std::vector<double> uEventTimes(1, rModel.initialTime());
  uEventTimes.insert(uEventTimes.end(), rAverTimes.begin(), 
		     rAverTimes.end());
  uEventTimes.push_back(dMaturity);
  rModel.assignEventTimes(uEventTimes);
  
  std::vector<unsigned> uResetIndexes(rAverTimes.size(), 1);
  std::transform(uResetIndexes.begin(), uResetIndexes.end()-1, 
		 uResetIndexes.begin()+1, 
		 [](unsigned iX){ return iX+1; });
  int iHistAverage = rModel.addState(histAverage(uResetIndexes, rModel));

  int iTime = rModel.eventTimes().size()-1;
  Slice uOption = max(rModel.state(iTime, iHistAverage)-dStrike, 0.);
  uOption.rollback(0);
  return interpolate(uOption, iHistAverage);	
}
