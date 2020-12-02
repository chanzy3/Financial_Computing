//Copyright (c) Dmitry Kramkov, 2000-2006. All rights reserved. 

#ifndef __cflSlice_hpp__
#define __cflSlice_hpp__

#include <algorithm>
#include "cfl/Model.hpp"
#include "cfl/Error.hpp"

/**
 * \file   Slice.hpp
 * \author Dmitry Kramkov
 * \date   2000-2006
 * 
 * \brief Random payoffs in cfl library. 
 *
 * Contains the main class (Slice) in cfl library. This class implements 
 * the payoffs of financial securities.
 */

namespace cfl
{
  class IModel;

  /**
   * \ingroup cflCommonElements 
   * \defgroup cflBasicElements Basic classes: Slice and IModel.
   * This module contains the basic classes of the library:
   * interface class IModel and concrete class Slice.
   */
  //@{

  //! Representation of random payoffs in the library. 
  /**
   * This class models random payoffs defined at a particular event
   * time of a model. More precisely, Slice is a random variable
   * determined by the current values of the state processes. \see
   * IModel
   */ 
  class Slice
  {
  public:
    /** 
     * Constructs a payoff with constant value \a dValue at
     * the event time with index \a iEventTime in the framework of the
     * financial model defined by an implementation of the interface
     * class IModel.
     * \param pModel A pointer to an implementation of the interface class IModel.  
     * \param iEventTime The index of current event time.
     * \param dValue The current constant value of the payoff. 
     */
    explicit Slice(const IModel * pModel = 0, unsigned iEventTime = 0, double dValue = 0);

    /** 
     * Constructs a random payoff at given event time. 
     * \param rModel A constant reference to the underlying model which implements 
     * the interface class IModel. 
     * \param iEventTime The index of the current time in the vector of event times 
     * of the underlying model. 
     * \param rDependence A constant reference to the vector of indexes 
     * of state processes of underlying model which determine the values 
     * of \p *this. 
     * \param rValues An array of values of the random payoff represented by \p *this. 
     * The size of this array should be equal the result of  
     * \p rModel.numberOfNodes(iEventTime, rDependence).
     */
    Slice(const IModel & rModel, unsigned iEventTime, const std::vector<unsigned> & rDependence, 
	  const std::valarray<double> & rValues);

    /** 
     * Assignment operator. Replaces \p *this with a copy of \a rSlice. 
     * \param rSlice Object that will be copied. 
     * \return Reference to \p *this. 
     */
    Slice & operator=(const Slice & rSlice);	

    /** 
     * Assignment operator. Replaces \p *this with the Slice object defined at the same 
     * event time and having constant value \a dValue. 
     * \param dValue The constant value that will be assigned to \p *this.
     * \return Reference to \p *this. 
     */
    Slice & operator=(double dValue);

    /**
     * Adds to \p *this the number \a dValue. 
     * \param dValue The constant value that will be added to \p *this. 
     * \return Reference to \p *this. 
     */
    Slice & operator+=(double dValue);

    /** 
     * Subtracts from \p *this the number \a dValue. 
     * \param dValue The constant value that will be subtracted from \p *this. 
     * \return Reference to \p *this. 
     */
    Slice & operator-=(double dValue);

    /**
     * Multiplies \p *this on the number \a dValue. 
     * \param dValue The constant multiplier.  
     * \return Reference to \p *this. 
     */    
    Slice & operator*=(double dValue);

    /**
     * Divides \p *this on the number \a dValue. 
     * \param dValue The constant divisor.  
     * \return Reference to \p *this. 
     */  
    Slice & operator/=(double dValue);

    /** 
     * Adds to \p *this the value of the payoff represented by \a rSlice. 
     * Both objects \p *this and \a rSlice should be 
     * defined on the same model and at the same event time. 
     * \param rSlice The payoff that will be added to \p *this. 
     * \return Reference to \p *this. 
     */
    Slice & operator+=(const Slice & rSlice);

    /** 
     * Subtracts from \p *this the value of payoff represented by \a rSlice. 
     * Both objects \p *this and \a rSlice should be 
     * defined on the same model and at the same event time. 
     * \param rSlice The payoff that will be subtracted from \p *this. 
     * \return Reference to \p *this. 
     */
    Slice & operator-=(const Slice & rSlice);

    /** Multiplies \p *this on the value of payoff represented by \a rSlice. 
     * Both objects \p *this and \a rSlice should be defined on the same model 
     * and at the same event time. 
     * \param rSlice The multiplier. 
     * \return Reference to \p *this. 
     */
    Slice & operator*=(const Slice & rSlice);

