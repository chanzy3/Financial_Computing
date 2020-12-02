#ifndef __testMain_hpp__
#define __testMain_hpp__

#include <iostream>
#include <fstream>
#include <valarray>
#include <vector>
#include "cfl/Auxiliary.hpp"
#include "cfl/Brownian.hpp"
#include "cfl/Data.hpp"
#include "cfl/BlackData.hpp"
#include "cfl/HullWhiteData.hpp"

/////////////////////////////////////////////////////////////////////////////////
//
// FUNCTIONS: collection of useful functions for testing
//
/////////////////////////////////////////////////////////////////////////////////

namespace cfl
{
  namespace test
  {
    void pause();

    template <class T>
    void print(T start, T end);

    void print(const char *);

    void printAtStart(const char *pProjectName);

    void printAtEnd(const char *pFileName);

    void print(const cfl::Slice &);

    void print(const cfl::Data::CashFlow &);

    void print(const cfl::Data::Swap &);

    void compare(const cfl::Slice &rExact, const cfl::Slice &rApprox);

    void analyse(const cfl::Slice &);

    void print(const cfl::Function &, unsigned iPoints, double dLeft, double dRight);

    std::valarray<double> getArg(double dL, double dR, unsigned iN);

    std::valarray<double> getRandArg(double dL, double dR, unsigned iN);

    std::valarray<double> getValues(const cfl::Function &rF,
				    const std::valarray<double> &rArg);

    void checkInterp(const cfl::Function &rApprox, const cfl::Function &rExact,
		     const std::valarray<double> &rX);

    const double c_dOnePercent = 0.01;

    void printRisk(const cfl::Function &rOption, double dShift = c_dOnePercent);

    std::vector<double> times(double dInitialTime, double dMaturity, unsigned iTimes);

    void project(void (*f)(const cfl::Black::Data &rBlackData, const cfl::HullWhite::Data &rHullWhiteData),
		 const char *pProject, const char *pFile);

    std::string fileName(const char *pDir, const char *pProject, const char *pFile);

  } // namespace test
} // namespace cfl

#include "test/Inline/iMain.hpp"
#endif // of __testMain_hpp__
