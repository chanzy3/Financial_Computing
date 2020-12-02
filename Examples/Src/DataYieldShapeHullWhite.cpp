#include "Examples/Examples.hpp"

using namespace cfl;
using namespace cfl::Data;

class YieldShapeHW: public cfl::IFunction
{
public:
  YieldShapeHW(double dLambda, double dInitialTime)
  {
    PRECONDITION(dLambda >=0);
    m_dLambda = dLambda;
    m_dInitialTime = dInitialTime;
  }

  double operator()(double dT) const 
  {
    PRECONDITION(belongs(dT));
    double dF = m_dLambda*(dT - m_dInitialTime);
    if (dF< cfl::c_dEps) {
      return 1.;
    }
    double dYield = (1.-std::exp(-dF))/dF;
    return dYield;
  }

  bool belongs(double dT) const {
    return (dT >= m_dInitialTime);
  }
private:
  double m_dInitialTime, m_dLambda;
};

cfl::Function prb::
yieldShapeHullWhite(double dLambda, double dInitialTime) 
{
  return cfl::Function(new YieldShapeHW(dLambda, dInitialTime));
}
