#include <iostream>
#include <vector>
#include <string>
#include "TH1F.h"
#include "TCanvas.h"
#include "THStack.h"
#include "TStyle.h"
#include "TLegend.h"
#include "TLine.h"
#include "TLatex.h"
#include "TChain.h"
#include "TCut.h"
#include "TROOT.h"
#include "dataMCplotMaker.h"

//Comparison to put smallest histogram on bottom of stack
bool Integral(PlotInfo plot1, PlotInfo plot2){
  return plot1.Plot->Integral(0,plot1.Plot->GetNbinsX()) < plot2.Plot->Integral(0,plot2.Plot->GetNbinsX());
}

//Parse Parameters from options input string
vector <std::string> GetParms(std::string blah){
  int a = -1;
  int length = blah.length();
  vector <std::string> options;
  while (a < length){
    int temp = a;
    a = blah.find("--", temp+1);
    if (a <= temp) break;
    int b = blah.find("--", a+3)-1;
    unsigned int myLength = b - a - 2;
    string mySubstring;
    if (a + 2 + myLength > blah.length()) mySubstring = blah.substr(a+2);
    else mySubstring = blah.substr(a+2, b-a-2);
    options.push_back(mySubstring);
  }
  return options;
}

//Function to determine maximum of each histogram, including error bars
float AdjustedMaximum(std::vector <TH1F*> Plots){
  vector <float> heights;
  for (int i = 0; i < Plots[0]->GetNbinsX()+2; i++){
    float temp = 0;
    for (unsigned int j = 0; j < Plots.size(); j++){
      temp += Plots[j]->GetBinContent(i)+Plots[j]->GetBinError(i);
    }
    heights.push_back(temp);
  }
  sort( heights.begin(), heights.end() );
  return heights[heights.size()-1];
}

//Turn parsed argument from string into const char*.  Remove leading and trailing whitespace
const char* getString(std::string initial, std::string result){
  int temp = initial.find(result); 
  std::string substring = initial.substr(temp+result.length());
  while (substring[0] == ' '){
    std::string temp2 = substring.substr(1,substring.length()-1); 
    substring = temp2;
  }
  while (substring[substring.length()-1] == ' '){
    std::string temp2 = substring.substr(0,substring.length()-1); 
    substring = temp2;
  }
  return substring.c_str();
}

