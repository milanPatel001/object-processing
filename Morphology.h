#include <fstream>
using namespace std;

class Morphology{
    public:
        int numImgRows;
        int numImgCols;
        int imgMin;
        int imgMax;
        int numStructRows;
        int numStructCols;
        int structMin;
        int structMax;
        int rowOrigin;
        int colOrigin;
        int rowFrameSize;
        int colFrameSize;
        int extraCols;
        int extraRows;
        int rowSize;
        int colSize;
        int** zeroFramedAry;
        int** morphAry;
        int** tempAry;
        int** structAry;

        Morphology(int a, int b, int c, int d, int e, int f, int g, int h, int i, int j);
        
        void zero2DAry(int** Ary, int nRows, int nCols);

        void loadImg(ifstream& imgFile, int** zeroFramedAry);

        void loadstruct(ifstream& structFile, int** structAry);

        void ComputeDilation(int** inAry, int** outAry);

        void onePixelDilation(int i,int j, int** inAry, int** outAry);

        void ComputeErosion(int** inAry, int** outAry);

        void onePixelErosion(int i,int j, int** inAry, int** outAry);


        void ComputeOpening(int** zeroFramedAry, int** morphAry, int** tempAry);

        void ComputeClosing(int** zeroFramedAry, int** morphAry, int** tempAry);

        void prettyPrint(int** Ary, ofstream& outFile, int startRow, int startCol ,int endRow, int endCol);

        void AryToFile(int** Ary, ofstream& outFile);

        void ObjectDecomp(ofstream& outFile);

};        