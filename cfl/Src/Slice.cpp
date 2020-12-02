//  Copyright (c) Dmitry Kramkov, 2000-2006. All rights reserved. 
// Implementation of classes and functions declared in the corresponding *.hpp file. 

#include <cmath>
#include <algorithm>
#include "cfl/Slice.hpp"
#include "cfl/Error.hpp"
#include "cfl/Auxiliary.hpp"

using namespace cfl;

cfl::Slice::Slice(const IModel * pModel, unsigned iTime, double dValue)
  :m_pModel(pModel), m_iEventTime(iTime), m_uValues(dValue,1)
{}

cfl::Slice::Slice(const cfl::IModel & rModel, unsigned iTime, 
		  const std::vector<unsigned> & rDependence, 
		  const std::valarray<double> & rValues)
  :m_pModel(&rModel), m_iEventTime(iTime),  m_uDependence(rDependence), 
   m_uValues(rValues) 
{
  POSTCONDITION(rValues.size() == m_pModel->numberOfNodes(iTime, rDependence));
}


namespace cflSlice
{
  void plus(std::valarray<double> & rA, const std::valarray<double> & rB) 
  {
    PRECONDITION(rA.size() == rB.size());
    rA+=rB;
  }
  void minus(std::valarray<double> & rA, const std::valarray<double> & rB) 
  {
    PRECONDITION(rA.size() == rB.size());
    rA-=rB;
  }
  void divides(std::valarray<double> & rA, const std::valarray<double> & rB) 
  {
    PRECONDITION(rA.size() == rB.size());
    rA/=rB;
  }
  void multiplies(std::valarray<double> & rA, const std::valarray<double> & rB) 
  {
    PRECONDITION(rA.size() == rB.size());
    rA*=rB;
  }
}

Slice & cfl::Slice::operator+=(const Slice & rSlice)
{
  PRECONDITION(m_pModel == rSlice.ptrToModel());
  PRECONDITION(timeIndex() == rSlice.timeIndex());

  if (rSlice.values().size()==1) {
    return operator+=(rSlice.values()[0]);
  }
  return apply(rSlice, cflSlice::plus);
}

Slice & cfl::Slice::operator-=(const Slice & rSlice)
{
  PRECONDITION(m_pModel == rSlice.ptrToModel());
  PRECONDITION(timeIndex() == rSlice.timeIndex());

  if (rSlice.values().size()==1) {
    return operator-=(rSlice.values()[0]);
  }
  return apply(rSlice, cflSlice::minus);
}

Slice & cfl::Slice::operator*=(const Slice & rSlice)
{
  PRECONDITION(m_pModel == rSlice.ptrToModel());
  PRECONDITION(timeIndex() == rSlice.timeIndex());

  if (rSlice.values().size()==1) {
    return operator*=(rSlice.values()[0]);
  }
  return apply(rSlice, cflSlice::multiplies);
}

Slice & cfl::Slice::operator/=(const Slice & rSlice)
{
  PRECONDITION(m_pModel == rSlice.ptrToModel());
  PRECONDITION(timeIndex() == rSlice.timeIndex());

  if (rSlice.values().size()==1) {
    return operator/=(rSlice.values()[0]);
  }
  return apply(rSlice, cflSlice::divides);
}

Slice & cfl::Slice::
apply(const Slice & rSlice, 
      void (*func)(std::valarray<double> & , const std::valarray<double> & ))
{
  PRECONDITION(m_pModel == rSlice.ptrToModel());
  PRECONDITION(timeIndex() == rSlice.timeIndex());
	
  if ((dependence().size() == rSlice.dependence().size()) && 
      (std::equal(dependence().begin(), dependence().end(), 
		  rSlice.dependence().begin()))) {
    func(m_uValues, rSlice.values());
  }
  else if ((dependence().size() > rSlice.dependence().size()) && 
	   (std::includes(dependence().begin(), dependence().end(), 
			  rSlice.dependence().begin(), rSlice.dependence().end()))) {
    Slice uSlice(rSlice);
    m_pModel->addDependence(uSlice, dependence());
    func(m_uValues, uSlice.values());
  }
  else if ((rSlice.dependence().size() > dependence().size()) && 
	   (std::includes(rSlice.dependence().begin(), rSlice.dependence().end(), 
			  dependence().begin(), dependence().end()))) {
    m_pModel->addDependence(*this, rSlice.dependence());
    func(m_uValues, rSlice.values());
  }
  else {
    m_pModel->addDependence(*this, rSlice.dependence());
    Slice uSlice(rSlice);
    m_pModel->addDependence(uSlice, dependence());
    func(m_uValues, uSlice.values());
  }
	
  return *this;
}

namespace cflSlice
{
  std::valarray<double> max(const std::valarray<double> & rA, double dV) 
  {
    std::valarray<double> uOut(rA.size());
    std::transform(cfl::begin(rA), cfl::end(rA), &uOut[0], 
		   [dV](double dX){return std::max(dX,dV);});
    return uOut;
  }

  std::valarray<double> min(const std::valarray<double> & rA, double dV) 
  {
    std::valarray<double> uOut(rA.size());
    std::transform(cfl::begin(rA), cfl::end(rA), &uOut[0],
		   [dV](double dX){return std::min(dX,dV);});
    return uOut;
  }

  std::valarray<double> max(const std::valarray<double> & rA, const std::valarray<double> & rB) 
  {
    PRECONDITION(rA.size() == rB.size());
    std::valarray<double> uOut(rA.size());
    std::transform(cfl::begin(rA), cfl::end(rA), cfl::begin(rB), &uOut[0], 
		   [](double dX, double dY){return std::max(dX,dY);});
    return uOut; 
  }

