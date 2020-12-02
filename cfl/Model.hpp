//Copyright (c) Dmitry Kramkov, 2000-2006. All rights reserved. 

#ifndef __cflModel_hpp__
#define __cflModel_hpp__

#include "cfl/MultiFunction.hpp"

/**
 * \file   Model.hpp
 * \author Dmitry Kramkov
 * \date  2000-2006
 * 
 * \brief Interface class for financial models.
 *
 * Contains interface class IModel for financial models in the library. 
 */

namespace cfl
{
  class Slice;

  /// \addtogroup cflBasicElements
  //@{

  //! Interface class for financial models. 
  /**
   * Interface class for financial models in the library. Its main role is to 
   * define the model-specific behavior of the class Slice. 
   * \see Slice
   */
  class IModel
  {
  public:
    /**
     * Virtual destructor. 
     */
    virtual ~IModel(){}

    /** 
     * Accessor function to the vector of event times in the model.  Event times 
     * are sorted in increasing order and are given as year fractions. 
     * The front event time equals the initial time. 
     * \return Vector of event times in the model. 
     */
    virtual const std::vector<double> & eventTimes() const = 0;

    /** 
     * Returns the dimension of the model, that is, the number of state processes. 
     * \return The number of state processes in the model. 
     */
    virtual unsigned numberOfStates() const = 0;

    /** 
     * Returns the size of array used in the representation 
     * of random variables (objects of the type Slice) defined at  a given
     * event time and dependent on a given state processes. 
     * \param iEventTime The index of event time. 
     * \param rStates The indexes of state processes. 
     * \return The size of array needed to construct Slice object at event time \a iEventTime
     * that depends on the state processes with indexes \a rStates. 
     */
    virtual unsigned numberOfNodes(unsigned iEventTime, 
				   const std::vector<unsigned> & rStates) const = 0;

    /** 
     * Returns the representation of the state process with index \a iState at 
     * the event time with index \a iEventTime.
     * \param iEventTime The index of the event time. 
     * \param iState The index of the state process. 
     * \return State process with index \a iState at event time with index \a iEventTime. 
     */
    virtual Slice state(unsigned iEventTime, unsigned iState) const = 0;

    /** 
     * Returns the initial values of all state processes, that is, those 
     * values that correspond to the initial data structure of the model. 
     * \return The initial values of the state processes. 
     */	
    virtual std::valarray<double> origin() const = 0;

    /** 
     * Transforms \a rSlice into the equivalent Slice object which, in
     * addition, depends on the state processes with indexes \a
     * rStates. This function is used to define arithmetic operations
     * between objects of type Slice relying on different state
     * processes.
     * \param rSlice The representation of some random variable in the model. After 
     * this operation this random variable will also be dependent on the state processes
     * with indexes \a rStates. 
     * \param rStates Additional indexes of state processes on which \a rSlice 
     * will be dependent after this operation.  
     */
    virtual void addDependence(Slice & rSlice, 
			       const std::vector<unsigned> & rStates) const = 0;

    /** 
     * "Rolls back"  \a rSlice to the event time with index \a iEventTime. 
     * \param rSlice Before the rollback operator this 
     * object represents the payoff of a financial security at an event time 
     * which index is larger than \a iEventTime. After the rollback operator it 
     * defines the equivalent value of this payoff at the event time with index 
     * \a iEventTime. 
     * \param iEventTime The index of the "target" event time for \a rSlice. 
     */
    virtual void rollback(Slice & rSlice, unsigned iEventTime) const = 0;

    /** 
     * Transforms \a rSlice into the indicator function of the event 
     * that the random variable represented by \a rSlice is greater than the barrier 
     * \a dBarrier. 
     * \param rSlice Before the operation \a rSlice represents some 
     * random variable.  After the operation \a rSlice becomes equal the 
     * indicator of the event that this random variable is greater than \a dBarrier.  
     * \param dBarrier The value of the barrier. 
     */
    virtual void indicator(Slice & rSlice, double dBarrier) const = 0;

    /** 
     * This function explicitly defines the dependence of the given Slice object 
     * on the state processes. The dimension of the returned MultiFunction object 
     * coincides with the number of state processes on which \a rSlice is dependent.  
     * \param rSlice A random variable in the model. 
     * \return Multi-dimensional function object that shows the dependence of 
     * \a rSlice on state processes. 
     */		
    virtual MultiFunction interpolate(const Slice & rSlice) const = 0;
  };
  //@}
}

#endif // of __cflModel_hpp__

