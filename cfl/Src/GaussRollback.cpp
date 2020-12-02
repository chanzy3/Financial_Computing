//  Copyright (c) Dmitry Kramkov, 2000-2006. All rights reserved. 
// Implementation of classes and functions declared in the corresponding *.hpp file. 

#include <cmath>
#include <vector>
#include <limits>
#include <algorithm>
#include "cfl/GaussRollback.hpp"
#include "cfl/Error.hpp"
#include "cfl/Auxiliary.hpp"



using namespace cfl;

cfl::GaussRollback::GaussRollback(IGaussRollback * pNewP)
    :m_uP(pNewP)
{}

namespace cflGaussRollback
{
    void oneStep(std::valarray<double> & rV, 
		 const double  & dB) 
    {
	double dL = rV[0];
	double dR = rV[rV.size()-1];
	std::valarray<double> uRight(rV.shift(-1));
	uRight[0] = dL;
	std::valarray<double> uLeft(rV.shift(1));
	uLeft[uLeft.size()-1] = dR;
	//rV += dB*(uRight - 2*rV + uLeft);
	rV*=(1.-2.*dB);
	uRight+=uLeft;
	uRight*=dB;
	rV += uRight;
	//the boundary conditions below will insure 
	//accuracy for linear functions
	rV[0] = dL;
	rV[rV.size()-1] = dR;
    }


    // CLASS: Explicit
	
    class  Explicit: public IGaussRollback
    {
    public:
	Explicit(double dVarStepCoeff) 
	    :m_dVarStepCoeff(dVarStepCoeff) 
	    {
		ASSERT(m_dVarStepCoeff <= 1);
	    }

	Explicit(unsigned iSize, double dH, 
		 double dVar, double dVarStepCoeff) 
	    :m_iSize(iSize), m_dVarStepCoeff(dVarStepCoeff)
	    {
		PRECONDITION(dVarStepCoeff <= 1);
		PRECONDITION((iSize%2==1)&&(iSize>0));
		PRECONDITION(dH>0);
		PRECONDITION(dVar>0);
			
		//number of steps
		m_iSteps = static_cast<unsigned>(::floor(dVar/(m_dVarStepCoeff*dH*dH) + 0.5));
		if (m_iSteps <1) { 
		    m_iSteps = 1; 
		}

//define weights for finite difference
		double dStepVar = dVar/double(m_iSteps);
		m_dB = 0.5 * dStepVar / ::pow(dH,2);
		ASSERT(m_dB>0);
		ASSERT(m_dB<=0.5);
	    }
		
	IGaussRollback * newObject(unsigned iSize, double dH, 
				   double dVar) const 
	    {
		return new Explicit(iSize, dH, dVar, m_dVarStepCoeff);
	    }
		
	void rollback(std::valarray<double> & rVec) const
	    {
		PRECONDITION(rVec.size() == m_iSize);
		for (unsigned int iI=0; iI<m_iSteps; iI++) {
		    cflGaussRollback::oneStep(rVec, m_dB);
		}
	    }
		
    private:
	unsigned int m_iSize, m_iSteps;
	double m_dVarStepCoeff, m_dB;
    };
	
    // CLASS: Theta
	
    class  Theta: public IGaussRollback
    {
    public:
	Theta(double dTheta, const cfl::Function & rVarStep)
	    :m_dTheta(dTheta), m_uVarStep(rVarStep) 
	    {}
	Theta(unsigned iSize, double dH, double dVar, 
	      double dTheta, const cfl::Function & rVarStep) 
	    :m_iSize(iSize), m_dTheta(dTheta), m_uVarStep(rVarStep)
	    {
		ASSERT((iSize%2==1)&&(iSize>0));
		ASSERT((dH>0)&&(dVar>0));

		m_iSteps = static_cast<unsigned>(::ceil(dVar / m_uVarStep(dH)));
			
		double dA = dVar/(2.*m_iSteps*dH*dH);
		m_dB = m_dTheta*dA;

		double dLower = -(1.-dTheta)*dA;
		std::valarray<double> uLower(dLower, iSize-1);
		std::valarray<double> uUpper(dLower, iSize-1);
		double dDiag = 1. + 2.*dA*(1.-m_dTheta);
		std::valarray<double> uDiag(dDiag, iSize);
		//boundary condition: the second derivative at the boundary is 0
		uDiag[0] =1;
		uLower[iSize-2] = 0;
		uDiag[iSize-1] = 1;
		uUpper[0] = 0;
		m_uTridiag.assign(uLower, uDiag, uUpper);
	    }
		
	IGaussRollback * newObject(unsigned iSize, double dH, double dVar) const 
	    { 
		return new Theta(iSize, dH, dVar, m_dTheta, m_uVarStep); 
	    }

