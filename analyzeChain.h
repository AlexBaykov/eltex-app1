#ifndef __ANALYZECHAIN_H_
#define __ANALYZECHAIN_H_
#include <algorithm>
#include <TChain.h>
#include <TObjArray.h>
#include <TChainElement.h>
#include <TFile.h>
#include <TLeaf.h>
#include <TTreeReader.h>
#include <TTreeReaderValue.h>
#include <TTreeReaderArray.h>
#include "getChains.h"
#include "getSection.h"
#include "analyzeFile.h"

std::vector<std::vector<double>> getLuminosity(std::string, std::string);

void getExpected(std::vector<double>, std::vector<std::vector<double>>, std::vector<float> &);

void analyzeChain(Chain, std::vector<float> &, std::vector<float> &, std::vector<float> &,
                  std::string, std::vector<double>,
                  std::vector<std::vector<double>>);

#endif // __ANALYZECHAIN_H_
