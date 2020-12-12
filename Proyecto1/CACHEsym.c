#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

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
		printf("LINEA RAM: %s\n", RAM);
	}
	fclose(ficheroRam);
	
	//Dividir la RAM a RAMSexy
	int i = 0; // Contador 1
	int ii = 0; // Contador 2
	for (i=0; i<128; i++){
		for(ii=0; ii<8; ii++){
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
		unsigned int etq = addr >> 7;
		unsigned int linea = (addr & 0b0001100000) >> 5;
		unsigned int palabra = addr & 0b0000011111;
		unsigned int bloque = addr >> 5;
		
		//Se busca la etiqueta en la cache
		pos=4;
		for (int i=0; i<4; ++i){
			if(cache[i].ETQ == etq){
				poss = i;
				printf("T: %d, Acierto de CACHE, ADDR %04X ETQ %X linea %02X palabra %02X DATO %02X\n", tiempoGlobal, addr, etq, linea, palabra, bloque);
				break;
			}
			else if (cache[i].ETQ==0xFF){
				numfallos++;
				tiempoGlobal+=10;
				pos = i;
				printf("T: %d, Fallo de CACHE %d, ADDR %04X ETQ %X linea %02X palabra %02X bloque %02X\n", tiempoGlobal, numfallos, addr, etq, linea, palabra, bloque);
				printf("Cargando el bloque %02X en la linea %02x.\n", bloque, linea);
				break;
			}
		}
		
		if(pos == 4){
			// Borrar dato
			cache[0].ETQ = 0xFF;
			for(int i=0; i<8; ii++){
				cache[0].Datos[ii] = 0;
			}
			pos = 0;
		}
		
		// Guaradar datos en su posicion.
		cache[pos].ETQ = etq;
		for(int i=0; i<8; i++){
			if(cache[pos].Datos[i] == 0){
				cache[pos].Datos[i]=RAMSexy[bloque][7-i];
				break;
			}
		}
		sleep(2);
	}
	fclose(ficheroMemoria);
  
	return 0;
}

//int huecoEnCache (unsigned int etq){}
//unsigned int HexToDec (char hex[32])

