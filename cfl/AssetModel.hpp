//Copyright (c) Dmitry Kramkov, 2006. All rights reserved. 

#ifndef __cflAssetModel_hpp__
#define __cflAssetModel_hpp__

#include <valarray>
#include "cfl/Extended.hpp"

/**
 * \file   AssetModel.hpp
 * \author Dmitry Kramkov
 * \date   2000-2006
 * 
 * \brief Financial model for a single asset. 
 * This file contains basic classes needed to evaluate
 * derivatives on a single asset. 
 */

namespace cfl
{
  /** 
   * \defgroup cflModel Financial models.
   * This module contains implementations of different financial
   * models. 
   */

  /**
   * \ingroup cflModel
   * \defgroup cflAssetModel Single asset models. 
   * This module is dealing with financial models for single asset. 
   */
  //@{

  //! Interface class for a single asset model. 
  /** 
   * This is the abstract class for single asset model. It is used 
   * to implement the class AssetModel. 
   * \see AssetModel
   */
  class  IAssetModel: public IModel
  {
  public:
    /**
     * Virtual destructor. 
     */
    virtual ~IAssetModel(){};

    /**
     * Virtual constructor. Constructs a dynamically allocated implementation of
     * the same model but with a new vector of event times. 
     * \param rEventTimes New vector of event times for the model. The first element is the initial time. 
     * \return Dynamically allocated implementation of the interface class IAssetModel with
     * the vector of event times \a rEventTimes. 
     */
    virtual IAssetModel * newModel(const std::vector<double> & rEventTimes) const = 0; 

    /**
     * \copydoc IInterestRateModel::discount
     */
    virtual Slice discount(unsigned iEventTime, double dBondMaturity) const = 0; 
 
    /** 
     * Returns forward price for delivery time \a dForwardMaturity at event time with index \a iEventTime. 
     * \param iEventTime The index of an event time. 
     * \param dForwardMaturity The maturity of the forward contract. 
     * \return Forward price of underlying asset for the contract with delivery time 
     * \a dForwardMaturity at event time with index \a iEventTime. 
     */   
    virtual Slice forward(unsigned iEventTime, double dForwardMaturity) const = 0;
  };  

  //! Concrete class for financial models with a single asset. 
  /**
   * This is the universal class for financial models with a single asset. 
   * It is constructed using an implementation of the interface class IModel. 
   */
  class AssetModel
  {
  public:
    /** 
     * The constructor.  
     * \param pNewModel Dynamic implementation of the interface class
     * IAssetModel.
     * \param rExtended An instance of the class Extended. This class
     * is used to implement function AssetModel::addState to allow the
     * valuation of  path dependent derivatives.
     */
    AssetModel(IAssetModel * pNewModel, const Extended & rExtended); 
    
   /** 
     * Resets the vector of event times to \a rEventTimes. This function also resets the 
     * vector of state processes. All state processes created previously by the function 
     * AssetModel::addState will be deleted. 
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
     * Returns the initial time of the model. Same as <code>AssetModel::eventTimes().front()</code>.  
     * \return Initial time of the model. 
     */
    double initialTime() const; 

    /**
     * \copydoc IModel::state
     */
    Slice state(unsigned iEventTime, unsigned  iState) const;

    /**
     * \copydoc InterestRateModel::cash
     */
    Slice cash(unsigned iEventTime, double dAmount) const;

    /**
     * \copydoc InterestRateModel::discount
     */
    Slice discount(unsigned iEventTime, double dBondMaturity) const;

    /**
     * \copydoc IAssetModel::forward
     */
    Slice forward(unsigned iEventTime, double dForwardMaturity) const; 

    /** 
     * Returns spot price at event time with index \a iEventTime. 
     * \param iEventTime Index of an event time. 
     * \return Spot price of underlying asset at event time with index \a iEventTime. 
     */
    Slice spot(unsigned iEventTime) const; 

  private:
    std::shared_ptr<IAssetModel> m_pModel;
    Extended m_uExtended; 
  };
  //@}
}

#include "cfl/Inline/iAssetModel.hpp" 
#endif // of __cflAssetModel_hpp__
