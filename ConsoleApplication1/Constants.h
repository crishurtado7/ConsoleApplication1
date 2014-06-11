#include <iostream>

/* Divisions de cada sector dels histogrames de flux òptic */
const int MOVIMENT_PLA = 4;
const int MOVIMENT_ALT = 3;
const int ESPAI_X = 3;
const int ESPAI_Y = 3;
const int ESPAI_Z = 3;

/* Origen del sistema de coordenades respecte la persona */
const int ORIGENX = 180;
const int ORIGENY = 200;
const float Z_CAMERA = 178;

/* Número de sectors de les divisions espaials */
const int SECTORS_X = 3;
const int SECTORS_Y = 3;
const int SECTORS_Z = 3;

/* Valors per a cadascuna de les divisions espaials */
const int DIVISIO_X = 200;
const int DIVISIO_Y = 200;
const int DIVISIO_Z = 200;

/* Valors per al nombre d'activitats i el nombre d'execucions per a cada una */
const int NUMERO_ACTIVITATS = 3;
const int REPETICIONS_ACTIVITAT = 5;