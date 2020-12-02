//Copyright (c) Dmitry Kramkov, 2000-2008. All rights reserved. 

#ifndef __Homework1_hpp__
#define __Homework1_hpp__

#include "cfl/Data.hpp"
#include "cfl/AssetModel.hpp"
#include "cfl/InterestRateModel.hpp"
#include "cfl/Interp.hpp"

/**
 * \file   Homework1.hpp
 * \author Dmitry Kramkov
 * \date   2000-2008
 * 
 * \brief Homework 1 for the course Financial Computing with C++. 
 *
 * This files contains problems on the construction of data curves. 
 */

//! Namespace for problems in the course. 
namespace prb
{
  /** 
   * \defgroup prbData Construction of data curves.
   * 
   * This module contains functions that construct input data curves,
   * such as volatility, forward, and discount, for financial models.
   */
  //@{

  /** 
   * Computes the forward exchange rate curve from spot exchange rate and 
   * discount curves in domestic and foreign currencies. The exchange rates 
   * are given as the number of units of domestic currency per one unit of 
   * foreign currency. 
   * 
   * \param dSpotFX The spot exchange rate. 
   * \param rDomesticDiscount The domestic discount curve. 
   * \param rForeignDiscount The foreign discount curve. 
   * 
   * \return The forward exchange rate curve. 
   */
  cfl::Function forwardFX(double dSpotFX, 
			  const cfl::Function & rDomesticDiscount, 
			  const cfl::Function & rForeignDiscount);

  /** 
   * Computes the curve of forward prices ("clean" or "dirty")  on 
   * coupon bond. 
   * 
   * \param rBond The parameters of the coupon bond. 
   * \param rDiscount The discount curve. 
   * \param dInitialTime The initial time. 
   * \param bClean This parameter controls the output. If its value
   * is <em>true</em> then we compute "clean" prices. Otherwise, we
   * compute "dirty" prices that include the accrued interest from
   * the next coupon. 
   * 
   * \return The forward price ("clean" or "dirty") curve for the coupon
   * bond. 
   */
  cfl::Function forwardCouponBond(const cfl::Data::CashFlow & rBond, 
				  const cfl::Function & rDiscount, 
				  double dInitialTime, bool bClean);

  /**
   * Computes forward curve by applying the log-linear interpolation to a 
   * given collection of forward prices and maturities.   
   * 
   * \param dSpot The spot price of the stock. 
   * \param rDeliveryTimes The vector of delivery times for
   * known forward prices. 
   * \param rForwardPrices The vector of known forward prices.  
   * \param dInitialTime The initial time. 
   * 
   * \return The forward curve obtained from known forward prices 
   * by log-linear interpolation. 
   */
  cfl::Function 
  forwardLogLinearInterp(double dSpot, 
			 const std::vector<double> & rDeliveryTimes, 
			 const std::vector<double> & rForwardPrices,
			 double dInitialTime);

  /** 
   * Computes the stationary volatility curve for Black model 
   * by the least squire approximation of market implied volatilities.
   * The stationary volatility form for Black model (used, for example, 
   * for commodities market) is given by 
   * \f[ 
   * \sigma(t) = \kappa \sqrt{ \frac{1 -\exp(-2\lambda
   *   (t-t_0))}{2\lambda (t-t_0)} } 
   * \f]
   * Here \f$ t \f$ is the maturity of the option, \f$t_0\f$ = \a dInitialTime, 
   * \f$ \lambda \f$ (=  \a dLambda) is the mean-reversion rate, and 
   * \f$ \kappa \f$ is the short term volatility. The parameter 
   * \f$ \kappa \f$ should be determined by the least square fit. 
   * 
   * \param rMaturities The maturities of the options, when the
   * volatilities are known. 
   * \param rVolatilities The vector of known implied volatilities. 
   * \param dLambda The mean-reversion rate. 
   * \param dInitialTime The initial time. 
   * 
   * \return The stationary form of volatility curve in Black model
   * obtained from known volatilities by least square fit. 
   */
  cfl::Function volatilityFitBlack(const std::vector<double> & rMaturities, 
				   const std::vector<double> & rVolatilities, 
				   double dLambda, double dInitialTime);

  //@}
}

#endif // of __Homework1_hpp__
