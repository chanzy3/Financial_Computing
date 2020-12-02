//  Copyright (c) Dmitry Kramkov, 2000-2006. All rights reserved. 
// Implementation of classes and functions declared in the corresponding *.hpp file. 

#include <limits>
#include "cfl/Error.hpp"
#include "cfl/PathDependent.hpp"
#include "cfl/Approx.hpp"
#include "cfl/Auxiliary.hpp"
#include "cfl/Extended.hpp"

using namespace cfl;

//class Extended
cfl::Extended::Extended(IExtend * pNewP)
  :m_uExtend(1, std::shared_ptr<IExtend>(pNewP)), m_uModels(0), m_pModel(0)
{}

cfl::Extended::Extended(const std::vector<std::shared_ptr<IExtend> > & rVecExtend)
  :m_uExtend(rVecExtend), m_uModels(0), m_pModel(0)
{}

namespace cflExtended
{
  class ChangeModel: public IResetValues
  {
  public:
    ChangeModel(const PathDependent & rState, const IModel & rModel) 
      :m_uState(rState), m_rModel(rModel) 
    {}

    Slice resetValues(unsigned iTime, double dBeforeReset) const 
    {
      Slice uSlice = m_uState.resetValues(iTime, dBeforeReset);
      uSlice.assign(m_rModel);
      return uSlice;
    }
  private:
    PathDependent m_uState;
    const IModel & m_rModel;
  };

  inline cfl::PathDependent changeModel(const PathDependent & rState, const IModel & rModel)
  {
    return PathDependent(new ChangeModel(rState, rModel), rState.timeIndexes(), 
			 rState.origin(), rState.interval());
  }
}

using namespace cflExtended;

unsigned cfl::Extended::addState(const PathDependent & rPathDependent)
{
  ASSERT(m_pModel != 0);
  ASSERT(m_uExtend.size()>0);
  const IModel & rModel = (m_uModels.size() > 0) ? *m_uModels.back() : *m_pModel;
  PathDependent uState = changeModel(rPathDependent, rModel);
  std::shared_ptr<IModel> pExtended(m_uExtend.front()->newModel(rPathDependent,rModel));
  if (m_uExtend.size() > 1) {
    m_uExtend.erase(m_uExtend.begin());
  }
  m_uModels.push_back(pExtended);
  return m_uModels.size();
}

namespace cflExtended
{
  // CLASS: AddState
 	
  class AddState: public IModel
  {
  public:
    AddState(const cfl::PathDependent & rState, const IModel & rModel, const Approx & rApprox);
		
    const std::vector<double> & eventTimes() const { return m_rModel.eventTimes(); }
		
    unsigned numberOfStates() const { return m_rModel.numberOfStates()+1; }
		
    std::valarray<double> origin() const;

    Slice state(unsigned iTime, unsigned iState) const;
		
    unsigned numberOfNodes(unsigned iTime,  const std::vector<unsigned> & rDependence) const;
		
    void addDependence(Slice & rSlice, const std::vector<unsigned> & rDependence) const;
		
    void rollback(Slice & rSlice, unsigned iTime) const;
		
    void indicator(Slice & rSlice, double dBarrier) const;
		
    MultiFunction interpolate(const Slice & rSlice) const;
		
  private:
    PathDependent m_uState;
    const IModel & m_rModel;
    std::vector<Approx> m_uVecApprox;
		
    const Approx & approxBefore(unsigned iTime) const 
    {
      unsigned iI= std::lower_bound(m_uState.timeIndexes().begin(), 
				    m_uState.timeIndexes().end(),iTime)-m_uState.timeIndexes().begin();
      ASSERT(iI < m_uVecApprox.size());
      return m_uVecApprox[iI];
    }

