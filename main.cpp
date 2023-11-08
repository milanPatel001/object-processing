#include <iostream>
#include <fstream>
#include "AutoThreshold.h"
#include "SpatialFiltering.h"
#include "Morphology.h"
using namespace std;

int main(int argc, char** argv){


    ifstream inFile;
    //ofstream outFile;

    inFile.open(argv[1]);
    //outFile.open(argv[2]);
  
    int numRows, numCols,minVal,maxVal;

    inFile>>numRows>>numCols>>minVal>>maxVal;

    SpatialFiltering spatialFiltering(numRows, numCols, minVal, maxVal);

    spatialFiltering.loadImage(inFile, spatialFiltering.mirrorFramedAry);

    spatialFiltering.mirrorFraming(spatialFiltering.mirrorFramedAry);

    //spatialFiltering.imgReformat(spatialFiltering.mirrorFramedAry, outFile);
   
    spatialFiltering.computeAvg5x5(spatialFiltering.mirrorFramedAry, spatialFiltering.avgAry);
    spatialFiltering.computeHist(spatialFiltering.avgAry ,spatialFiltering.histAvgAry);

    string nameAvg = (string) argv[1] + "_Avg5x5.txt";
        
    ofstream avgFile;

    avgFile.open(nameAvg);

    //outFile<<"\n";

    //spatialFiltering.imgReformat(spatialFiltering.avgAry, outFile);
    
    avgFile<<spatialFiltering.numRows<<" "<<spatialFiltering.numCols<<" "<<spatialFiltering.minVal<<" "<<spatialFiltering.maxVal<<"\n";
    
    for(int i=2;i<numRows+2;i++){
        for(int j=2;j<numCols+2;j++){
            avgFile<<spatialFiltering.avgAry[i][j]<<" ";
        }
        avgFile<<"\n";
    }

    string avgHist = (string) argv[1] + "_Avg5x5_hist.txt";
    ofstream histAvgFile;

    histAvgFile.open(avgHist);

    spatialFiltering.printHist(spatialFiltering.histAvgAry , histAvgFile, spatialFiltering.maxVal);

    avgFile.close();
    histAvgFile.close();

    
    inFile.close();
    //outFile.close();


/* -------------------- Thresholding process ---------------------------- */

    int r,c,mini,maxi;

    ifstream histFile, imgFile;

    ofstream outHistFile, outBinFile;

    imgFile.open(nameAvg);
    histFile.open(avgHist);

    outHistFile.open("outHist.txt");
    outBinFile.open("outBin.txt");

    imgFile>>r>>c>>mini>>maxi;

    AutoThreshold autoThreshold(r,c,mini,maxi);

    autoThreshold.loadHistory(histFile);
    autoThreshold.dispHist(outHistFile);
    autoThreshold.biGaussian();

    outHistFile<<"\nBi-Gaussian Threshold Value = "<<autoThreshold.BiGaussThrVal<<"\n\n";

    autoThreshold.thresholdProcess(imgFile, outBinFile, outHistFile, autoThreshold.BiGaussThrVal);

    imgFile.close();
    histFile.close();
    outHistFile.close();
    outBinFile.close();

    /* ------------------------------ Morphological Operations ---------------------------------------*/

    ifstream binImg, elem;
    ofstream morphOutFile;

    binImg.open("outBin.txt");
    elem.open(argv[2]);
    morphOutFile.open("morphOutput.txt");

    binImg>>r>>c>>mini>>maxi;

    int numStructRows,numStructCols,structMin,structMax,rowOrigin,colOrigin;

    elem>>numStructRows>>numStructCols>>structMin>>structMax;
    elem>>rowOrigin>>colOrigin;

    Morphology morphology(r, c, mini, maxi, numStructRows, numStructCols, structMin, structMax ,rowOrigin, colOrigin);

    morphology.zero2DAry(morphology.zeroFramedAry, morphology.rowSize, morphology.colSize);
    morphology.loadImg(binImg, morphology.zeroFramedAry);
    morphology.zero2DAry(morphology.structAry, morphology.numStructRows, morphology.numStructCols);
    morphology.loadstruct(elem, morphology.structAry);
    

    morphology.zero2DAry(morphology.morphAry, morphology.rowSize, morphology.colSize);
    morphology.ComputeOpening(morphology.zeroFramedAry, morphology.morphAry, morphology.tempAry);
   //morphology.prettyPrint(morphology.morphAry, morphOutFile, 0,0,morphology.rowSize,morphology.colSize);

    morphOutFile<<"\n";
    morphology.ObjectDecomp(morphOutFile);


    elem.close();
    binImg.close();
    morphOutFile.close();

    return 0;
   
}   