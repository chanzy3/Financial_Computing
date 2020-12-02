#ifndef __testBlack_h__
#define __testBlack_h__

#include <iostream>
#include <valarray>
#include <vector>
#include "test/Main.hpp"
#include "cfl/BlackModel.hpp"


/////////////////////////////////////////////////////////////////////////////////
//
// FUNCTIONS: useful functions for the testing of Black model. 
//
//////////////////////////////////////////////////////////////////////////////////

namespace cfl
{
  namespace test
  {
    namespace Black
    {		
      using namespace cfl;
      using namespace std;
		
      //default values for the parameters of the model
      const double c_dYield = 0.07;
      const double c_dSpot = 100;
      const double c_dDividendYield = 0.02;
      const double c_dBlackSigma = 0.2;
      const double c_dLambda = 0.05;
      const double c_dInitialTime = 0;
		
      //returns parameters of the extended Black model
      cfl::Black::Data data(double dYield, //interest rate
			    double dSpot, //spot price
			    double dDividendYield, //dividend yield
			    double dSigma, //volatility
			    double dLambda, //mean-reversion coefficient
			    double dInitialTime //initial time
			    );

      cfl::Black::Data data();
		
      //collection of parameters for options. 
      const double c_dMaturity = c_dInitialTime + 0.5;
      const double c_dIssueTime = c_dInitialTime + 0.05;
      const double c_dStrike = c_dSpot;
      const double c_dUpperBarrier = c_dSpot*1.1;
      const double c_dLowerBarrier = c_dSpot*0.9;
      const double c_dNotional = 100.; 
      const double c_dExerciseMaturity = c_dMaturity;
      const double c_dBarrierMaturity = c_dMaturity - 0.1;
      const int c_iExerciseTimes = 12;
      const int c_iBarrierTimes = 10;
      const std::vector<double> c_uExerciseTimes = 
	test::times(c_dInitialTime, c_dExerciseMaturity, c_iExerciseTimes);
      const std::vector<double> c_uBarrierTimes = 
	test::times(c_dInitialTime, c_dBarrierMaturity, c_iBarrierTimes);
		
      //collection of parameters for output
      const double c_dQuality = 200;
      const double c_dPathDependQuality = 200;
      const double c_dInterval = 0.2;
      const unsigned c_iPoints = 10;
		
      //prints values of a derivative security in Black model versus spot price.  
      void print(const cfl::Function & rOption, double dSpot = c_dSpot, 
		 double dInterval = c_dInterval, unsigned iPoints = c_iPoints);

      void print(const cfl::MultiFunction & rOption, double dSpot = c_dSpot, 
		 double dInterval = c_dInterval, unsigned iPoints =
		 c_iPoints);

      void report(MultiFunction (*f)(AssetModel & rModel), 
		  double dSpot, double dInterval, unsigned iPoints,
		  AssetModel & rModel);

      void report(MultiFunction (*f)(AssetModel & rModel, bool bPayFloat), 
		  double dSpot, double dInterval, unsigned iPoints,
		  AssetModel & rModel);

      void project(const cfl::Black::Data & rData, void (*f)(const cfl::Black::Data & rData), 
		   const char * pProject, const char * pFile);
      
      void project(void (*f)(const cfl::Black::Data & rData), 
		   const char * pProject, const char * pFile);

      void project(void (*f)(const cfl::Black::Data & rData), const char * pProject);
      
      void projectPath(void (*f)(const cfl::Black::Data & rData), const char * pProject);
    }
  }
}


#include "test/Inline/iBlack.hpp"
#endif // of __testBlack_h__
