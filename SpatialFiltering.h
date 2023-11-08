#include <fstream>
using namespace std;

class SpatialFiltering{
    public:
        int numRows;
        int numCols;
        int minVal;
        int maxVal;
        int maskRows;
        int maskCols;
        int maskMin;
        int maskMax;

        int** mirrorFramedAry;
        int** avgAry;
        int** GaussAry;
        int* neighborAry;
        int* maskAry;
        int maskWeight;
        int* histAvgAry;
        int* histGaussAry;

        SpatialFiltering();

        SpatialFiltering(int r, int c, int min, int max);

        void loadImage(ifstream &inFile, int** mirrorFramedAry);

        void mirrorFraming(int** mirrorFramedAry);

        // int loadMaskAry(ifstream &maskFile, int* maskAry);

        void loadNeighborAry(int** mirrorFramedAry, int i, int j, int* neighborAry);

        void computeAvg5x5(int** mirrorFramedAry, int** avgAry);

        int avg5x5(int** mirrorFramedAry, int i, int j);

        void computeGauss5x5(int** mirrorFramedAry, int** GaussAry, int* maskAry ,int* neighborAry, int weight);

        int convolution(int* neighborAry, int* maskAry, int weight);

        void computeHist(int** inAry, int* histAry);

        void imgReformat(int** inAry, ofstream& outFile);

        void printHist(int* histAry, ofstream& histFile, int maxVal);
};


