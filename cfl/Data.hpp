//Copyright (c) Dmitry Kramkov, 2000-2006. All rights reserved. 

#ifndef __cflData_hpp__
#define __cflData_hpp__

/**
 * \file   Data.hpp
 * \author Dmitry Kramkov
 * \date   2000-2006
 * 
 * \brief Useful data structures.  
 *
 * This file contains useful functions and classes that facilitate 
 * specifications of input data such as volatility, forward and discount curves
 * and parameters of financial derivatives.  
 */

#include "cfl/Function.hpp"

namespace cfl
{
  /** 
   * \ingroup cflCommonElements
   * \defgroup cflData Data structures. 
   * This module contains useful functions and classes that facilitate 
   * specifications of input data such as volatility, forward and discount curves
   * and parameters of financial derivatives.
   */
  //@{

  //! Data structures. 
  /**
   * This namespace contains useful functions and classes that facilitate 
   * specifications of input data such as volatility, forward and discount curves
   * and parameters of financial derivatives.
   */
  namespace Data
  {
    ///\addtogroup cflData
    //@{

    /** 
     * Constructs discount curve. The discount factor for maturity \p
     * dMaturity is given by 
     * 
     * \code exp(-dYield*(dMaturity - dInitialTime)) \endcode
     * 
     * Both \a dMaturity and \a dInitialTime are given as year
     * fractions.
     * 
     * \param dYield The constant continuously compounded yield.
     * \param dInitialTime The initial time as year fraction. 
     * 
     * \return The discount curve as a function of maturity.  
     */
    Function discount(double dYield, double dInitialTime);

    /** 
     * Constructs discount curve. The discount factor for maturity \p
     * dMaturity is given by
     * 
     * \code exp(-rYield(dMaturity)*(dMaturity - dInitialTime))
     * \endcode
     * 
     * Both \a dMaturity and \a dInitialTime are given as year
     * fractions
     * 
     * \param rYield The continuously compounded yield curve.
     * \param dInitialTime The initial time as year fraction. 
     * 
     * \return The discount curve as function of maturity.  
     */
    Function discount(const Function & rYield, double dInitialTime);

    /** 
     * Constructs stationary volatility curve. The value of volatility
     * for time \p dT, given as year fraction, equals 
     *
     * \code dSigma*sqrt((exp(2*m_dLambda*(dT-dInitialTime))-1)/(2*m_dLambda*(dT-dInitialTime))) \endcode
     * 
     * \param dSigma The short-term volatility
     * \param dLambda The mean-reversion coefficient. 
     * \param dInitialTime The initial time as year fraction. 
     * 
     * \return The stationary volatility curve. 
     */
    Function volatility(double dSigma, double dLambda, double dInitialTime);
	
    /** 
     * Constructs forward curve for an asset. The forward price at
     * maturity \p dMaturity equals 
     * 
     * \code dSpot*exp(dCostOfCarry*(dMaturity-dInitialTime))\endcode
     * 
     * \param dSpot The spot price. 
     * \param dCostOfCarry The cost-of-carry rate. 
     * \param dInitialTime The initial time as year fraction. 
     * 
     * \return The forward curve computed by cost-of-carry formula. 
     */
    Function forward(double dSpot, double dCostOfCarry, double dInitialTime);

    /** 
     * Constructs forward curve for an asset. The forward price at
     * maturity \p dMaturity is given by 
     * 
     * \code dSpot*exp(rCostOfCarry(dMaturity)*(dMaturity-dInitialTime))\endcode
     * 
     * \param dSpot The spot price. 
     * \param rCostOfCarry The cost-of-carry rate curve. 
     * \param dInitialTime The initial time as year fraction. 
     *
     * \return The forward curve computed by cost-of-carry formula.
     */
    Function forward(double dSpot, const Function & rCostOfCarry, 
		     double dInitialTime);

    /** 
     * Constructs forward curve for a stock. The forward price at
     * maturity \p dMaturity given as year fraction equals
     *
     * \code dSpot*exp(-dDividendYield*(dMaturity-dInitialTime)/rDiscount(dMaturity) \endcode
     * 
     * \param dSpot The spot price. 
     * \param dDividendYield The dividend yield for the stock. 
     * \param rDiscount The current discount curve. 
     * \param dInitialTime The initial time as year fraction. 
     * 
     * \return The forward curve for an asset paying constant continuous
     * dividend yield.
     */
    Function forward(double dSpot, double dDividendYield, 
		     const Function & rDiscount, double dInitialTime);

    /** 
     * Stationary form for relative changes in forward price curves.
     * The value of this function at time \p dT given as year fraction
     * equals \code exp(-dLambda*(dT-dInitialTime)) \endcode
     *
     * \param dLambda The mean-reversion rate. 
     * \param dInitialTime The initial time as year fraction. 
     * 
     * \return The stationary shape of relative changes in forward prices. 
     */
    Function assetShape(double dLambda, double dInitialTime);

    /** 
     * Stationary form for changes in an yield curve. The value of this
     * function at time \p dT given as year fraction equals \code
     * (1-exp(-dLambda*(dT-dInitialTime)))/dLambda \endcode
     * 
     * \param dLambda The mean-reversion rate. 
     * \param dInitialTime The initial time as year fraction. 
     * 
     * \return The stationary shape of changes in an yield curve. 
     */
    Function bondShape(double dLambda, double dInitialTime);

    //! Cash flow at fixed rate over regular time intervals. 
    /**
     * This class describes the cash flow which takes place 
     * at a given fixed interest rate and at regular time intervals. 
     * \code coupon = notional * rate * period \endcode
     * \see Swap
     */
    class CashFlow 
    {
    public:
      /**
       * The notional amount. 
       */
      double notional;

      /** 
       * The fixed interest rate. 
       */
      double rate;

      /** 
       * The interval between two payments as year fraction. 
       */
      double period; 

      /**
       * The total number of payments.
       */
      unsigned numberOfPayments;
    };

    //! Interest rate swap.  
    /**
     * This class describes the parameters of interest rate 
     * swap. One side makes fixed payments according to 
     * simple fixed CashFlow and another side makes float payments 
     * according to the market (LIBOR) interest rate.  
     * \see CashFlow
     */
    class Swap: public CashFlow
    {
    public:
      /**
       * Default constructor.
       */
      Swap(){};

      /** 
       * Constructs the interest rate swap from 
       * the class CashFlow and determines the side of the 
       * contract by \a bPayFloat.
       * \param rCashFlow A constant reference to CashFlow
       * \param bPayFloat A side of the contract. If \p true 
       * then we pay float and receive fixed, if  \p false
       * then otherwise. 
       */
      Swap(const CashFlow & rCashFlow, bool bPayFloat = true);

      /**
       * The side of the contract. If <code>payFloat = true</code>, then 
       * we pay float and receive fixed. If <code>payFloat = false</code>, then
       * we pay fixed and receive float. 
       */ 
      bool payFloat;
    };
    //@}
  }
  //@}
}

#include "cfl/Inline/iData.hpp"
#endif // of __cflData_hpp__
