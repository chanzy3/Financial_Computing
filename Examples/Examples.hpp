//Copyright (c) Dmitry Kramkov, 2000-2008. All rights reserved. 

#ifndef __Examples_hpp__
#define __Examples_hpp__

#include "cfl/Data.hpp"
#include "cfl/AssetModel.hpp"
#include "cfl/InterestRateModel.hpp"

/**
 * \file   Examples.hpp
 * \author Dmitry Kramkov
 * \date   2000-2008
 * 
 * \brief Examples for the course Financial Computing with C++. 
 *
 * This file contains examples of the construction of data curves 
 * and of the evaluation of derivative securities. 
 */


//! Problems for the course.   
/**
 * This namespace contains functions and classes related to the 
 * construction of data curves and to the computation of the prices 
 * of derivative securities.
 */
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
   * Computes the stationary form for changes in yield curve 
   * in the framework of Hull and White model. In other words, 
   * returns the function 
   * \f[
   * \Gamma(t) = \frac{1 - \exp(-\lambda (t-t_0))}{\lambda (t-t_0)},
   * \f]
   * where \f$\lambda\f$ (= \a dLambda) is the mean-reversion parameter,
   * \f$t\f$ is the maturity, and \f$t_0\f$  (= \a dInitialTime) is 
   * the initial time. 
   * 
   * \param dLambda The mean-reversion parameter.
   * \param dInitialTime The initial time. 
   * 
   * \return The stationary form of changes in yield shape curve for
   * Hull and White model.  
   */
  cfl::Function yieldShapeHullWhite(double dLambda, double dInitialTime);

  /**
   * Computes discount curve by log-linear interpolation of a given 
   * collection of discount factors. 
   * 
   * \param rDiscountTimes The vector of times, when the discount
   * factors are known. 
   * \param rDiscountFactors The vector of known discount factors. 
   * \param dInitialTime The initial time. 
   * 
   * \return The discount curve obtained from known discount factors
   * by log-linear interpolation. 
   */
  cfl::Function 
  discountLogLinearInterp(const std::vector<double> & rDiscountTimes, 
			  const std::vector<double> & rDiscountFactors,
			  double dInitialTime);

  /** 
   * Returns the stationary form of discount curve in Hull and White model 
   * by the least square fit of market yields. In the stationary form 
   * the discount factor with  maturity \f$ t \f$ is given by 
   * \f[
   * d(t) = \exp\left(-A
   * \left(\frac{1-e^{-\lambda (t-t_0)}}{\lambda}\right)\right),  
   * \f]
   * where \f$\lambda\f$ (= \a dLambda) is the mean reversion rate, \f$t_0\f$
   * (= \a dInitialTime) is the initial time and  
   * \f$A\f$ is a constant determined from the least square fit. 
   *
   * \param rDiscountTimes The vector of times when discount factors
   * are known. 
   * \param rDiscountFactors The vector of known discount factors. 
   * \param dLambda The mean-reversion rate. 
   * \param dInitialTime The initial time. 
   * 
   * \return The stationary form of discount curve in Hull and White
   * model obtained from known discount factors by least square
   * fit. 
   */
  cfl::Function 
  discountFitHullWhite(const std::vector<double> & rDiscountTimes, 
		       const std::vector<double> & rDiscountFactors, 
		       double dLambda, double dInitialTime);
  //@}

  /**
   * \defgroup prbAssetStd Standard and barrier options on a single stock. 
   *
   * This module contains functions that compute prices of standard
   * and barrier options in cfl::AssetModel.
   */
  //@{

  /** 
   * Computes the value of the <strong>European put option</strong>. 
   * In this contract, at maturity \a dMaturity a holder of the
   * option can sell the
   * stock at the strike \a dStrike. 
   * 
   * \param dStrike The strike of the option. 
   * \param dMaturity The maturity of the option.  
   * \param rModel Reference to implementation of AssetModel. 
   * 
   * \return The price of the option as the function of the initial
   * values of the state processes in the model.  
   */
  cfl::MultiFunction put(double dStrike, double dMaturity, 
			 cfl::AssetModel & rModel);

  /** 
   * Computes the value of the <strong>American put option</strong>. 
   * In this contract, at any exercise time (from \a rExerciseTimes)
   * a holder of the option can sell the stock at the strike \a dStrike.
   * 
   * \param dStrike The strike of the option.
   * \param rExerciseTimes The vector of exercise times. The first exercise
   * time is strictly greater than the initial time. 
   * \param rModel Reference to implementation of AssetModel.
   * 
   * \return The price of the option as the function of the initial
   * values of the state processes in the model. 
   */
  cfl::MultiFunction americanPut(double dStrike, 
				 const std::vector<double> & rExerciseTimes,
				 cfl::AssetModel & rModel);

  /** 
   * Computes the value of the <strong>up-or-down-and-out barrier 
   * option</strong>. The payoff  of the option at maturity (last barrier
   * time) is given by the notional amount \a dNotional if the stock
   * price has not crossed the lower and upper barriers for all
   * barrier times. Otherwise, the option expires worthless. 
   * 
   * \param dLowerBarrier The lower barrier. 
   * \param dUpperBarrier The upper barrier. 
   * \param dNotional The notional amount. 
   * \param rBarrierTimes The vector of barrier times. The first
   * time is greater than the initial time. The last barrier time is
   * the maturity of the option. 
   * \param rModel Reference to implementation of AssetModel. 
   * 
   * \return The price of the option as the function of the initial
   * values of the state processes in the model.  
   */
  cfl::MultiFunction 
  barrierUpOrDownAndOut(double dNotional, double dLowerBarrier, 
			double dUpperBarrier, 
			const std::vector<double> & rBarrierTimes, 
			cfl::AssetModel & rModel);

  /**
   * Computes the value of the <strong>down-and-out American call</strong>.
   * The option behaves as the American call option with the strike
   * \a dStrike and the exercise times \a rExerciseTimes until the
   * first barrier time when the stock price hits the lower
   * barrier \a dBarrier. At this exit time the option is canceled.
   * 
   * \param dBarrier The lower barrier (\a dBarrier < \a dStrike). 
   * \param rBarrierTimes The vector of barrier times. The first
   * time is greater than the initial time. 
   * \param dStrike The strike of the option. 
   * \param rExerciseTimes The vector of exercise times. The first exercise
   * time is strictly greater than the initial time.  
   * \param rModel Reference to implementation of AssetModel. 
   * 
   * \return The price of the option as the function of the initial
   * values of the state processes in the model.  
   */
  cfl::MultiFunction 
  downAndOutAmericanCall(double dBarrier, 
			 const std::vector<double> & rBarrierTimes,
			 double dStrike, 
			 const std::vector<double> & rExerciseTimes, 
			 cfl::AssetModel & rModel);

  /** 
   * Computes the value of the <strong>swing option</strong>.
   * The holder can exercise the option \a iNumberOfExercises times. 
   * At each exercise time he can buy only one stock for strike \a dStrike.  
   * 
   * \param dStrike The strike of the option.
   * \param rExerciseTimes The vector of exercise times. 
   * \param iNumberOfExercises The maximal number of exercises. 
   * \param rModel Reference to implementation of AssetModel. 
   * 
   * \return The price of the option as the function of the initial
   * values of the state processes in the model.  
   */

  cfl::MultiFunction swing(double dStrike, 
			   const std::vector<double> & rExerciseTimes, 
			   unsigned iNumberOfExercises, 
			   cfl::AssetModel & rModel);
  //@}

  /**
   * \defgroup prbInterestRateStd Standard and barrier options on interest rates.
   *
   * This module contains functions that compute prices of standard
   * and barrier options in cfl::InterestRateModel.
   */
  //@{

  /** 
   * Computes the price of <strong>interest rate cap</strong>. 
   * 
   * \param rCap The parameters of interest rate cap. 
   * \param rModel Reference to implementation of InterestRateModel.
   * 
   * \return The price of the option as the function of the initial
   * values of the state processes in the model.
   */
  cfl::MultiFunction cap(const cfl::Data::CashFlow & rCap, 
			 cfl::InterestRateModel & rModel);

  /** 
   * Computes the price of <strong>interest rate swaption</strong>. 
   * In this contract, at maturity \a dMaturity a holder of the option has the
   * right to enter the interest rate swap with the issue time \a dMaturity
   * and the parameters \a rSwap. 
   * 
   * \param rSwap The parameters of the underlying interest rate
   * swap. 
   * \param dMaturity The maturity of the option. This time is also
   * the issue time for the underlying swap contract. 
   * \param rModel Reference to implementation of InterestRateModel.
   * 
   * \return The price of the option as the function of the initial
   * values of the state processes in the model.
   */
  cfl::MultiFunction swaption(const cfl::Data::Swap & rSwap, double dMaturity, 
			      cfl::InterestRateModel & rModel);  
 
  /** 
   * Computes the price of <strong>cancellable collar</strong>. 
   * In this contract, holder of the option can terminate the
   * underlying interest rate collar at any payment time.
   * Note that in the case of termination the current payment 
   * is still taking place. 
   * 
   * \param rCap The parameters of the underlying interest rate
   * collar. Here <em>rCap.rate</em> defines the cap rate. 
   * \param dFloorRate The floor rate in the underlying interest
   * rate collar. 
   * \param rModel Reference to implementation of InterestRateModel.
   * 
   * \return The price of the option as the function of the initial
   * values of the state processes in the model.
   */
  cfl::MultiFunction cancellableCollar(const cfl::Data::CashFlow & rCap, 
				       double dFloorRate, 
				       cfl::InterestRateModel & rModel);

  /** 
   * Computes the price of <strong>down-and-out cap</strong>. 
   * The down-and-out cap generates the same cash flow as a standard
   * interest rate cap
   * up to (and including) the payment time when the float rate 
   * hits the lower barrier \a dLowerBarrier. After that, the cap is
   * terminated. 
   * 
   * \param rCap The parameters of the underlying interest rate
   * cap. 
   * \param dLowerBarrier The lower barrier for float rate. 
   * \param rModel Reference to implementation of InterestRateModel.
   * 
   * \return The price of the option as the function of the initial
   * values of the state processes in the model.
   */
  cfl::MultiFunction downAndOutCap(const cfl::Data::CashFlow & rCap, 
				   double dLowerBarrier, 
				   cfl::InterestRateModel & rModel);

  /** 
   * Computes  <strong>future price on LIBOR</strong>.
   * The set of times, when the future price is evaluated is given by 
   * \code 
   * t(i) = rModel.initialTime() + 
   * i*(dFutureMaturity-rModel.initialTime())/iFutureTimes, 
   * i = 0, ... , iFutureTimes-1
   * \endcode
   * Denote by <em>F(t(i))</em> the future price determined at time
   * <em>t(i)</em>. 
   * Recall that it costs nothing to enter into the future contract
   * at time <em>t(i)</em> and that short position entered at time
   * <em>t(i)</em> assumes the following transactions:
   * <ol>
   * <li> At time <em>t(k)</em> such that <em>i < k < iFutureTimes
   * </em> short position pays the amount <em>F(t(k+1)) - F(t(k))</em>.  
   * <li> At maturity \a dFutureMaturity short position receives future price 
   * <em>F(t(iFutureTimes-1))</em> defined at the previous time step
   * and pays the amount 
   * \code
   * 1.-LIBOR(dMaturity, dMaturity + dLiborPeriod)
   * \endcode
   * </ol> 
   */
  cfl::MultiFunction futureOnLibor(double dLiborPeriod, 
				   unsigned iFutureTimes, double dMaturity, 
				   cfl::InterestRateModel & rModel);
  //@}

  /** 
   * \defgroup prbPath Path dependent options.  
   *
   * This module contains functions that compute prices of path
   * dependent options in cfl::AssetModel and cfl::InterestRateModel.
   */
  //@{

  /** 
   * Computes the price of the <strong>Asian call option</strong>. 
   * At maturity \a dMaturity holder of the option can exercise it
   * by paying strike \a dStrike and receiving the historical
   * average of the price of the stock computed over the set of
   * averaging times \a rAverageTimes. 
   * 
   * \param rAverageTimes The vector of times participating in the
   * computation of the historical average for the price of the
   * stock. 
   * \param dStrike The strike of the option. 
   * \param dMaturity The maturity of the option. The maturity is
   * strictly greater than the largest time from \a rAverageTimes. 
   * \param rModel Reference to implementation of AssetModel.
   * 
   * \return The price of the option as the function of the initial
   * values of the state processes in the model.
   */
  cfl::MultiFunction asianCall(const std::vector<double> & rAverageTimes, 
			       double dStrike, double dMaturity, 
			       cfl::AssetModel & rModel);

  /** 
   * \copydoc prb::barrierUpOrDownAndOut
   * This implementation views barrier option as a path dependent
   * derivative security: a new state process representing barrier
   * event is added to \a rModel. 
   */
  cfl::MultiFunction
  barrierUpOrDownAndOut_path(double dNotional, double dLowerBarrier, 
			     double dUpperBarrier, 
			     const std::vector<double> & rBarrierTimes, 
			     cfl::AssetModel & rModel);

  /** 
   * Computes the price of <strong>savings account</strong>.
   * Savings account starts with notional amount \a dNotional and
   * then invests the capital at regular time intervals at float rate. 
   * 
   * \param dPeriod The period between two payments. 
   * \param iNumberOfPeriods The number of payments. 
   * \param dNotional The notional amount. 
   * \param rModel Reference to implementation of InterestRateModel.
   * 
   * \return The price of the option as the function of the initial
   * values of the state processes in the model.
   */
  cfl::MultiFunction savingsAccount(double dPeriod, unsigned iNumberOfPeriods, 
				    double dNotional, 
				    cfl::InterestRateModel & rModel);

  /** 
   * Computes the price of <strong>put option on savings
   * account</strong>.
   * At maturity the holder of the option has the right to 
   * receive the capital accumulated on the account with fixed 
   * rate <em> rAccount.rate </em> in exchange for the capital 
   * accumulated on the account with float rate (savings account). 
   * Both accounts are issued at the initial time of the model.
   * 
   * \param rAccount The parameters of the fixed and floating
   * accounts. The fixed rate is given by <em>rAccount.rate</em>. 
   * \param rModel Reference to implementation of InterestRateModel.
   * 
   * \return The price of the option as the function of the initial
   * values of the state processes in the model.
   */
  cfl::MultiFunction 
  putOnSavingsAccount(const cfl::Data::CashFlow & rAccount,  
		      cfl::InterestRateModel & rModel);
  //@}
}

#endif // of __Examples_hpp__
