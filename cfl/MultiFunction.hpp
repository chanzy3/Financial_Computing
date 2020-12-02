//Copyright (c) Dmitry Kramkov, 2000-2006. All rights reserved. 

#ifndef __cflMultiFunction_hpp__
#define __cflMultiFunction_hpp__

#include <cmath>
#include <vector>
#include "cfl/Function.hpp"

/**
 * \file   MultiFunction.hpp
 * \author Dmitry Kramkov
 * \date   2000-2006
 * \brief Multi-dimensional function object.
 *
 * Contains classes and functions related to a multi-dimensional function object. 
 */

namespace cfl
{
  /**
   * \ingroup cflFunctionObjects
   * \defgroup cflMultiFunction Multi-dimensional function object.
   * This module deals with a multi-dimensional function object.  
   */
  //@{

  //! Interface for multi-dimensional function objects.
  /**
   * This is the abstract class for multi-dimensional function
   * objects. A dynamically allocated implementation of this interface
   * is used to construct the standard multi-dimensional function
   * class MultiFunction.  \see MultiFunction
   */
  class  IMultiFunction
  {
  public:
    /**
     * Virtual destructor. 
     */ 
    virtual ~IMultiFunction(){};
    
    /**
     * Returns the value of the function at \a rX. 
     *  
     * \param rX The argument of the function. The size of this array
     * should equal the dimension of the function.
     * 
     * \return The value of the function at \a rX. 
     */
    virtual double operator()(const std::valarray<double> & rX) const = 0;
		
    /**
     * Returns \p true if argument belongs to the domain 
     * of the function. Returns \p false otherwise. 
     * 
     * \param rX The argument of the function. The size of this array
     * should equal the dimension of the function.
     * 
     * \return \p True if argument belongs to the domain of the
     * function and \p false otherwise.
     */
    virtual bool belongs(const std::valarray<double> & rX) const = 0;
		
    /** 
     * Returns the dimension of the function.
     * 
     * \return The dimension of the function. 
     */
    virtual unsigned dim() const = 0;
  };
	
  //! Standard concrete class for multi-dimensional function object. 
  /**
   * This is the standard class for a multi-dimensional function
   * object. It is constructed by a dynamically allocated object
   * derived from the interface IMultiFunction.  \see IMultiFunction
   */
  class MultiFunction: public std::function<double(std::valarray<double>)>
  {
  public:
    /**
     * Constructs constant function with the value \a dV and the
     * dimension \a iDim.  The domain of the function equals the whole
     * space.
     * 
     * \param dV The value of the function.
     * \param iDim The dimension of the function.
     */
    explicit MultiFunction(double dV=0, unsigned iDim=1);

    /**
     * Constructs \p *this from dynamically allocated implementation of
     * IMultiFunction.
     * 
     * \param pNewF A pointer to dynamically allocated implementation
     * of the interface class IMultiFunction.
     */
    explicit MultiFunction(IMultiFunction * pNewF);
		
    /**
     * Assigns to \p *this the constant function with the value \a dV.
     * The dimension of the function does not change. The domain
     * equals the whole space.
     * 
     * \param dV The value of the function.
     * \return Reference to \p *this.
     */
    MultiFunction & operator=(double dV);
		
    /**
     * Returns the value of the function for given argument. 
     * 
     * \param rX The argument of the function. The size of this array 
     * should equal the dimension of the function.
     *
     * \return The value of \p *this at the point \a rX.
     */
    double operator()(const std::valarray<double> & rX) const;

    /**
     * \copydoc IMultiFunction::belongs
     */
    bool belongs(const std::valarray<double> & rX) const;

    /**
     * \copydoc IMultiFunction::dim
     */
    unsigned dim() const; 
      
    /** 
     * Assigns to \p *this the sum of \p *this and \a rF.  The new
     * domain of \p *this equals the intersection of its old domain
     * with the domain of \a rF.  A deep copy of \a rF is created
     * inside of \p *this.
     * 
     * \param rF A constant reference to a multi-dimensional function
     * object. The dimension of this function should equal to the
     * dimension of \p *this.
     * 
     * \return Reference to \p *this.
     */
    MultiFunction & operator+=(const MultiFunction & rF);

    /** 
     * Assigns to \p *this the product of \p *this and \a rF.  The new
     * domain of \p *this equals the intersection of its old domain
     * with the domain of \a rF.  A deep copy of \a rF is created
     * inside of \p *this.
     * 
     * \param rF A constant reference to a multi-dimensional function
     * object. The dimension of this function should equal to the
     * dimension of \p *this.
     * 
     * \return Reference to \p *this.
     */
    MultiFunction & operator*=(const MultiFunction & rF);