//Set style -- this is completely ripped off from TDRStyle.cc
void SetTDRStyle(){

  tdrStyle = new TStyle("tdrStyle","Style for P-TDR");

  // For the canvas:
  tdrStyle->SetCanvasBorderMode(0);
  tdrStyle->SetCanvasColor(kWhite);
  tdrStyle->SetCanvasDefH(600); //Height of canvas
  tdrStyle->SetCanvasDefW(800); //Width of canvas
  tdrStyle->SetCanvasDefX(0);   //Position on screen
  tdrStyle->SetCanvasDefY(0);

  // For the Pad:
  tdrStyle->SetPadBorderMode(0);
  tdrStyle->SetPadColor(kWhite);
  tdrStyle->SetPadGridX(false);
  tdrStyle->SetPadGridY(false);
  tdrStyle->SetGridColor(0);
  tdrStyle->SetGridStyle(3);
  tdrStyle->SetGridWidth(1);

  // For the frame:
  tdrStyle->SetFrameBorderMode(0);
  tdrStyle->SetFrameBorderSize(1);
  tdrStyle->SetFrameFillColor(0);
  tdrStyle->SetFrameFillStyle(0);
  tdrStyle->SetFrameLineColor(1);
  tdrStyle->SetFrameLineStyle(1);
  tdrStyle->SetFrameLineWidth(1);

  //For the histo:
  tdrStyle->SetHistLineColor(1);
  tdrStyle->SetHistLineWidth(2);

  tdrStyle->SetEndErrorSize(2);
  tdrStyle->SetMarkerStyle(20);

  //For the fit/function:
  tdrStyle->SetOptFit(1);
  tdrStyle->SetFitFormat("5.4g");
  tdrStyle->SetFuncColor(2);
  tdrStyle->SetFuncStyle(1);
  tdrStyle->SetFuncWidth(1);

  //For the date:
  tdrStyle->SetOptDate(0);

  //For the statistics box:
  tdrStyle->SetOptFile(0);
  tdrStyle->SetOptStat(0); // To display the mean and RMS:   SetOptStat("mr");
  tdrStyle->SetOptFit(0); // To display the mean and RMS:   SetOptStat("mr");
  tdrStyle->SetStatColor(kWhite);
  tdrStyle->SetStatFont(42);
  tdrStyle->SetStatFontSize(0.025);
  tdrStyle->SetStatTextColor(1);
  tdrStyle->SetStatFormat("6.4g");
  tdrStyle->SetStatBorderSize(1);
  tdrStyle->SetStatH(0.1);
  tdrStyle->SetStatW(0.15);

  //Margins:
  tdrStyle->SetPadTopMargin(0.10);
  tdrStyle->SetPadBottomMargin(0.15);
  tdrStyle->SetPadLeftMargin(0.15);
  tdrStyle->SetPadRightMargin(0.04);

  //For the Global title:
  tdrStyle->SetOptTitle(1);
  tdrStyle->SetTitleFont(42);
  tdrStyle->SetTitleColor(1);
  tdrStyle->SetTitleTextColor(1);
  tdrStyle->SetTitleFillColor(10);
  tdrStyle->SetTitleFontSize(0.05);
  tdrStyle->SetTitleX(0.5); // Set the position of the title box
  tdrStyle->SetTitleY(0.985); // Set the position of the title box
  tdrStyle->SetTitleAlign(23);
  tdrStyle->SetTitleStyle(0);
  tdrStyle->SetTitleBorderSize(0);
  tdrStyle->SetTitleFillColor(0);

  //For the axis titles:
  tdrStyle->SetTitleColor(1, "XYZ");
  tdrStyle->SetTitleFont(42, "XYZ");
  tdrStyle->SetTitleSize(0.05, "XYZ");
  tdrStyle->SetTitleOffset(1.20, "X");
  tdrStyle->SetTitleOffset(1.10, "Y"); 

  //For the axis labels:
  tdrStyle->SetLabelColor(1, "XYZ");
  tdrStyle->SetLabelFont(42, "XYZ");
  tdrStyle->SetLabelOffset(0.007, "XYZ");
  tdrStyle->SetLabelSize(0.05, "XYZ");

  //For the axis:
  tdrStyle->SetAxisColor(1, "XYZ");
  tdrStyle->SetStripDecimals(kTRUE);
  tdrStyle->SetTickLength(0.03, "XYZ");
  tdrStyle->SetNdivisions(510, "XYZ");
  tdrStyle->SetPadTickX(1);  // To get tick marks on the opposite side of the frame
  tdrStyle->SetPadTickY(1);

  //Change for log plots:
  tdrStyle->SetOptLogx(0);
  tdrStyle->SetOptLogy(0);
  tdrStyle->SetOptLogz(0);

  //Postscript options:
  tdrStyle->SetPaperSize(20.,20.);
  tdrStyle->cd();
}