    /** Divides \p *this on the value of payoff represented by \a rSlice. 
     * Both objects \p *this and \a rSlice should be 
     * defined on the same model and at the same event time. 
     * \param rSlice The divisor. 
     * \return Reference to \p *this. 
     */
    Slice & operator/=(const Slice & rSlice);

    /** 
     * Returns payoff in the form <code>f(*this)</code>. 
     * \param f The transformation function. 
     * \return The payoff in the form <code> f(*this) </code>. 
     */
    Slice apply(double (*f)(double)) const;

    /** 
     * This member function is usually used if \p *this represents the 
     * value of a (derivative) security. It assigns to \p *this the equivalent 
     * value of this security at event time with smaller index \a iEventTime. 
     * \param iEventTime The index of the target event time. It should be smaller or 
     * equal the initial index of event time for \p *this. 
     */
    void rollback(unsigned iEventTime);

    /** 
     * Accessor function to the implementation of IModel that was used to support 
     * current Slice object. 
     * \return The pointer to the underlying model. 
     */
    const IModel * ptrToModel() const;

    /** 
     * Accessor function to the index of event time where \p *this is defined. 
     * \return Index of event time where \p *this is defined. 
     */
    unsigned timeIndex() const;

    /** 
     * Accessor function to the vector of indexes of state processes which determine 
     * the random variable represented by \p *this. 
     * \return Constant reference to the vector of indexes on state processes 
     * that participate in the construction of the given Slice object. 
     */
    const std::vector<unsigned> & dependence() const;

    /** 
     * Accessor function to the values of \p *this. 
     * \return The array of values of the given Slice object. 
     */
    const std::valarray<double>  & values() const;

    /** 
     * Replaces the underlying model, the index of event times, the vector of indexes 
     * of state processes, and array of values with \a rModel, \a iEventTime, \a rDependence,
     * and \a rValues. 
     * \param rModel The reference to an implementation of interface class IModel. 
     * \param iEventTime The index of new event time for \p *this. 
     * \param rDependence The new vector of indexes of state processes. 
     * \param rValues The new array of values. 
     */ 
    void assign(const IModel & rModel, 
		unsigned iEventTime, 
		const std::vector<unsigned> & rDependence, 
		const std::valarray<double> & rValues);

    /** 
     * Replaces the index of event times, the vector of indexes 
     * of state processes, and array of values with \a iEventTime, \a rDependence, 
     * and \a rValues. 
     * \param iEventTime The index of new event time for \p *this. 
     * \param rDependence The new vector of indexes of state processes. 
     * \param rValues The new array of values.
     */
    void assign(unsigned iEventTime, const std::vector<unsigned> & rDependence, 
		const std::valarray<double> & rValues);

    /** 
     * Replaces vector of indexes of state processes and array of values 
     * with \a rDependence and \a rValues.
     * \param rDependence The new vector of indexes of state processes. 
     * \param rValues The new array of values. 
     */
    void assign(const std::vector<unsigned> & rDependence, 
		const std::valarray<double> & rValues);

    /** 
     * Replaces the array of values with \a rValues 
     * \param rValues The new array of values. 
     */
    void assign(const std::valarray<double> & rValues);		

    /** 
     * Replaces underlying model with \a rModel. 
     * \param rModel The reference to an implementation of interface class IModel.
     */
    void assign(const IModel & rModel);

  private:
    const IModel * m_pModel;
    unsigned m_iEventTime;
    std::vector<unsigned> m_uDependence;
    std::valarray<double>  m_uValues;
    Slice & apply(const Slice & rSlice, 
		  void (*func)(std::valarray<double> & , const std::valarray<double> & ));
  };

  /** 
   * Returns minus \a rSlice. 
   * \param rSlice Some Slice object. 
   * \return Minus of \a rSlice. 
   */
  Slice operator-(const Slice & rSlice);

  /** 
   * Returns the sum of  \a rSlice1 and \a rSlice2. Both input 
   * parameters should be defined on the same model and at the 
   * same event time. 
   * \param rSlice1 The first element of the sum. 
   * \param rSlice2 The second element of the sum
   * \return The sum of \a rSlice1 and \a rSlice2. 
   */
  Slice operator+(const Slice & rSlice1, const Slice & rSlice2);

  /** 
   * Returns the difference between  \a rSlice1 and \a rSlice2. 
   * Both input parameters should be defined on the same model 
   * and at the same event time. 
   * \param rSlice1 The first element of the difference. 
   * \param rSlice2 The second element of the difference.
   * \return The difference between \a rSlice1 and \a rSlice2. 
   */
  Slice operator-(const Slice & rSlice1, const Slice & rSlice2);

