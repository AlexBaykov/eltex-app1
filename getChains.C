#include "getChains.h"

using map_ss = std::map<std::string, std::string>;

Chain::Chain (std::string data_filename, std::string section_filename){
    data_file = data_filename;
    section_file = section_filename;
  };

map_ss getFileNames(){
  map_ss file_names;

  file_names["expfiles.dat"] = " ";
  file_names["2pi0gsimfiles.dat"] = "2pi0gwithrad_thr.his";
  file_names["f2simfiles.dat"] = "f2withrad_thr.his";
  file_names["pi0gsimfiles.dat"] = "pi0gwithrad_thr.his";
  file_names["etgsimfiles.dat"] = "etagwithrad_thr.his";
  file_names["4gsimfiles.dat"] = "4gwithrad_thr.his";
  file_names["3gsimfiles.dat"] = "3gwithrad_thr.his";

  return file_names;
}

std::vector< Chain > getChains(map_ss file_names){
  std::ifstream file;
  std::string line;
  std::string dir = "/home/alex/work/f2/csfl/";

  std::vector< Chain > chain_list;
  map_ss::iterator it;
  for( it = file_names.begin(); it != file_names.end(); it++){

    Chain tmp = Chain(it->first, it->second);
    file.open( (dir + it->first).c_str() );
    TChain * chain = new TChain("h1");
    while( std::getline(file, line) ){
      chain->Add( Form( "%s", line.c_str() ) );
    }
    tmp.chain = chain;
    chain_list.push_back(tmp);
    file.close();
  }

  return chain_list;
}
