//  Copyright (c) Dmitry Kramkov, 2000-2006. All rights reserved. 
// Implementation of classes and functions declared in the corresponding *.hpp file. 

#include <limits>
#include "cfl/Error.hpp" 
#include "cfl/PathDependent.hpp"
#include "cfl/Approx.hpp"
#include "cfl/Auxiliary.hpp"

using namespace cfl;

cfl::PathDependent::
PathDependent(IResetValues * pNewP, 
	      const std::vector<unsigned> & rTimeIndexes, 
	      double dOrigin, 
	      double dInterval 
	      ) 
  :m_pP(pNewP), m_uTimeIndexes(rTimeIndexes), 
   m_dOrigin(dOrigin), m_dInterval(dInterval) 
{
  ASSERT(m_dInterval >= 0);
}
