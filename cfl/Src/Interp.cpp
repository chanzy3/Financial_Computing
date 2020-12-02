//  Copyright (c) Dmitry Kramkov, 2006. All right reserved. 
// Implementation of classes and functions declared in the corresponding *.hpp file. 

#include <cmath>
#include <algorithm>
#include <functional>
#include <limits>
#include "cfl/Interp.hpp"
#include "cfl/Error.hpp"
#include "cfl/Auxiliary.hpp"


using namespace cfl;

cfl::Interp::Interp(IInterp * pNewP)
  :m_uP(pNewP)
{}

namespace cflInterp 
{
  class Linear: public IFunction
  {
  public:
    Linear(const std::vector<double> & rArg, const std::vector<double> & rVal)
      :m_uArg(rArg), m_uVal(rVal)
    {
      POSTCONDITION(m_uArg.size() == m_uVal.size());		
    }
		
    double operator()(double dX) const 
    {
      ASSERT(dX >= m_uArg.front());
      ASSERT(dX <= m_uArg.back());
      if (belongs(dX)==false) {
	throw(NError::range("linear interpolation"));
      }
      if (dX == m_uArg.front()) { return m_uVal.front(); }
      std::vector<double>::const_iterator itArg = 
	std::lower_bound(m_uArg.begin(), m_uArg.end(), dX);
      std::vector<double>::const_iterator itVal = 
	m_uVal.begin() + (itArg - m_uArg.begin());
      double dX1 = *(itArg--);
      double dY1 = *(itVal--);
      double dX2 = *itArg;
      double dY2 = *itVal;
	
      return dY1 + (dY2 - dY1)*(dX - dX1)/(dX2 - dX1);			
    }
		
    bool belongs(double dX) const 
    { 
      return (dX>=m_uArg.front())&&(dX<=m_uArg.back()); 
    }
		
  private:
    std::vector<double> m_uArg, m_uVal;
  };

  class Spline: public IFunction
  {
  public:
    Spline(const std::vector<double> & rArg, const std::vector<double> & rVal)
      :m_uArg(rArg), m_uVal(rVal), m_uSD(rArg.size(), 0.)
    {
      ASSERT(m_uArg.size() == m_uVal.size());	
      ASSERT(m_uArg.size() >= 3);
			
      //calculate the vector of second derivatives
      int iSize = rArg.size();
      if (iSize == 3) {
	m_uSD[1] = 3.* ((rVal[2] - rVal[1])/(rArg[2] - rArg[1])
			- (rVal[1] - rVal[0])/(rArg[1] - rArg[0]))
	  /(rArg[2] - rArg[0]);
      }
      else if (iSize > 3) {
	//define tridiagonal matrix
	std::valarray<double> uL(iSize-3);
	std::transform(rArg.begin()+2, rArg.end()-1, rArg.begin()+1, 
		       &uL[0], std::minus<double>());
	uL/=6.;

	std::valarray<double> uD(iSize-2);
	std::transform(rArg.begin()+2, rArg.end(), rArg.begin(), &uD[0],
		       std::minus<double>());
	uD/=3.;

	Tridiag uTridiag(uL,uD,uL);
	std::valarray<double> uR(iSize-2);
	for (unsigned int iI=0; iI<uR.size(); iI++) {
	  uR[iI] = (rVal[iI+2] - rVal[iI+1])/(rArg[iI+2] - rArg[iI+1])
	    - (rVal[iI+1] - rVal[iI])/(rArg[iI+1] - rArg[iI]);
	}
	uTridiag.solve(uR);
	std::copy(begin(uR), end(uR), m_uSD.begin()+1);
      }
    }
		
    double operator()(double dX) const 
    {
      ASSERT(dX >= m_uArg.front());
      ASSERT(dX <= m_uArg.back());
      if (belongs(dX)==false) {
	throw(NError::range("spline interpolation"));
      }
      if (dX == m_uArg.front()) { return m_uVal.front(); }
			
      std::vector<double>::const_iterator itArg = 
	std::lower_bound(m_uArg.begin(), m_uArg.end(), dX);

      std::vector<double>::const_iterator itVal = 
	m_uVal.begin() + (itArg - m_uArg.begin());
      std::vector<double>::const_iterator itSD = 
	m_uSD.begin() + (itArg-m_uArg.begin());
			
      ASSERT(itArg > m_uArg.begin());

      double dX1 = *(itArg--);
      double dY1 = *(itVal--);
      double dSD1 = *(itSD--);
      double dX2 = *itArg;
      double dY2 = *itVal;
      double dSD2 = *itSD;
			
      double dDist = dX1 - dX2;
      double dA = (dX1 - dX)/dDist;
      double dB = 1. - dA;
      double dC = (dA*dA*dA - dA)*dDist*dDist/6.;
      double dD = (dB*dB*dB - dB)*dDist*dDist/6.;
			
      double dResult = dA*dY2 + dB*dY1 + dC*dSD2 + dD*dSD1;
      return dResult;		
    }
		
    bool belongs(double dX) const { 
      return (dX>=m_uArg.front())&&(dX<=m_uArg.back()); 
    }
		
  private:
    std::vector<double> m_uArg, m_uVal, m_uSD;
  };

  template <class InterpFunc> 
  class Helper: public IInterp
  {
  public:
    Function interpolate(const std::vector<double> & rArg, 
			 const std::vector<double> & rVal) const 
    {
      if (rArg.size() <=2) {
	return Function(new Linear(rArg, rVal));
      }
      else {
	return Function(new InterpFunc(rArg, rVal));
      }
    }
  };
}

cfl::Interp cfl::NInterp::linear() 
{
  return Interp(new cflInterp::Helper<cflInterp::Linear>());
}

cfl::Interp cfl::NInterp::spline() 
{
  return Interp(new cflInterp::Helper<cflInterp::Spline>());
}
