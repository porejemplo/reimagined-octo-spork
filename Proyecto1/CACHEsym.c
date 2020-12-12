#include <stdio.h>
#include <stdlib.h>

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
	}fclose(ficheroRam);
	
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
		
		// Fallo
		if(addr != cache[0].ETQ){
			numfallos++;
			tiempoGlobal += 10;
			printf("T: %d, Fallo de CACHE %d, ADDR %04X ETQ %X linea %02X palabra %02X bloque %02X\n", tiempoGlobal, numfallos, addr, etq, linea, palabra, bloque);
			
			// Añadir a cache los datos leidos
			// Se busca un hueco de cache vacio
			/*int posicionDeCache = 0;
			for(posicionDeCache=0; posicionDeCache<4;posicionDeCache++){
				if(cache[posicionDeCache])
			}*/
			//printf("T: %d, Fallo de CACHE %d, ADDR %04X ETQ %X linea %02X palabra %02X bloque %02X", tiempoGlobal)
		}
		// Hacierto
		else{
			printf("\n---Si esta en memoria---\n");
		}
	}fclose(ficheroMemoria);
  
	return 0;
}

//unsigned int HexToDec (char hex[32])

