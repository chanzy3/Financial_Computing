//  Copyright (c) Dmitry Kramkov, 2000-2006. All rights reserved. 
// Implementation of classes and functions declared in the corresponding *.hpp file. 

#include <cmath>
#include <limits>
#include "cfl/MultiFunction.hpp"
#include "cfl/Function.hpp"
#include "cfl/Error.hpp" 
#include "cfl/Auxiliary.hpp"


using namespace cfl;
using namespace std;

cfl::MultiFunction::MultiFunction(IMultiFunction * pNewF)
  :m_pF(pNewF)
{}

namespace cflMultiFunction
{
  // CLASS: Const
	
  class Const: public IMultiFunction
  {
  public:			
    Const(double dConst, unsigned iDim)
      :m_dConst(dConst), m_iDim(iDim)
    {}

    double operator()(const std::valarray<double> & rArg) const 
    { 
      PRECONDITION(rArg.size() == m_iDim);
      return m_dConst; 
    }	

    bool belongs(const std::valarray<double> & rX) const 
    { 
      PRECONDITION(rX.size() == m_iDim);
      return true; 
    }
    unsigned dim() const 
    { 
      return m_iDim; 
    }
		
  private:
    double m_dConst;
    unsigned m_iDim;
  };


  // CLASS: Composite

  class Composite: public IMultiFunction
  {
  public:
    Composite(const MultiFunction & rFunc, const std::function<double(double)> & rUnOp)
      :m_uFunc(rFunc), m_uUnOp(rUnOp)
    {}

    double operator()(const std::valarray<double> & rX) const 
    {
      PRECONDITION(rX.size() == m_uFunc.dim());
      return m_uUnOp(m_uFunc(rX)); 
    }
    bool belongs(const std::valarray<double> & rX) const 
    { 
      PRECONDITION(rX.size() == m_uFunc.dim());
      return m_uFunc.belongs(rX); 
    }
    unsigned dim() const 
    { 
      return m_uFunc.dim(); 
    }
  private:
    MultiFunction m_uFunc;
    std::function<double(double)> m_uUnOp;
  };


  // CLASS: BinComposite

  class BinComposite: public IMultiFunction
  {
  public:
    BinComposite(const MultiFunction & rFunc1, 
		 const MultiFunction & rFunc2, const std::function<double(double,double)> & rBinOp) 
      :m_uFunc1(rFunc1), m_uFunc2(rFunc2), m_uBinOp(rBinOp) 
    {
      POSTCONDITION(m_uFunc1.dim() == m_uFunc2.dim());
    }

		
    double operator()(const std::valarray<double> & rX) const 
    { 
      PRECONDITION(rX.size() == m_uFunc1.dim());
      return m_uBinOp(m_uFunc1(rX), m_uFunc2(rX)); 
    }

    bool belongs(const std::valarray<double> & rX) const 
    { 
      PRECONDITION(rX.size() == m_uFunc1.dim());
      return (m_uFunc1.belongs(rX))&&(m_uFunc2.belongs(rX)); 
    }

    unsigned dim() const 
    { 
      return m_uFunc1.dim(); 
    }
		
  private:
    MultiFunction m_uFunc1;
    MultiFunction m_uFunc2;
    std::function<double(double,double)> m_uBinOp;
  };
}

// CLASS: MultiFunction

cfl::MultiFunction::MultiFunction(double dV, unsigned iDim): 
  m_pF(new cflMultiFunction::Const(dV, iDim))
{}

MultiFunction & cfl::MultiFunction::operator=(double dV)
{
  m_pF.reset(new cflMultiFunction::Const(dV, dim()));
  return *this;
}

MultiFunction & cfl::MultiFunction::operator+=(const MultiFunction & rFunc)
{
  m_pF.reset(new cflMultiFunction::BinComposite
	     (*this, rFunc, std::plus<double>()));
  return *this;
}

MultiFunction & cfl::MultiFunction::operator*=(const MultiFunction & rFunc)
{
  m_pF.reset(new cflMultiFunction::BinComposite
	     (*this, rFunc, std::multiplies<double>()));
  return *this;
}

MultiFunction & cfl::MultiFunction::operator-=(const MultiFunction & rFunc)
{
  m_pF.reset(new cflMultiFunction::BinComposite
	     (*this, rFunc, std::minus<double>()));
  return *this;
}

MultiFunction & cfl::MultiFunction::operator/=(const MultiFunction & rFunc)
{
  m_pF.reset(new cflMultiFunction::BinComposite
	     (*this, rFunc, std::divides<double>()));
  return *this;
}

MultiFunction & cfl::MultiFunction::operator+=(double dX)
{
  m_pF.reset(new cflMultiFunction::Composite
	     (*this, [dX](double dY){return dY+dX;}));
  return *this;
}

MultiFunction & cfl::MultiFunction::operator-=(double dX)
{
  m_pF.reset(new cflMultiFunction::Composite
	     (*this, [dX](double dY){return dY-dX;}));
  return *this;
}

