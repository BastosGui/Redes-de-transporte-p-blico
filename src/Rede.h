#ifndef REDE_H
#define REDE_H

//#pragma hdrstop
//#pragma argsused

#include <stdlib.h>
#include <iostream>
#include <string.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <math.h>
#include <vector>
#include <list>
#include <algorithm>

#include "Limites.h"

using std::ifstream;
using namespace std;

struct Max_Demanda_Rota
{
  int demanda_maxima = 0;
  int no1, no2 = 0;
};

class Solucao{;
    public:
    Solucao () {};
    list<int> caminho;
    int fo;
    double prob;
    int rotas_por_arco[NOS_NA_REDE][NOS_NA_REDE] = {0};
    int demandas[NOS_NA_REDE][NOS_NA_REDE] = {0};
    Max_Demanda_Rota demandas_maximas_por_rota[NOS_NA_REDE];
    int frota_por_rota[15];
    int viagens_por_rota[15];
    int headways[15];
    int tempo_de_ciclo[15];
};

class Rede{
   public:
      Rede(){};
      int iarc[MAX_ARC];            // origem do arco
      int jarc[MAX_ARC];            // destino do arco
      int custo[MAX_ARC];           // custos nos arcos
      int custo_orig[MAX_ARC];      // custo original para recuperar
      int spur_arc[MAX_ARC];        // arco de desvio
      int cap[MAX_ARC];             // capacidade do arco
      int na_rede[MAX_ARC];         // 1 se o arco esta em alguma rota, 0 caso contr�rio
      int maq_no[MAX_NOD];          // maquina/tarefa jornada associado ao n� na rede de melhoria
      int d[MAX_NOD];               // dist�ncia minima do no origem
      int b[MAX_NOD];
      int demanda[NOS_NA_REDE][NOS_NA_REDE]; // tabela de demandas
      int pred[MAX_NOD];            // n� predecessor no caminho m�nimo
      int cam_min[MAX_NOD];         // arc pred no caminho m�nimo
      int cam_spur[MAX_NOD];        // arcos no ultimo caminho minimo
      int s_barra[MAX_NOD];         // 1 se pertence ao S barra e 0 cc
      int m;                        // contador de arcos (total de arcos)
      int n;                        // contador de n�s
      int arc_max_custo;            // arco com o maior custo
      int src, snk;                 // origem e destino do caminho
      int marca[MAX_NOD];           //marcar se o n� j� foi visitado ou n�o na busca
      list <int> lista_saida[MAX_NOD];// lista dos arcos que saem do no[i]
      list <int> lista_entrada[MAX_NOD];// lista dos arcos que chegam no no[i]
      void zera_rede();             // limpa todas as informa��es da rede
};

class nodo {
public: nodo(){};
  int indice;
  int fo;
  float prob;
};

class Caminho{ int cam_min[MAX_NOD];         // arc pred no caminho m�nimo
public:
    Caminho(){};
    int cam_spur[MAX_NOD];
    int custo;
    int custo_passageiro;
    int custo_operador;
    list<int> list_nos;
};

//------------------------------------------------------------------------------
void le_rede(Rede &rede, char arquivo[]);
//------------------------------------------------------------------------------
bool le_demandas(Rede &rede, char arquivo[]);
//------------------------------------------------------------------------------
void atualiza_arc_spur(Rede &rede, Caminho A1);
//------------------------------------------------------------------------------
void imprime_rede(Rede rede);
//------------------------------------------------------------------------------
void inicializarDistancias(Rede rede, int d[], int nalista[], int janalista[]);
void preencherU(list<int> U, int n);
void Dijkstra(Rede &rede);
list<int> Dijkstra_reverso(Rede &rede, int, int);
list<int> Tarjan1 (Rede rede);
list<int> BFCT(Rede rede);
list<int> Rot_Modif(Rede &rede);     //Agoritmo de Dijkstra
list<int> caminhoCiclo(int ant[], int arc[], int v, int w);
void imprimirDados(int n, int d[], int ant[], int suc[]);
//void k_shortest_path(Rede &rede, int origem, int destino, int K);
int k_shortest_path1(Rede &rede, int origem, int destino, int K, Caminho A[]);
int custo_cam(Rede &rede, int origem, int destino);
//void k_shortest_path_simple(Rede &rede, int origem, int destino, int K);
void retorna_custo_arco(Rede &rede);
void limpa_B(Caminho B[], int pos_min, int cont);
void atualiza_arc_pred(Rede &rede , int spurNode);
//int calc_custo_sub_cam(int spurNode, list<int>);
void copia_vetor(int vet_orig[], int vet_dest[], int dim);
int cria_banco_rotas(Rede rede, Caminho B[MAX_ARC], int K, float taxa, list <int> sBO[MAX_NOD], list <int> sBOD[MAX_NOD][MAX_NOD], list <int> rota_no[MAX_NOD], int tam_min, int tempo_max);

void calcula_rotas_por_arco(Solucao &solucao, Caminho B[MAX_ARC]);
void calcula_demandas_maximas_por_rota(Rede rede, Solucao &solucao, Caminho B[MAX_ARC]);
void calcula_tempos_de_ciclo(Caminho B[MAX_ARC], Solucao &solucao);
void calcula_frota_por_rota(Solucao &solucao);
void calcula_viagens_por_rota(Solucao &solucao);
//------------------------------------------------------------------------------
//list <int> rot_modificado(class Rede1 &rede, DadosEntrada LeDados);
//------------------------------------------------------------------------------
#endif
