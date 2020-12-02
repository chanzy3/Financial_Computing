//  Copyright (c) Dmitry Kramkov, 2000-2006. All rights reserved. 
// Implementation of classes and functions declared in the corresponding *.hpp file. 

#include <limits>
#include <iostream>
#include "cfl/Auxiliary.hpp"
#include "cfl/Error.hpp"

using namespace cfl;
using namespace std;

namespace cflTridiag
{
  void factor(std::valarray<double> & rL, std::valarray<double> & rD, 
	      std::valarray<double> & rU)
  {
    PRECONDITION((rU.size() == rL.size()) && 
		 (rU.size()+1 == rD.size()));
    int iSize = rD.size();
    for (int iI=0; iI< iSize-1; iI++) {
      rL[iI] /=rD[iI];
      rD[iI+1] -= rL[iI]*rU[iI];
    }
  }

  void solve(const std::valarray<double> & rL, const std::valarray<double> & rD, 
	     const std::valarray<double> & rU, std::valarray<double> & rX) 
  {
    PRECONDITION(rL.size() == rU.size());
    PRECONDITION(rD.size() == rL.size()+1);
    PRECONDITION(rX.size() == rD.size());

    int iSize = rD.size();
    //forward substitution
    for (int iI=0; iI<iSize-1; iI++) {
      rX[iI+1] -= rL[iI]*rX[iI];
    }
    //backward substitution
    rX[iSize-1] /= rD[iSize-1];
    for (int iI=iSize-2; iI>=0; iI--) {
      rX[iI] = (rX[iI] - rU[iI]*rX[iI+1])/rD[iI];
    }		
  }		
}

cfl::Tridiag::Tridiag()
{}

cfl::Tridiag::
Tridiag(const std::valarray<double> & rL, const std::valarray<double> & rD, 
	const std::valarray<double> & rU)
  :m_uL(rL), m_uD(rD), m_uU(rU) 
{
  cflTridiag::factor(m_uL, m_uD, m_uU);
}

void cfl::Tridiag::solve(std::valarray<double> & rX) const 
{
  cflTridiag::solve(m_uL, m_uD, m_uU, rX);
}

void cfl::Tridiag::
assign(const std::valarray<double> & rL, const std::valarray<double> & rD,
       const std::valarray<double> & rU) 
{
  m_uL.resize(rL.size());
  m_uU.resize(rU.size());
  m_uD.resize(rD.size());
  m_uL = rL;
  m_uU = rU;
  m_uD = rD;
  cflTridiag::factor(m_uL, m_uD, m_uU);
}
