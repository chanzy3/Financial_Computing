#ifndef __testHullWhite_h__
#define __testHullWhite_h__

#include <iostream>
#include <valarray>
#include <vector>
#include "test/Main.hpp"
#include "cfl/HullWhiteModel.hpp"
#include "cfl/Data.hpp"

/////////////////////////////////////////////////////////////////////////////////
//
// FUNCTIONS: useful functions for the testing of HullWhite model. 
//
//////////////////////////////////////////////////////////////////////////////////

namespace cfl
{
  namespace test
  {
    namespace HullWhite
    {		
      using namespace cfl;
      using namespace std;
		
      //default values for the parameters of the model
      const double c_dYield = 0.07;
      const double c_dHullWhiteSigma = 0.02;
      const double c_dLambda = 0.05;
      const double c_dInitialTime = 0;
		
      //returns parameters of the extended HullWhite model
      cfl::HullWhite::Data data(double dYield, //interest rate
				double dSigma, //volatility
				double dLambda, //mean-reversion coefficient
				double dInitialTime //initial time
				);

      cfl::HullWhite::Data data();

		
      //collection of parameters for options. 
      const double c_dMaturity = c_dInitialTime + 1.5;
      const double c_dExerciseMaturity = c_dMaturity;
      const double c_dBarrierMaturity = c_dMaturity - 0.1;
      const int c_iExerciseTimes = 12;
      const int c_iBarrierTimes = 10;
      const double c_dStrike = c_dYield;
      const double c_dUpperBarrier = c_dYield*1.1;
      const double c_dLowerBarrier = c_dYield*0.9;
      const double c_dUpperRate = c_dYield*1.1;
      const double c_dLowerRate = c_dYield*0.9;
      const double c_dCapRate = c_dUpperBarrier;
      const double c_dFloorRate = c_dLowerBarrier;
      const double c_dNotional = 1000.; 
      const double c_dPeriod = 0.25;
      const unsigned c_iNumberOfPeriods = 6;
      const double c_dSwapRate = c_dYield;
      const std::vector<double> c_uExerciseTimes = 
	test::times(c_dInitialTime, c_dExerciseMaturity, c_iExerciseTimes);
      const std::vector<double> c_uBarrierTimes = 
	test::times(c_dInitialTime, c_dBarrierMaturity, c_iBarrierTimes);

      cfl::Data::Swap swapParameters();
		
      //collection of parameters for output
      const double c_dQuality = 200;
      const double c_dPathDependQuality = 200;
      const double c_dInterval = 0.2;
      const unsigned c_iPoints = 10;
		
      //prints values of a derivative security in HullWhite model versus short rate.  
      void print(const cfl::Function & rOption, double dShortRate = c_dYield, 
		 double dInterval = c_dInterval, unsigned iPoints = c_iPoints);

      void print(const cfl::MultiFunction & rOption, double dShortRate = c_dYield, 
		 double dInterval = c_dInterval, unsigned iPoints =
		 c_iPoints);

      void report(MultiFunction (*f)(InterestRateModel & rModel), 
		  double dSpot, double dInterval, unsigned iPoints, InterestRateModel & rModel);

      void report(MultiFunction (*f)(InterestRateModel & rModel, bool bPayFloat), 
		  double dSpot, double dInterval, unsigned iPoints, InterestRateModel & rModel);

      void project(const cfl::HullWhite::Data & rData, void (*f)(const cfl::HullWhite::Data & rData), 
		   const char * pProject, const char * pFile);

      void project(void (*f)(const cfl::HullWhite::Data & rData), const char * pProject, const char * pFile);
      
      void project(void (*f)(const cfl::HullWhite::Data & rData), const char * pProject);

      void projectPath(void (*f)(const cfl::HullWhite::Data & rData), const char * pProject);
    }
  }
}
#include "test/Inline/iHullWhite.hpp"
#endif // of __testHullWhite_h__
