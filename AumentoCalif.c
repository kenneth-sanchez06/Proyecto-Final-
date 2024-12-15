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

void ListaCandidatas(char szPalabrasSugeridas[][TAMTOKEN], int iNumSugeridas, char szPalabras[][TAMTOKEN],
    int iEstadisticas[], int iNumElementos, char szListaFinal[][TAMTOKEN],
    int iPeso[], int& iNumLista) {
    iNumLista = 0;
    for (int i = 0; i < iNumSugeridas; i++) {
        for (int j = 0; j < iNumElementos; j++) {
            if (strcmp(szPalabrasSugeridas[i], szPalabras[j]) == 0) {
                strcpy(szListaFinal[iNumLista], szPalabras[j]);
                iPeso[iNumLista] = iEstadisticas[j];
                iNumLista++;
            }
        }
    }
    char palabra[TAMTOKEN];
    iNumElementos = 0;

    while (fscanf_s(archivo, "%49s", palabra, (unsigned)_countof(palabra)) != EOF) {
        int inicio = 0; // Índice de inicio para dividir palabras tras separadores
        for (int i = 0; palabra[i]; i++) {
            // Si encontramos una coma o punto, procesamos la subpalabra actual
            if (palabra[i] == ',' || palabra[i] == '.') {
                if (i > inicio) {
                    char subPalabra[TAMTOKEN];
                    int j = 0;
                    for (int k = inicio; k < i && j < TAMTOKEN - 1; k++) {
                        if (isalnum((unsigned char)palabra[k]) || strchr("áéíóúÁÉÍÓÚ'&$#%!<[]>‐-+/\".", palabra[k])) {
                            subPalabra[j++] = tolower((unsigned char)palabra[k]);
                        }
                    }
                    subPalabra[j] = '\0'; // Termina la subpalabra

                    if (strlen(subPalabra) > 0) {
                        // Agregar la subpalabra al diccionario
                        int existe = -1;
                        for (int l = 0; l < iNumElementos; l++) {
                            if (strcmp(szPalabras[l], subPalabra) == 0) {
                                existe = l;
                                break;
                            }
                        }

                        if (existe == -1 && iNumElementos < NUMPALABRAS) {
                            strcpy_s(szPalabras[iNumElementos], TAMTOKEN, subPalabra);
                            iEstadisticas[iNumElementos] = 1;
                            iNumElementos++;
                        }
                        else if (existe != -1) {
                            iEstadisticas[existe]++;
                        }
                    }
                }
                inicio = i + 1; // Reiniciar después del separador
            }
        }

        // Procesar cualquier palabra restante después del último separador
        if (palabra[inicio]) {
            char subPalabra[TAMTOKEN];
            int j = 0;
            for (int k = inicio; palabra[k] && j < TAMTOKEN - 1; k++) {
                if (isalnum((unsigned char)palabra[k]) || strchr("áéíóúÁÉÍÓÚ'&$#!%<[]>‐-+/\".", palabra[k])) {
                    subPalabra[j++] = tolower((unsigned char)palabra[k]);
                }
            }
            subPalabra[j] = '\0'; // Termina la subpalabra

            if (strlen(subPalabra) > 0) {
                // Agregar la subpalabra al diccionario
                int existe = -1;
                for (int l = 0; l < iNumElementos; l++) {
                    if (strcmp(szPalabras[l], subPalabra) == 0) {
                        existe = l;
                        break;
                    }
                }

                if (existe == -1 && iNumElementos < NUMPALABRAS) {
                    strcpy_s(szPalabras[iNumElementos], TAMTOKEN, subPalabra);
                    iEstadisticas[iNumElementos] = 1;
                    iNumElementos++;
                }
                else if (existe != -1) {
                    iEstadisticas[existe]++;
                }
            }
        }
    }
    fclose(archivo);

    // Ordenar el diccionario alfabéticamente
    for (int i = 0; i < iNumElementos - 1; i++) {
        for (int j = i + 1; j < iNumElementos; j++) {
            if (strcmp(szPalabras[i], szPalabras[j]) > 0) {
                char temp[TAMTOKEN];
                strcpy_s(temp, TAMTOKEN, szPalabras[i]);
                strcpy_s(szPalabras[i], TAMTOKEN, szPalabras[j]);
                strcpy_s(szPalabras[j], TAMTOKEN, temp);

                int tempEstadistica = iEstadisticas[i];
                iEstadisticas[i] = iEstadisticas[j];
                iEstadisticas[j] = tempEstadistica;
            }
        }
    } 
}
