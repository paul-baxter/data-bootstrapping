# data-bootstrapping

Simple utility to calculate the bootstrapped mean difference between two sets of (double) data, basic statistics of the two data sets, and a basic histogram of the bootstrapped difference of means.

Uses cmake to build:

* $ mkdir build && cd build

* $ cmake .. && cd ..

Between 0 and 3 arguments can be provided:
<ol start="0">
  <li> A simulation will be run with sample data, using 1000000 iterations</li>
  <li> A simulation will be run with the specified number of iterations (N), using sample data</li>
  <li> A simulation will be run with the specified number of iterations, using data from the specified file (see file syntax below)</li>
  <li> A simulation will be run with the specified number of iterations, using data from the specified file (see file syntax below), and outputs data to a specified file</li>
</ol>

Usage:

* $ ./Bootstrapping N "input/data/file" "output/data/file"

Format of input data file:

* Simple text file, only first four lines are processed

* Line 0: name of data set 1

* Line 1: data set 1, comma-separated floats/doubles

* Line 2: name of data set 2

* Line 3: data set 2, comma-separated floats/doubles