  std::valarray<double> min(const std::valarray<double> & rA, const std::valarray<double> & rB) 
  {
    PRECONDITION(rA.size() == rB.size()); 
    std::valarray<double> uOut(rA.size());
    std::transform(cfl::begin(rA), cfl::end(rA), cfl::begin(rB), &uOut[0], 
		   [](double dX, double dY){return std::min(dX,dY);}); 
    return uOut; 
  }

  inline Slice 
  apply(const Slice & rA, const Slice rB, 
	std::valarray<double> (*func)(const std::valarray<double> &, const std::valarray<double> & )) 
  {
    PRECONDITION(rA.ptrToModel() == rB.ptrToModel());
    PRECONDITION(rA.timeIndex() == rB.timeIndex());
		
    const IModel & rModel = *rA.ptrToModel();
    unsigned iTime = rA.timeIndex();
		
    if ((rA.dependence().size() == rB.dependence().size()) && 
	(std::equal(rA.dependence().begin(), rA.dependence().end(), 
		    rB.dependence().begin()))) {
      return Slice(rModel, iTime, rA.dependence(),func(rA.values(), rB.values())); 
    }
    else if ((rA.dependence().size() > rB.dependence().size()) && 
	     (std::includes(rA.dependence().begin(), rA.dependence().end(), 
			    rB.dependence().begin(), rB.dependence().end()))) {
      Slice uB(rB);
      rModel.addDependence(uB, rA.dependence());
      return Slice(rModel, iTime, rA.dependence(),func(rA.values(), uB.values())); 
    }
    else if ((rB.dependence().size() > rA.dependence().size()) && 
	     (std::includes(rB.dependence().begin(), rB.dependence().end(), 
			    rA.dependence().begin(), rA.dependence().end()))) {
      Slice uA(rA);
      rModel.addDependence(uA, rB.dependence());
      return Slice(rModel, iTime, rB.dependence(), func(uA.values(),rB.values()));
    }
    else {
      Slice uA(rA);
      rModel.addDependence(uA, rB.dependence());
      Slice uB(rB); 
      rModel.addDependence(uB, rA.dependence());
      return Slice(rModel, iTime, uA.dependence(), func(uA.values(),uB.values()));
    }
  }
}

Slice cfl::max(const Slice & rSlice, double dValue)
{
  std::valarray<double> uValues = cflSlice::max(rSlice.values(), dValue);
  return Slice(*rSlice.ptrToModel(),rSlice.timeIndex(),rSlice.dependence(), uValues);
}

Slice cfl::min(const Slice & rSlice, double dValue)
{
  std::valarray<double> uValues = cflSlice::min(rSlice.values(), dValue);
  return Slice(*rSlice.ptrToModel(),rSlice.timeIndex(),rSlice.dependence(), uValues);
}

Slice cfl::max(const Slice & rSlice1, const Slice & rSlice2) {
  return cflSlice::apply(rSlice1, rSlice2, cflSlice::max);
}

Slice cfl::min(const Slice & rSlice1, const Slice & rSlice2) 
{
  return cflSlice::apply(rSlice1, rSlice2, cflSlice::min);
}

MultiFunction cfl::interpolate(const Slice & rSlice, 
			       const std::vector<unsigned> & rState) 
{
  PRECONDITION(rSlice.ptrToModel()->numberOfStates() >= rSlice.dependence().size());
  PRECONDITION(std::equal(rState.begin(), rState.end()-1, rState.begin()+1, std::less<unsigned>()));
  PRECONDITION(rState.size()>0);
  PRECONDITION(rState.back() < rSlice.ptrToModel()->numberOfStates());

  Slice uSlice(rSlice);
  rSlice.ptrToModel()->addDependence(uSlice, rState);
  ASSERT(std::includes(uSlice.dependence().begin(), uSlice.dependence().end(), 
		       rState.begin(), rState.end()));

  MultiFunction uF = interpolate(uSlice);
  ASSERT(uF.dim() == uSlice.dependence().size());
  std::valarray<bool> uMask(false, uSlice.ptrToModel()->numberOfStates());
  for (unsigned iI=0; iI< uMask.size(); iI++) {
    if ( (std::binary_search(uSlice.dependence().begin(), 
			     uSlice.dependence().end(), iI)) && 
	 (!std::binary_search(rState.begin(), rState.end(), iI)) ) {
      uMask[iI] = true;
    }
  }
  std::valarray<double> uPoint(uSlice.ptrToModel()->origin()[uMask]);
  ASSERT(uF.dim() == rState.size() + uPoint.size());
  MultiFunction uResult = toMultiFunction(uF, rState, uPoint);
  POSTCONDITION(uResult.dim() == rState.size());
  return uResult;
}	

double cfl::atOrigin(const Slice & rSlice) 
{
  if (rSlice.dependence().size()==0) {
    return rSlice.values()[0];
  }
  std::valarray<bool> uMask(rSlice.ptrToModel()->numberOfStates());
  for (unsigned iI=0; iI<uMask.size(); iI++) {
    uMask[iI] = std::binary_search(rSlice.dependence().begin(), 
				   rSlice.dependence().end(), iI);
  }
  std::valarray<double> uPoint(rSlice.ptrToModel()->origin()[uMask]);
  ASSERT(uPoint.size() == rSlice.dependence().size());
  return interpolate(rSlice)(uPoint);
}
