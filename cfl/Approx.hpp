//Copyright (c) Dmitry Kramkov, 2000-2006. All rights reserved. 

#ifndef __cflApprox_hpp__
#define __cflApprox_hpp__

#include "cfl/Interp.hpp"

/**
 * \file   Approx.hpp
 * \author Dmitry Kramkov
 * \date   2000-2008
 * 
 * \brief  Approximation of one-dimensional functions.
 * 
 * This file is dealing with numerical approximation of one-dimensional functions. 
 */

//!Main namespace for cfl library.
/**
 * Contains everything in cfl library.
 */
namespace cfl
{
  /**
   * \defgroup cflNumeric Numerical methods.
   * This module contains numerical routines used in implementations
   * of financial models. 
   */

  /**
   * \ingroup cflNumeric
   * \defgroup cflApprox One-dimensional approximation.
   * This module is dealing with approximation of one-dimensional functions.
   */
  //@{

  //! Interface class for numerical approximation.
  /**
   * This is the abstract class for numerical approximation. Its implementations 
   * are used to construct concrete class Approx.
   * \see Approx and NApprox
   */
  class  IApprox
  {
  public:
    /** 
     * Virtual destructor. 
     */
    virtual ~IApprox(){};

    /**
     * Constructs approximation scheme on the interval [\a dLeft, \a dRight ]. 
     * \param dLeft The left point of the interval.
     * \param dRight The right point of the interval.
     * \return A dynamically allocated implementation of IApprox. 
     */
    virtual IApprox* newApprox(double dLeft, double dRight) const = 0;	

    /**
     * Returns the nodes of the approximation scheme, that is, the
     * arguments where an approximated function should be
     * evaluated. For example, in the case of Chebyshev approximation
     * this function returns zeros of Chebyshev polynomials.
     * \return The nodes of the approximation scheme. 
     */
    virtual const std::valarray<double>& arg() const = 0;

    /**
     * Recovers a one-dimensional function by using its values at the nodes of the
     * approximation scheme. 
     * \param rValues Values of the function at the nodes of the approximation scheme. 
     * \returns The result of numerical approximation of the function.
     */
    virtual Function approximate(const std::valarray<double>& rValues) const = 0;		
  };

	
  //!Standard concrete class for numerical approximation. 
  /** 
   * This is the standard class for different approximation schemes. 
   * It is implemented by a dynamically allocated object derived from 
   * the interface class IApprox. 
   * \see IApprox and NApprox
   */
  class Approx
  {
  public:
    /**
     * A constructor.
     * \param pNewP A dynamically allocated implementation of IApprox.
     */
    explicit Approx(IApprox * pNewP = 0);

    /**
     * Constructs an approximation scheme on the interval [\a dLeft, \a dRight]. 
     * \param dLeft The left point of the interval.
     * \param dRight The right point of the interval.
     */
    void assign(double dLeft, double dRight);

    /**
     * \copydoc IApprox::arg() 
     */
    const std::valarray<double> & arg() const;

    /**
     * \copydoc IApprox::approximate()
     */
    Function approximate(const std::valarray<double> & rValues) const;

  private:
    std::shared_ptr<IApprox> m_uP;
  };

  //! Implementations of one-dimensional approximation methods.
  /** 
   * This namespace contains implementations of approximation
   * schemes for one-dimensional functions.  
   * \see Approx and IApprox
   */
  namespace NApprox
  {
    /** 
     * Constructs approximation scheme based on Chebyshev polynomials. 
     * \param rSize The function defines the number of nodes of the 
     * approximation scheme depending on the width of the interval. 
     * \return The class Approx that implements Chebyshev approximation. 
     */
    Approx chebyshev(const Function & rSize); 

    /**
     * Adapts interpolation to approximation. Hence, this function permits 
     * to use interpolation method for numerical approximation. 
     * The nodes of approximation are equally spaced.
     * \param rSize The function defines the number of nodes of the 
     * approximation scheme depending on the width of the interval. 
     * \param rInterp Interpolation scheme.
     * \return The class Approx that is based on interpolation method
     * given by \a rInterp. 
     */
    Approx toApprox(const Function & rSize, const Interp & rInterp);
  }
  //@}
}

#include "cfl/Inline/iApprox.hpp" 
#endif // of __cflApprox_hpp__