MultiFunction & cfl::MultiFunction::operator*=(double dX)
{
  m_pF.reset(new cflMultiFunction::Composite
	     (*this, [dX](double dY){return dY*dX;}));
  return *this;
}

MultiFunction & cfl::MultiFunction::operator/=(double dX)
{
  m_pF.reset(new cflMultiFunction::Composite
	     (*this, [dX](double dY){return dY/dX;}));
  return *this;
}
	
// GLOBAL FUNCTIONS

MultiFunction cfl::operator-(const MultiFunction & rFunc)
{
  return MultiFunction(new cflMultiFunction::Composite
		       (rFunc, std::negate<double>()));
}

MultiFunction cfl::abs(const MultiFunction & rFunc)
{
  return MultiFunction(new cflMultiFunction::Composite
		       (rFunc, [](double dX){ return std::abs(dX); })); 
}

MultiFunction cfl::exp(const MultiFunction & rFunc)
{
  return MultiFunction(new cflMultiFunction::Composite
		       (rFunc, [](double dX){ return std::exp(dX); })); 
}

MultiFunction cfl::log(const MultiFunction & rFunc)
{
  return MultiFunction(new cflMultiFunction::Composite
		       (rFunc, [](double dX){ return std::log(dX); })); 
}

MultiFunction cfl::sqrt(const MultiFunction & rFunc)
{
  return MultiFunction(new cflMultiFunction::Composite
		       (rFunc, [](double dX){ return std::sqrt(dX); })); 
}

MultiFunction cfl::operator*(const MultiFunction & rFunc1, const MultiFunction & rFunc2)
{
  return MultiFunction(new cflMultiFunction::BinComposite
		       (rFunc1, rFunc2, std::multiplies<double>()));
}

MultiFunction cfl::operator*(double dX, const MultiFunction & rFunc)
{
  return MultiFunction(new cflMultiFunction::Composite
		       (rFunc, [dX](double dY){return dX*dY;}));
}

MultiFunction cfl::operator*(const MultiFunction & rFunc, double dX)
{
  return dX*rFunc;
}

MultiFunction cfl::operator+(const MultiFunction & rFunc1, const MultiFunction & rFunc2)
{
  return MultiFunction(new cflMultiFunction::BinComposite
		       (rFunc1, rFunc2, std::plus<double>()));
}

MultiFunction cfl::operator+(double dX, const MultiFunction & rFunc)
{
  return MultiFunction(new cflMultiFunction::Composite
		       (rFunc, [dX](double dY){return dX+dY;})); 
}

MultiFunction cfl::operator+(const MultiFunction & rFunc,double dX)
{
  return dX+rFunc; 
}

MultiFunction cfl::operator-(const MultiFunction & rFunc1, const MultiFunction & rFunc2)
{
  return MultiFunction(new cflMultiFunction::BinComposite
		       (rFunc1, rFunc2, std::minus<double>()));
}

MultiFunction cfl::operator-(double dX, const MultiFunction & rFunc)
{
  return MultiFunction(new cflMultiFunction::Composite
		       (rFunc, [dX](double dY){return dX-dY;})); 
}

MultiFunction cfl::operator-(const MultiFunction & rFunc, double dX)
{
  return MultiFunction(new cflMultiFunction::Composite
		       (rFunc, [dX](double dY){return dY-dX;}));
}

MultiFunction cfl::operator/(const MultiFunction & rFunc1, const MultiFunction & rFunc2)
{
  return MultiFunction(new cflMultiFunction::BinComposite
		       (rFunc1, rFunc2, std::divides<double>()));
}

MultiFunction cfl::operator/(double dX, const MultiFunction & rFunc)
{
  return MultiFunction(new cflMultiFunction::Composite
		       (rFunc, [dX](double dY){return dX/dY;})); 
}

MultiFunction cfl::operator/(const MultiFunction & rFunc, double dX)
{
  return MultiFunction(new cflMultiFunction::Composite
		       (rFunc, [dX](double dY){return dY/dX;})); 
}

MultiFunction cfl::max(const MultiFunction & rFunc1, const MultiFunction & rFunc2)
{
  return MultiFunction(new cflMultiFunction::BinComposite
		       (rFunc1, rFunc2, [](double dX, double dY){return std::max(dX,dY);}));
}

MultiFunction cfl::max(double dX, const MultiFunction & rFunc)
{
  return MultiFunction(new cflMultiFunction::Composite
		       (rFunc, [dX](double dY){return std::max(dX,dY);})); 
}

MultiFunction cfl::max(const MultiFunction & rFunc, double dX)
{
  return max(dX,rFunc); 
}

MultiFunction cfl::min(const MultiFunction & rFunc1, const MultiFunction & rFunc2)
{
  return MultiFunction(new cflMultiFunction::BinComposite
		       (rFunc1, rFunc2, [](double dX, double dY){return std::min(dX,dY);}));
}

