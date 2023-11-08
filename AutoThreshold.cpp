#include "AutoThreshold.h"
#include <iostream>
#include <fstream>
#include <cmath>
using namespace std;

AutoThreshold::AutoThreshold(int r, int c, int min, int max){
        numRows = r;
        numCols = c;
        minVal = min;
        maxVal = max;

        GaussAry = new int[maxVal+1];
        histAry = new int[maxVal+1];
}

void AutoThreshold::loadHistory(ifstream& inFile){
        int maximum = -1;

        int index;

        while(inFile>>index){
            inFile>>histAry[index];
            maximum = max(maximum, histAry[index]);
        }

        maxHeight = maximum;
}

void AutoThreshold::dispHist(ofstream& outFile){
        outFile <<numRows<<" "<<numCols<<" "<<minVal<<" "<<maxVal<<"\n";

        for(int i=0;i<maxVal+1;i++){
            outFile<<i<<" ("<<histAry[i]<<"): ";

            for(int j=0;j<histAry[i];j++){
                outFile<<"+";
            }

            outFile<<"\n";
        }
}

void AutoThreshold::setZero(int* Ary){
        for(int i=0;i<maxVal+1;i++) Ary[i] = 0;
}

void AutoThreshold::biGaussian(){
       
        double sum1, sum2, total, minSumDiff;
        int offSet = (int)(maxVal-minVal)/10;
        int dividePt = offSet;
        int bestThr = dividePt;
        minSumDiff = 99999.0;

        while(dividePt < (maxVal-offSet)) {
            setZero(GaussAry);
            sum1 = fitGauss(0, dividePt, histAry, GaussAry);
            sum2 = fitGauss(dividePt, maxVal, histAry, GaussAry);

            total = sum1 + sum2;

            if (total < minSumDiff) {
                minSumDiff = total;
                bestThr = dividePt;
            }

            dividePt++;
        }

        BiGaussThrVal = bestThr;
}

double AutoThreshold::computeMean(int leftIndex, int rightIndex, int maxHeight, int* histAry){
   
        maxHeight = 0;
        int sum = 0;
        int numPixels = 0;

        int index = leftIndex;

        while(index< rightIndex) {
            sum += (histAry[index] * index);
            numPixels += histAry[index];

            if (histAry[index] > maxHeight) maxHeight = histAry[index];

            index++;
        }

        double result = (double) sum/(double) numPixels;
       
        return result;
}

double AutoThreshold::computeVar(int leftIndex, int rightIndex, double mean, int* histAry){
       
        double sum = 0.0;
        int numPixels = 0;

        int index = leftIndex;

        while(index < rightIndex) {
            sum += (double) histAry[index] * ((double) index - mean) * ((double) index - mean);
            numPixels += histAry[index];
            index++;
        }

        double result = sum/(double) numPixels;

     
        return result;
}

double AutoThreshold::modifiedGauss(int x, double mean, double var, int maxHeight){

            double temp = (double) (x-mean) * (double) (x-mean);
           return (double) (maxHeight * exp(-1*temp/(2*var)));
}

double AutoThreshold::fitGauss(int leftIndex, int rightIndex, int* histAry, int* GaussAry){
      
        double mean,var,sum=0.0,Gval,maxGval;

        mean = computeMean(leftIndex, rightIndex, maxHeight, histAry);
        var = computeVar(leftIndex, rightIndex, mean, histAry);

    
        int index = leftIndex;

        while(index<=rightIndex){
            Gval = modifiedGauss(index, mean, var, maxHeight);
            sum += abs(Gval - (double)histAry[index] );
            GaussAry[index] = (int)Gval;
            index++;
        }
      
        return sum;
}

void AutoThreshold::thresholdProcess(ifstream &inFile, ofstream &outFile1, ofstream &outFile2, int thrValue){
        outFile1<<numRows<<" "<<numCols<<" 0 1"<<"\n";
        outFile2<<numRows<<" "<<numCols<<" "<<minVal<<" "<<maxVal<<"\n";

        int pixelVal, count = 0;

        while(inFile>>pixelVal){
            if(pixelVal >= thrValue){
                outFile1<<"1 ";
                outFile2<<pixelVal<<" ";
            }else{
                outFile1<<"0 ";
                outFile2<<"0 ";
            }

            count++;
            if(count >= numCols){
                outFile1<<"\n"; 
                outFile2<<"\n";
                count = 0; 
            }
        }
}