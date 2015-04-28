// Include ROOT classes used below
#include "TF1.h"
#include "TTree.h"
#include "TFile.h"

// These two lines are included inorder to make use of cout.
#include <iostream>
using namespace std;


// some variable used below
Float_t	array_a[1000][1000];
Float_t	array_b[1000][1000];
Float_t	array_c[1000][1000];
Int_t	max_row = 0;
Int_t	max_col = 0;

// some functions used below
void read_text_file(char InputFileName[], Float_t input_array[1000][1000]);

//---------------------------------------------------------------------------//

//! The main function of this little root script
void plotEfVsTh(char InputFileA[], char InputFileB[], char OutPutName[]="pretty.plots.root"){

	read_text_file( InputFileA, array_a);
	read_text_file( InputFileB, array_b);

	for (Int_t i=0; i<max_col; i++){
		array_c[i][0] = 0.1 + i*0.1;
		//cout << " array_c["<<i<<"][0]: " << array_c[i][0] << endl;
	}

	Int_t basket = 64000;
	Float_t value_a, value_b, value_c;

	TTree *plot_tree = new TTree("plot_tree","plot_tree");
	plot_tree->Branch( "det_eff",	&value_a,	"det_eff/F",	basket);
	plot_tree->Branch( "neut_ene",	&value_b,	"neut_ene/F",	basket);
	plot_tree->Branch( "thresh",	&value_c,	"thresh/F",		basket);

	for (Int_t i=0; i<max_row; i++){ //original
	//for (Int_t i=25; i<26; i++){

		for (Int_t j=0; j<max_col; j++){

			value_a = array_a[i][j];
			value_b = array_b[i][0];
			value_c = array_c[j][0];

			plot_tree->Fill();
		}
	}


	TCanvas *myCanvas=new TCanvas();
	myCanvas->Divide(2,4);
	double *Thresholds = new double[8];
	Thresholds[0] = 3.6;
	Thresholds[1] = 3.6;
	Thresholds[2] = 2.3;
	Thresholds[3] = 1.5;
	Thresholds[4] = 1.6;
	Thresholds[5] = 1.4;
	Thresholds[6] = 1.0;
	Thresholds[7] = 0.6;

gROOT->SetStyle("Plain");
gStyle->SetPadTickX(1);
gStyle->SetPadTickY(1);
gStyle->SetPadTopMargin(0.05);
gStyle->SetPadRightMargin(0.05);
gStyle->SetPadBottomMargin(0.16);
gStyle->SetPadLeftMargin(0.12);
gStyle->SetPadBorderMode(0);
gStyle->SetOptStat(0);
gStyle->SetOptTitle(0);

	std::stringstream sstm;
	for(Int_t k=0; k<8; k++){
		sstm.str("");
		sstm << "thresh < " << Thresholds[k] + 0.05 << " && thresh > " << Thresholds[k] - 0.05;
		myCanvas->cd(k+1);
		gPad->SetBottomMargin(0.00);
		gPad->SetTopMargin(0.0);
		gPad->SetRightMargin(0.01);
		//plot_tree->Draw("det_eff:thresh","neut_ene == 10.5 || neut_ene == 15","");//:thresh");
		//plot_tree->Draw("det_eff:neut_ene","thresh < 2.35 && thresh >2.25","l");//:thresh");
		plot_tree->Draw("det_eff:neut_ene", sstm.str().c_str(),"l");//:thresh");
	}
	//cout << " max_row: " << max_row << ", max_col: "<< max_col << endl;


	TFile *fFile = new TFile( OutPutName,"RECREATE","fROOTfile",1);
	plot_tree->Write();
	fFile->Close();


	return;
}

//! Read a text file, save values into an array
void read_text_file(char InputFileName[], Float_t input_array[1000][1000]){


	//cout << "  Looking at file: " << InputFileName <<  endl;

	char	delim[20]=" \t";
	Int_t	linesize = 2000;
	char	line[2000];
	char	OutPutValue[200];

	Int_t	i_row =  0;
	Int_t	i_col =  0;

	FILE	*InputTextFile;


	// Open the text file.
	InputTextFile = fopen( InputFileName, "r");

	//! I'm sure there must be a much better way to do this, but I am too lazy to find it!

	// Loop over each row
	while( fgets( line , linesize, InputTextFile) ){ // jason is a horrible programmer ;(
		//cout << line << endl;

		char *LineChunk;
		LineChunk = strtok (line,delim);

		// Loop over each column.
		i_col=0;
		while (LineChunk != NULL)
		{
			sprintf( OutPutValue,"%s", LineChunk);
			input_array[i_row][i_col] = atof(OutPutValue);
			//cout << " OutPutValue: " << OutPutValue ;
			//cout <<  " input_array["<<i_row<<"]["<<i_col<<"]: " << input_array[i_row][i_col] << endl;

			LineChunk = strtok (0, delim);

			i_col++;
			if(i_col>max_col) max_col=i_col;
		}

		i_row++;
		if(i_row>max_row) max_row=i_row;
	}

	// Close the text file.
	fclose(InputTextFile);

	return;
}

