//  Copyright (c) Dmitry Kramkov, 2000-2006. All rights reserved. 
// Implementation of classes and functions declared in the corresponding *.hpp file. 

#include <limits>
#include "cfl/HullWhiteModel.hpp"
#include "cfl/Error.hpp"

using namespace cfl::HullWhite;
using namespace cfl;

namespace cflHullWhite
{
  class Model: public IInterestRateModel
  {
  public:
    Model(const HullWhite::Data & rData, const std::vector<double> & rEventTimes, 
	  double dInterval, const Brownian & rBrownian);

    IInterestRateModel * newModel(const std::vector<double> & rEventTimes) const; 

    Slice discount(unsigned iTime, double dBondMaturity) const; 

    const std::vector<double> & eventTimes() const;
 
    unsigned numberOfStates() const;

    unsigned numberOfNodes(unsigned iEventTime, const std::vector<unsigned> & rDependence) const; 

    std::valarray<double> origin() const;

    Slice state(unsigned iEventTime, unsigned iState) const;

    void addDependence(Slice & rSlice, 
		       const std::vector<unsigned> & rStates) const;

    void rollback(Slice & rSlice, unsigned iEventTime) const;

    void indicator(Slice & rSlice, double dBarrier) const;

    MultiFunction interpolate(const Slice & rSlice) const;

  private:
    HullWhite::Data m_uData; 
    double m_dInterval;
    Brownian m_uBrownian; 
  };
}

// FUNCTION: cfl::HullWhite::model

InterestRateModel cfl::HullWhite::model(const HullWhite::Data & rData, 
					double dInterval, const Brownian & rBrownian, 
					const Extended & rExtended)
{
  std::vector<double> uEventTimes(1, rData.initialTime());
  return InterestRateModel(new cflHullWhite::Model(rData, uEventTimes, dInterval, rBrownian), 
			   rExtended);
}

// CLASS: cflHullWhite::Model

cflHullWhite::Model::Model(const HullWhite::Data & rData, const std::vector<double> & rEventTimes, 
			   double dInterval, const Brownian & rBrownian)
  :m_uData(rData), m_dInterval(dInterval), m_uBrownian(rBrownian)
{
  std::vector<double> uVar(rEventTimes.size());
  std::transform(rEventTimes.begin(), rEventTimes.end(), uVar.begin(), 
		 cfl::pow(rData.volatility(), 2));
  m_uBrownian.assign(uVar, rEventTimes, dInterval);
}

IInterestRateModel * cflHullWhite::Model::newModel(const std::vector<double> & rEventTimes) const 
{
  return new Model(m_uData, rEventTimes, m_dInterval, m_uBrownian);
}

Slice cflHullWhite::Model::discount(unsigned iTime, double dBondMaturity) const 
{
  PRECONDITION(iTime < eventTimes().size());
  PRECONDITION(dBondMaturity >= eventTimes()[iTime]);
  double dRefTime = eventTimes()[iTime];	
  if (dBondMaturity==dRefTime) {
    return Slice(this, iTime, 1.);
  }
 
  double dA = m_uData.shape()(dRefTime);
  double dB = m_uData.shape()(dBondMaturity);
  double dC = m_uData.shape()(eventTimes().back());
  double dVar = ::pow(m_uData.volatility()(dRefTime),2)*
    (dRefTime-m_uData.initialTime());
  double dForwardDiscount = 
    m_uData.discount()(dBondMaturity)/m_uData.discount()(dRefTime);
  Slice uDiscount = exp(state(iTime,0)*(dB-dA));
  uDiscount*=(dForwardDiscount*std::exp(- 0.5*(dB-dA)*(dA+dB-2.*dC)*dVar));
  return uDiscount; 
} 

const std::vector<double> & cflHullWhite::Model::eventTimes() const
{
  return m_uBrownian.eventTimes();
}
 
unsigned cflHullWhite::Model::numberOfStates() const 
{ 
  return m_uBrownian.numberOfStates();
}

std::valarray<double> cflHullWhite::Model::origin() const 
{ 
  return m_uBrownian.origin();
}


unsigned cflHullWhite::Model::numberOfNodes(unsigned iEventTime, const std::vector<unsigned> & rDepend) const
{ 
  return m_uBrownian.numberOfNodes(iEventTime, rDepend);
}

Slice cflHullWhite::Model::state(unsigned iEventTime, unsigned iState) const 
{ 
  Slice uState = m_uBrownian.state(iEventTime, iState);
  uState.assign(*this);
  return uState;
}

void cflHullWhite::Model::addDependence(Slice & rSlice, 
					const std::vector<unsigned> & rStates) const 
{
  rSlice.assign(m_uBrownian);
  m_uBrownian.addDependence(rSlice, rStates);
  rSlice.assign(*this);
}

void cflHullWhite::Model::rollback(Slice & rSlice, unsigned iEventTime) const 
{
  rSlice /= discount(rSlice.timeIndex(), eventTimes().back());
  rSlice.assign(m_uBrownian);
  rSlice.rollback(iEventTime);
  rSlice.assign(*this);
  rSlice *= discount(iEventTime, eventTimes().back());
}

void cflHullWhite::Model::indicator(Slice & rSlice, double dBarrier) const
{
  rSlice.assign(m_uBrownian);
  m_uBrownian.indicator(rSlice, dBarrier);
  rSlice.assign(*this);
}

MultiFunction cflHullWhite::Model::interpolate(const Slice & rSlice) const
{
  Slice uSlice(rSlice);
  uSlice.assign(m_uBrownian);
  return cfl::interpolate(uSlice);
}
