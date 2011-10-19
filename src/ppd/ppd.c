#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <stdlib.h>
#include "getconfig.h"
#include "ppd.h"


config_t vecConfig;
char * bufferConsola;

int main()
{
	vecConfig = getconfig("config.txt");
	dirMap = abrirArchivoV(vecConfig.rutadisco);
	//conectar()
	//CREAR TRES HILOS, UNO PARA ESCUCHAR PEDIDOS Y ENCOLARLOS, OTRO PARA ATENDER LOS PEDIDOS Y OTRO PARA LA CONSOLA
	escucharPedidos();
	//escucharConsola()
	return 1;
}


void escucharPedidos(void)
{
	//HACER UN WHILE Q ESCUCHE PEDIDOS
	atenderPedido(/*dirmap, payloadDescriptor, sect, param*/);

return;
}

void atenderPedido(void)
{

	//switch(payloadDescriptor)   TODO
	//{
		//case LEER:
			//leerPedido(sect);
			//break;
		//case ESCRIBIR:
			//escribirPedido(sect, payload);
			//break;
		//default:
			//printf("Error comando PPD";
			//break;
	//}

	//    ### HAY Q DEFINIR LOS VALORES PARA LEER Y ESCRIBIR Y DEJAR EL SWITCH DE ARRIBA ###

	//recv()   ###Queda esperando a q lleguen pedidos###
	if(strcmp(comando,"leer") == 0)
	{
		//pasar lo q me mandan como parametro a un int
		leerSect(sect, dirArch);
	}
	else
	{
		if(strcmp(comando,"escribir") == 0)
		{
			escribirSect(param, sect, dirArch);
		}
		else
		{
			printf("Ha ingresado un comando invalido\n");
		}
	}
}

void escucharConsola()
{
	//while que escuche consola
	atenderConsola(/*se le pasa lo que llega desde la consola*/);
}

void atenderConsola()
{
	if(strcmp(comando,"info") == 0)
		{
			funcInfo();
		}
		else
		{
			if(strcmp(comando,"clean") == 0)
			{
				funcClean();
			}
			else
			{
			if(strcmp(comando,"trace") == 0)
			{
				funcTrace();
			}
			else
			{
				printf("Ha ingresado un comando invalido desde la consola\n");
			}
			}

		}
}


//---------------Funciones PPD------------------//


void leerSect(int sect, FILE * dirArch)
{

	if( (0 >= sect) && (cantSect <= sect))
	{
		dirMap = (int *) paginaMap(sect, dirArch);
		res = div(sect, 8);
		dirSect = dirMap + ((res.rem *8 *512 ) - 1);  //NO SE SI VA O NO EL -1    TODO
		memcpy(buffer, dirSect, TAM_SECT);
		if(0 != munmap(dirMap,TAM_PAG))
			printf("Fallo la eliminacion del mapeo\n");
		//mando el buffer por el protocolo al raid
	}
	else
	{
		printf("El sector no es valido\n");
	}


}


void escribirSect(char param[TAM_SECT], int sect, FILE * dirArch)
{
	//validar numero de sector
	//buscar en el puntero del archivo la direccion donde arrancaria el sector
	//buffer = (char *) malloc (TAM_SEC);
	//inicializo un buffer (menset) y copio lo que hay q escribir
	//copio el buffer a la memoria mapeada
	//doy aviso de OK

}

FILE * abrirArchivoV(char * pathArch)			//Se le pasa el pathArch del config. Se mapea en esta funcion lo cual devuelve la direccion en memoria
{
	if (NULL == (dirArch = fopen(pathArch, "w+")))
	{
		printf("%s\n",pathArch);
		printf("Error al abrir el archivo de mapeo\n");
	}
	else
	{
			printf("El archivo se abrio correctamente\n");
			return dirArch;
	}
return 0;
}

//---------------Funciones Aux PPD------------------//

void * paginaMap(int sect, FILE * dirArch)
{
	res = div(sect, 8);
	offset = (res.quot * 512);
	dirMap = mmap(NULL,TAM_PAG, PROT_WRITE, MAP_SHARED, (int) dirArch , offset);

	return dirArch;
}

//---------------Funciones Consola------------------//


void funcInfo()
{
	sprintf(bufferConsola, "%d", vecConfig.posactual);
	//send(socket,bufferConsola,strlen(bufferConsola),0);
	return;
}

void funcClean(/*char * parametros*/)
{
	//char * strprimSec, * strultSec;
	//int primSec, ultSec;

	//strprimSec = strtok(parametros, ",");
	//strultSec = strtok(NULL, "\0");
	//primSec = atoi(strprimSec);
	//ultSec = atoi(strultSec);
	//memset(bufferConsola, '\0', TAM_SECT);
	//while(primsec <= ultsec)
	//{
	//	escribirSect(bufferConsola, primsec, dirArch);
	//	primsec++;
	//}

	strcpy(bufferConsola, "Se han limpiado correctamente los sectores");
	//send(socket,bufferConsola,strlen(bufferConsola),0);
	return;
}

void funcTrace(/*char * parametros*/)
{
	//int cantparam = 0;
	//simular el disco
	return;
}

