#ifndef __INVM_H_
#define __INVM_H_

#include <math.h>
#include <vector>
#include<TROOT.h>
#include <tuple>

class Pair{
    public:
        float imass;
        std::vector<int> ind;
        float energy;
        Pair(int, int);
        ~Pair();
};

float getInvMass(ROOT::VecOps::RVec<float> p, ROOT::VecOps::RVec<float>  theta, ROOT::VecOps::RVec<float>  phi, int i, int j);
float getInvMass3(ROOT::VecOps::RVec<float> p, ROOT::VecOps::RVec<float>  theta, ROOT::VecOps::RVec<float>  phi, int i, int j, int k);
float getRecMass(ROOT::VecOps::RVec<float> p , float beam, int i);
int findPi0(ROOT::VecOps::RVec<float> imass);
std::vector<float> findOmega( Pair pair1, Pair pair2, ROOT::VecOps::RVec<float> p, ROOT::VecOps::RVec<float> theta, ROOT::VecOps::RVec<float> phi);
std::vector<float> findOmega( float *, float *,  ROOT::VecOps::RVec<float> p, ROOT::VecOps::RVec<float> theta, ROOT::VecOps::RVec<float> phi);

#endif // __INVM_H_
