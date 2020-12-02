//Copyright (c) Dmitry Kramkov, 2000-2008. All rights reserved. 

#ifndef __Homework2_hpp__
#define __Homework2_hpp__

#include "cfl/Data.hpp"
#include "cfl/AssetModel.hpp"
#include "cfl/InterestRateModel.hpp"

/**
 * \file   Homework2.hpp
 * \author Dmitry Kramkov
 * \date   2000-2008
 * 
 * \brief Homework 2 for the course Financial Computing with C++. 
 *
 * This file contains problems on standard derivatives in single asset 
 * model. 
 */

//! Problems for the course.   
namespace prb
{

  /**
   * \defgroup prbAssetStd Standard and barrier options on a single stock. 
   *
   * This module contains functions that compute prices of standard
   * and barrier options in cfl::AssetModel.
   */
  //@{

  /** 
   * Computes the value of the <strong>European straddle option</strong>. 
   * In this contract, at maturity \a dMaturity a holder of the option 
   * can either buy or sell the stock at the strike \a dStrike. In other 
   * words, the straddle option equals the sum of standard European put 
   * and call options with the same strike and maturity.
   * 
   * \param dStrike The strike of the option. 
   * \param dMaturity The maturity of the option.  
   * \param rModel Reference to implementation of AssetModel. 
   * 
   * \return The price of the option as the function of the initial
   * values of the state processes in the model.  
   */
  cfl::MultiFunction straddle(double dStrike, double dMaturity, 
			      cfl::AssetModel & rModel);

  /** 
   * Computes the value of the <strong>American call on forward
   * contract</strong>.  
   * In this contract, at any exercise time (from \a rExerciseTimes)
   * a holder of the option can enter long position in the forward contract 
   * with forward price \a dForwardPrice and the time to maturity 
   * \a dTimeToMaturity. 
   * 
   * @param dForwardPrice The forward price.
   * @param dTimeToMaturity The time to maturity of forward contract.
   * @param rExerciseTimes The vector of exercise times. The first exercise
   * time is strictly greater than the initial time. 
   * @param rModel Reference to implementation of AssetModel.
   * 
   * @return The price of the option as the function of the initial
   * values of the state processes in the model. 
   */
  cfl::MultiFunction 
  americanCallOnForward(double dForwardPrice, 
			double dTimeToMaturity,
			const std::vector<double> & rExerciseTimes,
			cfl::AssetModel & rModel);


  /** 
   * Computes the value of the <strong>down-and-rebate barrier 
   * option</strong>. The option pays notional \a dNotional at the first 
   * barrier time when the spot price is below the lower barrier 
   * \a dLowerBarrier. Otherwise, the option expires worthless. 
   * 
   * \param dLowerBarrier The lower barrier.  
   * \param dNotional The notional amount.  
   * \param rBarrierTimes The vector of barrier times. The first
   * time is greater than the initial time. 
   * \param rModel Reference to implementation of AssetModel. 
   * 
   * \return The price of the option as the function of the initial
   * values of the state processes in the model.  
   */
  cfl::MultiFunction 
  downAndRebate(double dLowerBarrier, double dNotional, 
		const std::vector<double> & rBarrierTimes,
		cfl::AssetModel & rModel);

  /** 
   * Computes the value of the <strong>up-and-in American put</strong>. 
   * The option becomes American put option with the strike \a dStrike
   * and the exercise times \a rExerciseTimes after the first barrier
   * time when the stock price is above the barrier \a dBarrier. If the
   * price of the stock has been lying below \a dBarrier for all barrier
   * times, then the option expires worthless. 
   * 
   * \param dBarrier The upper barrier.  
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
  upAndInAmericanPut(double dBarrier, 
		     const std::vector<double> & rBarrierTimes, 
		     double dStrike, 
		     const std::vector<double> & rExerciseTimes, 
		     cfl::AssetModel & rModel);
  //@}
}

#endif // of __Homework2_hpp__
