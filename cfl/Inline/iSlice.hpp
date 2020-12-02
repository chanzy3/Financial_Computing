//Copyright (c) Dmitry Kramkov, 2000-2006. All rights reserved. 
// do not include this file

inline cfl::Slice & cfl::Slice::operator=(const cfl::Slice & rSlice) 
{
  assign(*rSlice.ptrToModel(), rSlice.timeIndex(), 
	 rSlice.dependence(), rSlice.values());
  return *this;
}

inline cfl::Slice & cfl::Slice::operator=(double dValue) 
{
  m_uDependence.clear();
  m_uValues.resize(1);
  m_uValues[0] = dValue;
  return *this;
}

inline cfl::Slice & cfl::Slice::operator+=(double dValue) 
{
  m_uValues += dValue;
  return *this;
}

inline cfl::Slice & cfl::Slice::operator-=(double dValue) 
{
  m_uValues -= dValue;
  return *this;
}

inline cfl::Slice & cfl::Slice::operator*=(double dValue) 
{
  m_uValues *= dValue;
  return *this;
}

inline cfl::Slice & cfl::Slice::operator/=(double dValue) 
{
  m_uValues /= dValue;
  return *this;
}

inline cfl::Slice cfl::Slice::apply(double (*func)(double)) const 
{
  return cfl::Slice(*m_pModel, m_iEventTime, m_uDependence, m_uValues.apply(func));
}

inline void cfl::Slice::rollback(unsigned iTime) 
{ 
  PRECONDITION(iTime <= this->timeIndex());
  m_pModel->rollback(*this, iTime); 
}

inline const cfl::IModel * cfl::Slice::ptrToModel() const 
{ 
  return m_pModel; 
}

inline unsigned cfl::Slice::timeIndex() const 
{ 
  return m_iEventTime; 
}

inline const std::vector<unsigned> & cfl::Slice::dependence() const 
{ 
  return m_uDependence; 
}

inline const std::valarray<double>  & cfl::Slice::values() const 
{ 
  return m_uValues; 
}

inline void cfl::Slice::assign(const cfl::IModel & rModel, 
			       unsigned iTime, 
			       const std::vector<unsigned> & rDependence, 
			       const std::valarray<double> & rValues) 
{
  m_pModel = &rModel;
  assign(iTime, rDependence, rValues);
  POSTCONDITION(m_pModel->numberOfNodes(iTime, rDependence) == rValues.size());
}

inline void cfl::Slice::assign(unsigned iTime, 
			       const std::vector<unsigned> & rDependence, 
			       const std::valarray<double> & rValues) 
{
  m_iEventTime = iTime;
  assign(rDependence, rValues);
  POSTCONDITION(m_pModel->numberOfNodes(iTime, rDependence) == rValues.size());
}

inline void cfl::Slice::assign(const std::vector<unsigned> & rDependence, 
			       const std::valarray<double> & rValues) 
{
  m_uDependence = rDependence; 
  assign(rValues);
  POSTCONDITION(m_pModel->numberOfNodes(m_iEventTime, rDependence) == rValues.size());
}

inline void cfl::Slice::assign(const std::valarray<double> & rValues) 
{
  if (rValues.size() != m_uValues.size()) {
    m_uValues.resize(rValues.size());
  }
  m_uValues = rValues;
  POSTCONDITION(m_pModel->numberOfNodes(m_iEventTime, m_uDependence) == rValues.size());	
}

inline void cfl::Slice::assign(const IModel & rModel) 
{
  m_pModel = &rModel; 
  POSTCONDITION(m_pModel->numberOfNodes(m_iEventTime, m_uDependence) == m_uValues.size());
}

//Arithmetic operators and functions. 
inline cfl::Slice cfl::operator-(const cfl::Slice & rSlice) 
{
  return cfl::Slice(*rSlice.ptrToModel(), rSlice.timeIndex(), 
		    rSlice.dependence(),-rSlice.values());
}

inline cfl::Slice cfl::operator+(const cfl::Slice & rSlice1, const cfl::Slice & rSlice2) 
{
  cfl::Slice uSlice(rSlice1);
  uSlice+=rSlice2;
  return uSlice;
}

inline cfl::Slice cfl::operator-(const cfl::Slice & rSlice1, const cfl::Slice & rSlice2) 
{
  cfl::Slice uSlice(rSlice1);
  uSlice-=rSlice2;
  return uSlice;
}

inline cfl::Slice cfl::operator*(const cfl::Slice & rSlice1, const cfl::Slice & rSlice2) 
{
  cfl::Slice uSlice(rSlice1);
  uSlice*=rSlice2;
  return uSlice;
}

inline cfl::Slice cfl::operator/(const cfl::Slice & rSlice1, const cfl::Slice & rSlice2) 
{
  cfl::Slice uSlice(rSlice1);
  uSlice/=rSlice2;
  return uSlice;
}


