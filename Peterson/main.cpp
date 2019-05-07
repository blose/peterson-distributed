#ifdef NO_MPI
#define MPI_Init(a,b) 
#define MPI_Comm_rank(a,b) *(b)=0;
#define MPI_Finalize()
#else
#include <mpi.h>
#endif
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include <iostream>
#include <fstream>
#include <vector>
#include <cstring>
#include <map>

#include "definitions.h"
#include "Propagator.h"
#include "Vacuum.h"
#include "Method.h"
#include "Peterson.h"


#define MODE_SINGLE 0
#define MODE_MPI_BATCH 1

#define OUTPUT_MODE OM_ASCII//OM_ASCII//OM_JSON//OM_BINARY // regim vyvoda!!!


#define MAX_FILENAME_LENGTH 1000
#define DEFAULT_INPUT_FILENAME  "input.txt.0000"
#define DEFAULT_OUTPUT_FILENAME "output.txt"

//#define TMP //vklyuchenie rascheta pravoj chasti dlya otladki

// move ptr forward up to first non-whitespace symbol or to the end of line
void skip_spaces(char *&ptr) {

    while (( *ptr == ' ' ) || (*ptr == '\t') || (*ptr=='\n')) {
	ptr++;
    }
}
    

std::map<std::string,real> parse_line(const char* line) {
    std::map<std::string,real> data;
    size_t len=strlen(line);
    char* input = new char[len+1];
    strcpy(input,line);
    
    char* ptr=input;
    char* next;
    do {
	skip_spaces(ptr);
	next = strpbrk(ptr," \t");
	if (next) {
	    *next = 0;
	    //printf("-->%s<--\n",ptr);
	    std::string key(ptr);
	    ptr = next+1;
	    
	    skip_spaces(ptr);
	    next = strpbrk(ptr," \t");
	    if (next) {
		*next = 0;
	    }
	    //printf("-=>%s<=-\n",ptr);
	    real value;
	    if (sscanf(ptr,"%lf",&value)!=1) {
		printf("Unable to read value for %s: '%s'\n",key.c_str(),ptr);
		break;
	    };
	    data[key]=value;

	}
	
	ptr = next+1;
	
    } while (next);
    delete[] input;
    return data;
    
}

