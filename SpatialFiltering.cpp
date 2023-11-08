#include <iostream>
#include <fstream>
#include "SpatialFiltering.h"
using namespace std;


    SpatialFiltering::SpatialFiltering(){}

    SpatialFiltering::SpatialFiltering(int r, int c, int min, int max){
            numRows = r;
            numCols = c;
            minVal = min;
            maxVal = max;
            
            maskRows = 5;
            maskCols = 5;
            maskMin = 2;
            maskMax = 24;


            int ary[] = {  
            2, 4, 8, 4, 2,
            4, 8, 12, 8, 4,
            8, 12, 24, 12, 8,
            4, 8, 12, 8, 4,
            2, 4, 8, 4, 2
            };

            maskAry = ary;

            neighborAry = new int[maskRows*maskCols];

            maskWeight = 0;

            for(int i=0;i<maskRows*maskCols;i++){
                neighborAry[i] = 0;
                maskWeight += maskAry[i];
            }

            mirrorFramedAry = new int*[numRows+4];
            avgAry = new int*[numRows+4];
            GaussAry = new int*[numRows+4];

            for(int i=0;i<numRows+4;i++){
                mirrorFramedAry[i] = new int[numCols+4];
                avgAry[i] = new int[numCols+4];
                GaussAry[i] = new int[numCols+4];

                for(int j=0; j<numCols+4;j++){
                    mirrorFramedAry[i][j] = 0;  
                    avgAry[i][j] = 0;
                    GaussAry[i][j] = 0;  
                }
            }

    
            histAvgAry = new int[maxVal+1];
            histGaussAry = new int[maxVal+1];

            for(int i=0;i<maxVal+1;i++){
                histAvgAry[i] = 0;
                histGaussAry[i] = 0;
            }
    }

    void SpatialFiltering::loadImage(ifstream &inFile, int** mirrorFramedAry){
            for(int i=2;i<numRows+2;i++){
                for(int j=2;j<numCols+2;j++){
                    inFile>>mirrorFramedAry[i][j];
                }
            }
    }

    void SpatialFiltering::mirrorFraming(int** mirrorFramedAry){
            for(int col=2;col<numCols+2;col++){
                mirrorFramedAry[1][col] = mirrorFramedAry[2][col]; 
                mirrorFramedAry[0][col] = mirrorFramedAry[2][col]; 

                mirrorFramedAry[numRows+3][col] = mirrorFramedAry[numRows+1][col];
                mirrorFramedAry[numRows+2][col] = mirrorFramedAry[numRows+1][col];
            }

            for(int row=2; row < numRows+2; row++){
                mirrorFramedAry[row][0] = mirrorFramedAry[row][2];
                mirrorFramedAry[row][1] = mirrorFramedAry[row][2];

                mirrorFramedAry[row][numCols+3] = mirrorFramedAry[row][numCols+1];
                mirrorFramedAry[row][numCols+2] = mirrorFramedAry[row][numCols+1];
            }
        
    }


    void SpatialFiltering::loadNeighborAry(int** mirrorFramedAry, int i, int j, int* neighborAry){
                
                int startingPosX = i-2;
                int startingPosY = j-2;
                int index = 0;

                for(int x=startingPosX; x<startingPosX+5; x++){
                    for(int y=startingPosY; y<startingPosY+5; y++){
                        neighborAry[index] = mirrorFramedAry[x][y];
                        index++;
                    }
                }
    }

    void SpatialFiltering::computeAvg5x5(int** mirrorFramedAry, int** avgAry){
         
            int i = 2;
            int j;
            int newMax = -1;

            while(i<(numRows+2)){
                j = 2;
                while(j<(numCols+2)){
                    avgAry[i][j] = avg5x5(mirrorFramedAry,i,j);
                    newMax = max(newMax, avgAry[i][j]);
                    j++;
                }
                i++;
            }

            maxVal = newMax;
        

    }

    int SpatialFiltering::avg5x5(int** mirrorFramedAry, int i, int j){
                int sum = 0;

                int startingPosX = i-2;
                int startingPosY = j-2;

                for(int x=startingPosX; x<startingPosX+5; x++){
                    for(int y=startingPosY; y<startingPosY+5; y++){
                        sum += mirrorFramedAry[x][y];
                    }
                }

                return sum/25;

    }

    void SpatialFiltering::computeGauss5x5(int** mirrorFramedAry, int** GaussAry, int* maskAry ,int* neighborAry, int weight){
      
            int i=2;
            int j;
            int newMax = -1;
            while(i < (numRows + 2)){
                j = 2;
                while(j < (numCols+2)){
                    loadNeighborAry(mirrorFramedAry, i, j, neighborAry);
                    GaussAry[i][j] = convolution(neighborAry, maskAry, weight);
                    newMax = max(newMax, GaussAry[i][j]);
                    j++;
                }
        
                i++;
            }

            maxVal = newMax;
        
    }

    int SpatialFiltering::convolution(int* neighborAry, int* maskAry, int weight){
      
            int result = 0;
            int i = 0;

            while(i<25){
                result += neighborAry[i] * maskAry[i];
                i++;
            }

        
            return (result/weight);
    }

    void SpatialFiltering::computeHist(int** inAry, int* histAry){
    
            int i=2;
            int j;
            while(i < (numRows+2)){
                j = 2;
                while(j < (numCols+2)){
                    histAry[inAry[i][j]]++;
                    j++;    
                }
                i++;
            }

    }

    void SpatialFiltering::imgReformat(int** inAry, ofstream& outFile){
            outFile<<numRows<<" "<<numCols<<" "<<minVal<<" "<<maxVal<<"\n";
            string str = to_string(maxVal);
            int Width = str.length();

            int r = 2;
            int c;
            while(r < (numRows+2)){
                c = 2;
                while(c <(numCols+2)){
                    outFile<<inAry[r][c];
                   
                    str = to_string(inAry[r][c]);

                    int WW = str.length();

                   do{
                        outFile<<" ";
                        WW++;
                    } while(WW <= Width);

                    c++;
                }

                outFile<<"\n";

                r++;
            }
            
        }

    void SpatialFiltering::printHist(int* histAry, ofstream& histFile, int maxVal){
               
            histFile<<numRows<<" "<<numCols<<" "<<minVal<<" "<<maxVal<<"\n";

            int index = 0;

            while(index <= maxVal){
                histFile<<index<<" ";
                histFile<<histAry[index]<<"\n";

                index++;
            }


    }


