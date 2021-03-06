#include <iostream>
#include <fstream>
#include <string>
#include <random>
#include <mathimf.h>
#include <mkl.h>

std::random_device rd;
VSLStreamStatePtr stream;
int status = vslNewStream(&stream,VSL_BRNG_MT19937,rd());
double qadran;
void get_random() {
    int status = vdRngUniform(VSL_RNG_METHOD_UNIFORM_STD,stream,1,&qadran,0.0,1.0);
}

const int SIZE=100;
int Lattice[SIZE][SIZE];
int OldSpin;
int Reflection=-1;
int reps_eq=10000;
int reps=1000000;
int reps_total;
int count;
double T; //Tc is 2.269

void BuildCluster();
void AddSpin(int n, int m);
void TakeImage(std::ofstream& outfile);

int main(int argc, char** argv) {
    std::string value(argv[1]);
    T=stof(value);
    std::string filename("T_");
    filename+=value;
    filename+=".csv";
    filename[3]='_';
    std::ofstream outfile;
    outfile.open(filename);
    
    reps_total=reps+reps_eq;
    int i,j; double spin;
    for (i=0; i<SIZE; i++) {
        for (j=0; j<SIZE; j++) {
            get_random();
            spin=qadran;
            if (spin<0.50) {
                Lattice[i][j] = -1;
            }
            else {
                Lattice[i][j] = 1;
            }
        }
    }
    for (count=0; count<reps_total; count++) {
        if ((count-reps_eq)%100000==0) {
            std::cout << "This is temp " << T << " and iteration " <<(count-reps_eq)<<std::endl;
        }
        BuildCluster();
        if (count>=reps_eq && count%100==0) {
            TakeImage(outfile);
        }
    }
    outfile.close();
    
    return 0;
}

void BuildCluster() {
    int n,m;
    
    get_random();
    n = int(qadran*double(SIZE));
    get_random();
    m = int(qadran*double(SIZE));
    OldSpin=Lattice[n][m];
    AddSpin(n,m);
}

void AddSpin(int n, int m)
{
    int nPrev, nNext, mPrev, mNext;
    double P;
    
    Lattice[n][m] *= Reflection;
    nPrev = (n==0 ? SIZE-1 : n-1); nNext = (n==SIZE-1 ? 0 : n+1);
    mPrev = (m==0 ? SIZE-1 : m-1); mNext = (m==SIZE-1 ? 0 : m+1);
    P = 1.0-exp(-2.0/T);
    
    get_random();
    if (Lattice[nPrev][m]==OldSpin && P>qadran) {AddSpin(nPrev, m);}
    get_random();
    if (Lattice[nNext][m]==OldSpin && P>qadran) {AddSpin(nNext, m);}
    get_random();
    if (Lattice[n][mPrev]==OldSpin && P>qadran) {AddSpin(n, mPrev);}
    get_random();
    if (Lattice[n][mNext]==OldSpin && P>qadran) {AddSpin(n, mNext);}
}

void TakeImage(std::ofstream& outfile) {
    int i,j;
    for (i=0; i<SIZE; i++) {
        for (j=0; j<SIZE; j++) {
            if (Lattice[i][j]==-1) {
                outfile << 0 << ',';
            }
            else {
                outfile << 1 << ',';
            }
        }
    }
    outfile << "0,1,0,0,0" << std::endl;
}
