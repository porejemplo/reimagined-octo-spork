#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#define SLEEP 2

typedef struct{
	short int ETQ;
	short int Datos[8];
}T_LINEA_CACHE;

int tiempoGlobal = 0;

int main (){
	int numfallos = 0;
	unsigned char RAM[1024] = {'\0'};
	unsigned char RAMSexy[128][8];
	char leerLinea[4]= {'\0'};
	unsigned int addr = 0;
	char texto[100] = {'\0'};
	
	T_LINEA_CACHE cache[4];
	for (int i=0; i<4; i++){
		cache[i].ETQ = 0xFF;
		for(int ii=0; ii<8; ii++){
			cache[i].Datos[ii] = 0;
		}
	}
	
	FILE *ficheroRam = fopen("RAM.bin","r");
	
	if(ficheroRam == NULL){
		printf("Error en dicehro accesos_memoria.txt\n");
		exit(-1);
	}
	
	while(fscanf(ficheroRam,"%s",RAM)!=EOF){
		//printf("LINEA RAM: %s\n", RAM);
	}
	fclose(ficheroRam);
	
	//Dividir la RAM a RAMSexy
	for (int i=0; i<128; i++){
		for(int ii=0; ii<8; ii++){
			RAMSexy[i][ii] = RAM[i*8+ii];
		}
	}
	
	FILE *ficheroMemoria = fopen("accesos_memoria.txt", "r");
	
	if(ficheroMemoria == NULL){
		printf("Error en dicehro accesos_memoria.txt\n");
		exit(-1);
	}
	
	while(fscanf(ficheroMemoria, "%s",leerLinea) != EOF){
		
		sscanf(leerLinea, "%x", &addr);
		
		// Se sacan los valores de del addr
		unsigned int bloque = addr >> 5;
		unsigned int linea = (addr & 0b0000011000) >> 3;
		unsigned int palabra = addr & 0b0000000111;
		unsigned int etq = addr >> 3;
		
		//Se busca la etiqueta en la cache
		int pos=4;
		for (int i=0; i<4; ++i){
			if(cache[i].ETQ == etq || cache[i].ETQ==0xFF){
				pos = i;
				break;
			}
		}
		
		if(pos == 4){
			// Borrar dato
			pos = 0;
			cache[pos].ETQ = 0xFF;
			for(int i=0; i<8; i++){
				cache[pos].Datos[i]=0;
			}
		}
		
		if (cache[pos].ETQ != etq){
			printf("T: %d, FALLO de CACHE %d, addr %04X etq %X linea %02X palabra %02X bloque %02X\n", tiempoGlobal, numfallos, addr, bloque, linea, palabra, etq);
			// Penalizaciones
			numfallos++;
			tiempoGlobal+=10;
			// Guaradar datos en su posicion.
			printf("Cargando el bloque %02X en la linea %02X.\n", bloque, linea);
			cache[pos].ETQ = etq;
			for(int i=0; i<8; i++){
				cache[pos].Datos[i]=RAMSexy[etq][7-i];
			}
		}
		//Guardar datos en palabra.
		char c = cache[pos].Datos[7-palabra];
		printf("T: %d, Acierto de CACHE, ADDR %04X ETQ %X linea %02X palabra %02X DATO %02X\n", tiempoGlobal, addr, bloque, linea, palabra, c);
		strncat(texto, &c, 1);
		//sleep(SLEEP);
	}
	fclose(ficheroMemoria);
	
	for (int i=0; i<4; i++){
        printf("ETQ: %X\tDatos", cache[i].ETQ >> 2);
        for(int ii=0; ii<8; ii++){
            printf(" %02X", cache[i].Datos[ii]);
        }
        printf("\n");
    }
	
	printf("%s.\n", texto);
  
	return 0;
}
