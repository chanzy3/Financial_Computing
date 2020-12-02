//  Copyright (c) Dmitry Kramkov, 2000-2006. All rights reserved. 
// Implementation of classes and functions declared in the corresponding *.hpp file. 

#include <cmath>
#include <algorithm>
#include <limits>
#include "cfl/Brownian.hpp"
#include "cfl/GaussRollback.hpp"
#include "cfl/Ind.hpp"
#include "cfl/Interp.hpp"
#include "cfl/Approx.hpp"
#include "cfl/PathDependent.hpp"
#include "cfl/Auxiliary.hpp"

using namespace cfl;
using namespace cfl::NBrownian;

//class Brownian

cfl::Brownian::Brownian(IBrownian * pNewP) 
  :m_pBrownian(pNewP)
{}	

namespace cflBrownian
{
  class TotalVar: public std::function<double(double, double)>
  {
  public:
    TotalVar(double dToday)
      :m_dToday(dToday)
    {}
    double operator()(double dVar, double dTime) const 
    {
      ASSERT(dTime >= m_dToday);
      return dVar*(dTime - m_dToday);
    }
  private:
    double m_dToday;
  };

  class Model: public cfl::IBrownian
  {
  public:
    Model(const std::vector<double> & rVar, const std::vector<double> & rEventTimes, 
	  double dInterval, double dQuality, const GaussRollback & rRollback, 
	  const Ind & rInd, const Interp & rInterp);
    Model(double dQuality, const GaussRollback & rRollback, 
	  const Ind & rInd, const Interp & rInterp);

    IBrownian * newModel(const std::vector<double> & rVar, const std::vector<double> & rEventTimes, 
			 double dInterval) const;

    const std::vector<double> & eventTimes() const;

    unsigned numberOfStates() const;
    Slice state(unsigned iTime, unsigned iState) const;
    unsigned numberOfNodes(unsigned iTime,  const std::vector<unsigned> & rDependence) const;
    std::valarray<double> origin() const;
    void addDependence(Slice & rSlice, const std::vector<unsigned> & rDependence) const;

    void rollback(Slice & rSlice, unsigned iTime) const;

    void indicator(Slice & rSlice, double dBarrier) const;

    MultiFunction interpolate(const Slice & rSlice) const;

  private:
    std::vector<double> m_uTotalVar, m_uEventTimes;
    double m_dInterval, m_dNumberOfStd, m_dH, m_dQuality;
    std::vector<unsigned> m_uSize;
    GaussRollback m_uGaussRollback;
    Ind m_uInd;
    Interp m_uInterp;
  };
}

// CLASS cflBrownian::Model

cflBrownian::Model::Model(double dQuality, const GaussRollback & rRollback, 
			  const Ind & rInd, const Interp & rInterp)
  :m_uGaussRollback(rRollback), m_uInd(rInd), m_uInterp(rInterp)
{
  m_dQuality = (dQuality > 1.) ? dQuality : 1.;
}

cflBrownian::Model::Model(const std::vector<double> & rVar, const std::vector<double> & rEventTimes, 
			  double dInterval, double dQuality, const GaussRollback & rRollback, 
			  const Ind & rInd, const Interp & rInterp)
  :m_uTotalVar(rVar.size()), m_uEventTimes(rEventTimes), m_uSize(rEventTimes.size()), 
   m_uGaussRollback(rRollback), m_uInd(rInd), m_uInterp(rInterp)
{
  PRECONDITION(rEventTimes.size() == rVar.size());
  if (std::equal(m_uEventTimes.begin()+1, m_uEventTimes.end(), m_uEventTimes.begin(), 
		 std::greater_equal<double>()) == false) {
    throw(cfl::NError::sort("vector of event times"));
  }

  m_dQuality = (dQuality > 1.) ? dQuality : 1.;
  m_dNumberOfStd = 3+std::log(1+m_dQuality);
  double dToday = rEventTimes.front();
  std::transform(rVar.begin(), rVar.end(), rEventTimes.begin(),  
		 m_uTotalVar.begin(), TotalVar(dToday));
  if (std::equal(m_uTotalVar.begin()+1, m_uTotalVar.end(), m_uTotalVar.begin(), 
		 std::greater_equal<double>()) == false) 
    {
      throw(cfl::NError::sort("vector of accumulated variances"));
    }

  double dH1 = 1./m_dQuality;
  m_dH = dH1;
  ASSERT(m_dH > 0);

  for (unsigned iI=0; iI<m_uSize.size(); iI++) 
    {
      m_uSize[iI] = 
	static_cast<unsigned>(2*::ceil((dInterval/2. + m_dNumberOfStd*std::sqrt(m_uTotalVar[iI]))/m_dH)+1) + 2; 
      ASSERT(m_uSize[iI]>0);
      ASSERT(m_uSize[iI]%2==1);
    }
  ASSERT(m_uSize[0]*m_dH >= dInterval);
  POSTCONDITION(m_uTotalVar.size() == m_uEventTimes.size());
  POSTCONDITION(m_uTotalVar.size() == m_uSize.size());
  POSTCONDITION(m_dH>0);
  POSTCONDITION(m_dNumberOfStd > 0);
}