	void rollback(std::valarray<double> & rV) const
	    {
		PRECONDITION(rV.size() == m_iSize);
			
		if (m_dTheta > 0) {
		    for (unsigned int iI=0; iI<m_iSteps; iI++) {
			cflGaussRollback::oneStep(rV, m_dB);
			m_uTridiag.solve(rV);
		    }
		}
		else if (m_dTheta == 0) { //pure implicit
		    for (unsigned int iI=0; iI<m_iSteps; iI++) {
			m_uTridiag.solve(rV);
		    }
		}
		else {
		    ASSERT(false);
		    throw(NError::range("theta"));
		}
	    }
		
    private:
	unsigned m_iSize, m_iSteps;
	double m_dTheta, m_dB;
	Function m_uVarStep;
	Tridiag m_uTridiag;
    };

    const double c_dBinomial = 1.;
    const double c_dUniform = 2./3.;
    const double c_dImplicit = 0.;
    const double c_dCrankNicolson = 0.5;

    // CLASS: Improved

    class Improved: public IGaussRollback
    {
    public:
	Improved(const GaussRollback & rFast, const Function & rUniformSteps, 
		 const Function & rImplicitSteps)
	    :m_uFast(rFast), m_uUniformSteps(rUniformSteps), m_uImplicitSteps(rImplicitSteps)
	    {}
	Improved(unsigned iSize, double dH, double dVar, const GaussRollback & rFast, 
		 const Function & rUniformSteps, const Function & rImplicitSteps) 
	    :m_uFast(rFast), m_uUniform(new Explicit(c_dUniform)), m_uImplicit(new Theta(c_dImplicit, rImplicitSteps)), 
	     m_uUniformSteps(rUniformSteps), m_uImplicitSteps(rImplicitSteps)
	    {
		unsigned iUniformSteps = static_cast<unsigned>(m_uUniformSteps(dH));
		unsigned iImplicitSteps = static_cast<unsigned>(m_uImplicitSteps(dH));
		double dVarUniform = iUniformSteps*dH*dH*c_dUniform;
		double dVarImplicit = iImplicitSteps*dH*dH;
			
		if (dVarUniform + dVarImplicit >=dVar) {
		    m_bOnlyUniform = true;
		    m_uUniform.assign(iSize, dH, dVar);
		}
		else {
		    m_bOnlyUniform = false;
		    double dVarFast = dVar - dVarUniform - dVarImplicit;
		    ASSERT(dVarFast > 0);
		    m_uUniform.assign(iSize, dH, dVarUniform);
		    m_uFast.assign(iSize, dH, dVarFast);
		    m_uImplicit.assign(iSize, dH, dVarImplicit);
		}
	    }

	IGaussRollback * newObject(unsigned iSize, double dH, double dVar) const 
	    {
		return new Improved(iSize, dH, dVar, m_uFast, m_uUniformSteps, m_uImplicitSteps);
	    }

	void rollback(std::valarray<double> & rV) const 
	    {
		m_uUniform.rollback(rV);
		if (m_bOnlyUniform==false) {
		    m_uFast.rollback(rV);
		    m_uImplicit.rollback(rV);
		}			
	    }
    private:
	GaussRollback m_uFast, m_uUniform, m_uImplicit;
	bool m_bOnlyUniform;
	Function m_uUniformSteps, m_uImplicitSteps;
    };
}


GaussRollback cfl::NGaussRollback::binomial() 
{
    return GaussRollback(new cflGaussRollback::Explicit(cflGaussRollback::c_dBinomial));
}

GaussRollback cfl::NGaussRollback::uniform() 
{
    return GaussRollback(new cflGaussRollback::Explicit(cflGaussRollback::c_dUniform));
}

GaussRollback cfl::NGaussRollback::implicit(const cfl::Function & rVarStep) 
{
    return GaussRollback(new cflGaussRollback::Theta(cflGaussRollback::c_dImplicit, rVarStep));
}

GaussRollback cfl::NGaussRollback::implicit(double dVarStepCoeffCoeff) 
{
  Function uVarStep = toFunction([](double dX){return dX*dX;}) * dVarStepCoeffCoeff;
    return GaussRollback(new cflGaussRollback::Theta(cflGaussRollback::c_dImplicit, uVarStep));
}

GaussRollback cfl::NGaussRollback::crankNicolson(const cfl::Function & rVarStep) 
{
    return GaussRollback(new cflGaussRollback::Theta(cflGaussRollback::c_dCrankNicolson, rVarStep));
}

GaussRollback cfl::NGaussRollback::crankNicolson(double dVarStepCoeffCoeff) {
  Function uVarStep = toFunction([](double dX){return dX;}) * dVarStepCoeffCoeff;
    return GaussRollback(new cflGaussRollback::Theta(cflGaussRollback::c_dCrankNicolson, uVarStep));
}

GaussRollback cfl::NGaussRollback::
improved(const GaussRollback & rFast, const cfl::Function & rUniformSteps, 
	 const cfl::Function & rImplicitSteps)
{
    return GaussRollback(new cflGaussRollback::Improved(rFast, rUniformSteps, rImplicitSteps));
}



