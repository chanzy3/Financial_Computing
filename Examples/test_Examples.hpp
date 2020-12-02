#ifndef __test_Examples_hpp__
#define __test_Examples_hpp__

#include "cfl/AssetModel.hpp"

namespace prb
{
  namespace test
  {
    void yieldShapeHullWhite();
    void discountLogLinearInterp();
    void discountFitHullWhite();

    cfl::MultiFunction put(cfl::AssetModel & rModel);
    cfl::MultiFunction americanPut(cfl::AssetModel & rModel);
    cfl::MultiFunction barrierUpOrDownAndOut(cfl::AssetModel & rModel);
    cfl::MultiFunction downAndOutAmericanCall(cfl::AssetModel & rModel);
    cfl::MultiFunction swing(cfl::AssetModel & rModel);

    cfl::MultiFunction cap(cfl::InterestRateModel & Model);
    cfl::MultiFunction swaption(cfl::InterestRateModel & Model, 
				bool bPayFloat);
    cfl::MultiFunction cancellableCollar(cfl::InterestRateModel & Model);
    cfl::MultiFunction downAndOutCap(cfl::InterestRateModel & Model);
    cfl::MultiFunction futureOnLibor(cfl::InterestRateModel & rModel);

    cfl::MultiFunction asianCall(cfl::AssetModel & rModel);
    cfl::MultiFunction barrierUpOrDownAndOut_path(cfl::AssetModel & rModel);
    cfl::MultiFunction savingsAccount(cfl::InterestRateModel & rModel);
    cfl::MultiFunction putOnSavingsAccount(cfl::InterestRateModel & rModel);
  }
}

#endif // of __test_Examples_hpp__