    void oneStepRollback(Slice & rSlice, unsigned iTime) const;
  };


	
  AddState::AddState(const cfl::PathDependent & rState, const IModel & rModel, 
		     const Approx & rApprox)
    :m_uState(rState), m_rModel(rModel) 
  {
    double dLeft = rState.origin() - rState.interval()/2.;
    double dRight = rState.origin() + rState.interval()/2.;
    m_uVecApprox.push_back(rApprox);
    m_uVecApprox.back().assign(dLeft,dRight);

		
    for (unsigned iI=0; iI<rState.timeIndexes().size(); iI++) {
      dLeft = std::numeric_limits<double>::max();
      dRight = std::numeric_limits<double>::min();
      for (unsigned iJ=0; iJ<m_uVecApprox.back().arg().size(); iJ++) {
	std::valarray<double> uV(rState.resetValues(rState.timeIndexes()[iI], 
						    m_uVecApprox.back().arg()[iJ]).values());
	dLeft = std::min(dLeft, uV.min()); 
	dRight = std::max(dRight, uV.max()); 
      }
      m_uVecApprox.push_back(rApprox);
      ASSERT(dLeft <= dRight);
      m_uVecApprox.back().assign(dLeft, dRight);
    }
		
    POSTCONDITION(m_uVecApprox.size() == rState.timeIndexes().size()+1);
  }
	
	
  Slice AddState::state(unsigned iTime, unsigned iState) const 
  {
		
    ASSERT(iTime < eventTimes().size());
    ASSERT(iState < numberOfStates());

    if (iState < m_rModel.numberOfStates()) { //old state
      Slice uState = m_rModel.state(iTime, iState);
      uState.assign(*this);
      return uState;
    }

    ASSERT(iState == m_rModel.numberOfStates());
    std::vector<unsigned> uDepend(0); //dependences of the state
    std::valarray<double> uValues(0); //values of the state
		
    if(std::binary_search(m_uState.timeIndexes().begin(),
			  m_uState.timeIndexes().end(), iTime)) { //event time is a reset time
      std::vector<Slice> uSlices;
      std::vector<unsigned> uTemp(numberOfStates());				
			
      for (unsigned iI=0; iI<approxBefore(iTime).arg().size(); iI++) {
	uSlices.push_back(m_uState.resetValues(iTime, approxBefore(iTime).arg()[iI]));
	std::vector<unsigned>::iterator itI = std::set_union(uDepend.begin(), 
							     uDepend.end(), uSlices.back().dependence().begin(), 
							     uSlices.back().dependence().end(), uTemp.begin());
	uDepend.assign(uTemp.begin(), itI);
      }
			
      ASSERT(uSlices.size() == approxBefore(iTime).arg().size());
      ASSERT((uDepend.size()==0) || (uDepend.back() < iState));
			
      unsigned iS0 = m_rModel.numberOfNodes(iTime, uDepend);
      unsigned iS1 = approxBefore(iTime).arg().size();
      unsigned iSize = iS0 * iS1;
      uValues.resize(iSize);
      for (unsigned iI=0; iI<iS1; iI++) {
	if  (uSlices[iI].values().size() < iS0) {
	  m_rModel.addDependence(uSlices[iI], uDepend);
	}
	ASSERT(uSlices[iI].values().size() == iS0);
	uValues[std::slice(iI*iS0,iS0,1)] = uSlices[iI].values();
      }
    }
    else {
      //event time is not a reset time
      ASSERT(uDepend.size() == 0);
      uValues.resize(approxBefore(iTime).arg().size());
      uValues = approxBefore(iTime).arg();
    }
    uDepend.push_back(iState);
    ASSERT(uValues.size() == numberOfNodes(iTime, uDepend));
    return Slice(*this, iTime, uDepend, uValues);
  }
	
  inline unsigned AddState::numberOfNodes(unsigned iTime,  const std::vector<unsigned> & rDependence) const 
  {
    std::vector<unsigned>::const_iterator itI = 
      std::lower_bound(rDependence.begin(), rDependence.end(), m_rModel.numberOfStates());
    int iN = m_rModel.numberOfNodes(iTime, std::vector<unsigned>(rDependence.begin(), itI));
    if (itI<rDependence.end()) {
      ASSERT(*itI == m_rModel.numberOfStates());
      iN *= approxBefore(iTime).arg().size();
    }
    return iN;
  }

