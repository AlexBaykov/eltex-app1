#ifndef _GETCHAINS_H_
#define _GETCHAINS_H_

#include <vector>
#include <string>
#include <map>
#include <fstream>
#include <iostream>
#include <iterator>
#include <TChain.h>
#include <TCut.h>

struct Chain{
  std::string data_file, section_file;
  TChain * chain;
  Chain(std::string, std::string);
};

std::map< std::string, std::string > getFileNames();
std::vector< Chain > getChains(std::map<std::string, std::string >);

#endif