inline cfl::Slice cfl::operator+(const cfl::Slice & rSlice, double dValue) 
{
  return cfl::Slice(*rSlice.ptrToModel(), rSlice.timeIndex(), rSlice.dependence(), 
		    rSlice.values()+dValue);
}

inline cfl::Slice cfl::operator-(const cfl::Slice & rSlice, double dValue) 
{
  return cfl::Slice(*rSlice.ptrToModel(), rSlice.timeIndex(), rSlice.dependence(), 
		    rSlice.values()-dValue);
}

inline cfl::Slice cfl::operator*(const cfl::Slice & rSlice, double dValue) 
{
  return cfl::Slice(*rSlice.ptrToModel(), rSlice.timeIndex(), rSlice.dependence(), 
		    rSlice.values()*dValue);
}
inline cfl::Slice cfl::operator/(const cfl::Slice & rSlice, double dValue) 
{
  return cfl::Slice(*rSlice.ptrToModel(), rSlice.timeIndex(), rSlice.dependence(), 
		    rSlice.values()/dValue);
}

inline cfl::Slice cfl::operator+(double dValue, const cfl::Slice & rSlice) 
{
  return operator+(rSlice, dValue);
}
inline cfl::Slice cfl::operator-(double dValue, const cfl::Slice & rSlice) 
{
  return cfl::Slice(*rSlice.ptrToModel(), rSlice.timeIndex(), rSlice.dependence(), 
		    dValue-rSlice.values());
}

inline cfl::Slice cfl::operator*(double dValue, const cfl::Slice & rSlice) 
{
  return operator*(rSlice, dValue);
}
inline cfl::Slice cfl::operator/(double dValue, const cfl::Slice & rSlice) 
{
  return cfl::Slice(*rSlice.ptrToModel(), rSlice.timeIndex(), 
		    rSlice.dependence(), dValue/rSlice.values());
}

inline cfl::Slice cfl::max(double dValue, const cfl::Slice & rSlice) 
{
  return max(rSlice,dValue);
}

inline cfl::Slice cfl::min(double dValue, const cfl::Slice & rSlice) 
{
  return min(rSlice, dValue);
}

inline cfl::Slice cfl::pow(const cfl::Slice & rSlice, double dPower) 
{
  return cfl::Slice(*rSlice.ptrToModel(), rSlice.timeIndex(), 
		    rSlice.dependence(), std::pow(rSlice.values(), dPower));
}

inline cfl::Slice cfl::abs(const cfl::Slice & rSlice) 
{
  return cfl::Slice(*rSlice.ptrToModel(), 
		    rSlice.timeIndex(), rSlice.dependence(), 
		    std::abs(rSlice.values()));
}

inline cfl::Slice cfl::exp(const cfl::Slice & rSlice) 
{
  return cfl::Slice(*rSlice.ptrToModel(), rSlice.timeIndex(), rSlice.dependence(), 
		    std::exp(rSlice.values()));
}

inline cfl::Slice cfl::log(const cfl::Slice & rSlice) 
{
  return cfl::Slice(*rSlice.ptrToModel(), rSlice.timeIndex(), rSlice.dependence(), 
		    std::log(rSlice.values()));
}

inline cfl::Slice cfl::sqrt(const cfl::Slice & rSlice) 
{
  return cfl::Slice(*rSlice.ptrToModel(), rSlice.timeIndex(), rSlice.dependence(), 
		    std::sqrt(rSlice.values()));
}

inline cfl::Slice cfl::indicator(const cfl::Slice & rSlice, double dBarrier) 
{
  Slice uInd(rSlice);
  rSlice.ptrToModel()->indicator(uInd, dBarrier);
  return uInd;
}

inline cfl::Slice cfl::indicator(double dBarrier, const cfl::Slice & rSlice) 
{
  return 1. - indicator(rSlice, dBarrier);
}

inline cfl::Slice cfl::indicator(const cfl::Slice & rSlice, const cfl::Slice & rBarrier) 
{
  return indicator(rSlice-rBarrier, 0.);
}

inline cfl::Slice cfl::rollback(const cfl::Slice & rSlice, unsigned iTime) 
{
  cfl::Slice uSlice(rSlice);
  uSlice.rollback(iTime);
  return uSlice;
}

inline cfl::MultiFunction cfl::interpolate(const cfl::Slice & rSlice) 
{
  return rSlice.ptrToModel()->interpolate(rSlice);
}

inline cfl::MultiFunction cfl::interpolate(const cfl::Slice & rSlice,
unsigned iStates) 
{
  std::vector<unsigned> uDepend(iStates, 0);
  std::transform(uDepend.begin(), uDepend.end()-1, uDepend.begin()+1, 
		 [](unsigned iX){ return iX+1; });
  return interpolate(rSlice, uDepend);
}
