#include "analyzeFile.h"
#include "invm.h"

using Farray_t = ROOT::VecOps::RVec<float>;
using Pairs = std::vector<Pair>;

bool filterMass(Farray_t &imass, float mass, float sigma){

    int n = 0;
    for(int i = 0; i < imass.size(); i++){
        if( fabs(imass[i] - mass) < sigma ){
            n += 1;
        }
    }
    return n == 0;
}

void analyzeFile(TFile * f, int &N_fin, int &N_min, int &N_inter){
    ROOT::RDataFrame d("h1", f);

    //Initializing cuts.
    auto logic = [](int trin, unsigned char cact, unsigned char act){return trin > 0 && act == 0 && cact == 0;};
    auto four_neutral = [](int nn, int nc, unsigned char nwr){return nn == 4 && nc == 0 && nwr < 4;};
    auto emp = [](float ptrt, float eton){return ptrt < 0.3 && eton - ptrt > 0.7;};
    auto x2 = [](float x2kf2){return x2kf2 < 10;};
    auto xi2 = [](float x2kf2, float x2ikf2){return x2ikf2 - x2kf2 < 5;};
    auto min_en = [](Farray_t& cenergy, float beam){return ROOT::VecOps::Min(cenergy) / beam > 0.1;};
    auto xinm = [](Farray_t& cx2g){return ROOT::VecOps::Max(cx2g) < -5;};

    auto filtered1 = d.Filter(logic, {"trin", "act", "cact"}, "logic");
    auto filtered2 = filtered1.Filter(four_neutral, {"nn", "nc", "nwr"}, "4 + no charge");
    auto filtered3 = filtered2.Filter(emp, {"ptrt", "eton"}, "emp > 0.7");


    //Creating a column with pairwise invariant masses in an event.
    auto augmented = filtered3.Define("pions",
                                      [](Farray_t& ppkf2, Farray_t& thetakf2, Farray_t& phikf2, Farray_t &cenergy){
                                            Farray_t imass;
                                            Pairs pairs;
                                            for(int i = 0; i < 4; i++){
                                                for(int j = i + 1; j < 4; j++){
                                                    Pair pair(i, j);
                                                    pair.imass = getInvMass(ppkf2, thetakf2, phikf2, i, j);
                                                    pair.energy = cenergy[i] + cenergy[j];
                                                    if(fabs(pair.imass - 135) < 35){
                                                        pairs.push_back(pair);
                                                    }
                                                }
                                            }
                                            return pairs;},
                                      {"ppkf2", "thetakf2", "phikf2", "cenergy"})
                          .Define("rmass",
                                  [](Farray_t& ppkf2, float beam){
                                      Farray_t recm;
                                      for(int i = 0; i < 4; i++){
                                          recm.push_back(getRecMass(ppkf2, beam, i));
                                      }
                                      return recm;
                                  }, {"ppkf2", "beam"})
                          .Define("omega",
                                   [](Pairs &pairs, Farray_t& ppkf2, Farray_t& thetakf2, Farray_t& phikf2){
                                       Farray_t omega_mass;
                                       std::vector<float> temp_mass;
                                       int size = pairs.size();
                                       for(int i = 0; i < size; i++){
                                           for(int j = i+1; j < size; j++){
                                               temp_mass = ( (pairs[i].energy > pairs[j].energy) ?
                                                              findOmega(pairs[i], pairs[j], ppkf2, thetakf2, phikf2) :
                                                              findOmega(pairs[j], pairs[i], ppkf2, thetakf2, phikf2)
                                                            );
                                               omega_mass.push_back(temp_mass[0]);
                                               omega_mass.push_back(temp_mass[1]);
                                            }

                                        }
                                       return omega_mass;
                                   }, {"pions", "ppkf2", "thetakf2", "phikf2"});

    auto filtered4 = augmented.Filter(min_en, {"cenergy", "beam"}, "energy/beam > 0.1");
    auto filtered5 = filtered4.Filter(x2, {"x2kf2"}, "x2kf2 < 10");
    auto filtered6 = filtered5.Filter(xinm, {"cx2g"}, "xinm < -5");
    auto filtered7 = filtered6.Filter(xi2, {"x2kf2", "x2ikf2"}, "x2ikf2 - x2kf2 < 5");


    auto augmented1 = filtered7.Filter([](Pairs& pions){return pions.size() >= 2;}, {"pions"}, "n_pi == 2");
    auto augmented2 = augmented1.Filter([](Farray_t& rmass){ return filterMass(rmass, 782, 75);}, {"rmass"}, "rmass != 782");
    auto augmented3 = augmented2.Filter([](Farray_t& omega){ return filterMass(omega, 782, 75);}, {"omega"}, "omega != 782");

/*    auto histo1 = augmented.Histo1D("omega");
    auto histo2 = augmented.Filter([](Farray_t& rmass){ return filterMass(rmass, 782, 120);}, {"omega"}).Histo1D("omega");

    TCanvas * canvas = new TCanvas();
    canvas->Divide(2, 1);
    canvas->cd(1);
    histo1->DrawCopy();
    canvas->cd(2);
    histo2->DrawCopy();*/

    N_min = *filtered5.Count();
    N_inter = *filtered7.Count();
    N_fin = *augmented3.Count();

}
