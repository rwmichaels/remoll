#ifndef __HISTOUTIL_H
#define __HISTOUTIL_H

#include "TAxis.h"
#include "TH1.h"

void niceLogXBins(TH1*h)
{
  TAxis *axis = h->GetXaxis();
  int bins = axis->GetNbins();

  double from = axis->GetXmin();
  double to = axis->GetXmax();
  double width = (to - from) / bins;
  double *new_bins = new double[bins + 1];

  for (int i = 0; i <= bins; i++) {
    new_bins[i] = pow(10, from + i * width);
  }
  axis->Set(bins, new_bins);
  delete[] new_bins;
}

void drawQuartzOutline(TVirtualPad *pad){

  pad->SetMargin(0.13,0.13,0.13,0.13);

  /* const Int_t NRGBs = 5; */
  /* const Int_t NCont = 255; */
  /* Double_t stops[NRGBs] = { 0.00, 0.34, 0.61, 0.84, 1.00 }; */
  /* Double_t red[NRGBs]   = { 0.00, 0.00, 0.87, 1.00, 0.51 }; */
  /* Double_t green[NRGBs] = { 0.00, 0.81, 1.00, 0.20, 0.00 }; */
  /* Double_t blue[NRGBs]  = { 0.51, 1.00, 0.12, 0.00, 0.00 }; */
  /* TColor::CreateGradientColorTable(NRGBs, stops, red, green, blue, NCont); */
  /* gStyle->SetNumberContours(NCont); */

  /* gStyle->SetOptStat(0); */
  /* gPad->SetLogz(); */

  Float_t degrad= acos(-1)/180.;

  Float_t detinnerr[11]= {640, 680, 750, 855,  935, 920/* 90.0*/,  875/* 91.5*/,  1080,  640,  500};
  Float_t detouterr[11]= {680, 750, 855, 920, 1075,1080/* 106.0*/,1090 /*105.5*/, 1190, 1170, 1050};

  Float_t detnum;
  Float_t detcenterphi;
  Float_t detphiplus;
  Float_t detphiminus;

  Float_t detx1;
  Float_t detx2;
  Float_t detx3;
  Float_t detx4;
 
  Float_t dety1;
  Float_t dety2;
  Float_t dety3;
  Float_t dety4;

  Int_t totaldets[11] = {4, 4, 4, 4, 12,12,12, 4, 1, 1};
  Int_t extradets[11] = {1, 1, 1, 1, 3,3,3, 1, 0, 0};

  Int_t color1[15] = {4,4,4,3,3,3,2,2,2,3,3,3,4,4,4};
  Int_t color2[5] ={4,3,2,3,4};
  Int_t width = 2;
  
  TLine* l1;
  TLine* l2;
  TLine* l3;
  TLine* l4;
 
  //------------------------------------------------
  for (Int_t ringnum=0;ringnum<9;ringnum++){
    for(Int_t deti=0;deti<totaldets[ringnum]+extradets[ringnum];deti++){

      Bool_t drawline1= ringnum>=0 && ringnum<=3;
      Bool_t drawline2= ringnum==4 && (deti==6||deti==7||deti==8);
      Bool_t drawline3= ringnum==5 && (deti==3||deti==4||deti==5||deti==9||deti==10||deti==11);
      Bool_t drawline4= ringnum==6 && (deti==0||deti==1||deti==2||deti==12||deti==13||deti==14);
      Bool_t drawline5= ringnum==7||ringnum==8;
      Bool_t drawline=drawline1||drawline2||drawline3||drawline4||drawline5;

      detnum = deti-(totaldets[ringnum]+extradets[ringnum]-1)/2;
      detcenterphi=(detnum*360./(7*totaldets[ringnum]));
      if(((ringnum<4 || ringnum>6)&& detnum==2)|| (ringnum==6&& detnum==6)){
	detphiplus=detcenterphi;
      }else{
	detphiplus=detcenterphi+360./(14*(totaldets[ringnum])); 
      }
      if(((ringnum<4 || ringnum>6)&& detnum==-2)|| (ringnum==6&& detnum==-6)){
	detphiminus=detcenterphi;
      }else{
	detphiminus=detcenterphi-360./(14*(totaldets[ringnum])); 
      }
      
      detx1 = -detinnerr[ringnum]*TMath::Cos(detphiminus*degrad);
      dety1 = -detinnerr[ringnum]*TMath::Sin(detphiminus*degrad);
      detx2 = -detinnerr[ringnum]*TMath::Cos(detphiplus*degrad);
      dety2 = -detinnerr[ringnum]*TMath::Sin(detphiplus*degrad);
      detx3 = -detouterr[ringnum]*TMath::Cos(detphiplus*degrad);
      dety3 = -detouterr[ringnum]*TMath::Sin(detphiplus*degrad);
      detx4 = -detouterr[ringnum]*TMath::Cos(detphiminus*degrad);
      dety4 = -detouterr[ringnum]*TMath::Sin(detphiminus*degrad);

      l1 = new TLine(detx1,dety1,detx2,dety2);
      l2 = new TLine(detx2,dety2,detx3,dety3);
      l3 = new TLine(detx3,dety3,detx4,dety4);
      l4 = new TLine(detx4,dety4,detx1,dety1);

      if (ringnum==4||ringnum==5||ringnum==6){
	l1->SetLineColor(color1[deti]);
	l2->SetLineColor(color1[deti]);
	l3->SetLineColor(color1[deti]);
	l4->SetLineColor(color1[deti]);
      }else{
	l1->SetLineColor(color2[deti]);
	l2->SetLineColor(color2[deti]);
	l3->SetLineColor(color2[deti]);
	l4->SetLineColor(color2[deti]);
      }
      
      
      l1->SetLineWidth(width);
      l2->SetLineWidth(width); 
      l3->SetLineWidth(width);
      l4->SetLineWidth(width); 
      
      
      
      if (drawline && !(ringnum==6 && (detnum==7||detnum==-7)) ){
	if (ringnum==8){
	  l2->SetLineColor(1);
	  l4->SetLineColor(1);
	  l2->SetLineStyle(2);
	  l4->SetLineStyle(2);
	}else{
	  l1->SetLineStyle(1);
	  l2->SetLineStyle(1) ;
	  l3->SetLineStyle(1);
	  l4->SetLineStyle(1);
	  l1->Draw();
	  l3->Draw();
	}
	l2->Draw();
	l4->Draw();
      }
	      	
    }
  }
}

#endif //__HISTOUTIL_H