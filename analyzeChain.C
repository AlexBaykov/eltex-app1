#include "analyzeChain.h"

using vec_d = std::vector<double>;
using vec_f = std::vector<float>;

std::vector<vec_d> getLuminosity(std::string dir, std::string file_name){
  std::ifstream file(dir + file_name);
  std::vector<vec_d> cols;

  read(file, cols);
  return cols;
}

void getExpected(vec_d section, std::vector<vec_d> luminosity, vec_f & counts){
     for(int i = 0; i < counts.size(); i++){
       counts[i] = section[i]*luminosity[i][2]*counts[i]/5e4;
     }
}


void analyzeChain(Chain chain, vec_f &counts_fin, vec_f &counts_min, vec_f &counts_inter,
                  std::string dir, vec_d energy, std::vector<vec_d> luminosity){
  TChain * inchain = chain.chain;

  TObjArray *fileElements = inchain->GetListOfFiles();
  TIter next(fileElements);
  TChainElement *chainElement = 0;

  vec_d section;
  vec_d coeff;

  while( ( chainElement = (TChainElement *)next() ) ){
    TFile * f = new TFile( chainElement->GetTitle() );
    int N_fin = 0;
    int N_min = 0;
    int N_inter = 0;
    analyzeFile(f, N_fin, N_min, N_inter);
    counts_fin.push_back(N_fin);
    counts_min.push_back(N_min);
    counts_inter.push_back(N_inter);
  }

  if(chain.section_file != " "){
    section = getSection(energy, dir, chain.section_file);
    getExpected(section, luminosity, counts_fin);
    getExpected(section, luminosity, counts_min);
    getExpected(section, luminosity, counts_inter);
  }
}
