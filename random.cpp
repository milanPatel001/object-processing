#include <iostream>
#include <fstream>
#include <random>
using namespace std;

int main(int argc, char** argv){
    ofstream out;
    ifstream in;

    in.open(argv[1]);
    out.open(argv[2]);


    int r,c,mini,maxi;
    in>>r>>c>>mini>>maxi;

    int arr[r][c];

    int trueMax = -1;
    int trueMin = 9999;
    
    random_device rd; // Obtain a random number from a hardware device
            mt19937 gen(rd()); // Mersenne Twister pseudo-random number generator
            uniform_int_distribution<int> distribution(1, 20); // Define the range
            uniform_int_distribution<int> distribution2(40, 63); // Define the range

      

    for(int i=0;i<r;i++){
        for(int j=0;j<c;j++){

              int data;  
              in>>data;

              if(data==0){
                    arr[i][j] = distribution(gen);
              }else{
                    arr[i][j] = distribution2(gen);
               }

              trueMax = max(trueMax, arr[i][j]);  
              trueMin = min(trueMin, arr[i][j]);  

        }
    }

    out<<r<<" "<<c<<" "<<trueMin<<" "<<trueMax<<"\n";

    for(int i=0;i<r;i++){
        for(int j=0;j<c;j++){

                out<<arr[i][j]<<" ";

        }
        out<<"\n";
    }

    return 0;

}