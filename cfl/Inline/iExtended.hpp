//Copyright (c) Dmitry Kramkov, 2000-2006. All rights reserved. 
//do not include this file directly

inline void cfl::Extended::assign(const IModel & rModel)
{
  m_pModel = &rModel;
  m_uModels.clear();
  POSTCONDITION(m_uModels.size()==0);
}

inline const std::vector<double> & cfl::Extended::eventTimes() const 
{
  return m_pModel->eventTimes();
}

inline unsigned cfl::Extended::numberOfStates() const
{
  return m_uModels.size()+m_pModel->numberOfStates();
}

inline unsigned cfl::Extended::numberOfNodes(unsigned iEventTime, 
					     const std::vector<unsigned> & rStates) const
{
  const IModel & rModel = (m_uModels.size()>0) ? *m_uModels.back() : *m_pModel;
  return rModel.numberOfNodes(iEventTime, rStates);
}

inline std::valarray<double> cfl::Extended::origin() const
{
  const IModel & rModel = (m_uModels.size()>0) ? *m_uModels.back() : *m_pModel;
  return rModel.origin();
}

inline cfl::Slice cfl::Extended::state(unsigned iEventTime, unsigned iState) const
{
  const IModel & rModel = (m_uModels.size()>0) ? *m_uModels.back() : *m_pModel;
  Slice uState = rModel.state(iEventTime, iState);
  uState.assign(*this);
  return uState;
}

inline void cfl::Extended::addDependence(Slice & rSlice, 
					 const std::vector<unsigned> & rStates) const
{
  PRECONDITION(rSlice.ptrToModel() == this);
  const IModel & rModel = (m_uModels.size()>0) ? *m_uModels.back() : *m_pModel;
  rSlice.assign(rModel);
  rModel.addDependence(rSlice, rStates);
  rSlice.assign(*this);
}

inline void cfl::Extended::rollback(Slice & rSlice, unsigned iEventTime) const
{
  PRECONDITION(rSlice.ptrToModel() == this);
  const IModel & rModel = (m_uModels.size()>0) ? *m_uModels.back() : *m_pModel;
  rSlice.assign(rModel);
  rModel.rollback(rSlice, iEventTime);
  rSlice.assign(*this);
}

inline void cfl::Extended::indicator(Slice & rSlice, double dBarrier) const
{
  PRECONDITION(rSlice.ptrToModel() == this);
  const IModel & rModel = (m_uModels.size()>0) ? *m_uModels.back() : *m_pModel;
  rSlice.assign(rModel);
  rModel.indicator(rSlice, dBarrier);
  rSlice.assign(*this);
}

inline cfl::MultiFunction cfl::Extended::interpolate(const Slice & rSlice) const
{
  PRECONDITION(rSlice.ptrToModel() == this);
  const IModel & rModel = (m_uModels.size()>0) ? *m_uModels.back() : *m_pModel;
  Slice uSlice(rSlice);
  uSlice.assign(rModel);
  cfl::MultiFunction uFunction = rModel.interpolate(uSlice);
  return uFunction;
}

//inline functions 

inline cfl::Extended cfl::NExtended::model(const Approx & rApprox)
{
  return model(std::vector<Approx>(1, rApprox));
}

inline cfl::Extended cfl::NExtended::model(double dQuality)
{
  return model(std::vector<double>(1, dQuality));
}


