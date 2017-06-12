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
int Lattice[SIZE][SIZE];
double Field[SIZE][SIZE];
int reps_eq=10000;
int reps=1000000;
int reps_total;
int count;
double T=2.269; //Tc is 2.269

void BuildCluster();
void FlipSpin();
void TakeImage(std::ofstream& outfile);

int main(int argc, char** argv) {
    std::string value(argv[1]);
    R=stof(value);
    std::string filename("R2D_");
    filename+=value;
    filename+=".csv";
    filename[5]='_';
    std::ofstream outfile;
    outfile.open(filename);
    
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
            get_random_h();
            Field[i][j]=qadran;
        }
    }
    for (count=0; count<reps_total; count++) {
        if ((count-reps_eq)%100000==0) {
            std::cout << "This is R " << R << " and iteration " <<(count-reps_eq)<<std::endl;
        }
        for (j=0; j<SIZE*SIZE; j++) {
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
    int n,m;
    get_random();
    n = int(qadran*double(SIZE));
    get_random();
    m = int(qadran*double(SIZE));
    int nPrev, nNext, mPrev, mNext;
    int OldSpin; int NewSpin;
    double delta_E; double P;
    
    OldSpin=Lattice[n][m]; NewSpin=Lattice[n][m]*(-1);
    nPrev = (n==0 ? SIZE-1 : n-1); nNext = (n==SIZE-1 ? 0 : n+1);
    mPrev = (m==0 ? SIZE-1 : m-1); mNext = (m==SIZE-1 ? 0 : m+1);
    
    delta_E=-(NewSpin-OldSpin)*(Lattice[nPrev][m]+Lattice[nNext][m]+Lattice[n][mPrev]+Lattice[n][mNext])-(NewSpin-OldSpin)*Field[n][m];
    if (delta_E<0) {Lattice[n][m]=NewSpin; return;}
    P=exp(-delta_E/T);
    get_random();
    if (P>qadran) {Lattice[n][m]=NewSpin;}
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
    outfile << "0,0,0,1,0" << std::endl;
}
