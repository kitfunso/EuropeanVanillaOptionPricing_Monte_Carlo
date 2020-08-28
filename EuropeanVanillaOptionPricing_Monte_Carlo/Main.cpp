#include <algorithm>
#include <cmath>
#include <iostream>
using namespace std;

//use box-muller algorithm to generate gaussian random numbers
// can also use std::normal_distribution<> in <random> library
double gaussian_box_muller() {
	double x = 0.0;
	double y = 0.0;
	double euclid_sq = 0.0;

	//continue generating 2 uniform rv
	//until the square of their "euclidean distance" is less than unity
	do {
		x = 2.0 * rand() / static_cast<double>(RAND_MAX) - 1;
		x = 2.0 * rand() / static_cast<double>(RAND_MAX) - 1;
		euclid_sq = x * x + y * y;
	} while (euclid_sq >= 1.0);
	return x * sqrt(-2 * log(euclid_sq) / euclid_sq);
}

double monte_carlo_call_price(const int& num_sims, const double& S, const double& K, const double& r, const double& v, const double& T) {
	double S_adjust = S * exp(T * (r - 0.5 * v * v));
	double S_cur = 0.0;
	double payoff_sum = 0.0;

	for (int i = 0;i < num_sims;i++) {
		double gauss_bm = gaussian_box_muller();
		S_cur = S_adjust * exp(sqrt(v * v * T) * gauss_bm);
		payoff_sum += std::max(S_cur - K, 0.0);
	}
	return (payoff_sum / static_cast<double>(num_sims)) * exp(-r * T);
}

double monte_carlo_put_price(const int& num_sims, const double& S, const double& K, const double& r, const double& v, const double& T) {
	double S_adjust = S * exp(T * (r - 0.5 * v * v));
	double S_cur = 0.0;
	double payoff_sum = 0.0;

	for (int i = 0;i < num_sims;i++) {
		double gauss_bm = gaussian_box_muller();
		S_cur = S_adjust * exp(sqrt(v * v * T) * gauss_bm);
		payoff_sum += std::max(K - S_cur, 0.0);
	}
	return (payoff_sum / static_cast<double>(num_sims)) * exp(-r * T);
}

int main(int argc, char **argv) {
	//parameters
	int num_sims = 10000000; //number of iterations
	double S = 100.0; //option price
	double K = 100.0; //strike price
	double r = 0.05; //risk-free rate
	double v = 0.2; //volatility of the underlying (20%)
	double T = 1.0; //one year until expiry

	double call = monte_carlo_call_price(num_sims, S, K, r, v, T);
	double put = monte_carlo_put_price(num_sims, S, K, r, v, T);

	cout << "Number of Paths:		" << num_sims << endl;
	cout << "Underlying:			" << S << endl;
	cout << "Strike:				" << K << endl;
	cout << "Risk-Free Rate:			" << r << endl;
	cout << "Volatility:			" << v << endl;
	cout << "Maturity:			" << T << endl;

	cout << "Call Price:			" << call << endl;
	cout << "Put Pirce:			" << put << endl;

	return 0;
}