  inline std::valarray<double> AddState::origin() const 
  {
    std::valarray<double> uOrigin(numberOfStates());
    uOrigin[std::slice(0, m_rModel.numberOfStates(),1)] = m_rModel.origin();
    uOrigin[uOrigin.size()-1] = m_uState.origin();
    return uOrigin;
  }	
	
  void AddState::addDependence(Slice & rSlice, 
			       const std::vector<unsigned> & rDependence) const 
  {
    PRECONDITION(rSlice.ptrToModel() == this);

    //easy cases
    if (std::includes(rSlice.dependence().begin(), rSlice.dependence().end(), 
		      rDependence.begin(), rDependence.end())) { //do nothing
      return;
    }
		
    std::vector<unsigned> uUnion(numberOfStates());
    unsigned iUnionSize = 
      std::set_union(rSlice.dependence().begin(), rSlice.dependence().end(), 
		     rDependence.begin(), rDependence.end(), uUnion.begin()) - uUnion.begin();
    uUnion.resize(iUnionSize);
    ASSERT(iUnionSize>0);

    if (rSlice.values().size() == numberOfNodes(rSlice.timeIndex(), uUnion)) {
      //artificial extension
      rSlice.assign(uUnion, rSlice.values());
      return;
    }

    if (uUnion.back() < m_rModel.numberOfStates()) {
      rSlice.assign(m_rModel);
      m_rModel.addDependence(rSlice, uUnion);
      rSlice.assign(*this);
      return;
    }

    //non-trivial case		
    ASSERT(uUnion.back() == m_rModel.numberOfStates());

    std::valarray<double> uValues(numberOfNodes(rSlice.timeIndex(), uUnion));

    std::vector<unsigned> uDepend(uUnion.begin(), uUnion.end()-1);
    unsigned iS0 = m_rModel.numberOfNodes(rSlice.timeIndex(), uDepend);

    unsigned iS1 = approxBefore(rSlice.timeIndex()).arg().size();
    ASSERT(iS1*iS0 == uValues.size());
		
    if ((rSlice.dependence().size() > 0) && (uUnion.back() == rSlice.dependence().back())) { 
      //rSlice depends on additional state
      std::vector<unsigned> uSliceDepend(rSlice.dependence().begin(), 
					 rSlice.dependence().end()-1);
      unsigned iS = m_rModel.numberOfNodes(rSlice.timeIndex(), uSliceDepend);
      ASSERT(iS*iS1 == rSlice.values().size());
      Slice uSlice(&m_rModel, rSlice.timeIndex(), 0.);
      for (unsigned iI=0; iI<iS1; iI++) {
	uSlice.assign(uSliceDepend, std::valarray<double>(rSlice.values()[std::slice(iI*iS,iS,1)]));
	m_rModel.addDependence(uSlice, uDepend);
	uValues[std::slice(iI*iS0,iS0,1)] = uSlice.values();
      }
    }
    else { //rSlice does not depend on an additional state
      ASSERT((rSlice.dependence().size() == 0) || 
	     (uUnion.back() > rSlice.dependence().back()));
      rSlice.assign(m_rModel);
      m_rModel.addDependence(rSlice, uDepend);
      ASSERT(rSlice.values().size() == iS0);
      for (unsigned iI=0; iI<iS1; iI++) {
	uValues[std::slice(iI*iS0, iS0, 1)] = rSlice.values();
      }
      rSlice.assign(*this);
    }
    rSlice.assign(uUnion, uValues);
    POSTCONDITION(rSlice.ptrToModel() == this);
    POSTCONDITION(rSlice.values().size() == 
		  numberOfNodes(rSlice.timeIndex(), rSlice.dependence()));
  }

