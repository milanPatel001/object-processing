#include <fstream>
using namespace std;

class AutoThreshold{
    public:
        int numRows;
        int numCols;
        int minVal;
        int maxVal;
        int* histAry;
        int* GaussAry;
        int BiGaussThrVal;
        int maxHeight;

        AutoThreshold(int numRows, int numCols, int minVal, int maxVal);
        
        void loadHistory(ifstream& inFile);

        void dispHist(ofstream& outFile);

        void setZero(int* Ary);

        void biGaussian();

        double computeMean(int leftIndex, int rightIndex, int maxHeight, int* histAry);

        double computeVar(int leftIndex, int rightIndex, double mean, int* histAry);

        double modifiedGauss(int x, double mean, double var, int maxHeight);

        double fitGauss(int leftIndex, int rightIndex, int* histAry, int* GaussAry);

        void thresholdProcess(ifstream &inFile, ofstream &outFile1, ofstream &outFile2, int thrValue);

};