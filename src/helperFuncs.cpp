#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <string>
#include <cmath>
#include <sstream>
#include <algorithm>

#include "helperFuncs.h"

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
