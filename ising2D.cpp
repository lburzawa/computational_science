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

const int SIZE=16;
int Lattice[SIZE][SIZE];
int OldSpin;
int Reflection=-1;
int reps_eq=10000;
int reps=100000;
int reps_total;
int count;
double T=0.1; //Tc is 2.269
double binder;

void BuildCluster();
void AddSpin(int n, int m);
void CalculateMagnetization();

int main(int argc, char** argv) {
    std::ofstream outfile;
    outfile.open("output16.csv");
    reps_total=reps+reps_eq;
    
    int i,j;
    for (i=0; i<SIZE; i++) {
        for (j=0; j<SIZE; j++) {
            get_random();
            if (qadran<0.50) {
                Lattice[i][j] = -1;
            }
            else {
                Lattice[i][j] = 1;
            }
        }
    }
    for (i=0; i<50; i++) {
        for (count=0; count<reps_total; count++) {
            BuildCluster();
            if (count>reps_eq) {
                CalculateMagnetization();
            }
        }
        outfile << T << ',' << binder << std::endl;
        std::cout << T << ',' << binder << std::endl;
        T+=0.1;
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

void CalculateMagnetization() {
    double mag;
    static double total_mag2=0.0;
    static double total_mag4=0.0;
    
    int i,j; int sum=0;
    for (i=0; i<SIZE; i++) {
        for (j=0; j<SIZE; j++) {
            sum+=Lattice[i][j];
        }
    }
    mag=double(sum)/double(SIZE*SIZE);
    total_mag2+=pow(mag,2.0);
    total_mag4+=pow(mag,4.0);
    
    if (count==reps_total-1) {
        double ave_mag2=(total_mag2/double(reps));
        double ave_mag4=(total_mag4/double(reps));
        binder=1.0-(ave_mag4/(3.0*ave_mag2*ave_mag2));
        total_mag2=0.0;
        total_mag4=0.0;
    }
}



