//do not include this file

inline void 
cfl::InterestRateModel::assignEventTimes(const std::vector<double> & rEventTimes) 
{
  //initial time should be the same 
  PRECONDITION(rEventTimes.front() == eventTimes().front());
  m_pModel.reset(m_pModel->newModel(rEventTimes));
  m_uExtended.assign(*m_pModel);
}

inline unsigned 
cfl::InterestRateModel::addState(const PathDependent & rState)
{
  return m_uExtended.addState(rState);
}

inline unsigned 
cfl::InterestRateModel::numberOfStates() const
{
  return m_uExtended.numberOfStates();
}

inline const std::vector<double> & 
cfl::InterestRateModel::eventTimes() const 
{ 
  return m_uExtended.eventTimes(); 
}

inline double 
cfl::InterestRateModel::initialTime() const 
{ 
  return eventTimes().front(); 
}


inline cfl::Slice 
cfl::InterestRateModel::state(unsigned iTimeIndex, unsigned iState) const 
{
  return m_uExtended.state(iTimeIndex, iState);
}

inline cfl::Slice 
cfl::InterestRateModel::cash(unsigned iTimeIndex, double dAmount) const 
{
  return Slice(&m_uExtended, iTimeIndex, dAmount);
}

inline cfl::Slice 
cfl::InterestRateModel::discount(unsigned iTimeIndex, double dBondMaturity) const 
{
  PRECONDITION(eventTimes()[iTimeIndex] <= dBondMaturity);
  Slice uDiscount = m_pModel->discount(iTimeIndex, dBondMaturity);
  uDiscount.assign(m_uExtended);
  return uDiscount; 
}
