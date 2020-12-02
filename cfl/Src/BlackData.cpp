/*-----------------------------------------------------------------------------
  Copyright (c) Dmitry Kramkov, Carnegie Mellon University (kramkov@andrew.cmu.edu)
  Description	: implementation of the classes defined in *.h file
  --------------------------------------------------------------------------------*/
#include "cfl/BlackData.hpp"
#include "cfl/Data.hpp"

using namespace cfl::Black;
using namespace cfl;

cfl::Black::Data::Data(const Function & rDiscount, 
		       const Function & rForward, 
		       const Function  & rVolatility,	
		       double dInitialTime)
  :m_uDiscount(rDiscount), 
   m_uForward(rForward), 
   m_uVolatility(rVolatility), 
   m_uShape(1.), 
   m_dInitialTime(dInitialTime)
{}

cfl::Black::Data::Data(const Function & rDiscount, 
		       const Function & rForward, 
		       double dSigma, 
		       double dInitialTime)
  :m_uDiscount(rDiscount), 
   m_uForward(rForward), 
   m_uVolatility(dSigma), 
   m_uShape(1.), 
   m_dInitialTime(dInitialTime)
{}

cfl::Black::Data::Data(const Function & rDiscount, 
		       const Function & rForward, 
		       const Function  & rVolatility,	
		       const Function  & rShape, 
		       double dInitialTime)
  :m_uDiscount(rDiscount), 
   m_uForward(rForward), 
   m_uVolatility(rVolatility), 
   m_uShape(rShape), 
   m_dInitialTime(dInitialTime) 
{}

cfl::Black::Data::Data(const Function & rDiscount, 
		       const Function & rForward, 
		       double dSigma, 
		       double dLambda, 
		       double dInitialTime)
  :m_uDiscount(rDiscount), m_uForward(rForward), 
   m_uVolatility(cfl::Data::volatility(dSigma, dLambda, dInitialTime)), 
   m_uShape(cfl::Data::assetShape(dLambda, dInitialTime)), 
   m_dInitialTime(dInitialTime)
{}

