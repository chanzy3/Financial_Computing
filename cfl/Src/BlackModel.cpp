//Copyright (c) Dmitry Kramkov, 2000-2006. All rights reserved. 

#include <limits>
#include "cfl/BlackModel.hpp"
#include "cfl/Error.hpp"

using namespace cfl::Black;
using namespace cfl;

namespace cflBlack
{
  class Model: public IAssetModel
  {
  public:
    Model(const Black::Data & rData, const std::vector<double> & rEventTimes, 
	  double dInterval, const Brownian & rBrownian);

    IAssetModel * newModel(const std::vector<double> & rEventTimes) const; 

    Slice discount(unsigned iTime, double dBondMaturity) const; 

    Slice forward(unsigned iTime, double dForwardMaturity) const;

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
    Black::Data m_uData; 
    double m_dInterval;
    Brownian m_uBrownian; 
  };
}

// FUNCTION: cfl::Black::model

AssetModel cfl::Black::model(const Data & rData, 
			     double dInterval, const Brownian & rBrownian, 
			     const Extended & rExtended)
{
  std::vector<double> uEventTimes(1, rData.initialTime());
  return AssetModel(new cflBlack::Model(rData, uEventTimes, dInterval, rBrownian), 
		    rExtended);
}

// CLASS: cflBlack::Model

cflBlack::Model::Model(const Black::Data & rData, const std::vector<double> & rEventTimes, 
		       double dInterval, const Brownian & rBrownian)
  :m_uData(rData), m_dInterval(dInterval), m_uBrownian(rBrownian)
{
  ASSERT(rEventTimes.front() == rData.initialTime());
  std::vector<double> uVar(rEventTimes.size());
  std::transform(rEventTimes.begin(), rEventTimes.end(), uVar.begin(), 
		 cfl::pow(rData.volatility(), 2));
  m_uBrownian.assign(uVar, rEventTimes, dInterval);
}

IAssetModel * cflBlack::Model::newModel(const std::vector<double> & rEventTimes) const 
{
  return new Model(m_uData, rEventTimes, m_dInterval, m_uBrownian);
}

Slice cflBlack::Model::discount(unsigned iTime, double dMaturity) const 
{
  double dTime = eventTimes()[iTime];
  double dFactor = m_uData.discount()(dMaturity)/m_uData.discount()(dTime); 
  return Slice(this, iTime, dFactor);
} 

const std::vector<double> & cflBlack::Model::eventTimes() const
{
  return m_uBrownian.eventTimes();
}
 
unsigned cflBlack::Model::numberOfStates() const 
{ 
  return m_uBrownian.numberOfStates();
}

std::valarray<double> cflBlack::Model::origin() const 
{ 
  return m_uBrownian.origin();
}

unsigned cflBlack::Model::numberOfNodes(unsigned iEventTime, const std::vector<unsigned> & rDepend) const
{ 
  return m_uBrownian.numberOfNodes(iEventTime, rDepend);
}

Slice cflBlack::Model::state(unsigned iEventTime, unsigned iState) const 
{ 
  Slice uState = m_uBrownian.state(iEventTime, iState);
  uState.assign(*this);
  return uState;
}

void cflBlack::Model::addDependence(Slice & rSlice, 
				    const std::vector<unsigned> & rStates) const 
{
  rSlice.assign(m_uBrownian);
  m_uBrownian.addDependence(rSlice, rStates);
  rSlice.assign(*this);
}

void cflBlack::Model::rollback(Slice & rSlice, unsigned iEventTime) const 
{
  double dToday = eventTimes()[iEventTime];
  double dMaturity = eventTimes()[rSlice.timeIndex()];
  double dDiscount = m_uData.discount()(dMaturity)/m_uData.discount()(dToday);
  rSlice.assign(m_uBrownian);
  rSlice.rollback(iEventTime);
  rSlice.assign(*this);
  rSlice *= dDiscount; 
}

void cflBlack::Model::indicator(Slice & rSlice, double dBarrier) const
{
  rSlice.assign(m_uBrownian);
  m_uBrownian.indicator(rSlice, dBarrier);
  rSlice.assign(*this);
}

MultiFunction cflBlack::Model::interpolate(const Slice & rSlice) const
{
  Slice uSlice(rSlice);
  uSlice.assign(m_uBrownian);
  return cfl::interpolate(uSlice);
}

Slice cflBlack::Model::
forward(unsigned iTime, double dForwardMaturity) const
{
  PRECONDITION(iTime < eventTimes().size());
  double dRefTime = eventTimes()[iTime];
  PRECONDITION(dForwardMaturity >= dRefTime);

  if ((iTime >= eventTimes().size())||(dForwardMaturity < dRefTime)) {
    throw(NError::range("event time"));
  }	
  //forward price = exp(shape*factor + const);
  double dForward = m_uData.forward()(dForwardMaturity);
  double dVolat = m_uData.volatility()(dRefTime);
  double dShape = m_uData.shape()(dForwardMaturity);
  double dConst = std::log(dForward) 
    - 0.5*::pow(dVolat*dShape, 2)*(dRefTime - m_uData.initialTime());
  Slice uSlice(state(iTime, 0));
  uSlice*=dShape;
  uSlice+=dConst;
  return exp(uSlice);	
}
