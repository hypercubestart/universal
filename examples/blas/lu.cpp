// lu.cpp: example program comparing float vs posit Gaussian Elimination (LU Decomposition) equation solver
//
// Copyright (C) 2017-2020 Stillwater Supercomputing, Inc.
//
// This file is part of the HPRBLAS project, which is released under an MIT Open Source license.

#include <chrono>
//
// enable posit arithmetic exceptions
#define POSIT_THROW_ARITHMETIC_EXCEPTION 1
#include <universal/blas/blas.hpp>
#include <universal/functions/isrepresentable.hpp>

template<size_t nbits, size_t es, size_t capacity = 10>
void ComparePositDecompositions(sw::unum::blas::matrix< sw::unum::posit<nbits, es> >& A, sw::unum::blas::vector< sw::unum::posit<nbits, es> >& x, sw::unum::blas::vector< sw::unum::posit<nbits, es> >& b) {
	using namespace std;
	using namespace sw::unum;
	using namespace sw::unum::blas;
	assert(num_rows(A) == num_cols(A));

	size_t N = num_cols(A);
	matrix< posit<nbits, es> > LU(N,N);

	{
		using namespace std::chrono;
		steady_clock::time_point t1 = steady_clock::now();
		Crout(A, LU);
		steady_clock::time_point t2 = steady_clock::now();
		duration<double> time_span = duration_cast<duration<double>>(t2 - t1);
		double elapsed = time_span.count();
		std::cout << "Crout took " << elapsed << " seconds." << std::endl;
		std::cout << "Performance " << (uint32_t)(N*N*N / (1000 * elapsed)) << " KOPS/s" << std::endl;

		SolveCrout(LU, b, x);
		cout << "Crout LU\n" << LU << endl;
		cout << "Solution\n" << x << endl;
		cout << "RHS\n" << b << endl;
	}

	std::cout << std::endl;
}

template<size_t nbits, size_t es>
void GaussianEliminationTest(size_t N) {
	using namespace std;
	using namespace sw::unum;
	using namespace sw::unum::blas;
	using Scalar = sw::unum::posit<nbits, es>;
	using Vector = sw::unum::blas::vector<Scalar>;
	using Matrix = sw::unum::blas::matrix<Scalar>;
	cout << "Using " << dynamic_range<nbits, es>() << endl;

	// repeat set up for posits
	cout << "Posit inputs\n";
	Matrix U = {     // define the upper triangular matrix
		{ 1.0, 2.0, 3.0, 4.0, 5.0 },
		{ 0.0, 1.0, 2.0, 3.0, 4.0 },
		{ 0.0, 0.0, 1.0, 2.0, 3.0 },
		{ 0.0, 0.0, 0.0, 1.0, 2.0 },
		{ 0.0, 0.0, 0.0, 0.0, 1.0 },
	};
	Matrix L = {     // define the lower triangular matrix
		{ 1.0, 0.0, 0.0, 0.0, 0.0 },
		{ 2.0, 1.0, 0.0, 0.0, 0.0 },
		{ 3.0, 2.0, 1.0, 0.0, 0.0 },
		{ 4.0, 3.0, 2.0, 1.0, 0.0 },
		{ 5.0, 4.0, 3.0, 2.0, 1.0 },
	};
	Matrix A(N, N);
	A = L * U;   // construct the A matrix to solve
	cout << "L\n" << L << endl;
	cout << "U\n" << U << endl;
	cout << "A\n" << A << endl;
	// define a difficult solution
	Scalar epsplus = Scalar(1) + numeric_limits<Scalar>::epsilon();
	Vector x = {
		epsplus,
		epsplus,
		epsplus,
		epsplus,
		epsplus
	};
	Vector b(N);
	b = fmv(A, x);   // construct the right hand side
	cout << "b" << b << endl;
	cout << endl << ">>>>>>>>>>>>>>>>" << endl;
	cout << "LinearSolve fused-dot product" << endl;
	ComparePositDecompositions(A, x, b);
}

int main(int argc, char** argv)
try {
	using namespace std;
	using namespace sw::unum;
	using namespace sw::unum::blas;

	float eps = std::numeric_limits<float>::epsilon();
	float epsminus = 1.0f - eps;
	float epsplus  = 1.0f + eps;

	// We want to solve the system Ax=b
	GaussianEliminationTest<32, 2>(1);

#if 1
	cout << "posit<25,1>\n";
	cout << "1.0 - FLT_EPSILON = " << setprecision(17) << epsminus << " converts to " << posit<25, 1>(epsminus) << endl;
	cout << "1.0 + FLT_EPSILON = " << setprecision(17) << epsplus << " converts to " << posit<25, 1>(epsplus) << endl;
	cout << "posit<26,1>\n";
	cout << "1.0 - FLT_EPSILON = " << setprecision(17) << epsminus << " converts to " << posit<26, 1>(epsminus) << endl;
	cout << "1.0 + FLT_EPSILON = " << setprecision(17) << epsplus << " converts to " << posit<26, 1>(epsplus) << endl;
	cout << "posit<27,1>\n";
	cout << "1.0 - FLT_EPSILON = " << setprecision(17) << epsminus << " converts to " << posit<27, 1>(epsminus) << endl;
	cout << "1.0 + FLT_EPSILON = " << setprecision(17) << epsplus << " converts to " << posit<27, 1>(epsplus) << endl;
#endif

	return EXIT_SUCCESS;
}
catch (char const* msg) {
	std::cerr << msg << std::endl;
	return EXIT_FAILURE;
}
catch (const posit_arithmetic_exception& err) {
	std::cerr << "Uncaught posit arithmetic exception: " << err.what() << std::endl;
	return EXIT_FAILURE;
}
catch (const quire_exception& err) {
	std::cerr << "Uncaught quire exception: " << err.what() << std::endl;
	return EXIT_FAILURE;
}
catch (const posit_internal_exception& err) {
	std::cerr << "Uncaught posit internal exception: " << err.what() << std::endl;
	return EXIT_FAILURE;
}
catch (std::runtime_error& err) {
	std::cerr << err.what() << std::endl;
	return EXIT_FAILURE;
}
catch (...) {
	std::cerr << "Caught unknown exception" << std::endl;
	return EXIT_FAILURE;
}
