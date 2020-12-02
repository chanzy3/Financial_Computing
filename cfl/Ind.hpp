//Copyright (c) Dmitry Kramkov, 2000-2006. All rights reserved. 

#ifndef __cflInd_hpp__
#define __cflInd_hpp__

#include "cfl/Macros.hpp"
#include <memory>
#include <valarray>

/**
 * \file   Ind.hpp
 * \author Dmitry Kramkov
 * \date   2000-2006
 * 
 * \brief One-dimensional indicator functions.  
 * 
 * This file is dealing with numerically efficient construction of 
 * one-dimensional indicator functions. 
 */

namespace cfl
{
  /**
   * \ingroup cflNumeric
   * \defgroup cflInd	One-dimensional indicator functions. 
   * This module is dealing with with numerically efficient 
   * implementations of one-dimensional indicator functions.  
   */
  // @{

  //! Interface class for one-dimensional indicator function.
  /**
   * This is the abstract class for one-dimensional indicator function. Its implementation 
   * is used to construct concrete class Ind. The goal is to accelerate the 
   * numerical integration of discontinuous functions. 
   * \see Ind and NInd
   */
  class  IInd
  {
  public:
    /**
     * Virtual destructor. 
     */
    virtual ~IInd(){}
		
    /**
     * Constructs the indicator function of the event: "the function is greater then 
     * the barrier". It is going to be used to represent discontinuous 
     * functions and is intended to accelerate numerical integration of 
     * these functions. 
     * \param rValues Before the operation this array represents the values of 
     * the function on a grid. After the operation it contains the values of the 
     * event "the function is greater than the barrier".
     * \param dBarrier The level of the barrier. 
     */
    virtual void indicator(std::valarray<double> & rValues, double dBarrier) const = 0; 
  };
	
  //! Standard concrete class for indicator functions. 
  /** 
   * This is the standard class for indicator functions. It is implemented by a	
   *  dynamically allocated object derived from the interface class IInd. This class should 
   *  be used to represent discontinuous functions on a grid to accelerate the 
   *  numerical integration of these functions. 
   * \see IInd and NInd
   */
  class Ind
  {
  public:
    /** 
     * Constructs indicator function on a grid.
     * \param pNewInd A dynamically allocated implementation of the interface 
     * class IInd. 
     */
    explicit Ind(IInd * pNewInd = 0);

    /**
     * \copydoc IInd::indicator()
     */
    void indicator(std::valarray<double> & rValues, 
		   double dBarrier) const;		
  private:
    std::shared_ptr<IInd> m_pInd;
  };
		
	
  //! Implementations of indicator functions.
  /** This namespace contains implementations of indicator functions with the 
   * goal to accelerate the integration of discontinuous functions. 
   * \see IInd and Ind
   */
  namespace NInd 
  {
    /**
     *  Constructs an "efficient" implementation of an indicator function. Use it. 
     * \return An efficient implementation of the class Ind. 
     */
    Ind smart();

    /** 
     * Constructs "naive" implementation of an indicator function. It assigns 
     * one to any point where the value of the function is greater then the barrier; 
     * assigns zero otherwise. This implementation is not efficient. Do not use it. 
     * It is given here only for comparison. 
     * \return Naive and not efficient implementation of the class Ind. 
     */
    Ind naive();
  }
  // @}
}

#include "cfl/Inline/iInd.hpp"
#endif // of __cflInd_hpp__
