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
TH1F *null = new TH1F("", "", 1,0,1);
float AdjustedMaximum(std::vector <TH1F*> Plots, TH1F* data = null){
  vector <float> heights;
  for (int i = 0; i < Plots[0]->GetNbinsX()+2; i++){
    float temp = 0;
    for (unsigned int j = 0; j < Plots.size(); j++){
      temp += Plots[j]->GetBinContent(i)+Plots[j]->GetBinError(i);
    }
    heights.push_back(temp);
  }
  std::sort( heights.begin(), heights.end() );
  float bkgd_height = heights[heights.size()-1];
  heights.clear();
  for (int i = 0; i < data->GetNbinsX()+2; i++){
    heights.push_back(data->GetBinContent(i)+data->GetBinError(i));
  }
  std::sort( heights.begin(), heights.end() );
  float data_height = heights[heights.size()-1];
  return max(data_height, bkgd_height);
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

  tdrStyleAG = new TStyle("tdrStyleAGAG","Style for P-TDR");

  // For the canvas:
  tdrStyleAG->SetCanvasBorderMode(0);
  tdrStyleAG->SetCanvasColor(kWhite);
  tdrStyleAG->SetCanvasDefH(600); //Height of canvas
  tdrStyleAG->SetCanvasDefW(800); //Width of canvas
  tdrStyleAG->SetCanvasDefX(0);   //Position on screen
  tdrStyleAG->SetCanvasDefY(0);

  // For the Pad:
  tdrStyleAG->SetPadBorderMode(0);
  tdrStyleAG->SetPadColor(kWhite);
  tdrStyleAG->SetPadGridX(false);
  tdrStyleAG->SetPadGridY(false);
  tdrStyleAG->SetGridColor(0);
  tdrStyleAG->SetGridStyle(3);
  tdrStyleAG->SetGridWidth(1);

  // For the frame:
  tdrStyleAG->SetFrameBorderMode(0);
  tdrStyleAG->SetFrameBorderSize(1);
  tdrStyleAG->SetFrameFillColor(0);
  tdrStyleAG->SetFrameFillStyle(0);
  tdrStyleAG->SetFrameLineColor(1);
  tdrStyleAG->SetFrameLineStyle(1);
  tdrStyleAG->SetFrameLineWidth(1);

  //For the histo:
  tdrStyleAG->SetHistLineColor(1);
  tdrStyleAG->SetHistLineWidth(2);

  tdrStyleAG->SetEndErrorSize(2);
  tdrStyleAG->SetMarkerStyle(20);

  //For the fit/function:
  tdrStyleAG->SetOptFit(1);
  tdrStyleAG->SetFitFormat("5.4g");
  tdrStyleAG->SetFuncColor(2);
  tdrStyleAG->SetFuncStyle(1);
  tdrStyleAG->SetFuncWidth(1);

  //For the date:
  tdrStyleAG->SetOptDate(0);

  //For the statistics box:
  tdrStyleAG->SetOptFile(0);
  tdrStyleAG->SetOptStat(0); // To display the mean and RMS:   SetOptStat("mr");
  tdrStyleAG->SetOptFit(0); // To display the mean and RMS:   SetOptStat("mr");
  tdrStyleAG->SetStatColor(kWhite);
  tdrStyleAG->SetStatFont(42);
  tdrStyleAG->SetStatFontSize(0.025);
  tdrStyleAG->SetStatTextColor(1);
  tdrStyleAG->SetStatFormat("6.4g");
  tdrStyleAG->SetStatBorderSize(1);
  tdrStyleAG->SetStatH(0.1);
  tdrStyleAG->SetStatW(0.15);

  //Margins:
  tdrStyleAG->SetPadTopMargin(0.10);
  tdrStyleAG->SetPadBottomMargin(0.15);
  tdrStyleAG->SetPadLeftMargin(0.15);
  tdrStyleAG->SetPadRightMargin(0.04);

  //For the Global title:
  tdrStyleAG->SetOptTitle(1);
  tdrStyleAG->SetTitleFont(42);
  tdrStyleAG->SetTitleColor(1);
  tdrStyleAG->SetTitleTextColor(1);
  tdrStyleAG->SetTitleFillColor(10);
  tdrStyleAG->SetTitleFontSize(0.05);
  tdrStyleAG->SetTitleX(0.5); // Set the position of the title box
  tdrStyleAG->SetTitleY(0.985); // Set the position of the title box
  tdrStyleAG->SetTitleAlign(23);
  tdrStyleAG->SetTitleStyle(0);
  tdrStyleAG->SetTitleBorderSize(0);
  tdrStyleAG->SetTitleFillColor(0);

  //For the axis titles:
  tdrStyleAG->SetTitleColor(1, "XYZ");
  tdrStyleAG->SetTitleFont(42, "XYZ");
  tdrStyleAG->SetTitleSize(0.05, "XYZ");
  tdrStyleAG->SetTitleOffset(1.20, "X");
  tdrStyleAG->SetTitleOffset(1.10, "Y"); 

  //For the axis labels:
  tdrStyleAG->SetLabelColor(1, "XYZ");
  tdrStyleAG->SetLabelFont(42, "XYZ");
  tdrStyleAG->SetLabelOffset(0.007, "XYZ");
  tdrStyleAG->SetLabelSize(0.05, "XYZ");

  //For the axis:
  tdrStyleAG->SetAxisColor(1, "XYZ");
  tdrStyleAG->SetStripDecimals(kTRUE);
  tdrStyleAG->SetTickLength(0.03, "XYZ");
  tdrStyleAG->SetNdivisions(510, "XYZ");
  tdrStyleAG->SetPadTickX(1);  // To get tick marks on the opposite side of the frame
  tdrStyleAG->SetPadTickY(1);

  //Change for log plots:
  tdrStyleAG->SetOptLogx(0);
  tdrStyleAG->SetOptLogy(0);
  tdrStyleAG->SetOptLogz(0);

  //Postscript options:
  tdrStyleAG->SetPaperSize(20.,20.);
  tdrStyleAG->cd();
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
  char* type = "CMS Preliminary                                             ";
  char* outputName = "data_MC_plot";
  bool preserveBackgroundOrder = 0;
  bool showDivisionLabel = 1;
  float setMaximum = -1;
  float setMinimum = -1;
  float legendUp = 0;
  float legendRight = 0;
  float legendTextSize = 0.04;
  vector <float> vLines;
  bool doHalf = 0;
  Int_t nDivisions = -1;

  //Loop over options and change default settings to user-defined settings
  for (unsigned int i = 0; i < Options.size(); i++){
    if (Options[i].find("isLinear") < Options[i].length()) linear = 1; 
    else if (Options[i].find("preserveBackgroundOrder") < Options[i].length()) preserveBackgroundOrder = 1; 
    else if (Options[i].find("noDivisionLabel") < Options[i].length()) showDivisionLabel = 0; 
    else if (Options[i].find("noOverflow") < Options[i].length()) doOverflow = 0; 
    else if (Options[i].find("noXaxisUnit") < Options[i].length()) showXaxisUnit = 0; 
    else if (Options[i].find("divHalf") < Options[i].length()) doHalf = 1; 
    else if (Options[i].find("energy") < Options[i].length()){
      energy = new char[strlen(getString(Options[i], "energy")) + 1];
      std::strcpy(energy, getString(Options[i], "energy"));
    }
    else if (Options[i].find("lumi") < Options[i].length()){
      lumi = new char[strlen(getString(Options[i], "lumi")) + 1];
      std::strcpy(lumi, getString(Options[i], "lumi"));
    }
    else if (Options[i].find("yAxisLabel") < Options[i].length()){
      yAxisLabel = new char[strlen(getString(Options[i], "yAxisLabel")) + 1];
      std::strcpy(yAxisLabel, getString(Options[i], "yAxisLabel"));
    }
    else if (Options[i].find("yAxisUnit") < Options[i].length()){
      yAxisUnit = new char[strlen(getString(Options[i], "yAxisUnit")) + 1];
      std::strcpy(yAxisUnit, getString(Options[i], "yAxisUnit"));
    }
    else if (Options[i].find("yAxisOverride") < Options[i].length()){
      yAxisOverride = new char[strlen(getString(Options[i], "yAxisOverride")) + 3];
      std::strcpy(yAxisOverride, getString(Options[i], "yAxisOverride"));
      strcat( yAxisOverride, "  " );
    }
    else if (Options[i].find("xAxisLabel") < Options[i].length()){
      xAxisLabel = new char[strlen(getString(Options[i], "xAxisLabel")) + 1];
      std::strcpy(xAxisLabel, getString(Options[i], "xAxisLabel"));
    }
    else if (Options[i].find("xAxisUnit") < Options[i].length()){
      xAxisUnit = new char[strlen(getString(Options[i], "xAxisUnit")) + 1];
      std::strcpy(xAxisUnit, getString(Options[i], "xAxisUnit"));
    }
    else if (Options[i].find("xAxisOverride") < Options[i].length()){
      xAxisOverride = new char[strlen(getString(Options[i], "xAxisOverride")) + 1];
      strcat( xAxisOverride, "  " );
      std::strcpy(xAxisOverride, getString(Options[i], "xAxisOverride"));
    }
    else if (Options[i].find("dataName") < Options[i].length()){
      dataName = new char[strlen(getString(Options[i], "dataName")) + 1];
      std::strcpy(dataName, getString(Options[i], "dataName"));
    }
    else if (Options[i].find("topYaxisTitle") < Options[i].length()){
      topYaxisTitle = new char[strlen(getString(Options[i], "topYaxisTitle")) + 1];
      std::strcpy(topYaxisTitle, getString(Options[i], "topYaxisTitle"));
    }
    else if (Options[i].find("type") < Options[i].length()){
      type = new char[strlen(getString(Options[i], "type")) + 1];
      std::strcpy(type, getString(Options[i], "type"));
    }
    else if (Options[i].find("overrideHeader") < Options[i].length()){
      overrideHeader = new char[strlen(getString(Options[i], "overrideHeader")) + 1];
      std::strcpy(overrideHeader, getString(Options[i], "overrideHeader"));
    }
    else if (Options[i].find("outputName") < Options[i].length()){
      outputName = new char[strlen(getString(Options[i], "outputName")) + 1];
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
    else if (Options[i].find("nDivisions") < Options[i].length()){
      nDivisions = atoi( getString(Options[i], "nDivisions") );
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

  std::vector <Color_t> Colors;

  //Sort Backgrounds, with Titles and Colors
  if (preserveBackgroundOrder == 0){
    std::vector<PlotInfo> myPlotInfo;
 
    for (unsigned int i = 0; i < Backgrounds.size(); i++){
      PlotInfo temp;
      temp.Plot = Backgrounds[i];
      temp.Title = Titles[i];
      if (color_input.size() > 0){
        temp.Color = color_input[i];
      }
      else{
        temp.Color = kWhite;
      }
      myPlotInfo.push_back(temp);
    }

    std::sort(myPlotInfo.begin(), myPlotInfo.end(), Integral);
    Backgrounds.clear();
    Titles.clear();

    for (unsigned int i = 0; i < myPlotInfo.size(); i++){
      Backgrounds.push_back(myPlotInfo[i].Plot);
      Titles.push_back(myPlotInfo[i].Title);
      if (color_input.size() > 0) Colors.push_back(myPlotInfo[i].Color);
    }
  }

  //Set colors for histograms
  if (color_input.size() == 0){ 
    Colors.push_back(kMagenta-5);
    Colors.push_back(kCyan-3);
    Colors.push_back(kOrange-2);
    Colors.push_back(kRed-7);
    Colors.push_back(kGreen-2);
  }
  else{
    for (unsigned int i = 0; i < Backgrounds.size(); i++){
      Colors.push_back(color_input[i]);
    }
  }

  //Draw histogram with two pads
  TCanvas c0("c0", "c0", 600, 700);
  c0.SetLeftMargin(0.18);
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
  stack->Draw("hist");
  float myMax = 0;
  if (setMaximum != -1) myMax = setMaximum;
  else if (setMaximum == -1 && !linear && stack->GetMinimum() > 0) myMax = pow(stack->GetMinimum(), -1.0/3.0) * pow(AdjustedMaximum(Backgrounds, Data), 4.0/3.0);
  else if (setMaximum == -1 && linear && stack->GetMinimum() > 0)  myMax = (AdjustedMaximum(Backgrounds, Data))*(4.0/3.0) - (stack->GetMinimum())*(1.0/3.0);
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
  stack->Draw("hist");
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
    c0.Update();
    linecut.SetLineStyle(2);
    linecut.SetLineWidth(2);
    linecut.SetLineColor(kGray+2);
    linecut.DrawLine(vLines[i],0.,vLines[i],100000000000000);
  }
  c0.cd();
  tex->SetTextSize(0.035);
  if (overrideHeader == "") tex->DrawLatex(0.17,0.962,Form("%s        #sqrt{s} = %s TeV,  #scale[0.6]{#int}Ldt = %s fb^{-1}", type, energy, lumi));
  if (overrideHeader != "") tex->DrawLatex(0.17,0.962,Form("%s", overrideHeader));
  if (stack->GetMaximum() > 80000 && linear) finPad[0]->SetPad(0.0, 0.0, 1.0, 0.84);
  if (doHalf){
    Int_t sign = (stack->GetXaxis()->GetNdivisions() > 0) ? 1 : -1;
    Int_t orig = abs(stack->GetXaxis()->GetNdivisions());
    Int_t orig_high =orig-orig%100;
    Int_t orig_low = orig%100;
    Int_t new_ndiv = (orig_low%2==1) ? orig_high+orig_low/2 : orig_high + (orig_low-1)/2;
    stack->GetXaxis()->SetNdivisions(new_ndiv, (sign>0) ? kTRUE : kFALSE);
  }
  if (nDivisions != -1 && nDivisions > 0) stack->GetXaxis()->SetNdivisions(nDivisions, kTRUE);
  if (nDivisions != -1 && nDivisions < 0) stack->GetXaxis()->SetNdivisions(nDivisions, kFALSE);
  finPad[1]->cd();

  //Second pad: data/MC yields
  TH1F* err_hist = (TH1F*)Backgrounds[0]->Clone(); 
  err_hist->SetTitle("");
  err_hist->Draw();
  err_hist->GetYaxis()->SetTitle("Data/MC");
  err_hist->GetYaxis()->SetTitleSize(0.20);
  err_hist->GetYaxis()->SetTitleOffset(1.6);
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
  c0.Print(Form("%s.pdf", outputName));
}

void plotMaker(std::vector <TH1F*> Backgrounds, std::vector <char*> Titles, char* title, char* title2, std::string options_string, std::vector <Color_t> color_input){

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
  char* overrideHeader = "";
  char* type = "CMS Preliminary        ";
  char* outputName = "data_MC_plot";
  bool preserveBackgroundOrder = 0;
  bool showDivisionLabel = 1;
  float setMaximum = -1;
  float setMinimum = -1;
  float legendUp = 0;
  float legendRight = 0;
  float legendTextSize = 0.04;
  vector <float> vLines;
  bool doHalf = 0;
  Int_t nDivisions = -1;
  bool noLegend = false;

  //Loop over options and change default settings to user-defined settings
  for (unsigned int i = 0; i < Options.size(); i++){
    if (Options[i].find("isLinear") < Options[i].length()) linear = 1; 
    else if (Options[i].find("noLegend") < Options[i].length()) noLegend = 1; 
    else if (Options[i].find("preserveBackgroundOrder") < Options[i].length()) preserveBackgroundOrder = 1; 
    else if (Options[i].find("noDivisionLabel") < Options[i].length()) showDivisionLabel = 0; 
    else if (Options[i].find("noOverflow") < Options[i].length()) doOverflow = 0; 
    else if (Options[i].find("noXaxisUnit") < Options[i].length()) showXaxisUnit = 0; 
    else if (Options[i].find("divHalf") < Options[i].length()) doHalf = 1; 
    else if (Options[i].find("energy") < Options[i].length()){
      energy = new char[strlen(getString(Options[i], "energy")) + 1];
      std::strcpy(energy, getString(Options[i], "energy"));
    }
    else if (Options[i].find("lumi") < Options[i].length()){
      lumi = new char[strlen(getString(Options[i], "lumi")) + 1];
      std::strcpy(lumi, getString(Options[i], "lumi"));
    }
    else if (Options[i].find("yAxisLabel") < Options[i].length()){
      yAxisLabel = new char[strlen(getString(Options[i], "yAxisLabel")) + 1];
      std::strcpy(yAxisLabel, getString(Options[i], "yAxisLabel"));
    }
    else if (Options[i].find("yAxisUnit") < Options[i].length()){
      yAxisUnit = new char[strlen(getString(Options[i], "yAxisUnit")) + 1];
      std::strcpy(yAxisUnit, getString(Options[i], "yAxisUnit"));
    }
    else if (Options[i].find("yAxisOverride") < Options[i].length()){
      yAxisOverride = new char[strlen(getString(Options[i], "yAxisOverride")) + 3];
      std::strcpy(yAxisOverride, getString(Options[i], "yAxisOverride"));
      strcat( yAxisOverride, "  " );
    }
    else if (Options[i].find("xAxisLabel") < Options[i].length()){
      xAxisLabel = new char[strlen(getString(Options[i], "xAxisLabel")) + 1];
      std::strcpy(xAxisLabel, getString(Options[i], "xAxisLabel"));
    }
    else if (Options[i].find("xAxisUnit") < Options[i].length()){
      xAxisUnit = new char[strlen(getString(Options[i], "xAxisUnit")) + 1];
      std::strcpy(xAxisUnit, getString(Options[i], "xAxisUnit"));
    }
    else if (Options[i].find("xAxisOverride") < Options[i].length()){
      xAxisOverride = new char[strlen(getString(Options[i], "xAxisOverride")) + 1];
      strcat( xAxisOverride, "  " );
      std::strcpy(xAxisOverride, getString(Options[i], "xAxisOverride"));
    }
    else if (Options[i].find("dataName") < Options[i].length()){
      dataName = new char[strlen(getString(Options[i], "dataName")) + 1];
      std::strcpy(dataName, getString(Options[i], "dataName"));
    }
    else if (Options[i].find("type") < Options[i].length()){
      type = new char[strlen(getString(Options[i], "type")) + 1];
      std::strcpy(type, getString(Options[i], "type"));
    }
    else if (Options[i].find("overrideHeader") < Options[i].length()){
      overrideHeader = new char[strlen(getString(Options[i], "overrideHeader")) + 1];
      std::strcpy(overrideHeader, getString(Options[i], "overrideHeader"));
    }
    else if (Options[i].find("outputName") < Options[i].length()){
      outputName = new char[strlen(getString(Options[i], "outputName")) + 1];
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
    else if (Options[i].find("nDivisions") < Options[i].length()){
      nDivisions = atoi( getString(Options[i], "nDivisions") );
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
  }

  std::vector <Color_t> Colors;

  //Sort Backgrounds, with Titles and Colors
  if (preserveBackgroundOrder == 0){
    std::vector<PlotInfo> myPlotInfo;
 
    for (unsigned int i = 0; i < Backgrounds.size(); i++){
      PlotInfo temp;
      temp.Plot = Backgrounds[i];
      temp.Title = Titles[i];
      if (color_input.size() > 0){
        temp.Color = color_input[i];
      }
      else{
        temp.Color = kWhite;
      }
      myPlotInfo.push_back(temp);
    }

    std::sort(myPlotInfo.begin(), myPlotInfo.end(), Integral);
    Backgrounds.clear();
    Titles.clear();

    for (unsigned int i = 0; i < myPlotInfo.size(); i++){
      Backgrounds.push_back(myPlotInfo[i].Plot);
      Titles.push_back(myPlotInfo[i].Title);
      if (color_input.size() > 0) Colors.push_back(myPlotInfo[i].Color);
    }
  }

  //Set colors for histograms
  if (color_input.size() == 0){ 
    Colors.push_back(kMagenta-5);
    Colors.push_back(kCyan-3);
    Colors.push_back(kOrange-2);
    Colors.push_back(kRed-7);
    Colors.push_back(kGreen-2);
  }
  else{
    for (unsigned int i = 0; i < Backgrounds.size(); i++){
      Colors.push_back(color_input[i]);
    }
  }

  //Draw histogram with two pads
  TCanvas c0("c0", "c0", 600, 700);
  if (!linear) c0.SetLogy();
  c0.SetTopMargin(0.07); 
  c0.SetLeftMargin(0.18);

  //Histogram
  THStack *stack = new THStack("stack", ""); 
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
  stack->Draw("hist");
  float myMax = 0;
  if (setMaximum != -1) myMax = setMaximum;
  else if (setMaximum == -1 && !linear && stack->GetMinimum() > 0) myMax = pow(stack->GetMinimum(), -1.0/3.0) * pow(AdjustedMaximum(Backgrounds), 4.0/3.0);
  else if (setMaximum == -1 && linear)  myMax = (AdjustedMaximum(Backgrounds))*(4.0/3.0) - (stack->GetMinimum())*(1.0/3.0);
  else if (!linear) myMax = stack->GetMaximum()*20.0;
  else myMax = stack->GetMaximum()*2;
  stack->SetMaximum(myMax);
  if (yAxisOverride != "") stack->GetYaxis()->SetTitle(Form("%s", yAxisOverride));
  if (yAxisOverride == "" && showDivisionLabel && yAxisUnit != "") stack->GetYaxis()->SetTitle(Form("%s [%s] / %.0f %s  ", yAxisLabel, yAxisUnit, Backgrounds[0]->GetXaxis()->GetBinWidth(1), xAxisUnit));
  if (yAxisOverride == "" && showDivisionLabel && yAxisUnit == "") stack->GetYaxis()->SetTitle(Form("%s / %.0f %s  ", yAxisLabel, Backgrounds[0]->GetXaxis()->GetBinWidth(1), xAxisUnit));
  if (yAxisOverride == "" && showDivisionLabel && Backgrounds[0]->GetXaxis()->GetBinWidth(1) < 1 && yAxisUnit == "") stack->GetYaxis()->SetTitle(Form("%s / %.2f %s  ", yAxisLabel, Backgrounds[0]->GetXaxis()->GetBinWidth(1), xAxisUnit));
  if (yAxisOverride == "" && !showDivisionLabel && yAxisUnit != "") stack->GetYaxis()->SetTitle(Form("%s [%s]  ", yAxisLabel, yAxisUnit));
  if (yAxisOverride == "" && !showDivisionLabel && yAxisUnit == "") stack->GetYaxis()->SetTitle(Form("%s  ", yAxisLabel));
  if (xAxisOverride == "" && showXaxisUnit == 0) stack->GetXaxis()->SetTitle(Form("%s", xAxisLabel));
  if (xAxisOverride == "" && showXaxisUnit == 1) stack->GetXaxis()->SetTitle(Form("%s [%s]", xAxisLabel, xAxisUnit));
  if (xAxisOverride != "") stack->GetXaxis()->SetTitle(Form("%s", xAxisOverride));
  stack->GetYaxis()->SetTitleOffset(1.7);
  stack->Draw("hist");
  TLegend *leg;
  if (Backgrounds.size() == 1) leg = new TLegend(0.7+legendRight,0.79+legendUp,0.92+legendRight,0.87+legendUp);
  else leg = new TLegend(0.7+legendRight,0.59+legendUp,0.92+legendRight,0.87+legendUp);
  leg->SetTextSize(legendTextSize);
  for (int i = Titles.size()-1; i > -1; i--){
    leg->AddEntry(Backgrounds[i], Titles[i], "f");
  }
  leg->SetFillStyle(0);
  leg->SetBorderSize(0);
  if (!noLegend) leg->Draw();
  TLatex *tex = new TLatex();
  tex->SetNDC();
  tex->SetTextSize(0.04);
  tex->DrawLatex(0.23,0.86,title);
  tex->DrawLatex(0.23,0.81,title2);
  for (unsigned int i = 0; i < vLines.size(); i++){
    TLine linecut;
    c0.Update();
    linecut.SetLineStyle(2);
    linecut.SetLineWidth(2);
    linecut.SetLineColor(kGray+2);
    linecut.DrawLine(vLines[i],0.,vLines[i],100000000000000);
  }
  c0.cd();
  tex->SetTextSize(0.035);
  if (overrideHeader == "") tex->DrawLatex(0.17,0.962,Form("%s #sqrt{s} = %s TeV,  #scale[0.6]{#int}Ldt = %s fb^{-1}", type, energy, lumi));
  if (overrideHeader != "") tex->DrawLatex(0.17,0.962,Form("%s", overrideHeader));
  if (doHalf){
    Int_t sign = (stack->GetXaxis()->GetNdivisions() > 0) ? 1 : -1;
    Int_t orig = abs(stack->GetXaxis()->GetNdivisions());
    Int_t orig_high =orig-orig%100;
    Int_t orig_low = orig%100;
    Int_t new_ndiv = (orig_low%2==1) ? orig_high+orig_low/2 : orig_high + (orig_low-1)/2;
    stack->GetXaxis()->SetNdivisions(new_ndiv, (sign>0) ? kTRUE : kFALSE);
  }
  if (nDivisions != -1 && nDivisions > 0) stack->GetXaxis()->SetNdivisions(nDivisions, kTRUE);
  if (nDivisions != -1 && nDivisions < 0) stack->GetXaxis()->SetNdivisions(nDivisions, kFALSE);

  //Print plot as pdf 
  c0.Print(Form("%s.pdf", outputName));
}
