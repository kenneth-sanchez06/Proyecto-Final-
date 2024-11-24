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
void ClonaPalabras(char* szPalabraLeida, char szPalabrasSugeridas[][TAMTOKEN], int& iNumSugeridas) {
    iNumSugeridas = 0;
    int len = strlen(szPalabraLeida);

    // Eliminar caracteres
    for (int i = 0; i < len; i++) {
        char copia[TAMTOKEN];
        strcpy(copia, szPalabraLeida);
        memmove(&copia[i], &copia[i + 1], len - i);
        strcpy(szPalabrasSugeridas[iNumSugeridas++], copia);
    }

    // Transposición de caracteres
    for (int i = 0; i < len - 1; i++) {
        char copia[TAMTOKEN];
        strcpy(copia, szPalabraLeida);
        char temp = copia[i];
        copia[i] = copia[i + 1];
        copia[i + 1] = temp;
        strcpy(szPalabrasSugeridas[iNumSugeridas++], copia);
    }

    // Sustitución de caracteres
    const char alfabeto[] = "abcdefghijklmnopqrstuvwxyzáéíóú";
    for (int i = 0; i < len; i++) {
        for (int j = 0; j < strlen(alfabeto); j++) {
            char copia[TAMTOKEN];
            strcpy(copia, szPalabraLeida);
            copia[i] = alfabeto[j];
            strcpy(szPalabrasSugeridas[iNumSugeridas++], copia);
        }
    }

    // Inserción de caracteres
    for (int i = 0; i <= len; i++) {
        for (int j = 0; j < strlen(alfabeto); j++) {
            char copia[TAMTOKEN];
            strncpy(copia, szPalabraLeida, i);
            copia[i] = alfabeto[j];
            strcpy(&copia[i + 1], &szPalabraLeida[i]);
            strcpy(szPalabrasSugeridas[iNumSugeridas++], copia);
        }
    }
}