#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "corrector.h"

#define MAX_PALABRAS 50000

void Diccionario(char* szNombre, char szPalabras[][TAMTOKEN], int iEstadisticas[], int& iNumElementos) {
    FILE* archivo = fopen(szNombre, "r");
    if (!archivo) {
        printf("No se pudo abrir el archivo.\n");
        return;
    }

    char palabra[TAMTOKEN];
    iNumElementos = 0;

    while (fscanf(archivo, "%s", palabra) != EOF) {
        // Convertir a minúsculas
        for (int i = 0; palabra[i]; i++) {
            palabra[i] = tolower(palabra[i]);
        }

        // Buscar si la palabra ya está en el diccionario
        int existe = -1;
        for (int i = 0; i < iNumElementos; i++) {
            if (strcmp(szPalabras[i], palabra) == 0) {
                existe = i;
                break;
            }
        }

        if (existe == -1) {
            strcpy(szPalabras[iNumElementos], palabra);
            iEstadisticas[iNumElementos] = 1;
            iNumElementos++;
        }
        else {
            iEstadisticas[existe]++;
        }
    }
    fclose(archivo);
}