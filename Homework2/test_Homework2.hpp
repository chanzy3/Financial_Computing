#ifndef __test_Homework2_hpp__
#define __test_Homework2_hpp__

#include "cfl/AssetModel.hpp"

namespace prb
{
  namespace test
  {
    cfl::MultiFunction americanCallOnForward(cfl::AssetModel & rModel);
    cfl::MultiFunction downAndRebate(cfl::AssetModel & rModel);
    cfl::MultiFunction straddle(cfl::AssetModel & rModel);
    cfl::MultiFunction upAndInAmericanPut(cfl::AssetModel & rModel);
  }
}


#endif // of __test_Homework2_hpp__
