//  Copyright (c) Dmitry Kramkov, 2000-2006. All rights reserved. 
// Implementation of classes and functions declared in the corresponding *.hpp file. 

#include <cmath>
#include <algorithm>
#include <functional>
#include "cfl/Ind.hpp"
#include "cfl/Error.hpp"
#include "cfl/Auxiliary.hpp"


using namespace cfl;
using namespace cfl::NInd;

cfl::Ind::Ind(IInd * pNewInd)
  :m_pInd(pNewInd) 
{}

namespace cflInd 
{
  class SmartInd: public std::function<double(double, double)> 
  {
  public:
    SmartInd(double & rHistInd)
      :m_rHistInd(rHistInd)
    {}
    double operator()(double dValLeft, double dValRight) 
    {
      double dResult = m_rHistInd;
      bool bBelow = (dValLeft <= 0.);
      //assume first that dValLeft <= 0
      if ((bBelow && (dValRight > 0.)) || (!bBelow && (dValRight < 0.))) {
	m_rHistInd = 0.5*dValRight/(dValRight - dValLeft);
	ASSERT((m_rHistInd >0.)&&(m_rHistInd <= 0.5));
      }
      else {
	m_rHistInd = 0.;
      }
      //now account for the possibility that dValLeft > 0
      if (!bBelow) {
	m_rHistInd = 0.5 - m_rHistInd;
      }
      dResult += m_rHistInd;
      return dResult;
    }
  private:
    double & m_rHistInd;
  };

  class  Smart: public IInd
  {
  public:
    void indicator(std::valarray<double> & rValues, 
		   double dBarrier) const 
    {
      rValues -= dBarrier;
      double dIndLeft = (rValues[0] <=0) ? 0. : 0.5;
      std::transform(begin(rValues), end(rValues)-1, begin(rValues)+1, 
		     &rValues[0], SmartInd(dIndLeft));
      //finish at right end
      if (rValues[rValues.size()-1] <=0 ) {
	rValues[rValues.size()-1] = dIndLeft;
      }
      else {
	rValues[rValues.size()-1] = dIndLeft+0.5;
      }
    }
  };

  class NaiveInd: public std::function<double(double)> 
  {
  public:
    NaiveInd(double dBarrier): m_dBarrier(dBarrier)
    {}
		
    double operator()(const double & dX) const 
    {
      return (dX >= m_dBarrier) ? 1 : 0;
    }
  private:
    double m_dBarrier;
  };
	
  class Naive: public cfl::IInd 
  {
  public:
    void indicator(std::valarray<double> & rValues, 
		   double dBarrier) const 
    {	
      std::transform(&rValues[0], &rValues[rValues.size()], &rValues[0], NaiveInd(dBarrier));
    }
  };
}

cfl::Ind cfl::NInd::smart() 
{ 
  return Ind(new cflInd::Smart()); 
}

cfl::Ind cfl::NInd::naive() 
{ 
  return Ind(new cflInd::Naive()); 
}