  void AddState::rollback(Slice & rSlice, unsigned iTime) const 
  {
    ASSERT(rSlice.timeIndex() >= iTime);
    if (rSlice.timeIndex() == iTime) { return; }
		
    ASSERT(rSlice.timeIndex() > iTime);
    if ((rSlice.dependence().size() ==0)||(rSlice.dependence().back() < m_rModel.numberOfStates())) {
      rSlice.assign(m_rModel);
      rSlice.rollback(iTime);
      rSlice.assign(*this);
      return;
    }

    ASSERT(rSlice.dependence().back() == m_rModel.numberOfStates());

    std::vector<unsigned>::const_iterator itNextResetTime = 
      std::lower_bound(m_uState.timeIndexes().begin(), 
		       m_uState.timeIndexes().end(), rSlice.timeIndex());
    std::vector<unsigned>::const_iterator itStopResetTime =
      std::lower_bound(m_uState.timeIndexes().begin(), 
		       m_uState.timeIndexes().end(), iTime);

    while ((itNextResetTime > m_uState.timeIndexes().begin()) && (itNextResetTime > itStopResetTime) ){
      itNextResetTime--;
      oneStepRollback(rSlice, *itNextResetTime);
    }
   
    oneStepRollback(rSlice, iTime);
  }


  void AddState::oneStepRollback(Slice & rSlice, unsigned iTime) const 
  {
    if (rSlice.timeIndex() == iTime) {
      return;
    }
		
    ASSERT(rSlice.dependence().back() == m_rModel.numberOfStates());
    ASSERT(rSlice.timeIndex() > iTime);
		
    std::vector<unsigned> uDependBegin(rSlice.dependence().begin(), 
				       rSlice.dependence().end()-1);
    unsigned iS0 = m_rModel.numberOfNodes(rSlice.timeIndex(), uDependBegin);
    unsigned iS1 = approxBefore(rSlice.timeIndex()).arg().size();
    ASSERT(iS0*iS1 == rSlice.values().size());
		
    std::valarray<double> uCondVal(rSlice.values()[std::slice(0,iS0,1)]);
    Slice uSlice(m_rModel,rSlice.timeIndex(), uDependBegin, uCondVal);
    uSlice.rollback(iTime);
    unsigned iS2 = uSlice.values().size();
    std::vector<unsigned> uDependEnd(uSlice.dependence());
    ASSERT(iS2 == m_rModel.numberOfNodes(iTime, uDependEnd));
    std::valarray<double> uValues(iS2*iS1);
    uValues[std::slice(0,iS2,1)] = uSlice.values();
		
    for (unsigned iI=1; iI<iS1; iI++) {
      ASSERT(uCondVal.size() == iS0);
      uCondVal = rSlice.values()[std::slice(iI*iS0,iS0,1)];
      uSlice.assign(rSlice.timeIndex(), uDependBegin, uCondVal);
      uSlice.rollback(iTime);
      ASSERT(uSlice.values().size() == iS2);
      uValues[std::slice(iI*iS2, iS2, 1)] = uSlice.values();
    }
		
    if (std::binary_search(m_uState.timeIndexes().begin(), 
			   m_uState.timeIndexes().end(), iTime)==false) {
      //iTime is not a reset time
      uDependEnd.push_back(m_rModel.numberOfStates());
      ASSERT(uValues.size() == numberOfNodes(iTime, uDependEnd));
      rSlice.assign(iTime, uDependEnd, uValues);
      return;
    }

    //iTime is a reset time
    Slice uState = state(iTime, m_rModel.numberOfStates());
    addDependence(uState, uDependEnd);
    std::vector<unsigned> uD(uDependEnd);
    uDependEnd = uState.dependence();
    if ((uDependEnd.size()>0) && (uDependEnd.back() == m_rModel.numberOfStates())) {
      uDependEnd.pop_back();
    }
    ASSERT(uD.size() <= uDependEnd.size());
    ASSERT(std::includes(uDependEnd.begin(), uDependEnd.end(), uD.begin(), uD.end()));
    if (uD.size() < uDependEnd.size()) {
      unsigned iS = iS2; 
      iS2 = m_rModel.numberOfNodes(iTime, uDependEnd);
      std::valarray<double> uV(uValues);
      uValues.resize(iS2*iS1);
      ASSERT(uSlice.timeIndex() == iTime);
      for (unsigned iI=0; iI<iS1; iI++) {
	uSlice.assign(uD, std::valarray<double>(uV[std::slice(iI*iS,iS,1)]));
	m_rModel.addDependence(uSlice, uDependEnd);
	uValues[std::slice(iI*iS2,iS2,1)] = uSlice.values();
      }
    }

    const Approx & rApprox = approxBefore(rSlice.timeIndex());
    unsigned iS3 = approxBefore(iTime).arg().size();
    ASSERT(iS3*iS2 == numberOfNodes(iTime, uState.dependence()));
    std::valarray<double> uV(uState.values().size());
    unsigned iJ;
    for (unsigned iI=0; iI<iS2; iI++) {
      ASSERT(uValues.size() == iS1*iS2);
      Function uApprox = 
	rApprox.approximate(std::valarray<double>(uValues[std::slice(iI,iS1,iS2)]));
      for (iJ=iI; iJ<uV.size(); iJ+=iS2) {
	uV[iJ] = uApprox(uState.values()[iJ]);
      }
    }
    rSlice.assign(iTime, uState.dependence(), uV);
  }