    /** 
     * Assigns to \p *this the difference between \p *this and \a rF.
     * The new domain of \p *this equals the intersection of its old
     * domain with the domain of \a rF.  A deep copy of \a rF is
     * created inside of \p *this.
     * 
     * \param rF A constant reference to a multi-dimensional function
     * object. The dimension of this function should equal to the
     * dimension of \p *this.
     * 
     * \return Reference to \p *this.
     */
    MultiFunction & operator-=(const MultiFunction & rF);

    /** 
     * Assigns to \p *this the ratio of \p *this and \a rF.  The new
     * domain of \p *this equals the intersection of its old domain
     * with the domain of \a rF.  A deep copy of \a rF is created
     * inside of \p *this.
     * 
     * \param rF A constant reference to a multi-dimensional function
     * object. The dimension of this function should equal to the
     * dimension of \p *this.
     * 
     * \return Reference to \p *this.
     */
    MultiFunction & operator/=(const MultiFunction & rF);
      
    /**
     * Replaces \p *this with the sum of \p *this and \a dV. 
     * 
     * \param dV The number to be added to the function. 
     * 
     * \return Reference to \p *this.
     */
    MultiFunction & operator+=(double dV);

    /**
     * Replaces \p *this with the difference between \p *this and \a
     * dV.
     * 
     * \param dV The number to be subtracted from the function. 
     * 
     * \return Reference to \p *this.
     */
    MultiFunction & operator-=(double dV);

    /**
     * Replaces \p *this with the product of \p *this and \a dV. 
     * 
     * \param dV The number to be multiplied by the function. 
     * 
     * \return Reference to \p *this.
     */
    MultiFunction & operator*=(double dV);

    /**
     * Replaces \p *this with the ratio of \p *this and \a dV. 
     *
     * \param dV The divisor number. 
     * 
     * \return Reference to \p *this.
     */
    MultiFunction & operator/=(double dV);

  private:
    /**
     * The \p shared_ptr to an implementation of IMultiFunction
     */ 
    std::shared_ptr<IMultiFunction> m_pF;
  };		
	
  /** 
   * Returns minus \a rF. The result contains a deep copy of \a rF.
   * The domain of the result equals the domain of \a rF.
   * 
   * \param rF The function which minus is computed. 
   * 
   * \return The function given by <code>-rF</code>
   */
  MultiFunction operator-(const MultiFunction & rF);
	
  /** 
   * Returns the product of \a rF and \a rG.  The result contains deep
   * copies of \a rF and \a rG.  The domain of the result equals the
   * intersection of the domains of \a rF and \a rG.
   * 
   * \param rF First function multiplier.
   * \param rG Second function multiplier. 
   * 
   * \return The function given by <code>rF*rG</code>
   */
  MultiFunction 
  operator*(const MultiFunction & rF, const MultiFunction & rG);

  /** 
   * Returns the product of \a dV and \a rF. The result contains deep
   * copy of \a rF.  The domain of the result equals that of \a rF.
   * 
   * \param dV The multiplier number.
   * \param rF The multiplier function. 
   * 
   * \return The function given by <code>dV*rF</code>
   */
  MultiFunction 
  operator*(double dV, const MultiFunction & rF); 

  /** 
   * Returns the product of \a dV and \a rF.  The result contains a
   * deep copy of \a rF.  The domain of the result equals the domain
   * of \a rF.
   *
   * \param rF The multiplier function. 
   * \param dV The multiplier number.
   * 
   * \return The function given by <code>rF*dV</code>. 
   */
  MultiFunction 
  operator*(const MultiFunction & rF, double dV);
	
  /** 
   * Returns the sum of \a rF and \a rG.  The result contains
   * deep copies of \a rF and \a rG.  The domain of the result equals
   * the intersection of the domains of \a rF and \a rG.
   * 
   * \param rF First function term of the sum.
   * \param rG Second function term of the sum. 
   * 
   * \return The function given by <code>rF+rG</code>
   */
  MultiFunction 
  operator+(const MultiFunction & rF, const MultiFunction & rG);

  /** 
   * Returns the  sum of \a dV and \a rF. The result
   * contains a deep copy of \a rF.  The domain of the
   * result equals that of \a rF.
   * 
   * \param dV The number-term of the sum. 
   * \param rF The function-term of the sum. 
   * 
   * \return The function given by <code>dV+rF</code>
   */
  MultiFunction 
  operator+(double dV, const MultiFunction & rF);

