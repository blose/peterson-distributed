#include <cstdio>
#include <math.h>
#include <vector>
#include <cstring>

using namespace std;

#define THRESHOLD 0.0
#define LOGSCALE_INPUT

struct Maximum{
    int lineNumber;
    double value;
    int position;
    double halfWidth;
    double time;
    double energy;
};

int main(int argc, char **argv) {

	FILE *file, *file2;
	double I1, I2, I3;
//	
//	if (argc!=2) {
//	    printf("Error!\n");
//	    return 1;
//	}
	
	char *fileName=argv[1];
	//"/media/TI102805W0E/Documents and Settings/Lolita/Desktop/Luba/Modelling_QML/!!!/out_Menjayu_N0_Labsorber/1/out/Pnak10.000000-N01.922168e+27-Lmedium0.019222-Labsorber0.001000/Ir.dat";
	//char *fileName="/home/krylov/luba/24_05_Lrez_long/out.txt.0000";
	int lineNumber;
	int i,j,k;
	
	file=fopen(fileName, "r");
	if (!file) {
	    fprintf(stderr, "Error opening file %s\n",fileName);
	    return 1;
	}
	

//**************************  Nahodim vse maksimymy v Ir.dat   *************************************************************
	
	I1 = -1e37;
	I2 = -1e37;
	lineNumber = -1;
	
	Maximum *currentMaximum;
	currentMaximum=new Maximum;//sozdaem ukazatel na Maximum
	//foreach ($lines as $time => $I) 
	
	//chitaem maksimumy iz Ir.dat
	vector<Maximum> maximums;
	
	
	char buffer[100];
	int currentLength,previousLength;
	double currentTime,previousTime;
	while (1){
		lineNumber++;
		if (!fgets(buffer, 100, file)) {//schityvaem stroku iz faila
		    if(feof(file)) { 
			break;
		    } else {
			fprintf(stderr,"Error reading input file \"%s\", line %d\n",fileName,lineNumber);
			break;
		    }
		}
		previousLength = currentLength;
		currentLength = strlen(buffer);
		
		previousTime = currentTime;
		if (sscanf(buffer, "%lf %lf",&currentTime,&I3) != 2) {//parsim stroku, kotoruyu prochitali vyshe
			fprintf(stderr,"Error parsing input file \"%s\", line %d: \"%s\"\n",fileName,lineNumber,buffer);
			break;
		}
		
		if (I3 == I2) continue;
		if ((I2>I1) && (I2>I3)&& (I2>=THRESHOLD) ) {//I2>=... eto otkidyvaem melkie maksimumy
			
			currentMaximum->value=I2;
			currentMaximum->lineNumber=lineNumber;
			currentMaximum->position=ftell(file)-previousLength-currentLength; //ukazatel na chislo posle I3
			currentMaximum->time = previousTime;
			printf("Found maximum\n");
			maximums.push_back(*currentMaximum);
			//if (maximums.size()>=2) break;
		}
		I1=I2; 
		I2=I3;
		
	}
	
	//
	
	
	//return 0;
	printf("Processing maximums\n");

//******************************  Opredelim dlidelnost' kajdogo impulsa na poluvysote  **********************************************************************************
	double pulseEndTime, pulseBeginTime; //vremya nachala i kinca impulsa v sekundah
	int pulseEndOffset;
	int currentOffset;
	
	for (i=0; i<maximums.size(); i++){
	    
	    currentMaximum=&(maximums[i]);//dalee budem izmenyat' to, chto lejit v maximums
	    fseek(file, currentMaximum->position, SEEK_SET);  //perevodim ukazatel na ocherednoj impuls
	    //ischem pravuyu tochku dlya dlitelnosti Imax
	    
	    while (fscanf(file, "%lf %lf",&pulseEndTime, &I3)  == 2){
#ifdef LOGSCALE_INPUT
		if (I3<(currentMaximum->value)-log(2.0))  break;
#else
		if (I3<(currentMaximum->value)/2.0)  break;
#endif
	    }
	    currentOffset = ftell(file);
	    pulseEndOffset=currentOffset-currentMaximum->position; // raznica v bajtah mejdu Imax i Imax/2 
	    if (currentOffset<pulseEndOffset*3) {
		currentMaximum->halfWidth = -1;
		currentMaximum->energy = -1;
		continue;
		printf("Processed maximum %d\n",i);
	    }
	    
	    //ischem levuyu tochku dlya dlitelnostiImax
	    do {
		fseek(file, -2*pulseEndOffset, SEEK_CUR); // smestimsya vlevo ot tekuschej pozicii na 2*DMaxOffset
		fgets(buffer, 100, file);
		fscanf(file, "%lf %lf", &pulseBeginTime, &I3);
#ifdef LOGSCALE_INPUT
	    } while (I3>(currentMaximum->value) - log(2.0) ); //esli smestilis' malo (to est' I3>Imax/2) =>prodoljaem smeschatsja 
#else	
	    } while (I3>(currentMaximum->value)/2.0 ); //esli smestilis' malo (to est' I3>Imax/2) =>prodoljaem smeschatsja 
#endif
	    //neposredstvwnno poisk levoj tochki polushiriny
	    while (fscanf(file, "%lf %lf",&pulseBeginTime, &I3) == 2){   //nahodim tochku, v kotoroj I3 stanovitsya>Imax/2 

#ifdef LOGSCALE_INPUT
		if (I3>(currentMaximum->value) - log(2.0) )  break;  //
#else
		if (I3>(currentMaximum->value)/2.0)  break;  //
#endif
	    }
	    currentMaximum->halfWidth=pulseEndTime-pulseBeginTime;
	    

	    printf("Processed maximum %d\n",i);

	};
	
	char maximumsFileName[10000];
	sprintf(maximumsFileName,"%s.max",fileName); //formiruem imya faila s maximumami
	
	file2=fopen(maximumsFileName,"w");
	
	for (i=0; i<maximums.size(); i++){
	    fprintf(file2, "%e\t%e\t%e\t%e\t%d\t%d\n",maximums[i].time,maximums[i].value, maximums[i].halfWidth, maximums[i].energy,maximums[i].position, maximums[i].lineNumber);
	};
	
		
	fclose(file);
	fclose(file2);
	
	return 0;

}
