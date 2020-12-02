// Copyright (c) Dmitry Kramkov, 2000-2006. All rights reserved. 
// Implementation of classes and functions declared in the corresponding *.hpp file. 

#include "cfl/AssetModel.hpp"

using namespace cfl;

//class AssetModel

cfl::AssetModel::AssetModel(IAssetModel * pNewModel, const Extended & rExtended)
  :m_pModel(pNewModel), m_uExtended(rExtended)
{
  m_uExtended.assign(*m_pModel);
}	
