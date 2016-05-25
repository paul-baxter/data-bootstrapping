//helperFuncs.h
#ifndef HELPERFUNCS_H_INCLUDED
#define HELPERFUNCS_H_INCLUDED


std::vector<double> StoDvector (std::vector<std::string>);
std::vector<std::string> Split (const std::string&, char);
std::vector<double> Stats (std::vector<double>);
double Mean (std::vector<double>);
double DiffMean (int, std::vector<double>);
std::vector<int> Histogram (int, std::vector<double>, std::vector<double>&);
std::vector<double> FindCI (std::vector<double>);

#endif
