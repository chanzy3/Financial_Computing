//Copyright (c) Dmitry Kramkov, 2000-2006. All rights reserved. 

#ifndef __cflExtended_hpp__
#define __cflExtended_hpp__

#include "cfl/PathDependent.hpp"

  /**
   * \file   Extended.hpp
   * \author Dmitry Kramkov
   * \date   2000-2006
   * 
   * \brief Extension of models by path dependent state processes. 
   *
   * This file contains classes and functions needed for the extension of financial 
   * models by path dependent state processes. 
   */

  namespace cfl
  {
    /** 
     * \defgroup cflCommonElements Common elements of financial models. 
     * This module contains common elements of financial models. 
     */

    /** 
     * \ingroup cflCommonElements
     * \defgroup cflPathDepend Path dependent processes.
     * This module is dealing with path dependent 
     * processes that could be used as additional state processes. 
     */
    //@{

    //! Interface class for a one-dimensional extension of a model.
    /**
     * This is the interface class for the extension of the original financial model 
     * with path dependent state process. 
     * \see Extended, PathDependent and IResetValues
     */
    class IExtend
    {
    public:
      /**
       * Virtual destructor. 
       */
      virtual ~IExtend(){};
    
      /** 
       * The function returns the pointer on a free store to the model which extends 
       * the original model \a rModel by adding to it the path dependent process 
       * \a rState as an additional state process. 
       * \param rState The path dependent process which is used as an additional 
       * state process. 
       * \param rModel A constant reference to the original model. 
       * \return The pointer on a free store to the extended model. 
       */
      virtual IModel * newModel(const PathDependent & rState, 
				const IModel & rModel) const = 0;	
    };
	
    //! Concrete class for the "extended" financial models. 
    /** 
     * This class constructs "extended" financial models from some 
     * given model and a collection of path dependent state processes. 
     * 
     * \see IExtend, PathDependent and IResetValues
     */
    class Extended: public IModel
    {
    public:
      /** 
       * The constructor. 
       * \param pNewP A dynamically allocated implementation of the interface class IExtend. 
       */
      explicit Extended(IExtend * pNewP = 0);

      /** 
       * The constructor. 
       * \param rVecExtend The vector of pointers to implementations of the interface class IExtend. 
       */
      explicit Extended(const std::vector<std::shared_ptr<IExtend> > & rVecExtend);

      /**
       *	Replaces the original (non-extended) model with \a rModel. 
       * \param rModel The constant reference to the original model. 
       */ 
      void assign(const IModel & rModel);

      /** 
       * Adds another state process to the model.  
       * \param rState The description of a path dependent process which becomes
       * additional state process in the model. 
       * \return The index of the additional state process. 
       */
      unsigned addState(const PathDependent &  rState);

      /**
       * \copydoc IModel::eventTimes
       */
      const std::vector<double> & eventTimes() const;

      /**
       * \copydoc IModel::numberOfStates
       */
      unsigned numberOfStates() const;

      /**
       * \copydoc IModel::numberOfNodes
       */
      unsigned numberOfNodes(unsigned iEventTime, 
			     const std::vector<unsigned> & rStates) const;

      /**
       * \copydoc IModel::origin
       */
      std::valarray<double> origin() const;

      /**
       * \copydoc IModel::state
       */
      Slice state(unsigned iEventTime, unsigned iState) const;

      /**
       * \copydoc IModel::addDependence
       */
      void addDependence(Slice & rSlice, 
			 const std::vector<unsigned> & rStates) const;

      /**
       * \copydoc IModel::rollback
       */
      void rollback(Slice & rSlice, unsigned iEventTime) const;

      /**
       * \copydoc IModel::indicator
       */
      void indicator(Slice & rSlice, double dBarrier) const;

      /**
       * \copydoc IModel::interpolate
       */
      MultiFunction interpolate(const Slice & rSlice) const;

      /** 
       * Accessor function to the original (non-extended) model. 
       * \return A constant pointer to the implementation of the original (non-extended) model. 
       */
      const IModel * ptrToModel() const;

    private:
      std::vector<std::shared_ptr<IExtend> >  m_uExtend;
      std::vector<std::shared_ptr<IModel> > m_uModels;
      const IModel * m_pModel;
    }; 

    //! Implementations of "expandable" financial models. 
    /** 
     * This namespace contains implementations of "expandable"
     * financial models. These models can be extended by additional 
     * state processes. 
     */
    namespace NExtended
    {
      /** 
       * Implements the extended model by using supplied methods of numerical approximation. 
       * \param rApprox The vector of implementations for numerical approximation. 
       * \return Implementation of class Extended.  
       */
      Extended model(const std::vector<Approx> & rApprox);

      /** 
       * Implements the extended model by using the supplied method of numerical approximation. 
       * \param rApprox A method of numerical approximation.  
       *
       * \return Implementation of class Extended.
       */
      Extended model(const Approx & rApprox);

      /** 
       * Implements the extended model by using the default method of numerical approximation. 
       * \param dQuality The quality of the  default method of numerical approximation.
       * \return Implementation of class Extended.  
       */
      Extended model(double dQuality);

      /** 
       * Implements the extended model by using the default methods of numerical approximation. 
       * \param rQuality The qualities of the default methods for numerical approximation.  
       * \return Implementation of class Extended.  
       */
      Extended model(const std::vector<double> & rQuality);
    }
    //@} 
  }

#include "cfl/Inline/iExtended.hpp"
#endif // of __cflExtended_hpp__

