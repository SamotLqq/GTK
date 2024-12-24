#include "aux.h"
#include <stdio.h>

// obtenemos el ultimo caracter de una cadena con el formato "nombre_[idnum]", -1 error.
int obtener_id_numerico(char* idCompleto) {
    int idNumerico;

    if (sscanf(idCompleto, "editar_trabajo_%d", &idNumerico) == 1) {
        printf("El número extraído es: %d\n", idNumerico);
        return idNumerico;
    } else if (sscanf(idCompleto, "eliminar_trabajo_%d", &idNumerico) == 1) {
        printf("El número extraído es: %d\n", idNumerico);
        return idNumerico;
    } else if (sscanf(idCompleto, "trabajo_%d", &idNumerico) == 1) {
        printf("El número extraído es: %d\n", idNumerico);
        return idNumerico;
    } else {
        printf("Formato inválido.\n");
        return -1;
    }
}