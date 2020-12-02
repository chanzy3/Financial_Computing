//  Copyright (c) Dmitry Kramkov, 2006. All right reserved. 
// Implementation of classes and functions declared in the corresponding *.hpp file. 

#include "cfl/InterestRateModel.hpp"

using namespace cfl;

cfl::InterestRateModel::InterestRateModel(
IInterestRateModel * pNewModel, const Extended & rExtended) 
  :m_uExtended(rExtended), m_pModel(pNewModel)
{
  m_uExtended.assign(*m_pModel);
}
 
