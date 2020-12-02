//  Copyright (c) Dmitry Kramkov, 2000-2006. All rights reserved. 
// Implementation of classes and functions declared in the corresponding *.hpp file. 

#include <functional>
#include <algorithm>
#include <limits>
#include "cfl/Error.hpp"
#include "cfl/Data.hpp"


using namespace cfl;

namespace cflData 
{
  class Discount: public IFunction
  {
  public:
    Discount(const Function & rYield, double dInitialTime)
      :m_uYield(rYield), m_dInitialTime(dInitialTime) 
    {}

    Discount(double dYield, double dInitialTime)
      :m_uYield(dYield), m_dInitialTime(dInitialTime) 
    {}

    double operator()(double  dT) const 
    {
      ASSERT(belongs(dT));
      double dDiscount = std::exp(-m_uYield(dT) * (dT - m_dInitialTime));
      return dDiscount;
    }

    bool belongs(double  dT) const 
    { 
      bool bBelongs = (dT >= m_dInitialTime) && (m_uYield.belongs(dT));
      return bBelongs;
    }

  private:
    Function m_uYield;
    double m_dInitialTime;
  };
}

Function cfl::Data::discount(double dYield, double dInitialTime) 
{
  return Function(new cflData::Discount(dYield, dInitialTime));
}

Function cfl::Data::discount(const Function & rYield, double dInitialTime)
{
  return Function(new cflData::Discount(rYield, dInitialTime));
}

//creation of volatility curves
namespace cflData
{
  class Volatility: public IFunction
  {
  public:
    Volatility(double dSigma, double dLambda, double dInitialTime)
    {
      if (dSigma < 0) {
	throw(NError::range("volatility"));
      }
      m_dSigma = dSigma;
      m_dLambda = dLambda;
      m_dInitialTime = dInitialTime;
    }

    double operator()(double  dT) const 
    {
      if (dT<m_dInitialTime) {
	throw(NError::range("time for volatility"));
      }

      double dX = 2.* m_dLambda * (dT-m_dInitialTime);
      if (std::abs(dX) < c_dEps) {
	return m_dSigma;
      }
      return m_dSigma * std::sqrt((std::exp(dX) - 1.)/dX);
    }

    bool belongs(double  dT) const 
    { 
      return (dT >= m_dInitialTime); 
    }

  private:
    double m_dSigma, m_dLambda, m_dInitialTime;
  };
}

Function cfl::Data::volatility(double dSigma, double dLambda, double dInitialTime)
{
  return Function(new cflData::Volatility(dSigma, dLambda, dInitialTime)); 
}

//creation of forward curves
namespace cflData 
{
  class Forward: public cfl::IFunction
  {
  public:
    Forward(double dSpot, const cfl::Function & rCostOfCarry, 
	    double dInitialTime)
      :m_dSpot(dSpot), m_dInitialTime(dInitialTime), m_uCostOfCarry(rCostOfCarry)
    {}

    Forward(double dSpot, double dCostOfCarry, double dInitialTime)
      :m_dSpot(dSpot), m_dInitialTime(dInitialTime), 
       m_uCostOfCarry(dCostOfCarry)
    {}

    double operator()(double  dT) const 
    {
      ASSERT(belongs(dT));
      return m_dSpot*std::exp(m_uCostOfCarry(dT)*(dT-m_dInitialTime));
    }

    bool belongs(double  dT) const 
    { 
      return (dT >= m_dInitialTime) && (m_uCostOfCarry.belongs(dT));
    }

  private:
    double m_dSpot, m_dInitialTime;
    Function m_uCostOfCarry;
  };
}

Function cfl::Data::forward(double dSpot, double dCostOfCarry, double dInitialTime)
{
  return Function(new cflData::Forward(dSpot, dCostOfCarry, dInitialTime));
}

Function cfl::Data::forward(double dSpot, const Function & rCostOfCarry, 
		      double dInitialTime)
{
  return Function(new cflData::Forward(dSpot, rCostOfCarry, dInitialTime)); 
}

//aux class
namespace cflData
{
  class Forward2: public IFunction
  {
  public:
    Forward2(double dSpot, double dDividend, const Function & rDiscount, 
	     double dInitialTime)
      :m_dSpot(dSpot), m_dDividend(dDividend),m_dInitialTime(dInitialTime), 
       m_uDiscount(rDiscount)
    {}

    bool belongs(double  dT) const 
    { 
      return (dT>=m_dInitialTime) && (m_uDiscount.belongs(dT)); 
    }

    double operator()(double  dTime) const 
    { 
      return m_dSpot*std::exp(-m_dDividend*(dTime-m_dInitialTime))
	/m_uDiscount(dTime);
    }

  private:
    double m_dSpot, m_dDividend, m_dInitialTime;
    Function m_uDiscount;
  };
}

Function cfl::Data::forward(double dSpot, double dDividendYield, 
		      const Function & rDiscount, double dInitialTime)
{
  return Function(new cflData::Forward2(dSpot, dDividendYield, rDiscount, dInitialTime)); 
}


Function cfl::Data::assetShape(double dLambda, double dInitialTime)
{
  return discount(dLambda, dInitialTime);
}

// CLASS: ShapeBond
namespace cflData
{
  class ShapeBond: public IFunction
  {
  public:
    ShapeBond(double dLambda, double dInitialTime)
    {
      m_dLambda = dLambda;
      m_dInitialTime = dInitialTime;
    }

    double operator()(double  dTime) const
    {
      PRECONDITION(belongs(dTime));
      double dT = dTime - m_dInitialTime;

      double dResult;
      if ( std::abs(m_dLambda) <= c_dEps) {
	dResult = dT;
      }
      else {
	dResult = (1. - std::exp(-m_dLambda*dT))/m_dLambda;
      }
      return dResult;
    }

    bool belongs(double  dT) const 
    {
      return (dT>=m_dInitialTime);
    }

  private:
    double m_dLambda, m_dInitialTime;
  };
}

Function cfl::Data::bondShape(double dLambda, double dInitialTime)
{
  return Function(new cflData::ShapeBond(dLambda, dInitialTime));
}

// CLASS: Swap

cfl::Data::Swap::Swap(const CashFlow & rCashFlow, bool bPayFloat)
  :CashFlow(rCashFlow), payFloat(bPayFloat)
{}