  /** 
   * Returns the product of  \a rSlice1 and \a rSlice2. 
   * Both input parameters should be defined on the same model 
   * and at the same event time. 
   * \param rSlice1 The first multiplier. 
   * \param rSlice2 The second multiplier.
   * \return The product of \a rSlice1 and \a rSlice2. 
   */
  Slice operator*(const Slice & rSlice1, const Slice & rSlice2);

  /** 
   * Returns the ratio  \a rSlice1 and \a rSlice2. 
   * Both input parameters should be defined on the same model 
   * and at the same event time. 
   * \param rSlice1 The dividend. 
   * \param rSlice2 The divisor.
   * \return The ratio  between \a rSlice1 and \a rSlice2. 
   */
  Slice operator/(const Slice & rSlice1, const Slice & rSlice2);

  /** 
   * Returns the sum of \a rSlice and \a dValue. 
   * \param rSlice The first element of the sum. 
   * \param dValue The second element of the sum. 
   * \return The Slice object that is the sum of \a rSlice and \a dValue. 
   */
  Slice operator+(const Slice & rSlice, double dValue);

  /** 
   * Returns the difference between  \a rSlice and \a dValue. 
   * \param rSlice The first element in subtraction. 
   * \param dValue The second element in subtraction. 
   * \return The Slice object that is the difference between \a rSlice and \a dValue.  
   */
  Slice operator-(const Slice & rSlice, double dValue);

  /**
   *  Returns the product of  \a rSlice and \a dValue. 
   * \param rSlice The first multiplier. 
   * \param dValue The second multiplier. 
   * \return The Slice object that is the difference between \a rSlice and \a dValue.  
   */
  Slice operator*(const Slice & rSlice, double dValue);

  /** 
   * Returns the ratio of  \a rSlice and \a dValue. 
   * \param rSlice The dividend. 
   * \param dValue The constant divisor.
   * \return The ratio between \a rSlice and \a dValue. 
   */   
  Slice operator/(const Slice & rSlice, double dValue);

  /** 
   * Returns the sum of  \a dValue and \a rSlice. 
   * \param dValue The first element of the sum. 
   * \param rSlice The second element of the sum. 
   * \return The sum of \a dValue and \a rSlice. 
   */
  Slice operator+(double dValue, const Slice & rSlice);

  /** 
   * Returns the difference between  \a dValue and \a rSlice. 
   * \param dValue The first element in subtraction. 
   * \param rSlice The second element in subtraction. 
   * \return The difference of \a dValue and \a rSlice. 
   */
  Slice operator-(double dValue, const Slice & rSlice);

  /** 
   * Returns the product of  \a dValue and \a rSlice. 
   * \param dValue The first multiplier. 
   * \param rSlice The second multiplier. 
   * \return The product of \a dValue and \a rSlice. 
   */
  Slice operator*(double dValue, const Slice & rSlice);

  /** 
   * Returns the ratio of  \a dValue and \a rSlice. 
   * \param dValue The dividend. 
   * \param rSlice The divisor. 
   * \return The ratio of \a dValue and \a rSlice. 
   */
  Slice operator/(double dValue, const Slice & rSlice);

  /** 
   * Returns the maximum of \a rSlice and \a dValue. 
   * \param dValue A number. 
   * \param rSlice Some payoff. 
   * \return The maximum of \a dValue and \a rSlice. 
   */
  Slice max(const Slice & rSlice, double dValue);

  /** 
   * Returns the minimum of \a rSlice and \a dValue. 
   * \param dValue A number. 
   * \param rSlice Some payoff. 
   * \return The minimum of \a rSlice and \a dValue. 
   */
  Slice min(const Slice & rSlice, double dValue);

  /** 
   * Returns the maximum of \a rSlice1 and \a rSlice2. 
   * Both input parameters should be defined on the same model 
   * and at the same event time. 
   * \param rSlice1 Some payoff. 
   * \param rSlice2 Some payoff. 
   * \return The maximum of \a rSlice1 and \a rSlice2. 
   */
  Slice max(const Slice & rSlice1, const Slice & rSlice2);

  /** 
   * Returns the minimum of \a rSlice1 and \a rSlice2. 
   * Both input parameters should be defined on the same model 
   * and at the same event time. 
   * \param rSlice1 Some payoff. 
   * \param rSlice2 Some payoff. 
   * \return The minimum of \a rSlice1 and \a rSlice2. 
   */
  Slice min(const Slice & rSlice1, const Slice & rSlice2);

  /** 
   * Returns the maximum of \a rSlice and \a dValue. 
   * \param dValue A constant value.  
   * \param rSlice Some random payoff. 
   * \return The maximum of \a dValue and \a rSlice.
   */
  Slice max(double dValue, const Slice & rSlice);

