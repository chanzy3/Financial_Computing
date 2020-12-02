//  Copyright (c) Dmitry Kramkov, 2006, 2015. All rights reserved. 
// Implementation of classes and functions declared in the corresponding *.hpp file. 

#include <cmath>
#include <limits>
#include "cfl/Function.hpp"
#include "cfl/Auxiliary.hpp"
#include "cfl/MultiFunction.hpp"
#include "cfl/Error.hpp"

using namespace cfl;
using namespace std;

cfl::Function::Function(IFunction * pNewP)
  : m_pF(pNewP)
{}

namespace cflFunction
{
  // CLASS: Const
	
  class Const: public IFunction
  {
  public:			
    Const(double dConst, double dL = -std::numeric_limits<double>::infinity(), 
	  double dR=std::numeric_limits<double>::infinity())
      :m_dConst(dConst), m_dL(dL), m_dR(dR) 
    {
      ASSERT(m_dL <= m_dR);
    }
    double operator()(double  ) const 
    { 
      return m_dConst; 
    }	
    bool belongs(double  dX) const 
    { 
      return (dX>=m_dL) && (dX<=m_dR); 
    }
		
  private:
    double m_dConst, m_dL, m_dR;
  };

  // CLASS: Composite

  class Composite: public IFunction
  {
  public:
    Composite(const Function & rFunc, const std::function<double(double)> & rUnOp) 
      :m_uFunc(rFunc), m_uUnOp(rUnOp)
    {}

    double operator()(double dX) const 
    { 
      return m_uUnOp(m_uFunc(dX)); 
    }
    bool belongs(double dX) const 
    { 
      return m_uFunc.belongs(dX); 
    }
  private:
    Function m_uFunc;
    std::function<double(double)> m_uUnOp;
  };

  // CLASS: BinComposite

  class BinComposite: public IFunction
  {
  public:
    BinComposite(const Function & rFunc1, const Function & rFunc2, 
		 const std::function<double(double,double)> & rBinOp) 
      :m_uFunc1(rFunc1), m_uFunc2(rFunc2), m_uBinOp(rBinOp)
    {}

    double operator()(double dX) const 
    { 
      return m_uBinOp(m_uFunc1(dX), m_uFunc2(dX)); 
    }
    bool belongs(double dX) const 
    { 
      return (m_uFunc1.belongs(dX))&&(m_uFunc2.belongs(dX)); 
    }
		
  private:
    Function m_uFunc1;
    Function m_uFunc2;
    std::function<double(double,double)> m_uBinOp;
  };
}

// CLASS: Function

cfl::Function::Function(double dV, double dL, double dR)
  :m_pF(new cflFunction::Const(dV, dL, dR))
{}

Function & cfl::Function::operator=(double dV)
{
  m_pF.reset(new cflFunction::Const(dV));
  return *this;
}

Function & cfl::Function::operator+=(const Function & rFunc)
{
  m_pF.reset(new cflFunction::BinComposite(*this, rFunc, std::plus<double>())); 
  return *this;
}

Function & cfl::Function::operator*=(const Function & rFunc)
{
  m_pF.reset(new cflFunction::BinComposite(*this, rFunc, std::multiplies<double>()));
  return *this;
}

Function & cfl::Function::operator-=(const Function & rFunc)
{
  m_pF.reset(new cflFunction::BinComposite(*this, rFunc, std::minus<double>()));
  return *this;
}

Function & cfl::Function::operator/=(const Function & rFunc)
{
  m_pF.reset(new cflFunction::BinComposite(*this, rFunc, std::divides<double>()));
  return *this;
}

Function & cfl::Function::operator+=(double dX)
{
  m_pF.reset(new cflFunction::Composite
	     (*this, [dX](double dY) {return dY+dX;}));
  return *this;
}

Function & cfl::Function::operator-=(double dX)
{
  m_pF.reset(new cflFunction::Composite
	     (*this, [dX](double dY) {return dY-dX;}));
  return *this;
}

Function & cfl::Function::operator*=(double dX)
{
  m_pF.reset(new cflFunction::Composite
	     (*this, [dX](double dY) {return dY*dX;}));
  return *this;
}

Function & cfl::Function::operator/=(double dX)
{
  m_pF.reset(new cflFunction::Composite
	     (*this, [dX](double dY) {return dY/dX;}));
  return *this;
}
	
// GLOBAL FUNCTIONS

Function cfl::operator-(const Function & rFunc)
{
  return Function(new cflFunction::Composite
		  (rFunc, std::negate<double>()));
}

Function cfl::abs(const Function & rFunc)
{
  return Function(new cflFunction::Composite
		  (rFunc, [](double dX){ return std::abs(dX); })); 
}

Function cfl::exp(const Function & rFunc)
{
  return Function(new cflFunction::Composite
		  (rFunc, [](double dX){ return std::exp(dX); })); 
}

Function cfl::log(const Function & rFunc)
{
  return Function(new cflFunction::Composite
		  (rFunc, [](double dX){ return std::log(dX); })); 
}

