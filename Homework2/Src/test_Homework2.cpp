#include <iostream>
#include <fstream>
#include "test/Main.hpp"
#include "test/Black.hpp"
#include "Homework2/test_Homework2.hpp"
#include "Homework2/Homework2.hpp"

using namespace std;
using namespace prb;
using namespace cfl;
using namespace cfl::test;
using namespace cfl::test::Black;

namespace test_prb
{
  void standard(const cfl::Black::Data & rData);
}

int main()
{
  const char * c_pProject = "Homework2";
  using namespace cfl::test;
  using namespace std;
  try{
    cfl::test::Black::project(test_prb::standard, c_pProject);
    cfl::test::pause();
  }
  catch(const std::exception & rError) {
    std::cerr<< rError.what() << endl;
    cfl::test::pause();
  }
}

void test_prb::standard(const cfl::Black::Data & rData)
{
  double dQuality = cfl::test::Black::c_dQuality;
  cout << "quality = " << dQuality << endl;
  double dSpot = rData.forward()(rData.initialTime());
  double dInterval = cfl::test::Black::c_dInterval;
  unsigned iPoints = cfl::test::Black::c_iPoints;

  AssetModel uModel = cfl::Black::model(rData, dInterval, dQuality);

  using namespace prb::test; 

  report(straddle, dSpot, dInterval, iPoints, uModel);  
  report(americanCallOnForward, dSpot, dInterval, iPoints, uModel);
  report(downAndRebate, dSpot, dInterval, iPoints, uModel);
  report(upAndInAmericanPut, dSpot, dInterval, iPoints, uModel);
}

MultiFunction 
prb::test::americanCallOnForward(AssetModel & rModel) 
{
  cfl::test::print("AMERICAN CALL ON FORWARD IN ASSET MODEL");
  cout << endl;
  double dForwardPrice = cfl::test::Black::c_dStrike;
  double dTimeToMaturity = 0.5;
  const std::vector<double> uExerciseTimes = 
    cfl::test::Black::c_uExerciseTimes;
  cout << "forward price = " << dForwardPrice << endl;
  cout << "time to maturity" << dTimeToMaturity << endl;
  cout << "exercise times = "<< endl;
  cfl::test::print(uExerciseTimes.begin(), uExerciseTimes.end());
  return prb::americanCallOnForward(dForwardPrice, dTimeToMaturity,
				    uExerciseTimes, rModel);
}

MultiFunction prb::test::straddle(AssetModel & rModel) 
{
  cfl::test::print("EUROPEAN STRADDLE OPTION IN ASSET MODEL");
  cout << endl;
  double dStrike = cfl::test::Black::c_dStrike;
  double dMaturity = cfl::test::Black::c_dMaturity;
  cout << "strike = " << dStrike << endl;
  cout << "maturity = " << dMaturity << endl;
  return prb::straddle(dStrike, dMaturity, rModel);
}

MultiFunction prb::test::upAndInAmericanPut(AssetModel & rModel) 
{
  cfl::test::print("UP-AND-IN AMERICAN PUT OPTION IN ASSET MODEL");
  cout << endl;
  double dStrike = cfl::test::Black::c_dStrike;
  const std::vector<double> uExerciseTimes = 
    cfl::test::Black::c_uExerciseTimes;
  double dUpperBarrier = cfl::test::Black::c_dUpperBarrier;
  const std::vector<double> uBarrierTimes = 
    cfl::test::Black::c_uBarrierTimes;

  cout << "strike = " << dStrike << endl;
  cout << "exericse times = "<< endl;
  cfl::test::print(uExerciseTimes.begin(), uExerciseTimes.end());
  cout << "upper barrier = " << dUpperBarrier << endl;
  cout << "barrier times = "<< endl;
  cfl::test::print(uBarrierTimes.begin(), uBarrierTimes.end());
	
  return prb::upAndInAmericanPut(dUpperBarrier, uBarrierTimes, 
				 dStrike, uExerciseTimes, rModel);
}

MultiFunction prb::test::downAndRebate(AssetModel & rModel) 
{
  cfl::test::print("DOWN-AND-REBATE OPTION IN ASSET MODEL");
  cout << endl;
  double dLowerBarrier = cfl::test::Black::c_dLowerBarrier;
  double dNotional = cfl::test::Black::c_dNotional;
  const std::vector<double> uBarrierTimes = 
    cfl::test::Black::c_uBarrierTimes;
  cout << "lower barrier = " << dLowerBarrier << endl;
  cout << "notional = " << dNotional << endl;
  cout << "barrier times = "<< endl;
  cfl::test::print(uBarrierTimes.begin(), uBarrierTimes.end());
  return prb::downAndRebate(dLowerBarrier, 
			    dNotional, uBarrierTimes, rModel);
}
