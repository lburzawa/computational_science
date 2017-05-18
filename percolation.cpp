#include <random>
#include <iostream>
#include <fstream>
#include <string>
#include <mkl.h>

const int SIZE=10000;
std::random_device rd;
VSLStreamStatePtr stream;
int status = vslNewStream(&stream,VSL_BRNG_MT19937,rd());
int lattice[SIZE];
void get_random(double p) {
    int status = viRngBernoulli(VSL_RNG_METHOD_BERNOULLI_ICDF,stream,SIZE,&lattice[0],p);
}

const int reps=10000;
double p;

int main(int argc, char** argv) {
    std::string value(argv[1]);
    p=stof(value);
    std::string filename("p_");
    filename+=value;
    filename+=".csv";
    filename[3]='_';
    
    std::ofstream outfile;
    outfile.open(filename);
    int i,j;
    for (i=0; i<reps; i++) {
        if (i%1000==0) {
            std::cout << "This is prob " << p << " and iteration " << i << std::endl;
        }
        get_random(p);
        for (j=0; j<SIZE-1; j++) {
            outfile << lattice[j] << ',';
        }
        outfile << lattice[j] << std::endl;
    }
    outfile.close();
    return 0;
}
