#include <cstdlib>
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <time.h>
#include <math.h>
#include "helperFuncs.h"


int main (int argc, char* argv[])
{
	int N;			//number of bootstrap iterations
	std::string filename;

	if (argc == 1)
	{
		N = 1000000;	//default number of iterations
		std::cout << "Default number of iterations: " << N << std::endl;
	}
	else if (argc == 2)
	{
		N = atoi(argv[1]);
		std::cout << "Read-in number of iterations: " << N << std::endl;
	}
	else if (argc == 3)
	{
		//read in num iterations and file name
		N = atoi(argv[1]);
		filename = std::string(argv[2]);
		std::cout << "Set N to: " << N << std::endl;
		std::cout << "Set filename to: " << filename << std::endl;
	}
	std::cout << std::endl;

	//initialise random number generator
	srand (time(NULL));

	//some test data
	//double dat1[] = {30,31,36,37,38,39,46};
	//double dat2[] = {26,28,29,33,34,39};
	//some real data
	double P[] = {3.75,4,4.5,4.75,4.75,3.666666667,3.25,4.5,4.5,4,4.25,4,6.75,6,4,4.666666667,4,3.75,4.5,4.5,4.75,6,3.333333333,4.5,5.25,5.25,4.5,4.5,4.75,4.25};
	double NP[] = {5,5,7.666666667,3.666666667,4.333333333,3.666666667,3.333333333,5.666666667,7,5.333333333,3.666666667,3.333333333,4,6.333333333,4,3,5.333333333,3.666666667,4,3.333333333,5.333333333,3.333333333,4.666666667,3.5,4.333333333,8.333333333,5.333333333,4.666666667,3};

	//the two starting data structures
	std::vector<double> one (P, P + sizeof(P) / sizeof(double));
	std::vector<double> two (NP, NP + sizeof(NP) / sizeof(double));
	//the concatenated data containing both
	std::vector<double> concat;

	//one.clear();
	//two.clear();
	concat.clear();

	//read in values from some external source

	//get sizes - n_one and n_two are the sample sizes
	// for the bootstrapping
	int n_one = (int)one.size();
	int n_two = (int)two.size();

	//calculate the mean of the input data
	double mean_one = (double)Mean(one);
	double mean_two = (double)Mean(two);
	double mean_diff = mean_one - mean_two;

	std::cout << "Difference of the means: " << mean_diff << std::endl;

	//concatenate one and two to form concat
	concat = one;
	concat.insert( concat.end(), two.begin(), two.end() );

	//size of the concatenated vector
	int n_concat = (int)concat.size();

	//check contents of concat
	/*for (int a = 0; a < n_concat; a++)
	{
		std::cout << concat[a] << " ";
	}
	std::cout << std::endl;*/

	//have quick check for the length
	if (n_concat != (n_one + n_two))
	{
		//unequal length, something is wrong
		std::cout << "ERROR: incorrect sample sizes" << std::endl;
		return 0;
	}

	//set up the bootstrapping process
	std::vector<double> result_mean;	//all the means
	result_mean.clear();

	/////////////////////////////////////////////////////////////
	//			Start of the bootstrap process		    //
	/////////////////////////////////////////////////////////////
	std::vector<double> temp;

	for (int aa = 0; aa < N; aa++)
	{
		//recreate temp structure for the bootstrap
		temp.clear();

		for (int a = 0; a < n_concat; a++)
		{
			//fill up the vector with correct number of entries
			// ...WITH REPLACEMENT SELECTION PROCEDURE...
			// ensure I get complete potential coverage
			int ran_int = rand() % (n_concat);
			double val = concat[ran_int];
			temp.push_back(val);
		}

		//check that the correct number of values has been entered
		if (temp.size() != concat.size())
		{
			//sizes don't match, error
			std::cout << "ERROR: incorrect size of constructed bootstrap vector" << std::endl;
			return 0;
		}

		//now calculate the mean difference between two parts of the bootstrap temp data
		double outcome = DiffMean(n_one, temp);
		result_mean.push_back(outcome);
	}

	/////////////////////////////////////////////////////////////
	//			Stats on the bootstrap process		    //
	/////////////////////////////////////////////////////////////

	//06/05/15 - something in these calculations is very wrong!

	std::vector<double> Outcome = Stats(result_mean);
	double mean_diff_means = Outcome[0];
	double mean_diff_sd = Outcome[1];
	double mean_diff_n = Outcome[2];

	std::cout << "Mean bootstrapped difference of means: " << mean_diff_means << std::endl;

	double intermediate = mean_diff_sd / sqrt(mean_diff_n);
	double bar = intermediate * 1.96; //assuming normal distribution
	double lower = mean_diff_means - bar;
	double upper = mean_diff_means + bar;

	//18/08/15 - updated method to find 95%CI
	std::vector<double> CIrange = FindCI(result_mean);

	std::cout << std::endl;
	std::cout << "95% CI (old): \t[" << lower << ", " << upper << "]" << std::endl;
	std::cout << "95% CI (new): \t[" << CIrange[0] << ", " << CIrange[1] << "]" << std::endl;
	std::cout << std::endl;

	//histogram output:
	std::vector<int> his = Histogram(result_mean);

}//end of main
