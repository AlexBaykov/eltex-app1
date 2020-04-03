#include "invm.h"
using Farray_t = ROOT::VecOps::RVec<float>;

Pair::Pair(int i, int j){
    ind.push_back(i);
    ind.push_back(j);
}

Pair::~Pair(){

}

float getInvMass(Farray_t  p, Farray_t  theta, Farray_t  phi, int i, int j){
//Calculating invariant mass of i-th and j-th photons
	float imass = 0;
	float px = 0;
	float py = 0;
	float pz = 0;

	px = p[i]*sin(theta[i])*cos(phi[i]) + p[j]*sin(theta[j])*cos(phi[j]);
	px = px*px;

	py = p[i]*sin(theta[i])*sin(phi[i]) + p[j]*sin(theta[j])*sin(phi[j]);
	py = py*py;

	pz = p[i]*cos(theta[i]) + p[j]*cos(theta[j]);
	pz = pz*pz;

	imass = sqrt(pow(p[i] + p[j], 2) - (px + py + pz));

	return imass;
}

float getInvMass3(Farray_t  p, Farray_t  theta, Farray_t  phi, int i, int j, int k){
//Calculating invariant mass of i-th and j-th photons
	float imass = 0;
	float px = 0;
	float py = 0;
	float pz = 0;

	px = p[i]*sin(theta[i])*cos(phi[i]) + p[j]*sin(theta[j])*cos(phi[j]) + p[k]*sin(theta[k])*cos(phi[k]);
	px = px*px;

	py = p[i]*sin(theta[i])*sin(phi[i]) + p[j]*sin(theta[j])*sin(phi[j]) + p[k]*sin(theta[k])*sin(phi[k]);
	py = py*py;

	pz = p[i]*cos(theta[i]) + p[j]*cos(theta[j]) + p[k]*cos(theta[k]);
	pz = pz*pz;

	imass = sqrt(pow(p[i] + p[j] + p[k], 2) - (px + py + pz));

	return imass;
}

float getRecMass(Farray_t  p, float  beam, int i){
	return 2*beam*sqrt(1 - p[i]/beam);
}

int findPi0(Farray_t imass){

   int n_pi = 0;
   for(int i = 0; i < imass.size(); i++){
       if(fabs(imass[i] - 135) < 35){
           n_pi += 1;
        }
   }
   return n_pi;
}

std::vector<float> findOmega(Pair pair1, Pair pair2, Farray_t p, Farray_t theta, Farray_t phi){

  std::vector<float> imass3;
  float m_w = 0;

  imass3.push_back( getInvMass3(p, theta, phi, pair1.ind[0], pair1.ind[1], pair2.ind[0]) );
  imass3.push_back( getInvMass3(p, theta, phi, pair1.ind[0], pair1.ind[1], pair2.ind[1]) );

   return imass3;

}

std::vector<float> findOmega(float * ind1, float * ind2, Farray_t p, Farray_t theta, Farray_t phi){

  std::vector<float> imass3;
  float m_w = 0;

  imass3.push_back( getInvMass3(p, theta, phi, ind1[0], ind1[1], ind2[0]) );
  imass3.push_back( getInvMass3(p, theta, phi, ind1[0], ind1[1], ind2[1]) );

   return imass3;

}