IBrownian * cflBrownian::Model::newModel(const std::vector<double> & rVar, const std::vector<double> & rEventTimes, 
					 double dInterval) const 
{
  return new Model(rVar, rEventTimes, dInterval + c_dEps, m_dQuality, m_uGaussRollback, m_uInd, m_uInterp);
}

const std::vector<double> & cflBrownian::Model::eventTimes() const 
{ 
  return m_uEventTimes; 
}

unsigned cflBrownian::Model::numberOfStates() const 
{ 
  return 1; 
}

Slice cflBrownian::Model::state(unsigned iTime, unsigned iState) const 
{
  PRECONDITION(iState == 0);

  std::vector<unsigned> uDependence(1,0);

  int iSize = m_uSize[iTime];
  std::valarray<double> uValues(iSize);
  uValues[0] = -m_dH*(iSize-1)/2;
  std::transform(&uValues[0], &uValues[iSize-1], &uValues[1], 
		 [this](double dX){return m_dH+dX;}); 

  return Slice(*this, iTime, uDependence, uValues);
}

unsigned cflBrownian::Model::numberOfNodes(unsigned iTime,  const std::vector<unsigned> & rDependence) const 
{
  PRECONDITION(rDependence.size()<=1);
  if (rDependence.size()==0) {
    return 1;
  }
  else {
    ASSERT(rDependence.front() == 0);
    return m_uSize[iTime];
  }
}

std::valarray<double> cflBrownian::Model::origin() const 
{
  return std::valarray<double>(0.,1);
}

void cflBrownian::Model::addDependence(Slice & rSlice, const std::vector<unsigned> & rDependence) const
{
  PRECONDITION(rDependence.size()<=1);
  if ((rSlice.dependence().size()==0)&&(rDependence.size()==1)) {
    ASSERT(rSlice.values().size() ==1);
    std::valarray<double> uValues(rSlice.values()[0], m_uSize[rSlice.timeIndex()]);
    rSlice.assign(rDependence, uValues);
  }
}

void cflBrownian::Model::rollback(Slice & rSlice, unsigned iTime) const 
{
  PRECONDITION(rSlice.dependence().size() <=1);
  PRECONDITION(rSlice.ptrToModel() == this);
  PRECONDITION(rSlice.timeIndex() >= iTime);

  if (rSlice.timeIndex() > iTime) {
    std::valarray<double> uValues(rSlice.values());
    if (uValues.size() == 1) {
      rSlice.assign(iTime, rSlice.dependence(), uValues);
    }
    else {
      ASSERT(uValues.size() > 1); 
      ASSERT(rSlice.dependence().size() == 1);
      //double dToday = eventTimes().front();
      double dVar = m_uTotalVar[rSlice.timeIndex()] - m_uTotalVar[iTime];
      ASSERT(dVar >= 0);
      if (dVar == 0) {
	dVar = c_dEps;
      }
      GaussRollback uRoll(m_uGaussRollback);
      uRoll.assign(uValues.size(), m_dH, dVar);
      uRoll.rollback(uValues);
      unsigned iSize1 = m_uSize[iTime];
      if (iSize1 == uValues.size()) {
	rSlice.assign(iTime, rSlice.dependence(), uValues);
      }
      else {
	ASSERT(iSize1 < uValues.size());
	int iI = (uValues.size()-iSize1)/2;
	ASSERT(2*iI + iSize1 == uValues.size());
	std::valarray<double> uT(uValues[std::slice(iI, iSize1, 1)]);
	rSlice.assign(iTime, rSlice.dependence(), uT);
      }
    }
  }
}

void cflBrownian::Model::indicator(Slice & rSlice, double dBarrier) const
{
  std::valarray<double> uIndValues(rSlice.values());
  m_uInd.indicator(uIndValues, dBarrier);
  rSlice.assign(uIndValues);
}

MultiFunction cflBrownian::Model::interpolate(const Slice & rSlice) const
{
  std::valarray<double> uArg = state(rSlice.timeIndex(), 0).values();
  std::valarray<double> uVal = rSlice.values();
  return toMultiFunction(m_uInterp.interpolate(&uArg[0], &uArg[0] + uArg.size(), &uVal[0]),0,1);
}

cfl::Brownian 
cfl::NBrownian::model(double dQuality, 
		      const GaussRollback & rGaussRollback, 
		      const Ind & rInd, 
		      const Interp & rInterp
		      )
{
  return Brownian(new cflBrownian::Model(dQuality, rGaussRollback, 
					 rInd, rInterp));
}
