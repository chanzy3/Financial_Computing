//Copyright (c) Dmitry Kramkov, 2000-2006. All rights reserved. 

#ifndef __cflHullWhiteData_hpp__
#define __cflHullWhiteData_hpp__

#include "cfl/Data.hpp"

/**
 * \file   HullWhiteData.hpp
 * \author Dmitry Kramkov
 * \date   2000-2006
 * 
 * \brief Parameters of Hull and White model. 
 *
 * Contains the class which defines the parameters of Hull and White model. 
 */

namespace cfl
{
  /** 
   * \ingroup cflInterestRateModel
   * \defgroup cflHullWhite Hull and White model for interest rates. 
   * This module is dealing with the implementation of Hull and White model for interest rates. 
   * The Hull and White model is the most general one-factor model where forward prices 
   * of zero-coupon bonds are log normal under respective forward martingale measures. 
   * It belongs to the class of financial models where the main state 
   * process could be chosen to be a one-dimensional Brownian motion. 
   * \see cfl::InterestRateModel and cfl::Brownian.
   */
  //@{ 

  //! Hull and White model for interest rates. 
  /** This namespace contains classes and functions related with 
   *  Hull and White model for interest rates. 
   */
  namespace HullWhite
  {
    /// \addtogroup cflHullWhite
    //@{

    //! The parameters of Hull and White model. 
    /** This class defines the parameters of the Hull and White model. The set of parameters 
     * consists of discount, shape, and volatility curves. 
     */
    class Data
    {
    public:
      /**
       * Default constructor. 
       */
      Data(){};

      /** 
       * Constructs parameters of Hull and White model.
       * \param rDiscount A discount curve.
       * \param rVolatility A volatility curve for the factor process. 
       * \param rShape A shape function. This function defines the shape of 
       * changes in discount or yield curves. 
       * \param dInitialTime Initial time given as year fraction.
       */
      Data(const Function & rDiscount, 
	   const Function & rVolatility, 
	   const Function & rShape, 
	   double dInitialTime);

      /** 
       * Constructs parameters of Hull and White model with 
       * stationary volatility curve.
       * \param rDiscount A discount curve.
       * \param dSigma The volatility of short-term interest rate. 
       * \param dLambda The mean-reversion coefficient of short-term 
       * interest rate under the money market martingale measure. 
       * \param dInitialTime Initial time given as year fraction.
       */
      Data(const Function & rDiscount, 
	   double dSigma, 
	   double dLambda, 
	   double dInitialTime);

      /**
       * Accessor function to the initial time.  
       * \return The initial time as year fraction. 
       */
      double initialTime() const;

      /** 
       * Accessor function to discount curve. 
       * \return The discount curve. 
       */
      const Function & discount() const;

      /** 
       * Accessor function to volatility curve. 
       * \return The volatility curve for the state process. 
       */
      const Function & volatility() const;

      /** 
       * Accessor function to "shape" curve.  
       * \return The shape function. This function defines the shape of 
       * changes in discount or yield curves. 
       */
      const Function & shape() const;

    private:
      Function m_uDiscount;
      Function m_uVolatility;
      Function m_uShape;
      double m_dInitialTime;
    };
    //@}
  }
  //@} 
}

#include "cfl/Inline/iHullWhiteData.hpp"
#endif // of __cflHullWhiteData_hpp__

