//Copyright (c) Dmitry Kramkov, 2000-2006. All rights reserved. 

#ifndef __cflGaussRollback_hpp__
#define __cflGaussRollback_hpp__

#include "cfl/Function.hpp"

/**
 * \file   GaussRollback.hpp
 * \author Dmitry Kramkov
 * \date   2000-2006
 * 
 * \brief Conditional expectation with respect to gaussian distribution.
 *
 * This file is dealing with numerical implementation of the operator of 
 * conditional expectation with respect to gaussian distribution. 
 */

namespace cfl
{ 
  /** 
   * \ingroup cflNumeric
   * \defgroup cflGaussRollback Conditional expectation with respect to gaussian distribution. 
   * This module contains numerical implementations of the operator 
   * of conditional expectation with respect to gaussian distribution. 
   */
  // @{

  //! Interface class for the operator of conditional expectation with respect to gaussian distribution.
  /**
   * This is the abstract class for numerical implementations of the operator of conditional expectation with 
   * respect to gaussian distribution. Its implementation on a free store is used to construct standard 
   * concrete class GaussRollback. 
   * \see GaussRollback and NGaussRollback
   */
  class  IGaussRollback
  {
  public:
    /**
     * Virtual destructor. 
     */
    virtual ~IGaussRollback(){}

    /** 
     * Returns the pointer on a free store to the object which
     * implements the operator of conditional expectation with respect
     * to the gaussian distribution with given parameters for
     * functions defined on the grid.
     * \param dVar The variance of the gaussian distribution. 
     * \param iSize The number of points on the grid. 
     * \param dH The distance between the points on the grid. 
     * \return A dynamically allocated implementation of IGaussRollback. 
     */
    virtual IGaussRollback * newObject(unsigned iSize, 
				       double dH, 
				       double dVar 
				       ) const = 0;

    /** 
     * Replaces the values of the function on the grid with the values of its conditional 
     * expectation with respect to the gaussian distribution. 
     * \param rValues \em Before \p rollback this parameter represents the  
     * original values of the function. \em After \p rollback the original values 
     * are replaced with their conditional expectation with 
     * respect to the gaussian distribution.
     */
    virtual void rollback(std::valarray<double> & rValues) const = 0;
  };

  //! Concrete class for the operator of conditional expectation with respect to gaussian distribution.
  /**
   * This is the concrete class for numerical implementations of the operator of conditional 
   * expectation with respect to gaussian distribution. It is constructed by a dynamically 
   * allocated implementation of the interface class IGaussRollback.
   * \see IGaussRollback and NGaussRollback
   */	
  class GaussRollback
  {
  public:
    /** 
     * Constructs \p *this using dynamically allocated implementation of the 
     * interface class IGaussRollback. 
     * 
     * \param pNewP A pointer to a dynamic implementation of the
     * interface class IGaussRollback.
     */
    explicit GaussRollback(IGaussRollback * pNewP = 0);

    /** 
     * Transforms \p *this to the operator of conditional expectation with respect to the 
     * gaussian distribution for functions defined on the grid. 
     * \param dVar The variance of the gaussian distribution. 
     * \param iSize The number of points on the grid. 
     * \param dH The distance between the points on the grid. 
     */
    void assign(unsigned iSize, double dH, double dVar);

    /**
     * \copydoc IGaussRollback::rollback()
     */
    void rollback(std::valarray<double> & rValues) const;		
  private:
    std::shared_ptr<IGaussRollback> m_uP;
  };

  //! Implementations of the operator of conditional expectation with respect to gaussian distribution.
  /** This namespace contains different implementations of the class GaussRollback. 
   * \see IGaussRollback and GaussRollback
   */
  namespace NGaussRollback 
  {
    /** 
     * Implements the operator of conditional expectation with respect to gaussian distribution by  
     * an explicit finite difference scheme. The scheme is constructed so that the center weight is 
     * as close to zero as possible. 
     * \return Implemenation of GaussRollback by means of binomial explicit finite difference scheme. 
     */
    GaussRollback binomial();

    /** 
     * Implements the operator of conditional expectation with respect to gaussian distribution 
     * by an explicit finite difference scheme. The scheme is constructed so that all weights are 
     * almost equal to each other. 
     * \return Implemenation of GaussRollback by means of uniform explicit finite difference scheme. 
     */
    GaussRollback uniform();

