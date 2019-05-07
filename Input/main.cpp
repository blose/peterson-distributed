#include <iostream>
#include <fstream>
#include <cstring>
#include <cmath>
#include <math.h>


double gammaA = 7.13e-24;
double sigmaA = 2.7e-23;
double sigmaE = 8.4e-22;
double gammaE = 2.3e-23;
double tauG = 100.0e-6;
double tauS = 4.0e-6;
double Lg = 0.124e-2;
double Ls = 0.71e-3;
double NT = 4.3e26;
double N0 = 9.72e24;//Transmission=83%
double c = 3.0e8;
double lambda = 1.063e-6;
double lambdaNak = 808.0e-9;
double nRef = 1.828;
double alphaL0 = 0.00127254;
double a=10.0;


double dt=1.1e-12;//1.1e-14;//5.1e-13;
double tmax=5.0e-3;//1.0e-3;//100.0e-12;//8.4e-12;
double r1=1.0;
double r2=0.90;
int longitudinalPump=1;
double hPlank=6.64e-34;
double Sgen=0.50e-6;
double dGen=15.0e-6;
double rNak=70.0e-6;

#define INPUT_FILENAME "./input.txt"
#define INPUT_FILENAME1 "./var.txt"

using namespace std;

int main(int argc, char **argv) {
    
    double P, g1, alfa, gx,gy, a1, tauC, Lvacuum,alphaP, coeff_v_kWatt, PUMP_kWatt, Snak, loss, alphaL, T;
//    char file_var[1000];
    std::ofstream in_file1(INPUT_FILENAME1);

    int number=0;
//    for (dt=1.1e-13; dt>5.0e-14; dt/=2.0){

//      for (Lg=0.015; Lg<0.016; Lg+=0.010){
//	for(Ls=0.005; Ls>0.00001; Ls/=2.0){
//	cout<<"Ls="<<Ls<<"\n";
	alphaL=alphaL0*0.5+1.0e-300;
	for (Lg=0.104e-2; Lg<0.205e-2; Lg+=0.1e-2)
	for (T=0.83; T<0.91; T+=0.03){
	
	    for (P=5.0e23; P< 2.2e24; P*=1.4){
		  for (Lvacuum=0.0; Lvacuum<0.15; Lvacuum+=1.50){		
//			P= -(a*alphaL)/(gammaA*tauG*(a*alphaL - 2*Lg*NT*sigmaE));

			N0=-log(T)/(Ls*sigmaA);	
			tauC=((Ls+Lg)*nRef+Lvacuum)/c;
			
			a=Lg*NT*sigmaE*P*tauG*gammaA/(P*gammaA*tauG*alphaL+alphaL);
			
			g1 = tauC/(tauS*alphaL);

	    		alfa = ((sigmaA + sigmaE)*(1 + P*gammaA*tauG)*tauS)/(gammaE*tauG);

			gy=(tauC* P*gammaA)/(alphaL );
			
			gx=(tauC/(alphaL*tauG));

			a1=(Ls*N0*sigmaA)/alphaL;
		
			alphaP=gammaA*NT;
			
//			cout<<"\tN0="<<N0<<"\ta1="<<a1<< "\n";
//		        cout<< "P="<<P<<"\ta="<<a<<"\talphaL0="<<alphaL0<<"\n";
//			cout<<"\tdt="<<dt<<"\tN="<<lround((Lg+Ls+Lvacuum)/(2*c*dt))+2<<"\n";
			
			Sgen=3.1415926*dGen*dGen/4.0*10.0;
			
			coeff_v_kWatt=(1+gammaA*tauG*P)/(gammaE*tauG)*hPlank*(c/lambda)*Sgen;
			//Snak=Lg*1.0e-2;
			Snak=3.1415926*(2.0*rNak)*(2.0*rNak)*10.0;

			PUMP_kWatt=P*hPlank*(c/lambdaNak)*Snak;
			
			//prepare filename
			char filename[1000];
			sprintf(filename,"%s.%04d",INPUT_FILENAME,number);

			std::ofstream in_file(filename);
//			in_file1<<P<<"\tcoeff="<<coeff_v_kWatt_PUMP<<"\tPpump="<<P*coeff_v_kWatt_PUMP<<"\n";
			in_file1<<P<<"\t"<<PUMP_kWatt<<"\t"<<T<<"\t"<<N0<<"\t"<<Lg<<"\n";

			//in_file<<"number="<< number<<std::endl;
			in_file<< "dt\t"<<dt<<std::endl;
			in_file<< "tmax\t"<<tmax<<endl;
			in_file<<"r1\t"<<r1<<endl;
			in_file<<"r2\t"<<r2<<endl;
			in_file<<"coeff_v_kWatt\t"<<coeff_v_kWatt<<endl;
			in_file<<"NeodimPeterson"<< "\tLength\t"<<Lg<<"\ta\t"<<a<<"\tgx\t"<<gx<<"\tgy\t"<<gy<<"\talphaL\t"<<alphaL<<"\ttauC\t"<<tauC<<"\talphaP\t"<<alphaP<<"\tlongitudinalPump\t"<<longitudinalPump<<endl;
			in_file<<"AbsorberPeterson"<< "\tLength\t"<<Ls<<"\tg1\t"<<g1<<"\talfa\t"<<alfa<<"\ta1\t"<<a1<<"\talphaL\t"<<alphaL<<"\ttauC\t"<<tauC<<endl;
//			in_file<<"Vacuum"<< "\tLength\t"<<Lvacuum<<endl;
		
			in_file.close();
//			loss=1/2.0/Lg*log(1/r1*r2);
			number++;

		  };

/*P*/	    };
/*T*/	};
//Ls	};
    
//Lg    };
   // a= 10.00;	/*A*/
    cout<< "           "<<"\n";
//    };    
    cout << "created " << number <<" files totally\n";
    return 0;
}