Function cfl::sqrt(const Function & rFunc)
{
  return Function(new cflFunction::Composite
		  (rFunc, [](double dX){ return std::sqrt(dX); })); 
}

Function cfl::operator*(const Function & rFunc1, const Function & rFunc2)
{
  return Function(new cflFunction::BinComposite
		  (rFunc1, rFunc2, std::multiplies<double>()));
}

Function cfl::operator*(double dX, const Function & rFunc)
{
  return Function(new cflFunction::Composite
		  (rFunc, [dX](double dY){return dX*dY;})); 
}

Function cfl::operator*(const Function & rFunc, double dX)
{
  return dX*rFunc;
}

Function cfl::operator+(const Function & rFunc1, const Function & rFunc2)
{
  return Function(new cflFunction::BinComposite
		  (rFunc1, rFunc2, std::plus<double>()));
}

Function cfl::operator+(double dX, const Function & rFunc)
{
  return Function(new cflFunction::Composite
		  (rFunc, [dX](double dY){return dX+dY;}));
}

Function cfl::operator+(const Function & rFunc,double dX)
{
  return dX+rFunc; 
}

Function cfl::operator-(const Function & rFunc1, const Function & rFunc2)
{
  return Function(new cflFunction::BinComposite
		  (rFunc1, rFunc2, std::minus<double>()));
}

Function cfl::operator-(double dX, const Function & rFunc)
{
  return Function(new cflFunction::Composite
		  (rFunc, [dX](double dY){return dX-dY;})); 
}

Function cfl::operator-(const Function & rFunc, double dX)
{
  return Function(new cflFunction::Composite
		  (rFunc, [dX](double dY){return dY-dX;})); 
}

Function cfl::operator/(const Function & rFunc1, const Function & rFunc2)
{
  return Function(new cflFunction::BinComposite
		  (rFunc1, rFunc2, std::divides<double>()));
}

Function cfl::operator/(double dX, const Function & rFunc)
{
  return Function(new cflFunction::Composite
		  (rFunc, [dX](double dY){return dX/dY;})); 
}

Function cfl::operator/(const Function & rFunc, double dX)
{
  return Function(new cflFunction::Composite
		  (rFunc, [dX](double dY){return dY/dX;})); 
}

Function cfl::max(const Function & rFunc1, const Function & rFunc2)
{
  return Function(new cflFunction::BinComposite
		  (rFunc1, rFunc2, [](double dX, double dY){return std::max(dX,dY);})); 
}

Function cfl::max(double dX, const Function & rFunc)
{
  return Function(new cflFunction::Composite
		  (rFunc, [dX](double dY){return std::max(dX,dY);})); 
}

Function cfl::max(const Function & rFunc, double dX)
{
  return max(dX,rFunc); 
}

Function cfl::min(const Function & rFunc1, const Function & rFunc2)
{
  return Function(new cflFunction::BinComposite
		  (rFunc1, rFunc2, [](double dX, double dY){return std::min(dX,dY);})); 
}

Function cfl::min(double dX, const Function & rFunc)
{
  return Function(new cflFunction::Composite
		  (rFunc, [dX](double dY){return std::min(dX,dY);})); 
}

Function cfl::min(const Function & rFunc, double dX)
{
  return min(dX, rFunc);
}

Function cfl::pow(const Function & rFunc, double dX)
{
  return Function(new cflFunction::Composite(rFunc, [dX](double dY){return std::pow(dY,dX);})); 
}

namespace cflFunction
{
  class Adapter2: public IFunction
  {
  public:
    Adapter2(const MultiFunction & rFunc, unsigned iArg, 
	     const std::valarray<double> & rOtherArg)
      :m_uFunc(rFunc), m_iArg(iArg), m_uPoint(rFunc.dim())
    {
      ASSERT(m_uFunc.dim() <= 1);
      ASSERT(iArg < rFunc.dim());
      ASSERT(rOtherArg.size()+1 == rFunc.dim());

      if (rOtherArg.size()>0) {
	m_uPoint[std::slice(0,iArg,1)] = rOtherArg[std::slice(0,iArg,1)];
	m_uPoint[std::slice(iArg+1,rOtherArg.size()-iArg,1)] = 
	  rOtherArg[std::slice(iArg,rOtherArg.size()-iArg,1)];
      }
    }		
    double operator()(double dX) const 
    {
      PRECONDITION(belongs(dX));
      std::valarray<double> uPoint(m_uPoint);
      uPoint[m_iArg] = dX;
      return m_uFunc(uPoint);
    }
    bool belongs(double dX) const 
    {
      std::valarray<double> uPoint(m_uPoint);
      uPoint[m_iArg] = dX;
      return m_uFunc.belongs(uPoint);
    }
  private:
    MultiFunction m_uFunc;
    unsigned m_iArg;
    std::valarray<double> m_uPoint;
  };
}

Function cfl::toFunction(const cfl::MultiFunction & rFunc, unsigned iArg, const std::valarray<double> & rOtherArg)    
{
  return Function(new cflFunction::Adapter2(rFunc, iArg, rOtherArg));
}