void dataMCplotMaker(TH1F* Data, std::vector <TH1F*> Backgrounds, std::vector <char*> Titles, char* title, char* title2, std::string options_string, std::vector <Color_t> color_input){

  //Turn "options" string into vector of strings, one parm each
  std::vector <std::string> Options = GetParms(options_string);  

  //Default values of all user-adjustable parameters
  bool linear = 0;
  bool doOverflow = 1;
  bool showXaxisUnit = 1;
  char* energy = "8";
  char* lumi = "19.5";
  char* yAxisLabel = "Entries";
  char* yAxisUnit = "";
  char* yAxisOverride = "";
  char* xAxisLabel = "M_{T}";
  char* xAxisUnit = "GeV";
  char* xAxisOverride = "";
  char* dataName = "data";
  char* topYaxisTitle = "data/SM";
  char* overrideHeader = "";
  char* type = "CMS Preliminary                ";
  char* outputName = "data_MC_plot";
  bool preserveBackgroundOrder = 0;
  bool showDivisionLabel = 1;
  float setMaximum = -1;
  float setMinimum = -1;
  float legendUp = 0;
  float legendRight = 0;
  float legendTextSize = 0.04;
  vector <float> vLines;

  //Loop over options and change default settings to user-defined settings
  for (unsigned int i = 0; i < Options.size(); i++){
    if (Options[i].find("isLinear") < Options[i].length()) linear = 1; 
    else if (Options[i].find("preserveBackgroundOrder") < Options[i].length()) preserveBackgroundOrder = 1; 
    else if (Options[i].find("noDivisionLabel") < Options[i].length()) showDivisionLabel = 0; 
    else if (Options[i].find("noOverflow") < Options[i].length()) doOverflow = 0; 
    else if (Options[i].find("noXaxisUnit") < Options[i].length()) showXaxisUnit = 0; 
    else if (Options[i].find("energy") < Options[i].length()){
      energy = new char[sizeof(getString(Options[i], "energy")) + 1];
      std::strcpy(energy, getString(Options[i], "energy"));
    }
    else if (Options[i].find("lumi") < Options[i].length()){
      lumi = new char[sizeof(getString(Options[i], "lumi")) + 1];
      std::strcpy(lumi, getString(Options[i], "lumi"));
    }
    else if (Options[i].find("yAxisLabel") < Options[i].length()){
      yAxisLabel = new char[sizeof(getString(Options[i], "yAxisLabel")) + 1];
      std::strcpy(yAxisLabel, getString(Options[i], "yAxisLabel"));
    }
    else if (Options[i].find("yAxisUnit") < Options[i].length()){
      yAxisUnit = new char[sizeof(getString(Options[i], "yAxisUnit")) + 1];
      std::strcpy(yAxisUnit, getString(Options[i], "yAxisUnit"));
    }
    else if (Options[i].find("yAxisOverride") < Options[i].length()){
      yAxisOverride = new char[sizeof(getString(Options[i], "yAxisOverride")) + 3];
      std::strcpy(yAxisOverride, getString(Options[i], "yAxisOverride"));
      strcat( yAxisOverride, "  " );
    }
    else if (Options[i].find("xAxisLabel") < Options[i].length()){
      xAxisLabel = new char[sizeof(getString(Options[i], "xAxisLabel")) + 1];
      std::strcpy(xAxisLabel, getString(Options[i], "xAxisLabel"));
    }
    else if (Options[i].find("xAxisUnit") < Options[i].length()){
      xAxisUnit = new char[sizeof(getString(Options[i], "xAxisUnit")) + 1];
      std::strcpy(xAxisUnit, getString(Options[i], "xAxisUnit"));
    }
    else if (Options[i].find("xAxisOverride") < Options[i].length()){
      xAxisOverride = new char[sizeof(getString(Options[i], "xAxisOverride")) + 1];
      strcat( xAxisOverride, "  " );
      std::strcpy(xAxisOverride, getString(Options[i], "xAxisOverride"));
    }
    else if (Options[i].find("dataName") < Options[i].length()){
      dataName = new char[sizeof(getString(Options[i], "dataName")) + 1];
      std::strcpy(dataName, getString(Options[i], "dataName"));
    }
    else if (Options[i].find("topYaxisTitle") < Options[i].length()){
      topYaxisTitle = new char[sizeof(getString(Options[i], "topYaxisTitle")) + 1];
      std::strcpy(topYaxisTitle, getString(Options[i], "topYaxisTitle"));
    }
    else if (Options[i].find("type") < Options[i].length()){
      type = new char[sizeof(getString(Options[i], "type")) + 1];
      std::strcpy(type, getString(Options[i], "type"));
    }
    else if (Options[i].find("overrideHeader") < Options[i].length()){
      overrideHeader = new char[sizeof(getString(Options[i], "overrideHeader")) + 1];
      std::strcpy(overrideHeader, getString(Options[i], "overrideHeader"));
    }
    else if (Options[i].find("outputName") < Options[i].length()){
      outputName = new char[sizeof(getString(Options[i], "outputName")) + 1];
      std::strcpy(outputName, getString(Options[i], "outputName"));
    }
    else if (Options[i].find("vLine") < Options[i].length()){
      vLines.push_back(atof( getString(Options[i], "vLine") ));
    }
    else if (Options[i].find("setMaximum") < Options[i].length()){
      setMaximum = atof( getString(Options[i], "setMaximum") );
    }
    else if (Options[i].find("legendUp") < Options[i].length()){
      legendUp = atof( getString(Options[i], "legendUp") );
    }
    else if (Options[i].find("legendRight") < Options[i].length()){
      legendRight = atof( getString(Options[i], "legendRight") );
    }
    else if (Options[i].find("legendTextSize") < Options[i].length()){
      legendTextSize = atof( getString(Options[i], "legendTextSize") );
    }
    else if (Options[i].find("setMinimum") < Options[i].length()){
      setMinimum = atof( getString(Options[i], "setMinimum") );
    }
    else cout << "Warning: Option not recognized!  Option: " << Options[i] << endl;
  }

  //Set Style
  SetTDRStyle();
  gStyle->SetOptStat(0);

  //Make sure there is at least one background and enough titles
  if (Backgrounds.size() < 1){ 
    cout << "Error: Running with no backgrounds!" << endl;
    return;
  }
  if (Titles.size() != Backgrounds.size()) cout << "Warning! Not enough titles for your backgrounds!" << endl;
  while (Titles.size() < Backgrounds.size()) Titles.push_back("");

  //Format Titles
  for (unsigned int i = 0; i < Titles.size(); i++){
    string title_temp = Titles[i];
    if (title_temp == "ttsl" || title_temp == "1ltop" || title_temp == "1lep" || title_temp == "singlelep" || title_temp == "singlelepton") Titles[i] = "1 #font[12]{l} top";
    if (title_temp == "ttdl" || title_temp == "2lep" || title_temp == "dilep" || title_temp == "dilepton") Titles[i] = "t#bar{t}#rightarrow #font[12]{ll}";
    if (title_temp == "wjets") Titles[i] = "W+jets"; 
  }

  //Do Overflow
  if (doOverflow == 1){
    for (unsigned int i = 0; i < Backgrounds.size(); i++){
      Backgrounds[i]->SetBinContent(Backgrounds[i]->GetNbinsX(), Backgrounds[i]->GetBinContent(Backgrounds[i]->GetNbinsX())+Backgrounds[i]->GetBinContent(Backgrounds[i]->GetNbinsX()+1) );
    }
    Data->SetBinContent(Data->GetNbinsX(), Data->GetBinContent(Data->GetNbinsX())+Data->GetBinContent(Data->GetNbinsX()+1) );
  }

  //Sort Backgrounds and Titles
  if (preserveBackgroundOrder == 0){
    std::vector<PlotInfo> myPlotInfo;
 
    for (unsigned int i = 0; i < Backgrounds.size(); i++){
      PlotInfo temp = {Backgrounds[i], Titles[i]};
      myPlotInfo.push_back(temp);
    }

    sort(myPlotInfo.begin(), myPlotInfo.end(), Integral);
    Backgrounds.clear();
    Titles.clear();

    for (unsigned int i = 0; i < myPlotInfo.size(); i++){
      Backgrounds.push_back(myPlotInfo[i].Plot);
      Titles.push_back(myPlotInfo[i].Title);
    }
  }

  //Set colors for histograms
  std::vector <Color_t> Colors;
  if (color_input.size() == 0){ 
    Colors.push_back(kMagenta-5);
    Colors.push_back(kCyan-3);
    Colors.push_back(kOrange-2);
    Colors.push_back(kRed-7);
    Colors.push_back(kGreen-2);
  }
  else{
    Colors = color_input;
    while (Colors.size() < Backgrounds.size()){
      Colors.push_back(kWhite);
    }
  }

  //Draw histogram with two pads
  TCanvas *c0 = new TCanvas("c0", "c0", 600, 700);
  TPad* finPad[2];
  finPad[0] = new TPad("1", "1", 0.0, 0.0, 1.0, 0.885);
  if (!linear) finPad[0]->SetLogy();
  finPad[1] = new TPad("2", "2", 0.0, 0.8, 1.0, 0.95);
  finPad[0]->Draw();
  finPad[1]->Draw();
  finPad[0]->cd();

  //First pad: histogram
  THStack *stack = new THStack("stack", ""); 
  Data->SetMarkerStyle(20);
  Data->UseCurrentStyle();
  for (unsigned int i = 0; i < Backgrounds.size(); i++){
    Backgrounds[i]->UseCurrentStyle();
    Backgrounds[i]->SetFillColor(Colors[i]);
    stack->Add(Backgrounds[i]);
  }
  if (setMinimum != -1) stack->SetMinimum(setMinimum);
  if (setMinimum == -1 && !linear && Backgrounds[0]->GetMinimum() > 0) stack->SetMinimum(min(1.0, 0.9*Backgrounds[0]->GetMinimum()));
  else if (setMinimum == -1 && !linear && stack->GetMinimum() > 0) stack->SetMinimum(min(1.0, 0.1*stack->GetMinimum()));
  else if (setMinimum == -1 && !linear) stack->SetMinimum(0.5);
  if (setMinimum == -1 && linear) stack->SetMinimum(0);
  gStyle->SetTitleFontSize(0.03);
  stack->Draw();
  float myMax = 0;
  if (setMaximum != -1) myMax = setMaximum;
  else if (setMaximum == -1 && !linear && stack->GetMinimum() > 0) myMax = pow(stack->GetMinimum(), -1.0/3.0) * pow(AdjustedMaximum(Backgrounds), 4.0/3.0);
  else if (setMaximum == -1 && linear && stack->GetMinimum() > 0)  myMax = (AdjustedMaximum(Backgrounds))*(4.0/3.0) - (stack->GetMinimum())*(1.0/3.0);
  else if (!linear) myMax = stack->GetMaximum()*20.0;
  else myMax = stack->GetMaximum()*2;
  stack->SetMaximum(myMax);
  if (yAxisOverride != "") stack->GetYaxis()->SetTitle(Form("%s", yAxisOverride));
  if (yAxisOverride == "" && showDivisionLabel && yAxisUnit != "") stack->GetYaxis()->SetTitle(Form("%s [%s] / %.0f %s  ", yAxisLabel, yAxisUnit, Backgrounds[0]->GetXaxis()->GetBinWidth(1), xAxisUnit));
  if (yAxisOverride == "" && showDivisionLabel && yAxisUnit == "") stack->GetYaxis()->SetTitle(Form("%s / %.0f %s  ", yAxisLabel, Backgrounds[0]->GetXaxis()->GetBinWidth(1), xAxisUnit));
  if (yAxisOverride == "" && !showDivisionLabel && yAxisUnit != "") stack->GetYaxis()->SetTitle(Form("%s [%s]  ", yAxisLabel, yAxisUnit));
  if (yAxisOverride == "" && !showDivisionLabel && yAxisUnit == "") stack->GetYaxis()->SetTitle(Form("%s  ", yAxisLabel));
  if (xAxisOverride == "" && showXaxisUnit == 0) stack->GetXaxis()->SetTitle(Form("%s", xAxisLabel));
  if (xAxisOverride == "" && showXaxisUnit == 1) stack->GetXaxis()->SetTitle(Form("%s [%s]", xAxisLabel, xAxisUnit));
  if (xAxisOverride != "") stack->GetXaxis()->SetTitle(Form("%s", xAxisOverride));
  stack->GetYaxis()->SetTitleOffset(1.3);
  stack->Draw();
  THStack *stack2 = new THStack("stack2", "mtInCR1_data"); 
  stack2->Add(Data);
  stack2->Draw("PSAMEE");
  TLegend *leg = new TLegend(0.7+legendRight,0.59+legendUp,0.92+legendRight,0.87+legendUp);
  leg->SetTextSize(legendTextSize);
  leg->AddEntry(Data, dataName, "lp");
  for (int i = Titles.size()-1; i > -1; i--){
    leg->AddEntry(Backgrounds[i], Titles[i], "f");
  }
  leg->SetFillStyle(0);
  leg->SetBorderSize(0);
  leg->Draw();
  TLatex *tex = new TLatex();
  tex->SetNDC();
  tex->SetTextSize(0.04);
  tex->DrawLatex(0.2,0.83,title);
  tex->DrawLatex(0.2,0.78,title2);
  for (unsigned int i = 0; i < vLines.size(); i++){
    TLine linecut;
    c0->Update();
    linecut.SetLineStyle(2);
    linecut.SetLineWidth(2);
    linecut.SetLineColor(kGray+2);
    linecut.DrawLine(vLines[i],0.,vLines[i],100000000000000);
  }
  c0->cd();
  tex->SetTextSize(0.035);
  if (overrideHeader == "") tex->DrawLatex(0.17,0.962,Form("%s #sqrt{s} = %s TeV,  #scale[0.6]{#int}Ldt = %s fb^{-1}", type, energy, lumi));
  if (overrideHeader != "") tex->DrawLatex(0.17,0.962,Form("%s", overrideHeader));
  finPad[1]->cd();

  //Second pad: data/MC yields
  TH1F* err_hist = (TH1F*)Backgrounds[0]->Clone(); 
  err_hist->SetTitle("");
  err_hist->Draw();
  err_hist->GetYaxis()->SetTitle("Data/MC");
  err_hist->GetYaxis()->SetTitleSize(0.20);
  err_hist->GetYaxis()->SetTitleOffset(1.3);
  for (int ib = 1; ib < err_hist->GetNbinsX()+1; ib++){
    float MC_value = 0;
    float MC_error_2 = 0;
    for (unsigned int i = 0; i < Backgrounds.size(); i++){
      MC_value += Backgrounds[i]->GetBinContent(ib);
      MC_error_2 += pow(Backgrounds[i]->GetBinError(ib), 2);
    }
    float data_value = Data->GetBinContent(ib);
    float value = (MC_value > 0 ? data_value/MC_value : 1000);
    err_hist->SetBinContent(ib, value);
    float MC_err = sqrt(MC_error_2);
    float data_err = Data->GetBinError(ib);
    err_hist->SetBinError(ib, value * sqrt( pow(MC_err/MC_value, 2) + pow(data_err/data_value, 2) ));
  }
  err_hist->SetMarkerStyle(20);
  err_hist->Draw("PE");
  TText *blah = new TText();
  blah->SetTextFont(42);
  blah->SetTextSize(0.25);
  blah->SetTextAngle(90);
  blah->DrawTextNDC(0.065,0.06,topYaxisTitle);
  TLine line;
  line.SetLineColor(kGray+2);
  line.SetLineWidth(2);
  int maxbin = err_hist->GetXaxis()->GetNbins();
  line.DrawLine(err_hist->GetXaxis()->GetBinLowEdge(1),1,err_hist->GetXaxis()->GetBinUpEdge(maxbin),1);
  err_hist->Draw("PESAME");
  err_hist->GetXaxis()->SetLabelSize(0);
  err_hist->GetYaxis()->SetLabelSize(0.2);
  err_hist->GetYaxis()->SetRangeUser(0., 2.);
  err_hist->GetYaxis()->SetNdivisions(505);

  //Print plot as pdf 
  c0->Print(Form("%s.pdf", outputName));
}