    /** 
     * Implements the operator of conditional expectation with respect to gaussian distribution by   
     * an implicit finite difference scheme. 
     * \param rVarStep This functions determines the number of steps in the scheme by 
     * the formula \p dVar/(rVarStep(dH),  where 
     * \p dVar is the variance of distribution. 
     * \return Implementation of GaussRollback by means of a implicit finite difference scheme. 
     */		
    GaussRollback implicit(const Function & rVarStep);
		
    /** 
     * Default value for the coefficient that determines
     * the number of steps in the implicit scheme. 
     */
    const double c_dImplicitVarStepCoeff = 10.;

    /** 
     * Implements the operator of conditional expectation with respect to gaussian distribution by   
     * an implicit finite difference scheme. 
     * \param dVarStepCoeff This number determines the number of steps in the scheme by 
     * the formula \p dVar/(dVarStepCoeff*dH*dH),  where 
     * \p dVar is the variance of distribution and \p dH is the step on the grid. 
     * \return Implementation of GaussRollback by means of a implicit finite difference scheme. 
     */	
    GaussRollback implicit(double dVarStepCoeff = c_dImplicitVarStepCoeff);

    /** 
     * Returns the implementation of the operator of conditional expectation 
     * with respect to gaussian distribution by means of  
     * the Crank and Nicolson finite difference scheme. 
     * \param rVarStep This functions determines the number of steps in the scheme by 
     * the formula \p dVar/(rVarStep(dH),  where 
     * \p dVar is the variance of distribution. 
     * \return Implementation of GaussRollback by means of Crank-Nicolson scheme.
     */		
    GaussRollback crankNicolson(const Function & rVarStep);

    /** 
     * Default value for the coefficient that determines
     * the number of steps in the Crank and Nicolson scheme. 
     */
    const double c_dCrankNicolsonVarStepCoeff = 0.1;

    /** 
     * Returns the implementation of the operator of conditional expectation 
     * with respect to gaussian distribution by means of  
     * the Crank and Nicolson finite difference scheme. 
     * \param dVarStepCoeff This number determines the number of steps in the scheme by 
     * the formula \p dVar/(dVarStepCoeff*dH),  where 
     * \p dVar is the variance of distribution and \p dH is the step on the grid. 
     * \return Implementation of GaussRollback by means of Crank-Nicolson scheme.
     */			
    GaussRollback crankNicolson(double dVarStepCoeff = c_dCrankNicolsonVarStepCoeff);

    /** 
     * Default value for the number of steps of uniform
     * explicit scheme at the beginning of the
     * "three-layer" implementation of the operator of
     * conditional expectation for Gaussian distribution. 
     */
    const int c_iImprovedExplicitSteps = 30;

    /** 
     * Default value for the number of steps of implicit
     * scheme at the end of the
     * "three-layer" implementation of the operator of
     * conditional expectation for Gaussian distribution. 
     */
    const int c_iImprovedImplicitSteps = 10;

    /** 
     * Returns the implementation of the operator of conditional expectation 
     * with respect to gaussian distribution by means of sequential application 
     * of three schemes. 
     * First, we use explicit scheme with equal weights. This scheme efficiently 
     * handles  possible discontinuities of input functions. Second, we run 
     * some "fast" scheme (such as Crank and Nicolson). We finish with 
     * pure implicit scheme. This scheme cuts of random errors.  
     * \param rFast An implementation of "fast" numerical scheme (such as Crank and Nicolson). 
     * \param rUniformSteps This function determines the number of steps in the explicit 
     * scheme with equal weights. The number of steps in the schema equals 
     * \p rUniformSteps(dH).
     * \param rImplicitSteps This function determines the number of steps in the implicit 
     * scheme. The number of steps in the scheme equals  \p rImplicitSteps(dH).
     * \return Implementation of GaussRollback by a sequence of a three "rollback" operations:
     * start with uniform explicit for smoothness, proceed with some "fast" scheme and finish with a pure
     * implicit scheme for extra stability. 
     */			
    GaussRollback improved(const GaussRollback & rFast = crankNicolson(), 
			   const Function & rUniformSteps = Function(c_iImprovedExplicitSteps), 
			   const Function & rImplicitSteps = Function(c_iImprovedImplicitSteps)); 

  }
  // @}
}

#include "cfl/Inline/iGaussRollback.hpp"
#endif // of __cflGaussRollback_hpp__

