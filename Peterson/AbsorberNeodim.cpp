class Absorber: public Method{
public:
  static const real SigmaSdown=7.4e-23;
  static const real SigmaSup=7.2e-22;//-19;//20;//7.2e-22     	
  static const real TauS=1.0e-11;//88.0e-9;
  static const real internalLoss=0;//0.05;  
  static const real nRefSat=1.8;

  real Ns0;//=2.0e24;
  
  Absorber (real dt_, std::ifstream &file) : Method(dt_,file) {
    int i;
    std:: string tmp;
    file >> tmp >> Ns0;
    
    for (i=0; i<N; i++){
      population.push_back(Ns0);
    }  
    for (i=0; i<N-1; i++){
      populationHalf.push_back(Ns0);
    }
    
    
    refraction=nRefSat;
  };
  
  virtual real rightPartPopulation(real u, real population, int i){
    return ((Ns0-population)/TauS-u*(SigmaSup*population)/(hPlank*NuGen));
  };
  
  virtual real A(real u, real population, real x, real t){
    real loss =(internalLoss);
    real down =0;//SigmaSdown*(Ns0-population);
    real up   =SigmaSup*population;
    return ((-loss-down-up)*u*c/nRefSat); 
    //return ((-internalLoss-SigmaSdown*(Ns0-population_)-SigmaSup*population)*u);
    
  };

  
};

class Neodim: public Method{
public:
  real *I0;
  real N0;//=4.2e25;
  real Pnak;
  real alphaP;


  static const real Sigma21Gen=1.5e-23;//2.5e-23;
  static const real Sigma12Nak=0.88e-23;
  static const real Tau=90.0e-5;//22.0e-6;//90.0e-6;
  static const real nRefNeodim=2.19;//1.0;//2.19;
  static const real internalLoss=10.05;

  Neodim (real dt_, std::ifstream &file): Method(dt_, file){  //alphaP- coefficient zatuhaniya nakachki. Esli alphaP=0=> POPERECHNAYA nakachka.
    int i;
    real longitudinalPump, initialExcitation;
    std::string tmp;
    file >> tmp>> N0>>tmp >>Pnak>> tmp>>longitudinalPump;
    alphaP=longitudinalPump*Sigma12Nak*N0;// =0 POPERECHNAYA NAKACHKA!!!!!!!!!!!!!!!!!!!!!!!!
    file >> tmp >> initialExcitation;
	
    
    I0 = new real [N];

    refraction=nRefNeodim;
    
    
    for (i=0; i<N; i++){
      population.push_back(initialExcitation*N0);
    }  
    for (i=0; i<N-1; i++){
      populationHalf.push_back(initialExcitation*N0);
    }
    for (i=0; i<N; i++){
      I0[i]=ksi*Pnak*exp(-alphaP*i*dx)/Sp;
    //  printf("%20.20e %20.20e %20.20e \n", Sp, ksi*Pnak*exp(-alphaP*i*dx)/(Sp*1.0e7),I0[i]);
    };
  };
   
  virtual real rightPartPopulation(real u, real population_, int i){
    real stimulated=u*(Sigma21Gen*population_)/(hPlank*NuGen);
    real spontan=population_/Tau;
    real pump=Sigma12Nak*(N0-population_)/(hPlank*NuNak)*I0[i];
    return (-stimulated-spontan+pump);//(Sigma12Nak*(N0-population_)/(hPlank*NuNak)*I0[i]-u*(Sigma21Gen*population_)/(hPlank*NuGen)-population_/Tau);
  };
  
  virtual real A(real u, real population_, real x, real t){
    real stimulated=(Sigma21Gen*population_-internalLoss)*u*c/nRefNeodim;
    real luminescence=5.0e-4*1.0/Tau*population_*hPlank*NuGen;//5.0e-2
//    if (t>0.00001) {
//      luminescence=0;
//      stimulated=0;
//    }
    return /*speed_of_light/refraction**/ (stimulated+luminescence);//((Sigma21Gen*population_-internalLoss)*u+5.0e-4*1.0/Tau*population_*hPlank*NuGen);
  };
  
};
