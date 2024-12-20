// solicitudes.c

#include "solicitudes.h"
#include <stdio.h>

static int obtener_auto_by_id(void *NotUsed, int argc, char **argv, char **azColName) {
    int i;
    for (i = 0; i < argc; i++) {
        printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
    }
    printf("\n");
    return 0;
}


// Implementación de la función
int ejecutarConsulta(sqlite3 *db, const char *consulta) {
    char *zErrMsg = 0;
    int rc = sqlite3_exec(db, consulta, obtener_auto_by_id, 0, &zErrMsg);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }

    return rc;
}

int main() {
    sqlite3* db;
    int rc = sqlite3_open("../Database/DatosHuguis", &db);
    if(rc){
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return(1);
    }
    rc = ejecutarConsulta(db, "SELECT * FROM Trabajo;");
    sqlite3_close(db);
}