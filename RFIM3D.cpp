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
double R;
void get_random() {
    int status = vdRngUniform(VSL_RNG_METHOD_UNIFORM_STD,stream,1,&qadran,0.0,1.0);
}
void get_random_h() {
    int status = vdRngGaussian(VSL_RNG_METHOD_GAUSSIAN_BOXMULLER2,stream,1,&qadran,0.0,R);
}

const int SIZE=100;
int Lattice[SIZE][SIZE][SIZE];
double Field[SIZE][SIZE][SIZE];
int reps_eq=10000;
int reps=100000;
int reps_total;
int count;
double T=4.512; //Tc is 4.512

void FlipSpin();
void TakeImage(std::ofstream& outfile);

int main(int argc, char** argv) {
    std::string value(argv[1]);
    R=stof(value);
    std::string filename("R3D_");
    filename+=value;
    filename+=".csv";
    filename[5]='_';
    std::ofstream outfile;
    outfile.open(filename);
    
    reps_total=reps+reps_eq;
    int i,j,k;
    for (i=0; i<SIZE; i++) {
        for (j=0; j<SIZE; j++) {
            for (k=0; k<SIZE; k++) {
                get_random();
                if (qadran<0.50) {
                    Lattice[i][j][k] = -1;
                }
                else {
                    Lattice[i][j][k] = 1;
                }
                get_random_h();
                Field[i][j][k]=qadran;
            }
        }
    }
    for (count=0; count<reps_total; count++) {
        if (count>=reps_eq && (count-reps_eq)%10000==0) {
            std::cout << "This is R " << R << " and iteration " <<(count-reps_eq)<<std::endl;
        }
        for (j=0; j<SIZE*SIZE*SIZE; j++) {
            FlipSpin();
        }
        if (count>=reps_eq && count%100==0) {
            TakeImage(outfile);
        }
    }
    outfile.close();
    
    return 0;
}

void FlipSpin()
{
    int n,m,k;
    get_random();
    n = int(qadran*double(SIZE));
    get_random();
    m = int(qadran*double(SIZE));
    get_random();
    k = int(qadran*double(SIZE));
    int nPrev, nNext, mPrev, mNext, kPrev, kNext;
    int OldSpin; int NewSpin;
    double delta_E; double P;
    
    OldSpin=Lattice[n][m][k]; NewSpin=Lattice[n][m][k]*(-1);
    nPrev = (n==0 ? SIZE-1 : n-1); nNext = (n==SIZE-1 ? 0 : n+1);
    mPrev = (m==0 ? SIZE-1 : m-1); mNext = (m==SIZE-1 ? 0 : m+1);
    kPrev = (k==0 ? SIZE-1 : k-1); kNext = (k==SIZE-1 ? 0 : k+1);
    
    delta_E=-(NewSpin-OldSpin)*(Lattice[nPrev][m][k]+Lattice[nNext][m][k]+Lattice[n][mPrev][k]+Lattice[n][mNext][k]+Lattice[n][m][kPrev]+Lattice[n][m][kNext])-(NewSpin-OldSpin)*Field[n][m][k];
    if (delta_E<0) {Lattice[n][m][k]=NewSpin; return;}
    P=exp(-delta_E/T);
    get_random();
    if (P>qadran) {Lattice[n][m][k]=NewSpin;}
}

void TakeImage(std::ofstream& outfile) {
    int i,j,k;
    for (k=0; k<SIZE; k+=10) {
        for (i=0; i<SIZE; i++) {
            for (j=0; j<SIZE; j++) {
                if (Lattice[i][j][k]==-1) {
                    outfile << 0 << ',';
                }
                else {
                    outfile << 1 << ',';
                }
            }
        }
        outfile << "0,0,0,0,1" << std::endl;
    }
}

