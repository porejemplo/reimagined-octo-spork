#include <stdio.h>
#include <stdlib.h>

typedef struct{
	short int ETQ;
	short int Datos[8];
}T_LINEA_CACHE;

int variableglobal = 0;

int main (){
	int numfallos = 0;
	unsigned char RAM[1024];
	char leerLinea[4];
	int numero = 0;
	
	T_LINEA_CACHE test;
	test.ETQ == 0xFF;
	
	FILE *f = fopen("RAM.bin","r");
	
	FILE *f2 = fopen("accesos_memoria.txt", "r");
	while(fscanf(f2, "%s",leerLinea) != EOF){
		
		printf("LINEA: %s\n",leerLinea);
		sscanf(leerLinea, "%x", &numero);
		printf("LINEA (int): %x\n", numero);
		
		if(numero != test.ETQ){
			numfallos++;
			variableglobal += 10;
			printf("T: %d, Fallo de CACHE %d, ADDR %04X ETQ %X linea %02X palabra %02X bloque %02X");
		}
	}fclose(f2);
  
	return 0;
}