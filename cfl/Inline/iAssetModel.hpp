//Copyright (c) Dmitry Kramkov, 2000-2006. All rights reserved. 
//do not include this file

inline void 
cfl::AssetModel::assignEventTimes(const std::vector<double> & rEventTimes) 
{
  //initial time should be the same 
  PRECONDITION(rEventTimes.front() == eventTimes().front());
  m_pModel.reset(m_pModel->newModel(rEventTimes));
  m_uExtended.assign(*m_pModel);
}

inline unsigned 
cfl::AssetModel::addState(const PathDependent & rState)
{
  return m_uExtended.addState(rState);
}

inline unsigned 
cfl::AssetModel::numberOfStates() const
{
  return m_uExtended.numberOfStates();
}

inline const std::vector<double> & 
cfl::AssetModel::eventTimes() const 
{ 
  return m_uExtended.eventTimes(); 
}

inline double 
cfl::AssetModel::initialTime() const 
{ 
  return eventTimes().front(); 
}

inline cfl::Slice 
cfl::AssetModel::state(unsigned iTimeIndex, unsigned iState) const 
{
  return m_uExtended.state(iTimeIndex, iState);
}

inline cfl::Slice 
cfl::AssetModel::cash(unsigned iTimeIndex, double dAmount) const 
{
  return Slice(&m_uExtended, iTimeIndex, dAmount);
}

inline cfl::Slice 
cfl::AssetModel::discount(unsigned iTimeIndex, double dBondMaturity) const 
{
  PRECONDITION(eventTimes()[iTimeIndex] <= dBondMaturity);
  Slice uDiscount = m_pModel->discount(iTimeIndex, dBondMaturity);
  uDiscount.assign(m_uExtended);
  return uDiscount; 
}

inline cfl::Slice 
cfl::AssetModel::forward(unsigned iTimeIndex, double dForwardMaturity) const 
{
  PRECONDITION(eventTimes()[iTimeIndex] <= dForwardMaturity);
  Slice uForward = m_pModel->forward(iTimeIndex, dForwardMaturity);
  uForward.assign(m_uExtended);
  return uForward; 
}

inline cfl::Slice 
cfl::AssetModel::spot(unsigned iTimeIndex) const 
{
  return forward(iTimeIndex, eventTimes()[iTimeIndex]); 
}

