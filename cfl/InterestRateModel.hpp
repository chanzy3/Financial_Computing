//Copyright (c) Dmitry Kramkov, 2000-2006. All rights reserved. 

#ifndef __cflInterestRateModel_hpp__
#define __cflInterestRateModel_hpp__

#include <valarray>
#include "cfl/Extended.hpp"

/**
 * \file   InterestRateModel.hpp
 * \author Dmitry Kramkov
 * \date   2000-2006
 * 
 * \brief  Interest rate models. 
 * 
 * This file is dealing with a design of interest rate models. 
 */

namespace cfl
{
  /**
   * \ingroup cflModel
   * \defgroup cflInterestRateModel Interest rate models. 
   * This module is dealing with interest rate models. 
   */
  //@{

  //! Interface class for interest rate model. 
  /** 
   * This is the abstract class for interest rate models. It is used 
   * to implement the class InterestRateModel. 
   * \see InterestRateModel
   */
  class  IInterestRateModel: public IModel
  {
  public:
    /** 
     * Virtual destructor. 
     */
    virtual ~IInterestRateModel(){};

    /** 
     * Virtual constructor. Constructs a dynamically allocated implementation of 
     * the same model but with a new vector of event times. 
     * \param rEventTimes New vector of event times for the model. The first element is the initial time. 
     * \return Dynamically allocated implementation of the interface class IInterestRateModel with
     * the vector of event times \a rEventTimes. 
     */
    virtual IInterestRateModel * newModel(const std::vector<double> & rEventTimes) const = 0; 

    /** 
     * Constructs discount factor with maturity \a dBondMaturity at event time with index \a iEventTime. 
     * \param iEventTime Index of event time where the discount factor is constructed. 
     * \param dBondMaturity The maturity of the discount factor. 
     * \return Discount factor with maturity \a dBondMaturity at event time with index \a iEventTime. 
     */
    virtual Slice discount(unsigned iEventTime, double dBondMaturity) const = 0; 
  }; 
 
  //! Concrete class for interest rate models. 
  /** 
   * This is the main concrete class for interest rate models. 
   * \see IInterestRateModel
   */
  class InterestRateModel
  {
  public:

    /** 
     * The constructor. 
     * \param pNewModel Dynamically allocated implementation of the interface class
     * IInterestRateModel. 
     * \param rExtended An instance of the class Extended. 
     */
    InterestRateModel(IInterestRateModel * pNewModel, const Extended & rExtended); 

    /** 
     * Resets the vector of event times to \a rEventTimes. This function also resets the 
     * vector of state processes. All state processes created previously by the function 
     * InterestRateModel::addState will be deleted. 
     * \param rEventTimes The new vector of event times for the model. The front element of this 
     * vector should equal the initial time of the model. Otherwise, an error is thrown. 
     */
    void assignEventTimes(const std::vector<double> & rEventTimes);

    /**
     * \copydoc Extended::addState
     */
    unsigned addState(const PathDependent & rState);

    /**
     * \copydoc IModel::numberOfStates  
     */ 
    unsigned numberOfStates() const;

    /**
     * \copydoc IModel::eventTimes
     */
    const std::vector<double> & eventTimes() const;

    /**
     * Returns the initial time of the model. Same as <code>InterestRateModel::eventTimes().front()</code>.  
     * \return Initial time of the model. 
     */
    double initialTime() const;



    /**
     * \copydoc IModel::state
     */
    Slice state(unsigned iEventTime, unsigned  iState) const;

     /** 
     * Constructs constant payoff in the amount \a dAmount at the event 
     * time with index \a iEventTime. 
     * \param iEventTime Index of event time. 
     * \param dAmount The amount of cash. 
     * \return Representation of constant payoff in the amount \a dAmount 
     * taking place at the event time with index \a iEventTime. 
     */
    Slice cash(unsigned iEventTime, double dAmount) const;

    /**
     * \copydoc IInterestRateModel::discount
     */
    Slice discount(unsigned iEventTime, double dBondMaturity) const;

  private:
    Extended m_uExtended; 
    std::shared_ptr<IInterestRateModel> m_pModel;
  };
  //@}
}

#include "cfl/Inline/iInterestRateModel.hpp"  
#endif // of __cflInterestRateModel_hpp__
