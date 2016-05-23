#include <cstdlib>
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <time.h>
#include <math.h>


std::vector<double> Stats (std::vector<double>);
double Mean (std::vector<double>);
double DiffMean (int, std::vector<double>);
std::vector<int> Histogram (std::vector<double>);
std::vector<double> FindCI (std::vector<double>);


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



std::vector<double> Stats (std::vector<double> data)
{
	//calculate mean and standard deviation of the data
	// -- return [mean, sd, n]
	
	if ((int)data.size() < 2) 
	{
		std::vector<double> back(2, 0.0);
		std::cout << "ERROR: not enough data" << std::endl;
		return back;
	}
	int n = data.size();
	//double prev_mean = data[0];
	double mean = 0.0;
	//double prev_s = 0.0;
	//double s = 0.0;
	//double variance = 0.0;
	double sd = 0.0;
	
	/*for (int a = 1; a < n; a++)
	{
		//don't have to include the first element
		mean = prev_mean + ((data[a] - prev_mean)/(double)a);
		s = prev_s + ((data[a] - prev_mean)*(data[a] - mean));
	}*/
	
	//CALCULATE THE MEAN
	double tot_temp = 0.0;
	for (int a = 0; a < n; a++)
	{
		tot_temp += data[a];
	}
	mean = (double)(tot_temp / (double)n);
	
	//CALCULATE THE STANDARD DEVIATION
	double sum_deviation = 0.0;
	double deviation = 0.0;
	for (int b = 0; b < n; b++)
	{
		sum_deviation += ((data[b] - mean)*(data[b] - mean));
	}
	deviation = (double)(sum_deviation / (double)n);
	sd = sqrt(deviation);
	
	//variance = s / (n - 1);
	//sd = sqrt(variance);
	
	std::vector<double> stats;
	stats.clear();
	stats.push_back(mean);
	stats.push_back(sd);
	stats.push_back((double)n);
	
	return stats;
}

double Mean (std::vector<double> data)
{
	double tot = 0.0;
	int num = 0;
	for (int a = 0; a < (int)data.size(); a++)
	{
		num++;
		tot += data[a];
	}
	double mean = tot / (double)num;
	return mean;
}

double DiffMean (int n_first, std::vector<double> data)
{
	//calculate the difference between mean of first n_first samples
	// and the N-n_first remaining samples
	// return just the difference in means
	// (first - second)
	double mean_first = 0.0;
	double mean_second = 0.0;
	double tot_first = 0.0;
	double tot_second = 0.0;
	int count_one = 0;
	int count_two = 0;
	
	//first n_first samples
	for (int a = 0; a < n_first; a++)
	{
		tot_first += data[a];
		count_one++;
	}
	mean_first = tot_first / (double)count_one;
	
	//remaining samples in data
	for (int b = n_first; b < (int)data.size(); b++)
	{
		tot_second += data[b];
		count_two++;
	}
	mean_second = tot_second / (double)count_two;
	
	return (mean_first - mean_second);
}

std::vector<int> Histogram (std::vector<double> data)
{
	int N_bins = 11;		//number of histogram bins
						//should be able to automatically calculate this...
	double min = data[0];	//min value in data
	double max = data[0];	//max value in data
	double range = 0.0;
	double bin_size = 0.0;
	std::vector<int> result (N_bins, 0);
	
	//First - check for min and max values
	//may need to be careful about ints going out of bounds...
	for (int a = 0; a < (int)data.size(); a++)
	{
		if (data[a] < min) min = data[a];
		if (data[a] > max) max = data[a];
	}
	range = max - min;
	bin_size = (range / (double)N_bins);
	
	std::cout << "Min: " << min << "\tMax: " << max << "\tBin size: " << bin_size << std::endl;
	std::cout << std::endl;
	
	//following can be much improved!
	for (int b = 0; b < N_bins; b++)
	{
		//for each bin
		// check every value to see if in bin
		for (int aa = 0; aa < (int)data.size(); aa++)
		{
			//for every value, check if in this bin
			if ((data[aa] >= (min + (b * bin_size))) && (data[aa] < (min + ((b + 1) * bin_size))))
			{
				result[b] += 1;
			}
			if ((data[aa] == (min + ((b + 1) * bin_size))) && (b == N_bins -1))
			{
				//to catch the max value case
				result[b] += 1;
			}
		}
	}
	
	int tot = 0;
	for (int c = 0; c < (int)result.size(); c++)
	{
		std::cout << result[c] << ", ";
		tot += result[c];
	}
	std::cout << std::endl;
	
	return result;
}

std::vector<double> FindCI (std::vector<double> _in)
{
	std::vector<double> toreturn;
	toreturn.clear();
	double low = 0.025;		//from 5%
	double high = 0.975;	//from 5%
	double size = (double)_in.size();
	int indexLow = (int)((low * size) + 0.5);
	int indexHigh = (int)((high * size) + 0.5);
	double lowCI = 0.0;
	double highCI = 0.0;
	
	//first: sort the input vector
	std::sort (_in.begin(), _in.end()); 
	
	//second: find the 95% CI using quantile method
	lowCI = _in[indexLow];
	highCI = _in[indexHigh];
	
	//return values
	toreturn.push_back(lowCI);
	toreturn.push_back(highCI);
	
	return toreturn;
}