  /** 
   * Returns the minimum of \a rSlice and \a dValue. 
   * \param dValue A constant value.  
   * \param rSlice Some random payoff. 
   * \return The minimum of \a dValue and \a rSlice.
   */
  Slice min(double dValue, const Slice & rSlice);

  /** 
   * Returns the representation of the random variable given by \p rSlice
   * in the power \p dPower.
   * \param rSlice Some random payoff.
   * \param dPower The power. 
   * \return The random variable given by <code> rSlice^dPower </code>. 
   */
  Slice pow(const Slice & rSlice, double dPower);

  /** 
   * Returns the absolute value of \a rSlice. 
   * \param rSlice Some random payoff. 
   * \return The absolute value of \a rSlice. 
   */
  Slice abs(const Slice & rSlice);

  /** 
   * Returns exponential of \a rSlice. 
   * \param rSlice Some random payoff. 
   * \return The random variable given by <code>exp(rSlice)</code>. 
   */
  Slice exp(const Slice & rSlice);

  /** 
   * Returns logarithm of \a rSlice. 
   * \param rSlice Some random payoff. 
   * \return The random variable given by <code> log(rSlice) </code>. 
   */
  Slice log(const Slice & rSlice);

  /** 
   * Returns squire root of \a rSlice. 
   * \param rSlice Some random payoff. 
   * \return The random variable given by <code> sqrt(rSlice) </code>. 
   */
  Slice sqrt(const Slice & rSlice);

  /** 
   * Returns the indicator of the event: \a rSlice is greater than \a dBarrier.
   * \param rSlice Some random payoff. 
   * \param dBarrier Lower barrier. 
   * \return The random variable given by <code> I(rSlice > dBarrier) </code>. 
   */
  Slice indicator(const Slice & rSlice, double dBarrier);

  /** 
   * Returns the indicator of the event: \a dBarrier is greater than \a rSlice.
   * \param dBarrier Upper barrier. 
   * \param rSlice Some random payoff. 
   * \return The random variable given by <code> I(dBarrier > rSlice) </code>. 
   */
  Slice indicator(double dBarrier, const Slice & rSlice);

  /** 
   * Returns the indicator of the event: \a rSlice is greater than \a rBarrier.
   * Both input parameters should be defined on the same model 
   * and at the same event time. 
   * \param rSlice Some random payoff. 
   * \param rBarrier Random variable describing lower barrier. 
   * \return The random variable given by <code> I(rSlice > rBarrier) </code>. 
   */
  Slice indicator(const Slice & rSlice, const Slice & rBarrier);

  /** 
   * Returns the equivalent value of the derivative security 
   * represented by \a rSlice at event time with index \a iEventTime. 
   * The index of event time for \a rSlice should be greater or equal 
   * \a iEventTime. 
   * \param rSlice A random payoff. 
   * \param iEventTime Index of event time, when the price of \a rSlice will 
   * be computed. 
   * \return The price of the random payoff given by \a rSlice at 
   * event time with the index \a iEventTime. 
   */
  Slice rollback(const Slice & rSlice, unsigned iEventTime);

  /**
   * \copydoc IModel::interpolate
   */
  MultiFunction interpolate(const Slice & rSlice);

  /** 
   * Returns the function that interpolates \a rSlice with respect to 
   * state processes with indexes \a rStates. Other states are set to  
   * to their initial values. 
   * \param rSlice Some random payoff. 
   * \param rState The indexes of state processes that will be present in 
   * the result. 
   * \return The explicit functional dependence of the random payoff
   * represented by \a rSlice on state processes with indexes \a rState. 
   */
  MultiFunction interpolate(const Slice & rSlice, 
			    const std::vector<unsigned> & rState);

 /** 
   * Returns the function that interpolates \a rSlice with respect to 
   * state processes with indexes less than \a iStates. Other states are set to  
   * to their initial values. 
   * \param rSlice Some random payoff. 
   * \param iStates The number of first state processes that will be present in 
   * the result. 
   * \return The explicit functional dependence of the random payoff
   * represented by \a rSlice on state processes with indexes less
   * than \a iStates. 
   */
  MultiFunction interpolate(const Slice & rSlice, unsigned iStates);
 
  /** 
   * Returns the value of random variable represented by \a rSlice at
   * initial values of state processes. This function is 
   * usually used at initial time. 
   * \param rSlice Some random payoff. 
   * \return The value of random variable represented by \a rSlice at initial 
   * values of state processes. 
   */
  double atOrigin(const Slice & rSlice);

  //@}
}

#include "cfl/Inline/iSlice.hpp"
#endif // of__cflSlice_hpp__
