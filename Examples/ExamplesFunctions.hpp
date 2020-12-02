//Copyright (c) Dmitry Kramkov, 2000-2006. All rights reserved. 

#ifndef __ExamplesFunctions_hpp__
#define __ExamplesFunctions_hpp__

#include "cfl/AssetModel.hpp"
#include "cfl/InterestRateModel.hpp"
#include "cfl/Data.hpp"

/**
 * \file   ExamplesFunctions.hpp
 * \author Dmitry Kramkov
 * \date   2000-2006
 * 
 * \brief Auxiliary functions for the project Examples.  
 *
 * This file contains auxiliary functions and classes used in 
 * the project Examples. 
 */

namespace prb
{
  /**
   * \defgroup ExamplesFunctions Auxiliary functions for  project Examples.  
   * This module contains auxiliary functions and classes for  
   * project Examples. 
   */
  //@{

  /** 
   * Computes the value of coupon bond. 
   * 
   * \param iTime The index of the current event time. 
   * \param rBond The parameters of the coupon bond issued at the
   * event time with index \a iTime. 
   * \param rModel Reference to implementation of InterestRateModel.
   * 
   * \return The value of coupon bond at event time with index \a iTime.
   */
  cfl::Slice couponBond(unsigned iTime, const cfl::Data::CashFlow & rBond, 
			const cfl::InterestRateModel & rModel);

  /** 
   * \param dLowerBarrier The lower barrier. 
   * \param dUpperBarrier The upper barrier. 
   * \param rResetIndexes The vector of indexes of event times, when
   * the additional state process changes its value. 
   * \param rModel Reference to implementation of AssetModel.
   * 
   * \return The additional state process represented by the
   * indicator of the event that that the price of the stock 
   * has not crossed the upper and lower barriers at event times
   * represented by indexes \a rResetIndexes. 
   */
  cfl::PathDependent indUpDownOut(double dLowerBarrier, double dUpperBarrier, 
				  const std::vector<unsigned> & rResetIndexes, 
				  const cfl::AssetModel & rModel);

  /** 
   * \param rResetIndexes The vector of indexes of event times, when
   * the additional state process changes its value. 
   * \param rModel Reference to implementation of AssetModel.
   * 
   * \return The additional state process represented by the
   * historical average for the price of stock over the event times
   * with indexes \a rResetIndexes. 
   */
  cfl::PathDependent histAverage(const std::vector<unsigned> & rResetIndexes, 
				 const cfl::AssetModel & rModel);

  /** 
   * Computes float (LIBOR) rate. 
   * 
   * \param iTime The index of the current event time. 
   * \param dPeriod The time interval (given as year fraction) for
   * interest rate loan. 
   * \param rModel Reference to implementation of InterestRateModel.
   * 
   * \return The float rate at event time with index \a iTime for
   * loan with time interval \a dPeriod. 
   */
  cfl::Slice rate(unsigned iTime, double dPeriod, 
		  const cfl::InterestRateModel & rModel);

  /** 
   * Describes the capital on savings account at next payment time. 
   * 
   * \param dPeriod The time interval (given as year fraction) for
   * interest rate loan. 
   * \param dNotional The notional amount. 
   * \param rResetIndexes The vector of indexes of event times, when
   * the additional state process changes its value. 
   * \param rModel Reference to implementation of InterestRateModel.
   * 
   * \return Additional state process represented by the evolution
   * of the capital on savings account at next payment time. 
   */
  cfl::PathDependent 
  savingsAccountNextTime(double dPeriod, double dNotional, 
			 const std::vector<unsigned> & rResetIndexes, 
			 const cfl::InterestRateModel & rModel);

  /** 
   * Computes the value of interest rate swap. 
   * 
   * \param iTime The index of the current event time. 
   * \param rSwap The parameters of interest rate swap issued at
   * event time with index \a iTime. 
   * \param rModel Reference to implementation of InterestRateModel.
   * 
   * \return The value of interest rate swap issued at event time
   * with index \a iTime. 
   */		
  cfl::Slice swap(unsigned iTime, const cfl::Data::Swap & rSwap, 
		  const cfl::InterestRateModel & rModel);

  //@}
}

#endif // of __ExamplesFunctions_hpp__


