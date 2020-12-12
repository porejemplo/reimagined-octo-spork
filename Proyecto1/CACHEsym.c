#include <stdio.h>
#include <stdlib.h>

typedef struct{
	short int ETQ;
	short int Datos[8];
}T_LINEA_CACHE;

int variableglobal = 0;

int main (){
	int numfallos = 0;
	unsigned char RAM[1024] = {'\0'};
	unsigned char RAMSexy[128][8];
	char leerLinea[4]= {'\0'};
	int numero = 0;
	
	T_LINEA_CACHE test;
	test.ETQ = 0xFF;
	for(int i = 0; i < 8; i++){
		test.Datos[i] = 0;
	}
	
	FILE *ficheroRam = fopen("RAM.bin","r");
	if(ficheroRam == NULL){
		exit(-1);
	}
	while(fscanf(ficheroRam,"%s",RAM)!=EOF){
		printf("LINEA RAM: %s\n", RAM);
	}fclose(ficheroRam);
	
	//Dividir la RAM a RAMSexy
	int i = 0; // Contador 1
	int ii = 0; // Contador 2
	for (i=0; i<128; i++){
		for(ii=0; ii<8; ii++){
			RAMSexy[i][ii] = RAM[i*128+ii];
		}
	}
	
	FILE *ficheroMemoria = fopen("accesos_memoria.txt", "r");
	if(ficheroMemoria == NULL){
		exit(-1);
	}
	while(fscanf(ficheroMemoria, "%s",leerLinea) != EOF){
		
		printf("LINEA: %s\n",leerLinea);
		sscanf(leerLinea, "%x", &numero);
		printf("LINEA (int): %x\n", numero);
		
		if(numero != test.ETQ){
			numfallos++;
			variableglobal += 10;
			printf("Nº Fallos: %d\nNº Variableglobal: %d\n", numfallos, variableglobal);
		}
	}fclose(ficheroMemoria);
  
	return 0;
}

