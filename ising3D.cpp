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
int Lattice[SIZE][SIZE][SIZE];
int OldSpin;
int Reflection=-1;
int reps_eq=10000;
int reps=100000;
int reps_total;
int count;
double T=3.1; //Tc is 4.512
double binder;

void BuildCluster();
void AddSpin(int n, int m, int k);
void CalculateMagnetization();

int main(int argc, char** argv) {
    std::ofstream outfile;
    outfile.open("output.csv");
    reps_total=reps+reps_eq;
    
    int i,j,k; double spin;
    for (i=0; i<SIZE; i++) {
        for (j=0; j<SIZE; j++) {
            for (k=0; k<SIZE; k++) {
                get_random();
                spin=qadran;
                if (spin<0.50) {
                    Lattice[i][j][k] = -1;
                }
                else {
                    Lattice[i][j][k] = 1;
                }
            }
        }
    }
    for (i=0; i<30; i++) {
        for (count=0; count<reps_total; count++) {
            BuildCluster();
            if (count>reps_eq) {
                /*if (d%100==0) {
                 TakeImage();
                 }*/
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
    int n,m,k;
    
    get_random();
    n = int(qadran*double(SIZE));
    get_random();
    m = int(qadran*double(SIZE));
    get_random();
    k = int(qadran*double(SIZE));
    OldSpin=Lattice[n][m][k];
    AddSpin(n,m,k);
}

void AddSpin(int n, int m, int k)
{
    int nPrev, nNext, mPrev, mNext, kPrev, kNext;
    double P;
    
    Lattice[n][m][k] *= Reflection;
    nPrev = (n==0 ? SIZE-1 : n-1); nNext = (n==SIZE-1 ? 0 : n+1);
    mPrev = (m==0 ? SIZE-1 : m-1); mNext = (m==SIZE-1 ? 0 : m+1);
    kPrev = (k==0 ? SIZE-1 : k-1); kNext = (k==SIZE-1 ? 0 : k+1);
    P = 1.0-exp(-2.0/T);
    
    get_random();
    if (Lattice[nPrev][m][k]==OldSpin && P>qadran) {AddSpin(nPrev, m, k);}
    get_random();
    if (Lattice[nNext][m][k]==OldSpin && P>qadran) {AddSpin(nNext, m, k);}
    get_random();
    if (Lattice[n][mPrev][k]==OldSpin && P>qadran) {AddSpin(n, mPrev, k);}
    get_random();
    if (Lattice[n][mNext][k]==OldSpin && P>qadran) {AddSpin(n, mNext, k);}
    get_random();
    if (Lattice[n][m][kPrev]==OldSpin && P>qadran) {AddSpin(n, m, kPrev);}
    get_random();
    if (Lattice[n][m][kNext]==OldSpin && P>qadran) {AddSpin(n, m, kNext);}
}

void CalculateMagnetization() {
    double mag;
    static double total_mag2=0.0;
    static double total_mag4=0.0;
    
    int i,j,k; int sum=0;
    for (i=0; i<SIZE; i++) {
        for (j=0; j<SIZE; j++) {
            for (k=0; k<SIZE; k++) {
                sum+=Lattice[i][j][k];
            }
        }
    }
    mag=double(sum)/double(SIZE*SIZE*SIZE);
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