MultiFunction cfl::min(double dX, const MultiFunction & rFunc)
{
  return MultiFunction(new cflMultiFunction::Composite
		       (rFunc,  [dX](double dY){return std::min(dX,dY);})); 
}

MultiFunction cfl::min(const MultiFunction & rFunc, double dX)
{
  return min(dX,rFunc); 
}


MultiFunction cfl::pow(const MultiFunction & rFunc, double dX)
{
  return MultiFunction(new cflMultiFunction::Composite(rFunc, [dX](double dY){return std::pow(dY,dX);})); 
}

namespace cflMultiFunction
{
  template<class F, class B>
  class Adapter: public IMultiFunction
  {
  public:
    Adapter(const F & rF, const B & rB, unsigned iDim): 
      m_uF(rF), m_uB(rB), m_iDim(iDim)
    {}
    double operator()(const std::valarray<double> & rX) const 
    {
      PRECONDITION(rX.size() == m_iDim);
      return m_uF(rX);
    }
    bool belongs(const std::valarray<double> & rX) const 
    { 
      PRECONDITION(rX.size() == m_iDim);
      return m_uB(rX); 
    }
    unsigned dim() const 
    { 
      return m_iDim; 
    }
  private:
    F m_uF;
    B m_uB;
    unsigned m_iDim;
  };
}

MultiFunction cfl::toMultiFunction(double (*pF)(const std::valarray<double> &), 
				   bool (*pB)(const std::valarray<double> &), unsigned iDim)
{
  typedef std::function<double(const std::valarray<double> &)> F;
  typedef std::function<bool(const std::valarray<double> &)> B;
  return MultiFunction(new cflMultiFunction::Adapter<F, B>(F(pF), B(pB), iDim));
}


namespace cflMultiFunction
{
  class Adapter2: public IMultiFunction
  {
  public:
    Adapter2(const Function & rFunc, unsigned iArgIndex, unsigned iDim): 
      m_uFunc(rFunc), m_iArgIndex(iArgIndex), m_iDim(iDim)
    {
      POSTCONDITION(m_iArgIndex < m_iDim);		
    }

    double operator()(const std::valarray<double> & rX) const 
    {
      PRECONDITION(rX.size() == m_iDim);
      PRECONDITION(belongs(rX));
      return m_uFunc(rX[m_iArgIndex]);
    }
    bool belongs(const std::valarray<double> & rX) const 
    {
      PRECONDITION(rX.size() == m_iDim);
      return m_uFunc.belongs(rX[m_iArgIndex]);
    }
    unsigned dim() const { return m_iDim; }

  private:
    Function m_uFunc;
    unsigned m_iArgIndex, m_iDim;
  };
}

MultiFunction cfl::toMultiFunction(const cfl::Function & rFunc, unsigned iArgIndex, 
				   unsigned iDim) 
{
  return MultiFunction(new cflMultiFunction::Adapter2(rFunc, iArgIndex, iDim));
}

namespace cflMultiFunction
{
  class Section: public IMultiFunction
  {
  public:
    Section(const MultiFunction & rFunc, const std::vector<unsigned> & rState, 
	    const std::valarray<double> & rPoint)
      :m_uFunc(rFunc), m_uState(rState), m_uPoint(rPoint)
    {
      ASSERT(m_uFunc.dim() == m_uState.size() + m_uPoint.size());
      ASSERT(m_uState.size()>0);
    }

    double operator()(const std::valarray<double> & rX) const 
    {
      PRECONDITION(rX.size() == m_uState.size());
      PRECONDITION(belongs(rX));
      return m_uFunc(extend(rX));
    }
    bool belongs(const std::valarray<double> & rX) const 
    {
      PRECONDITION(rX.size() == dim());
      return m_uFunc.belongs(extend(rX));
    }
    unsigned dim() const { 
      return m_uState.size(); 
    }

  private:
    MultiFunction m_uFunc;
    std::vector<unsigned> m_uState;
    std::valarray<double> m_uPoint; 
    std::valarray<double> extend(const std::valarray<double> & rX) const 
    {
      ASSERT(rX.size() == m_uState.size());
      std::valarray<double> uP(m_uFunc.dim());
      ASSERT(uP.size()>= rX.size());
      unsigned int iJ=0;
      unsigned int iK=0;
      for (unsigned int iI=0; iI<uP.size(); iI++) {
	if ((iJ<m_uState.size())&&(m_uState[iJ]==iI)) {
	  uP[iI] = rX[iJ++];
	}
	else {
	  uP[iI] = m_uPoint[iK++];
	}
      }
      return uP;
    }
  };
}

MultiFunction cfl::toMultiFunction(const MultiFunction & rF, const std::vector<unsigned> & rS, 
				   const std::valarray<double> & rP)
{
  return MultiFunction(new cflMultiFunction::Section(rF, rS, rP));
}