  void AddState::indicator(Slice & rSlice, double dBarrier) const 
  {
    if ((rSlice.dependence().size() ==0)||
	(rSlice.dependence().back() < m_rModel.numberOfStates())) {
      rSlice.assign(m_rModel);
      m_rModel.indicator(rSlice, dBarrier);
      rSlice.assign(*this);
    }
    else{			
      ASSERT(rSlice.dependence().back() == m_rModel.numberOfStates());
      std::valarray<double> uValues(rSlice.values().size());
      std::vector<unsigned> uDep(rSlice.dependence().begin(), rSlice.dependence().end()-1);
      unsigned iS0 = numberOfNodes(rSlice.timeIndex(), uDep);
      unsigned iS1 = approxBefore(rSlice.timeIndex()).arg().size();
      ASSERT(rSlice.values().size()==iS0*iS1);

      std::valarray<double> uVal(iS0);
      Slice uS1(m_rModel, rSlice.timeIndex(), uDep, uVal);
			
      for(unsigned iI=0; iI<iS1; iI++) {
	uVal = rSlice.values()[std::slice(iI*iS0,iS0,1)];
	uS1.assign(uVal); 
	m_rModel.indicator(uS1,dBarrier);	
	uValues[std::slice(iI*iS0,iS0,1)] = uS1.values();
      }
      rSlice.assign(uValues);
    }
  }

  class MFunc: public IMultiFunction
  {
  public:
    MFunc(const Approx & rApprox, const std::vector<MultiFunction> & rVecFunc)
      :m_uApprox(rApprox), m_uVecFunc(rVecFunc), 
       m_uBelong(rApprox.approximate(rApprox.arg()))
    {
      PRECONDITION(rVecFunc.size() == rApprox.arg().size());
    }

    bool belongs(const std::valarray<double> & rX) const 
    {
      PRECONDITION(rX.size() == dim());
      double dY = rX[rX.size()-1];
      bool bResult = m_uBelong.belongs(dY);

      if (bResult) {
	std::valarray<double> uV(rX[std::slice(0,rX.size()-1,1)]);
	bResult = m_uVecFunc.front().belongs(uV);
      }
      return bResult;
    }

    unsigned dim() const 
    {
      if (m_uVecFunc.size()==0) { return 1;}
      return m_uVecFunc.front().dim()+1;
    }

