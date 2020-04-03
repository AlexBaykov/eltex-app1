#include "getSection.h"

using vec_d = std::vector<double>;

void fillSectionVec(vec_d energy, vec_d & section, std::vector< vec_d > & cols){
    for(int i = 0; i < energy.size(); i++){
       for(int j = 0; j < cols.size(); j++){
          if(energy[i] == cols[j][1]){
              section.push_back(cols[j][2]);
          }
       }
    }
}

void read(std::ifstream & file, std::vector< vec_d > & cols){
    std::string line;
    vec_d temp(3);

    while(std::getline(file, line)){
        std::istringstream iss(line);
        iss >> temp[0] >> temp[1] >> temp[2];
        cols.push_back(temp);
    }

}

vec_d getSection(vec_d energy, std::string file_dir, std::string file_name){
   std::ifstream file(file_dir + file_name);
   std::vector< vec_d > cols;
   vec_d section;
   read(file, cols);

   fillSectionVec(energy, section, cols);
   return section;
}
