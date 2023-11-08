#include <iostream>
#include <fstream>
#include "Morphology.h"
using namespace std;

    Morphology::Morphology(int a, int b, int c, int d, int e, int f, int g, int h, int i, int j){
            numImgRows = a;
            numImgCols = b;
            imgMin = c;
            imgMax = d;
            numStructRows = e;
            numStructCols = f;
            structMin = g;
            structMax = h;
            rowOrigin = i;
            colOrigin = j;
        
            
            rowFrameSize = numStructRows/2;
            colFrameSize = numStructCols/2;
            extraRows = rowFrameSize * 2;
            extraCols = colFrameSize * 2;
            rowSize = numImgRows + extraRows;
            colSize = numImgCols + extraCols;

            zeroFramedAry = new int*[rowSize];
            morphAry = new int*[rowSize];
            tempAry = new int*[rowSize];
            structAry = new int*[numStructRows];

            for(int i=0;i<rowSize;i++){
                zeroFramedAry[i] = new int[colSize];
                morphAry[i] = new int[colSize];
                tempAry[i] = new int[colSize];

                for(int j=0;j<colSize;j++){
                    zeroFramedAry[i][j] = 0;
                    morphAry[i][j] = 0;
                    tempAry[i][j] = 0;
                }
            }

            for(int i=0;i<numStructRows;i++){
                structAry[i] = new int[numStructCols];
                for(int j=0;j<numStructCols;j++){
                    structAry[i][j] = 0;
                }
            }
             
    }

    void Morphology::zero2DAry(int** Ary, int nRows, int nCols){
            for(int i=0;i<nRows;i++){
                for(int j=0;j<nCols;j++) Ary[i][j] = 0;
            }
    }

    void Morphology::loadImg(ifstream& imgFile, int** zeroFramedAry){
           for(int i=rowOrigin;i<rowSize-rowOrigin;i++){
                for(int j=colOrigin;j<colSize-colOrigin;j++){
                    imgFile>>zeroFramedAry[i][j];
                }
           }
    }

    void Morphology::loadstruct(ifstream& structFile, int** structAry){
            for(int i=0;i<numStructRows;i++){
                for(int j=0;j<numStructCols;j++){
                    structFile>>structAry[i][j];
                }
            }
    }

    void Morphology::ComputeDilation(int** inAry, int** outAry){
            int i = rowFrameSize;
            int j = colFrameSize;

            while(i<rowSize){
                j = colFrameSize;
                while(j<colSize){
                    if(inAry[i][j]>0) onePixelDilation(i, j, inAry, outAry);
                    j++;
                }
                i++;
            }
    }

    void Morphology::onePixelDilation(int i,int j, int** inAry, int** outAry){
            int iOffset = i - rowOrigin;
            int jOffset = j - colOrigin;

            int rIndex = 0; 
            int cIndex = 0;

            while(rIndex < numStructRows){
                cIndex = 0;
                while(cIndex < numStructCols){
                    if(structAry[rIndex][cIndex] > 0) outAry[iOffset + rIndex][jOffset + cIndex] = 1;
                    cIndex++;
                }
                rIndex++;
            } 
    }

    void Morphology::ComputeErosion(int** inAry, int** outAry){
            int i = rowFrameSize;
            int j = colFrameSize;

            while(i<rowSize){
                j = colFrameSize;
                while(j<colSize){
                    if(inAry[i][j]>0) onePixelErosion(i, j, inAry, outAry);
                    j++;
                }
                i++;
            }
    }

    void Morphology::onePixelErosion(int i,int j, int** inAry, int** outAry){
            int iOffset = i - rowOrigin;
            int jOffset = j - colOrigin;

            bool matchFlag = true;

            int rIndex = 0; 
            int cIndex = 0;

            while(matchFlag==true && rIndex < numStructRows){
                cIndex = 0;
                while(matchFlag==true && cIndex < numStructCols){
                    if(structAry[rIndex][cIndex] > 0 && inAry[iOffset + rIndex][jOffset+cIndex]<=0){ 
                        matchFlag = false;
                    }
                    
                    cIndex++;
                }
                rIndex++;
            } 

            if(matchFlag==true) outAry[i][j] = 1;
            else outAry[i][j] = 0;
    }

    void Morphology::ComputeOpening(int** zeroFramedAry, int** morphAry, int** tempAry){
            ComputeErosion(zeroFramedAry, tempAry);
            ComputeDilation(tempAry, morphAry);
    }

    void Morphology::ComputeClosing(int** zeroFramedAry, int** morphAry, int** tempAry){
            ComputeDilation(zeroFramedAry, tempAry);
            ComputeErosion(tempAry, morphAry);
    }

    void Morphology::prettyPrint(int** Ary, ofstream& outFile, int startRow, int startCol ,int endRow, int endCol){
          
           for(int i=startRow;i<endRow;i++){
                for(int j=startCol;j<endCol;j++){
                    outFile <<(Ary[i][j] == 0?". ":"1 ");  
                
                }
                outFile<<"\n";
          
           } 
    }
    void Morphology::AryToFile(int** Ary, ofstream& outFile){
            
            outFile<<numImgRows<<" "<<numImgCols<<" "<<imgMin<<" "<<imgMax<<"\n";
 
            for(int i=0;i<numImgRows;i++){
                for(int j=0;j<numImgCols;j++){
                    outFile<<Ary[i][j];
                }
                outFile<<"\n";
            }
        }
  
    void Morphology::ObjectDecomp(ofstream& outFile){

            int** subtractArr = new int*[rowSize];

            for(int i=0;i<rowSize;i++){
                subtractArr[i] = new int[colSize];
          
                for(int j=0;j<colSize;j++) {
                    subtractArr[i][j] = 0;
                   
                }
            }
          
   
            for(int i=0;i<rowSize;i++){
                for(int j=0;j<colSize;j++){

                    if(morphAry[i][j]==1 && zeroFramedAry[i][j]==1 || zeroFramedAry[i][j]==0) subtractArr[i][j] = 0;
                    else subtractArr[i][j] = 1;
                }
            }


            prettyPrint(subtractArr, outFile,0 ,0,rowSize,colSize);

        }