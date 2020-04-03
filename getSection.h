#ifndef __GETSECTION_H_
#define __GETSECTION_H_

#include <vector>
#include <iterator>
#include <fstream>
#include <string>
#include <iostream>
#include <sstream>
#include <algorithm>

void fillSectionVec(std::vector<double>, std::vector<double> &,
                    std::vector<std::vector< double>> & );

void read(std::ifstream &, std::vector<std::vector<double>> &);

std::vector<double> getSection(std::vector<double>,
                                std::string, std::string);


#endif // __GETSECTION_H_