    double operator()(const std::valarray<double> & rX) const 
    {
      PRECONDITION(rX.size() == dim());
      std::valarray<double> uY(rX[std::slice(0, rX.size()-1,1)]);
      std::valarray<double> uV(m_uVecFunc.size());
      for (unsigned iI=0; iI<uV.size(); iI++) {
	uV[iI] = m_uVecFunc[iI](uY);
      }

      return m_uApprox.approximate(uV)(rX[rX.size()-1]);
    }

  private:
    Approx m_uApprox;
    std::vector<MultiFunction> m_uVecFunc;
    Function m_uBelong;
  };


  MultiFunction AddState::interpolate(const Slice & rSlice) const
  {
    PRECONDITION(rSlice.ptrToModel() == this);
    if ((rSlice.dependence().size() == 0) || 
	(rSlice.dependence().back() < m_rModel.numberOfStates())) {
      Slice uSlice(m_rModel, rSlice.timeIndex(), rSlice.dependence(), rSlice.values());
      return m_rModel.interpolate(uSlice);
    }
    ASSERT(rSlice.dependence().back() == m_rModel.numberOfStates());
    const Approx & rApprox = approxBefore(rSlice.timeIndex());

    if (rSlice.dependence().size() == 1) {
      ASSERT(rSlice.values().size() == rApprox.arg().size());
      return toMultiFunction(rApprox.approximate(rSlice.values()), 0, 1);
    }

    ASSERT(rSlice.dependence().size() > 1);

    std::vector<unsigned> uDep(rSlice.dependence().begin(), rSlice.dependence().end()-1);
    unsigned iS0 = numberOfNodes(rSlice.timeIndex(), uDep);
    unsigned iS1 = rApprox.arg().size();
    ASSERT(iS0*iS1 == rSlice.values().size());
    std::vector<MultiFunction> uVecFunc;
    for (unsigned iI=0; iI<iS1; iI++) {
      std::valarray<double> uVal(rSlice.values()[std::slice(iI*iS0,iS0,1)]);
      uVecFunc.push_back(m_rModel.interpolate(Slice(m_rModel,rSlice.timeIndex(),uDep,uVal)));
    }
    return MultiFunction(new MFunc(rApprox,uVecFunc));
  }

  class Extend: public IExtend
  {
  public:
    Extend(const Approx & rApprox)
      :m_uApprox(rApprox)
    {}
    IModel * newModel(const PathDependent & rState, const IModel & rModel) const 
    {
      return new AddState(rState, rModel, m_uApprox);
    }
  private:
    Approx m_uApprox;
  };
}

cfl::Extended cfl::NExtended::model(const std::vector<Approx> & rApprox) 
{
  PRECONDITION(rApprox.size()>0);
  std::vector<std::shared_ptr<IExtend> > uExtend(0);
  for (unsigned iI=0; iI<rApprox.size(); iI++) {
    uExtend.push_back(std::shared_ptr<IExtend>(new cflExtended::Extend(rApprox[iI])));
  }
  return Extended(uExtend);
}

namespace cflExtended
{
  //CLASS: Size 

  class Size: public IFunction
  {
  public:
    Size(double dQuality) 
    {
      m_dQuality = (dQuality>3) ? dQuality: 3;
    }

    double operator()(double dInterval) const {
      PRECONDITION(dInterval >= 0);
      if (dInterval == 0) {
	return 1.;
      }
      else {
	double dSize = 
	  ::ceil(2.5 + (1 + std::sqrt(m_dQuality))*dInterval/(1.+dInterval));
	POSTCONDITION(dSize>=3);
	return dSize;
      }
    }

    bool belongs(double dInterval) const 
    {
      return (dInterval >= 0);
    }

  private:
    double m_dQuality;
  };
}

cfl::Extended cfl::NExtended::model(const std::vector<double> & rQuality)
{
  std::vector<Approx> uApprox(0);
  Interp uSpline = NInterp::spline();
  
  for (unsigned iI=0; iI<rQuality.size(); iI++) {
    Function uSize(new cflExtended::Size(rQuality[iI]));
    uApprox.push_back(cfl::NApprox::toApprox(uSize, uSpline));
  }

  return model(uApprox);
}

