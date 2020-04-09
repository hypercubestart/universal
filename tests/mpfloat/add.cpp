// add.cpp: functional tests for addition on multi-precison linear floating point
//
// Copyright (C) 2017-2020 Stillwater Supercomputing, Inc.
//
// This file is part of the universal numbers project, which is released under an MIT Open Source license.
#include <limits>
// minimum set of include files to reflect source code dependencies
#include <universal/mpfloat/mpfloat.hpp>
// test helpers, such as, ReportTestResults
#include "../utils/test_helpers.hpp"
//#include "mpfloat_test_helpers.hpp"

// generate specific test case that you can trace with the trace conditions in areal.hpp
// for most bugs they are traceable with _trace_conversion and _trace_add
template<typename Ty>
void GenerateTestCase(Ty a, Ty b) {
	Ty ref;
	sw::unum::mpfloat mpa, mpb, mpref, mpsum;
	mpa = a;
	mpb = b;
	ref = a + b;
	mpref = ref;
	mpsum = mpa + mpb;
	constexpr size_t ndigits = std::numeric_limits<Ty>::digits10;
	std::cout << std::setprecision(ndigits);
	std::cout << std::setw(ndigits) << a << " + " << std::setw(ndigits) << b << " = " << std::setw(ndigits) << ref << std::endl;
	std::cout << mpa << " + " << mpb << " = " << mpsum << " (reference: " << mpref << ")   " ;
	std::cout << (mpref == mpsum ? "PASS" : "FAIL") << std::endl << std::endl;
	std::cout << std::setprecision(5);
}

#define MANUAL_TESTING 1
#define STRESS_TESTING 0

int main(int argc, char** argv)
try {
	using namespace std;
	using namespace sw::unum;

	int nrOfFailedTestCases = 0;

	std::string tag = "multi-precision float addition failed: ";

#if MANUAL_TESTING
//	bool bReportIndividualTestCases = false;

	// generate individual testcases to hand trace/debug
	GenerateTestCase(INFINITY, INFINITY);

	mpfloat mpa;
	cout << mpa << endl;

#else

	cout << "multi-precision float addition validation" << endl;


#if STRESS_TESTING

#endif  // STRESS_TESTING

#endif  // MANUAL_TESTING

	return (nrOfFailedTestCases > 0 ? EXIT_FAILURE : EXIT_SUCCESS);
}
catch (char const* msg) {
	std::cerr << msg << std::endl;
	return EXIT_FAILURE;
}
catch (const std::runtime_error& err) {
	std::cerr << "Uncaught runtime exception: " << err.what() << std::endl;
	return EXIT_FAILURE;
}
catch (...) {
	std::cerr << "Caught unknown exception" << std::endl;
	return EXIT_FAILURE;
}