  /** 
   * Returns the sum of \a rF and \a dV.  The result contains a deep
   * copy of \a rF.  The domain of the result equals the domain of \a
   * rF.
   * 
   * \param rF The function-term of the sum.
   * \param dV The number-term of the sum. 
   * 
   * \return The function given by <code>rF+dV</code>. 
   */
  MultiFunction 
  operator+(const MultiFunction & rF,double dV);
	
  /** 
   * Returns the difference between \a rF and \a rG.  The result
   * contains deep copies of \a rF and \a rG.  The domain of the
   * result equals the intersection of the domains of \a rF and \a rG.
   * 
   * \param rF The function from which we subtract. 
   * \param rG The function which is subtracted. 
   * 
   * \return The function given by <code>rF-rG</code>
   */
  MultiFunction 
  operator-(const MultiFunction & rF, const MultiFunction & rG);

  /** 
   * Returns the difference between \a dV and \a rF. The result
   * contains a deep copy of \a rF.  The domain of the result equals
   * that of \a rF.
   * 
   * \param dV The number from which we subtract.
   * \param rF The function which is subtracted. 
   * 
   * \return The function given by <code>dV-rF</code>
   */
  MultiFunction 
  operator-(double dV, const MultiFunction & rF);

  /** 
   * Returns the difference between \a rF and \a dV.  The result
   * contains a deep copy of \a rF.  The domain of the result equals
   * the domain of \a rF.
   * 
   * \param rF The function from which we subtract. 
   * \param dV The number which is subtracted. 
   * 
   * \return The function given by <code>rF-dV</code>. 
   */
  MultiFunction 
  operator-(const MultiFunction & rF, double dV);
	
  /** 
   * Returns the ratio of \a rF and \a rG.  The result contains deep
   * copies of \a rF and \a rG.  The domain of the result equals the
   * intersection of the domains of \a rF and \a rG.
   * 
   * \param rF The function which is divided.
   * \param rG The divisor function. 
   * 
   * \return The function given by <code>rF/rG</code>
   */
  MultiFunction 
  operator/(const MultiFunction & rF, const MultiFunction & rG);

  /** 
   * Returns the ratio of \a dV and \a rF. The result contains a deep
   * copy of \a rF.  The domain of the result equals that of \a rF.
   * 
   * \param dV The number which is divided. 
   * \param rF The divisor function. 
   * 
   * \return The function given by <code>dV/rF</code>
   */
  MultiFunction 
  operator/(double dV, const MultiFunction & rF);

  /** 
   * Returns the ratio of \a rF and \a dV.  The result contains a deep
   * copy of \a rF.  The domain of the result equals the domain of \a
   * rF.
   * 
   * \param rF The function which is divided. 
   * \param dV The divisor number. 
   * 
   * \return The function given by <code>rF/dV</code>. 
   */
  MultiFunction 
  operator/(const MultiFunction & rF, double dV);
	
  /** 
   * Returns the maximum of \a rF and \a rG.  The result contains deep
   * copies of \a rF and \a rG.  The domain of the result equals the
   * intersection of the domains of \a rF and \a rG.
   * 
   * \param rF First function in the maximum. 
   * \param rG Second function in the maximum. 
   * 
   * \return The function given by <code> max(rF,rG) </code>. 
   */
  MultiFunction max(const MultiFunction & rF, const MultiFunction & rG);

  /** 
   * Returns the maximum between \a dV and \a rF.  The result contains
   * a deep copy of \a rF.  The domain of the result equals the domain
   * of \a rF.
   * 
   * \param dV The number-term of the maximum. 
   * \param rF The function-term of the maximum. 
   * 
   * \return The function given by <code>max(dV,rF)</code>. 
   */
  MultiFunction max(double dV, const MultiFunction & rF);


  /** 
   * Returns the maximum between \a rF and \a dV.  The result contains
   * a deep copy of \a rF.  The domain of the result equals the domain
   * of \a rF.
   * 
   * \param rF The function-term of the maximum. 
   * \param dV The number-term of the maximum. 
   * 
   * \return The function given by <code>max(rF,dV)</code>. 
   */
  MultiFunction max(const MultiFunction & rF, double dV);
	
  /** 
   * Returns the minimum of \a rF and \a rG.  The result contains deep
   * copies of \a rF and \a rG.  The domain of the result equals the
   * intersection of the domains of \a rF and \a rG.
   * 
   * \param rF The first function in the minimum. 
   * \param rG The second term in the minimum. 
   * 
   * \return The function given by <code> min(rF,rG) </code>. 
   */
  MultiFunction min(const MultiFunction & rF, const MultiFunction & rG);

