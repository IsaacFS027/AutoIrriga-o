#include "variaveis.h"

// Aloca a memória e define os valores iniciais
float g_temperatura = 25.0;
int g_humidade = 60;
int g_irrigacao08hr = 0;
int g_irrigacao17hr = 0;
bool g_bomba_ligada = false;
int64_t g_tempo_inicio_bomba;
int contador_dht = 0;