int main(int argc, char **argv) {
    int mode = MODE_SINGLE;
    int MPI_rank;
    
    real tmax;
    real dt;
    int i;
    int N_of_elements;	
    std::string str;
    real  r1, r2;
    real coeff_v_kWatt;
    
    char outfileName[MAX_FILENAME_LENGTH];
    sprintf(outfileName,"%s",DEFAULT_OUTPUT_FILENAME);
    char infileName[MAX_FILENAME_LENGTH]; 
    sprintf(infileName,"%s",DEFAULT_INPUT_FILENAME);

#ifdef TMP    
    char outfile2Name[MAX_FILENAME_LENGTH];
#endif    

    // read parameters
    i=1;
    while (i<argc) {
	if (strcmp("--input",argv[i]) == 0) {
	    i++;
	    strncpy(infileName,argv[i],MAX_FILENAME_LENGTH-6);
	    i++;
	    continue;
	}
	if (strcmp("--output",argv[i]) == 0) {
	    i++;
	    strncpy(outfileName,argv[i],MAX_FILENAME_LENGTH-6);
	    i++;
	    continue;
	}
	if (strcmp("--batch",argv[i]) == 0) { // MPI based batch mode
	    i++;
	    // will add .xxxx to input and output files
	    // where .xxxx is an MPI rank
	    mode = MODE_MPI_BATCH;
	    continue;
	}
	
	std::cerr << "Error - unknown command line parameter " << argv[i] << std::endl;
	return 1;
    }
#ifdef TMP
    sprintf(outfile2Name,"%s.tmp",outfileName);
#endif
    
    if (mode == MODE_MPI_BATCH) {
	MPI_Init(&argc,&argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &MPI_rank);
	// modify file names
	char number[6];
	snprintf(number,6,".%04d",MPI_rank);
	strcat(infileName,number);
	strcat(outfileName,number);
#ifdef TMP
	strcat(outfile2Name,number);
#endif	
    }
    
#ifdef TMP
    std::ofstream outfile2(outfile2Name);
#endif
    
    std::ifstream inp(infileName);
    if (inp.is_open()==0) {
      std::cerr << "input file \""<<infileName<<"\"not found"<<std::endl;
    return 1;
    } else {
	std::cout << "reading laser parameters from " << infileName << std::endl;
    }
    #if OUTPUT_MODE == OM_BINARY
    std::ofstream outfile(outfileName,std::ifstream::binary);
    #else
    std::ofstream outfile(outfileName);
    #endif
    if (outfile.is_open()==0) {
      std::cerr << "Cannot write output file \""<<outfileName<<"\""<<std::endl;
    return 1;
    };
    
    inp >> str >> dt;
    std::cout << str << "="<< dt <<std::endl;

    inp >> str >> tmax;
    std::cout << str << "="<< tmax <<std::endl;

    inp >> str >> r1;
    std::cout << str << "="<< r1 <<std::endl;
    
    inp >> str >> r2;
    std::cout << str << "="<< r2 <<std::endl;
    
    inp >> str >> coeff_v_kWatt;
    std::cout << str << "="<< coeff_v_kWatt <<std::endl;

    
    // allocate array for resonator elements
    std::vector <Propagator *> elements;// = new Propagator*[N_of_elements];
    
    while ( !inp.eof() ) {
      str="#";
      inp >> str;
      if (str[0] == '#') {
	  // ignore comments starting with #
	  std::getline(inp,str);
	  continue;
      }

      // read line and parse it into associative array
      std::string par;
      std::getline(inp,par);
      std::cout << "READ"<<par<<"*\n";
      std::map<std::string,real> params = parse_line(par.c_str());
      
      params["dt"]=dt;
      
      Propagator *element;
      if (str=="Vacuum") {
	element = new Vacuum(params);
      } 
      else if (str=="NeodimPeterson") {
	element = new NeodimPeterson(params);
      }	
      else if (str=="AbsorberPeterson") {
	element = new AbsorberPeterson(params);
      }
      else {
	  std::cerr<<"Element "<< str <<" not supported!!!"<<std::endl;
	  return 1;
      };
      
      std::cout<< "Created element "<< str<< " (";
      for(std::map<std::string,real>::iterator i = params.begin(); i!=params.end(); i++) {
      
	 std::cout  << i->first << "="<<i->second <<", ";
	}
	std::cout << ")"<<std::endl;
      elements.push_back(element);

    };
        
    N_of_elements = elements.size();
    std::cout<<"Totally created "<<N_of_elements<<" elements"<<std::endl;
    
    Propagator *lastElement=elements[N_of_elements-1];

	
        
  
    for (i=0; i<N_of_elements; i++){  //obnulim vse coefficienty otrajeniya: i vnutrennih elementov, i vhodnoe, i vyhodnoe zerkala. Potom naznachim vhodnoj i vyhodnoj coefficienty.
      elements[i]-> r_left=0.0;
      elements[i]-> r_right=0.0;
    };
	
    elements[0]-> r_left=r1;
    lastElement->r_right=r2;
    
    

    real right_Source, right_Receiver, left_Source, left_Receiver, t;
    right_Source=0.0;
    left_Source=0.0;
    t = 0.0;
    
    time_t start=time(0);
    
    real lastOutputTime = -1e30;
    
    printf("Entering main loop\n");
    
    for(t=0; t<tmax; t+=dt){//vremya vo vremenah prohoda rezonatora!!!    %%%Ubirayu vse istochniki i otrajeniya ot vnutrennih elementov!!!%%%

      *(elements[0]->in_Right)=/*right_Source+*/log(elements[0]->r_left)+(elements[0]->out_left); //menyaem "*" na "+", t.k. u=e^t i poyavlyaetsya Ln!!!
      for (i=1; i<N_of_elements; i++){
	*(elements[i]->in_Right)=/* (elements[i]->out_left*(elements[i]->r_left))+ */(elements[i-1]->out_right)*(1/* -(elements[i-1]->r_right) */);
      };
      
      right_Receiver=(lastElement->out_right)+log(1-(lastElement->r_right)); //menyaem "*" na "+", t.k. u=e^t i poyavlyaetsya Ln!!!
      
      
      *(lastElement->in_Left)=/*left_Source+*/log(lastElement->r_right)+(lastElement->out_right);//menyaem "*" na "+", t.k. u=e^t i poyavlyaetsya Ln!!!

      for (i=0; i<N_of_elements-1; i++){
	*(elements[i]->in_Left)=/* (elements[i]->out_right)*(elements[i]->r_right)+ */(elements[i+1]->out_left)*(1/* -(elements[i+1]->r_left) */);
      }
      
      left_Receiver=(elements[0]->out_left)+log(1-(elements[0]->r_left)); //menyaem "*" na "+", t.k. u=e^t i poyavlyaetsya Ln!!!
      
      
      if (t - lastOutputTime >=1.0e-12/*5.1e-10/*1.0e-5/* 1.0e-11/*e-11/*-10 /*1e-9 /* 5.0e-15*/)
      {
	  lastOutputTime = t;
	   
//std::cerr << t << std::endl;
#ifdef TMP	   
	outfile2 << t << "\t" <<((Method*)elements[0])-> tmp << "\t"<< ((Method*)elements[1])-> tmp <<std::endl;
#endif

	   
#if OUTPUT_MODE == OM_JSON

        outfile << "{ \"t\":" << t ;
	for (i=0; i< N_of_elements; i++){
		outfile << ","<< elements[i]->toString();
	}
	outfile << "}" << std::endl;

#elif OUTPUT_MODE == OM_BINARY
[Ma@3
	double tt = t;
	float ii = lastElement->out_right;
	outfile.write((const char*)&tt, sizeof(double) );
	outfile.write((const char*)&ii, sizeof(float)  );

	//std::cout <<t<<"\t" << lastElement->out_right <<"\n";
	  
#elif OUTPUT_MODE == OM_ASCII
	
	NeodimPeterson* media=(NeodimPeterson*) elements[0];
	AbsorberPeterson* absorber=(AbsorberPeterson*) elements[1];
	
/*	outfile <<
	media->uRight[1] << " " <<
	media->uRight[6] << " " <<
	
	media->uRight[11] << " " <<
	
	media->uRight[16] << " " <<
	
	absorber->uRight[2]  << " " <<
	absorber->uRight[4]  << " " <<
	absorber->uRight[6]  << " " <<
	absorber->uRight[8]  << " " << right_Receiver 
*/	
	outfile <<t<<"\t" << exp(right_Receiver) /*  *coeff_v_kWatt*/ <<"\t"
//	  <<(elements[1]-> out_right/elements[0]-> out_right)<<"\t"
//	    <<(elements[0]-> out_right/elements[0]-> out_left)<<"\t"
	      << media-> population[2] << "\t"
//	        <<media-> population[media-> N-3]<< "\t"
//	    	  <<absorber-> population[2]<<"\t"
//	    	    <<absorber-> population[absorber-> N-3]<< "\t"
	    	      
//	    	      <<media-> uRight[2]<<"\t"
//	        	<<media-> uRight[media-> N-3]<< "\t"
//	                  <<absorber-> uRight[2]<<"\t"
//	    		    <<absorber-> uRight[absorber-> N-3]<< "\t"
	    
	    <<"\n"<<std::flush;

#else
#error Unsupported OUTPUT_MODE
#endif

      };
      
      
      for (i=0; i< N_of_elements; i++){
	elements[i]-> calculation(t);
      }
      
      
    };
   if (mode == MODE_SINGLE) {
       
    std::cout<<"# calculation time "<<(time(0)-start)<<" sec" << std::endl;
   }
   if (mode == MODE_MPI_BATCH) {
       MPI_Finalize();
   }

   return 0;
}