  /** 
   * Returns the minimum between \a dV and \a rF.  The result contains
   * a deep copy of \a rF.  The domain of the result equals the domain
   * of \a rF.
   * 
   * \param dV The number-term of the minimum. 
   * \param rF The function-term of the minimum. 
   * 
   * \return The function given by <code> min(dV,rF) </code>. 
   */
  MultiFunction min(double dV, const MultiFunction & rF);

  /** 
   * Returns the minimum between \a rF and \a dV.  The result contains
   * a deep copy of \a rF.  The domain of the result equals the domain
   * of \a rF.
   * 
   * \param rF The function-term of the maximum. 
   * \param dV The number-term of the maximum. 
   * 
   * \return The function given by <code> min(rF,dV) </code>. 
   */
  MultiFunction min(const MultiFunction & rF, double dV);
	
  /** 
   * Returns \a rF in the power \a dV.  The result contains a deep
   * copy of \a rF.  The domain of the result equals the domain of \a
   * rF.
   * 
   * \param rF The function-base. 
   * \param dV The number-exponent. 
   * 
   * \return The function \a rF in the power \a dV. 
   */
  MultiFunction pow(const MultiFunction & rF, double dV);

  /** 
   * Returns the absolute value of \a rF.  The result contains a deep
   * copy of \a rF.  The domain of the result equals the domain of \a
   * rF.
   * 
   * \param rF The function which absolute value is computed. 
   *
   * \return The absolute value of \a rF. 
   */
  MultiFunction abs(const MultiFunction & rF);

  /**
   * Returns the exponent of \a rF.  The result contains a deep
   * copy of \a rF.  The domain of the result equals the domain of \a
   * rF.
   * 
   * \param rF The function which exponent is computed. 
   *
   * \return The exponent of  \a rF. 
   */
  MultiFunction exp(const MultiFunction & rF);

  /**
   * Returns the logarithm of \a rF. 
   * The result contains a deep copy of \a rF.
   * The domain of the result equals the domain of \a rF.
   * 
   * \param rF The function which logarithm is computed. 
   *
   * \return The logarithm of  \a rF.
   */
  MultiFunction log(const MultiFunction & rF);

  /** 
   * Returns the squire root of \a rF. 
   * The result contains the a deep copy of \a rF.
   * The domain of the result equals the domain of \a rF.
   * 
   * \param rF The function which square root is computed. 
   *
   * \return The square root of  \a rF.
   */
  MultiFunction sqrt(const MultiFunction & rF);
	
  /** 
   * Returns function with dimension \a iDim which values are given by
   * \a f and the domain is defined by \a g.
   * 
   * \param f This function determines the values of the result. 
   * \param g This function determines the domain of the result. 
   * \param iDim The dimension of the result.
   * 
   * \return The \a iDim-dimensional function object which values are
   * given by \a f and the domain is defined by \a g.
   */
  MultiFunction toMultiFunction(double (*f)(const std::valarray<double> &), 
				bool (*g)(const std::valarray<double> &), unsigned iDim);

  class Function;
	
  /** 
   * Adapter of one-dimensional function to a multi-dimensional one. 
   * 
   * \param rF The constant reference to a one-dimensional function object.
   * \param iDim The dimension of the domain of the result.
   * \param iArg The index of the coordinate which is used as an argument of 
   * the function \a rF to compute the value of the result.
   * 
   * \return The function with dimension \a iDim which value at a
   * vector with size \a iDim equals the value of \a rF at the element
   * of this vector index \a iArg.
   */
  MultiFunction toMultiFunction(const Function & rF, unsigned iArg, 
				unsigned iDim);
	
  /** 
   * Constrains multi-dimensional function to a domain of smaller dimension. 
   * 
   * \param rF The input multi-dimensional function.
   * \param rArg The vector of arguments of the flexible coordinates. 
   * \param rOtherArg The values of fixed coordinates. 
   * 
   * \return The restriction of function \a rF on the points where the
   * coordinates with indexes \a rArg are flexible and the other
   * coordinates are fixed and equal \a rOtherArg. The sum of sizes of
   * \a rArg and \a rOtherArg should equal the dimension of \a rF. The
   * dimension of the result equals the the size of rArg.
   */
  MultiFunction toMultiFunction(const MultiFunction & rF, 
				const std::vector<unsigned> & rArg, 
				const std::valarray<double> & rOtherArg);

  //@}
}

#include "cfl/Inline/iMultiFunction.hpp"
#endif // of __cflMultiFunction_hpp__

