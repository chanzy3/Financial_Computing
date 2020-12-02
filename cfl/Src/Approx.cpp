//  Copyright (c) Dmitry Kramkov, 2000-2006. All rights reserved. 
// Implementation of classes and functions declared in the corresponding *.hpp file. 

#include <cmath>
#include <algorithm>
#include <limits>
#include "cfl/Approx.hpp"
#include "cfl/Function.hpp"
#include "cfl/Interp.hpp"
#include "cfl/Auxiliary.hpp"

using namespace cfl;
const double c_dPi = ::acos(-1.);

// CLASS Approx

cfl::Approx::Approx(IApprox * pNewP)
  :m_uP(pNewP)
{}

namespace cflApprox
{
  // CLASS ChebyshevApprox 

  class ChebyshevApprox: public IFunction
  {
  public:
    ChebyshevApprox(const std::valarray<double> & rArg, 
		    const std::valarray<double> & rValues, 
		    double dLeft, double dRight)
      :m_uValues(rValues), m_uArg(rArg), m_uCoeff(m_uArg.size()), 
       m_dL(dLeft), m_dR(dRight)
    {
      PRECONDITION(m_dL <= m_dR);
      PRECONDITION(m_uArg.size()>0);
      double dFac=2.0/m_uArg.size();
      for (unsigned int iI=0;iI<m_uArg.size();iI++) {
	double dSum=0.0;
	for (unsigned int iK=0;iK<m_uArg.size();iK++) {
	  dSum += rValues[rValues.size()-1-iK]* 
	    cos(c_dPi*iI*(iK+0.5)/m_uArg.size()); 
	}
	m_uCoeff[iI]=dFac*dSum;		
      }
    }

    bool belongs(double dX) const 
    { 
      return (m_dL<=dX)&&(dX<=m_dR);
    }

    double operator()(double dT) const 
    {
      ASSERT((dT>=m_dL)&&(dT<=m_dR));
      double dRes = 0.5*m_uCoeff[0];
      if (m_uArg.size()>1) {
	double a = 0.5*(m_dR-m_dL);
	double b = 0.5*(m_dR+m_dL);
	double dX = (dT-b)/a;
	double dA = 1.; //T_{n-2}(x)
	double dB = dX; //T_{n-1}(x)
	dRes += dB*m_uCoeff[1];
	double dC = 0.;
	for (unsigned int iI=2;iI<m_uCoeff.size();iI++) {
	  dC = 2.*dX*dB - dA; //T_n(x)
	  dRes += dC*m_uCoeff[iI];
	  dA = dB;
	  dB = dC;
	}
      }
      return dRes;
    }
  private:
    std::valarray<double> m_uValues, m_uArg, m_uCoeff;
    double m_dL, m_dR;
  };
	

  //CLASS: Chebyshev
	
  class Chebyshev: public IApprox
  {
  public:
    Chebyshev(const Function & rSize) : m_uSize(rSize){}
    Chebyshev(double dLeft, double dRight,  const Function & rSize)
      :m_uSize(rSize), m_dL(dLeft), m_dR(dRight)
    {
      ASSERT(dRight>=dLeft);
      unsigned iArgSize = std::max<unsigned>(static_cast<unsigned>(rSize(dRight-dLeft)), 1);
      m_uArg.resize(iArgSize);
      if (m_uArg.size() > 1) {
	ASSERT(dRight>dLeft);
	double dA = 0.5*(dRight-dLeft);
	double dB = 0.5*(dRight+dLeft);
	for (unsigned int iI=0; iI<m_uArg.size(); iI++) {
	  m_uArg[m_uArg.size()-1-iI] = dA*::cos(c_dPi*(iI+0.5)/m_uArg.size()) + dB;
	}
	POSTCONDITION(std::equal(&m_uArg[1], &m_uArg[m_uArg.size()], &m_uArg[0], 
				 std::greater<double>()));
      }
      else {
	ASSERT(m_uArg.size() == 1);
	m_uArg[0] = (dRight+dLeft)/2.;
      }
    }
		
    IApprox * newApprox(double dLeft, double dRight) const 
    { 
      return new Chebyshev(dLeft, dRight, m_uSize); 
    }

    const std::valarray<double> & arg() const 
    { 
      return m_uArg; 
    }

    Function approximate(const std::valarray<double> & rValues) const 
    {
      PRECONDITION(rValues.size()==m_uArg.size());
      if (rValues.size() == 1) {
	return cfl::Function(rValues[0], m_dL, m_dR);
      }
      else { 
	return Function(new cflApprox::ChebyshevApprox(m_uArg, rValues, m_dL, m_dR)); 
      }
    }		
  private:
    std::valarray<double> m_uArg;
    Function m_uSize;
    double m_dL, m_dR;
  };

  // CLASS Adapter 

  class Adapter: public IApprox
  {
  public:
    Adapter(const Function & rSize, const Interp & rInterp)
      :m_uSize(rSize), m_uInterp(rInterp) {}

    Adapter(const Function & rSize, const Interp & rInterp, 
	    double dLeft, double dRight) 
      :m_uSize(rSize), m_uInterp(rInterp), m_dL(dLeft), m_dR(dRight), 
       m_uArg(std::max<unsigned>(static_cast<unsigned>(rSize(dRight-dLeft)),1))
    {
      PRECONDITION(m_uArg.size() > 0);
      PRECONDITION(dRight >= dLeft);
      if (m_uArg.size() > 1) {
	ASSERT(dRight > dLeft);
	double dH = (dRight-dLeft)/(m_uArg.size() - 1.);
	m_uArg[0] = m_dL;
	std::transform(begin(m_uArg), end(m_uArg)-2, &m_uArg[1], 
		       [dH](double dX){return dH+dX;});
	m_uArg[m_uArg.size()-1] = m_dR;
      }
      else {
	ASSERT(m_uArg.size() == 1);
	m_uArg[0] = (dLeft + dRight)/2.;
      }
    }

    IApprox * newApprox(double dLeft, double dRight) const 
    {
      return new Adapter(m_uSize, m_uInterp, dLeft, dRight);
    }

    const std::valarray<double> & arg() const 
    {
      return m_uArg;
    }

    Function approximate(const std::valarray<double> & rValues) const 
    {
      PRECONDITION(rValues.size() == m_uArg.size());
      if (rValues.size() == 1) {
	return cfl::Function(rValues[0], m_dL, m_dR);
      }
      else { 
	return m_uInterp.interpolate(cfl::begin(m_uArg), cfl::end(m_uArg), cfl::begin(rValues));
      }
    }

  private:
    Function m_uSize;
    Interp m_uInterp;
    double m_dL, m_dR;
    std::valarray<double> m_uArg;
  };
}


cfl::Approx cfl::NApprox::chebyshev(const Function & rSize) 
{
  return Approx(new cflApprox::Chebyshev(rSize));
}

cfl::Approx cfl::NApprox::toApprox(const Function & rSize, const cfl::Interp & rInterp) 
{
  return Approx(new cflApprox::Adapter(rSize, rInterp));
}
