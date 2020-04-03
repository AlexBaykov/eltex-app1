#include "analyzeChain.h"

using vec_d = std::vector<double>;
using vec_f = std::vector<float>;

std::vector<vec_d> getLuminosity(std::string dir, std::string file_name){
  std::ifstream file(dir + file_name);
  std::vector<vec_d> cols;

  read(file, cols);
  return cols;
}


void analyzeChain(Chain chain, vec_f &counts, std::string dir, vec_d energy, std::vector<vec_d> luminosity){
  TChain * inchain = chain.chain;

  TObjArray *fileElements = inchain->GetListOfFiles();
  TIter next(fileElements);
  TChainElement *chainElement = 0;

  vec_d section;
  vec_d coeff;
  float sum = 0;

  while( ( chainElement = (TChainElement *)next() ) ){
    TFile * f = new TFile( chainElement->GetTitle() );
    int N = 0;
    std::vector<TH1D> hists = analyzeFile(f, N);
    counts.push_back(N);
  }

  if(chain.section_file != " "){
    section = getSection(energy, dir, chain.section_file);
    for(int i = 0; i < counts.size(); i++){
      counts[i] = section[i]*luminosity[i][2]*counts[i]/5e4;
      sum += counts[i];
    }
  }else{
    std::cout << chain.section_file << std::endl;
    for(int i = 0; i < counts.size(); i++){
      sum += counts[i];
    }
  }

  std::cout << chain.data_file << " " << sum << std::endl;
}
