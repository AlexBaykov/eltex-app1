#include <vector>
#include <string>
#include "getChains.h"
#include "getSection.h"
#include "analyzeChain.h"
#include <TH1D.h>
#include <THStack.h>
#include <TCanvas.h>
#include <TObject.h>
#include <time.h>

using vec_f = std::vector<float>;
using vec_d = std::vector<double>;

vec_f add(vec_f a, vec_f b){
    int size = a.size();
    vec_f output;
    if(size !=  b.size()){
        std::cout << "Cannot add vectors of different lengths.";
    }else{
        for(int i = 0; i < size; i++){
           output.push_back(a[i] + b[i]);
        }
    }
    return output;
}


void fillHist(TH1D * hist, vec_f vec){
    hist->SetBinContent(1, vec[0] + vec[5]); //1087.5 - 1112.5
    hist->SetBinContent(2, vec[6]); // 1112.5 - 1137.5
    hist->SetBinContent(3, vec[7]); // 1137.5 - 1162.5
    hist->SetBinContent(4, vec[8]); // 1162.5 - 1187.5
    hist->SetBinContent(5, vec[9]); //1187.5 - 1212.5
    hist->SetBinContent(6, vec[10]); // 1212.5 - 1237.5
    hist->SetBinContent(7, vec[11]); //1237.5 - 1262.5
	hist->SetBinContent(8, vec[12] + vec[23] + vec[26]); // 1262.5 - 1287.5
	hist->SetBinContent(9, vec[13] + vec[2] + vec[27]);//1287.5 - 1312.5
	hist->SetBinContent(10, vec[14]);//1312.5 - 1337.5
	hist->SetBinContent(11, vec[15] + vec[28] + vec[24]);//1337.5 - 1362.5
	hist->SetBinContent(12, vec[16]);// 1362.5 - 1387.5
	hist->SetBinContent(13, vec[3] + vec[17] + vec[29]);//1387.5 - 1412.5
	hist->SetBinContent(14, vec[18]);//1412.5 - 1437.5
	hist->SetBinContent(15, vec[30] + vec[19] + vec[25]);//1437.5 - 1462.5
	hist->SetBinContent(16, vec[20]);//1462.5 - 1487.5
	hist->SetBinContent(17, vec[4] + vec[21] + vec[22] + vec[31]);//1487.5 - 1512.5
}

std::vector< TH1D * > getHists(std::vector<vec_f> result){
    TH1D * ppg = new TH1D("ppg", "#pi^{0}#pi^{0}#gamma", 17, 1087.5, 1512.5);
    TH1D * qed = new TH1D("qed", "3#gamma + 4#gamma", 17, 1087.5, 1512.5);
    TH1D * pgetg = new TH1D("pgetg", "#pi^{0}#gamma + #eta#gamma", 17, 1087.5, 1512.5);
    TH1D * pp = new TH1D("pp", "#pi^{0}#pi^{0}", 17, 1087.5, 1512.5);
    TH1D * exp = new TH1D("exp", "Experimental", 17, 1087.5, 1512.5);



    fillHist(ppg, result[0]);
    fillHist(qed, add(result[1], result[2]));
    fillHist(pgetg, add(result[3], result[6]));
    fillHist(pp, result[5]);
    fillHist(exp, result[4]);

	pgetg->SetLineColor(4);
	pgetg->SetFillColor(4);
	qed->SetLineColor(3);
	qed->SetFillColor(3);

	pp->SetMarkerStyle(kFullCircle);
	exp->SetMarkerStyle(kFullSquare);

    std::cout << "bkg: " << ppg->Integral() + qed->Integral() + pgetg->Integral() << " f2: " << pp->Integral() << std::endl;

    std::vector< TH1D * > out;
    out.push_back(ppg);
    out.push_back(qed);
    out.push_back(pgetg);
    out.push_back(pp);
    out.push_back(exp);

    return out;
}

void draw(){
    double time = 0;
    clock_t begin = clock();

    std::string dir = "/home/alex/work/f2/csfl/";
    std::string lum_file = "luminosities.dat";

    std::vector<vec_d> luminosity = getLuminosity(dir, lum_file);
    std::vector< Chain > chains = getChains(getFileNames());

    /*Chains are in the following order:
     * 0 ppg
     * 1 3g
     * 2 4g
     * 3 etg
     * 4 exp
     * 5 f2
     * 6 pg.
     * This is so because of the use of map. Need to fix*/

    std::vector<vec_f> result_fin(luminosity.size());
    std::vector<vec_f> result_min(luminosity.size());
    std::vector<vec_f> result_inter(luminosity.size());
    vec_d energy;

    for(int i = 0; i < luminosity.size(); i++){
        energy.push_back(luminosity[i][1]);
    }

    for(int i = 0; i < chains.size(); i++){
        analyzeChain(chains[i], result_fin[i], result_min[i], result_inter[i], dir, energy, luminosity);
    }

    std::vector<TH1D *> hists_fin = getHists(result_fin);
    std::vector<TH1D *> hists_inter = getHists(result_inter);
    std::vector<TH1D *> hists_min = getHists(result_min);

	THStack * hs_fin = new THStack("All cuts", "");
	hs_fin->Add(hists_fin[2]);
	hs_fin->Add(hists_fin[1]);
	hs_fin->Add(hists_fin[0]);

	THStack * hs_min = new THStack("Preliminary cuts", "");
	hs_min->Add(hists_min[2]);
	hs_min->Add(hists_min[1]);
	hs_min->Add(hists_min[0]);

	THStack * hs_inter = new THStack("xinm", "");
	hs_inter->Add(hists_inter[2]);
	hs_inter->Add(hists_inter[1]);
	hs_inter->Add(hists_inter[0]);


	TCanvas * c = new TCanvas();
	c->Divide(3, 1);

    c->cd(1);
	hs_min->SetMinimum(0);
	hs_min->SetMaximum(16);
	hs_min->Draw();
	hists_min[3]->Draw("SAME E0 X0");
	hists_min[4]->Draw("SAME E0 X0");

    c->cd(2);
	hs_inter->SetMinimum(0);
	hs_inter->SetMaximum(2.8);
	hs_inter->Draw();
	hists_inter[3]->Draw("SAME E0 X0");
    TH1D *left = (TH1D *)hists_inter[4]->Clone();
    TH1D *right = (TH1D *)hists_inter[4]->Clone();
    left->SetName("clone");
    right->SetName("clone1");
    left->GetXaxis()->SetRange(1, 4);
    right->GetXaxis()->SetRange(14, 17);
	left->Draw("SAME E0 X0");
	right->Draw("SAME E0 X0");

    c->cd(3);
	hs_fin->SetMinimum(0);
	hs_fin->SetMaximum(1.4);
	hs_fin->Draw();
	hists_fin[3]->Draw("SAME E0 X0");
    left = (TH1D *)hists_fin[4]->Clone();
    right = (TH1D *)hists_fin[4]->Clone();
    left->SetName("clone");
    right->SetName("clone1");
    left->GetXaxis()->SetRange(1, 4);
    right->GetXaxis()->SetRange(14, 17);
	left->Draw("SAME E0 X0");
	right->Draw("SAME E0 X0");
	c->Update();

    clock_t end = clock();
    time += (double) (end - begin)/CLOCKS_PER_SEC;
    std::cout << "time of exec: " << time << std::endl;

}
