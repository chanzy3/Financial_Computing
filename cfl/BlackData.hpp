//Copyright (c) Dmitry Kramkov, 2000-2006. All rights reserved. 

#ifndef __cflBlackData_hpp__
#define __cflBlackData_hpp__

/**
 * \file BlackData.hpp
 * \author Dmitry Kramkov
 * \date   2000-2006
 * 
 * \brief Parameters of Black model. 
 *
 * Contains the class which defines the parameters of Black model. 
 */

#include "cfl/Data.hpp"

namespace cfl
{
  /** 
   * \ingroup cflAssetModel
   * \defgroup cflBlack Black model for a single asset. 
   * This module is dealing with an implementation of the classical
   * Black model for a single asset. This model is the most general
   * one-factor model with log-normal forward prices (hence, the most
   * general one-factor model where one can use the Black and Scholes
   * formula). It belongs to the class of financial models where the state 
   * process can be chosen to be a one-dimensional Brownian motion. 
   * \see cfl::Brownian
   */
  //@{

  //! Black model for a single asset. 
  /** 
   * This namespace contains functions and classes related with the 
   * Black model for a single asset.
   */
  namespace Black
  {
    //! The parameters of Black model. 
    /** 
     * \ingroup cflBlack
     * This class defines the parameters of the Black model. The set of parameters 
     * consists of discount, forward, shape, and volatility curves. 
     */
    class Data
    {
    public:
      /**
       * Default constructor. 
       */ 
      Data(){};
 
      /** 
       * Constructs parameters of classical Black model.
       * \param rDiscount A discount curve.
       * \param rForward A forward curve.
       * \param rVolatility A volatility curve. 
       * \param dInitialTime Initial time given as year fraction.
       */
      Data(const Function & rDiscount, const Function & rForward, 
	   const Function  & rVolatility, double dInitialTime);

      /** 
       * Constructs parameters of classical Black model with constant volatility.
       * \param rDiscount A discount curve.
       * \param rForward A forward curve.
       * \param dSigma A volatility. 
       * \param dInitialTime Initial time given as year fraction.
       */
      Data(const Function & rDiscount, const Function & rForward, 
	   double dSigma, double dInitialTime);

      /** 
       * Constructs parameters of general Black model.
       * \param rDiscount A discount curve.
       * \param rForward A forward curve.
       * \param rVolatility A volatility curve. 
       * \param rShape A shape function. This function defines the shape of 
       * movements of the curve of forward prices. 
       * \param dInitialTime Initial time given as year fraction.
       */
      Data(const Function & rDiscount, const Function & rForward, 
	   const Function  & rVolatility, const Function  & rShape, 
	   double dInitialTime);

      /** 
       * Constructs parameters of general Black model with stationary volatility.
       * \param rDiscount A discount curve.
       * \param rForward A forward curve.
       * \param dSigma The volatility of spot price. 
       * \param dLambda The mean-reversion coefficient for log of spot price under 
       * the risk-neutral measure. 
       * \param dInitialTime Initial time given as year fraction.
       */
      Data(const Function & rDiscount, const Function & rForward, 
	   double dSigma, double dLambda, double dInitialTime);

      /**
       * \copydoc HullWhite::Data::initialTime 
       */
      double initialTime() const;

      /**
       * \copydoc HullWhite::Data::discount
       */
      const Function & discount() const;

      /**
       * Accessor function to forward curve. 
       * \return Forward curve for financial asset. 
       */
      const Function & forward() const;

      /**
       * \copydoc HullWhite::Data::volatility
       */
      const Function & volatility() const;

      /** 
       * Accessor function to "shape" curve.  
       * \return The shape function. This function defines the shape of 
       * changes in forward prices.  
       */
      const Function & shape() const;

    private:
      Function m_uDiscount;
      Function m_uForward;
      Function m_uVolatility;
      Function m_uShape;
      double m_dInitialTime;
    };
  }   
  //@}
}

#include "cfl/Inline/iBlackData.hpp"
#endif // of __cflBlackData_hpp__	
