#ifndef FUNCAO_TESTE_H_INCLUDED
#define FUNCAO_TESTE_H_INCLUDED
#include <random>
void desmarca_rede (Rede &rede1);
void pesquisa (Rede &rede1);
int pop_inicial (Solucao vetSol [MAX_NOD], int pop_total, int num_max_rotas, Rede rede, list <int> sBOD[MAX_NOD][MAX_NOD], list <int> sBO[MAX_NOD], list <int> rota_no[MAX_NOD], Caminho B[MAX_ARC]);
void ler_rede (Rede rede1);
void no_isolado (Rede rede, list <int> sBO[MAX_NOD], Solucao &S1, Caminho B[MAX_ARC],int atendidos [MAX_NOD]);
int randomiza(list <int> lista);
void emb_vet(int emb[], int max_pos);
int calc_n_atendidos(int n, int nao_atendidos[], int emb[]);
bool busca_aux(int numero, list<int> lista_de_numeros);
int fo_por_Transbordos(Solucao S1, list <int> rota_no[MAX_NOD],  Rede &rede);
int sol_viavel (Solucao &S1, Rede &rede, Caminho B[MAX_ARC], list <int> rota_no[MAX_NOD]);
void imprime(Solucao vetSol[MAX_NOD],int pop_total, int num_max_rotas, Rede rede, list <int> sBOD[MAX_NOD][MAX_NOD], list <int> sBO[MAX_NOD], list <int> rota_no[MAX_NOD], Caminho B[MAX_ARC], int &b);
void conjunto_Elite(Solucao vetSol[MAX_NOD],int pop_total, int num_max_rotas, Rede rede, list <int> sBOD[MAX_NOD][MAX_NOD], list <int> sBO[MAX_NOD], list <int> rota_no[MAX_NOD], Caminho B[MAX_ARC], int b, int n_elite, list<int> &elite);
list <int> fora_elite(list<int>elite,int b);
void probabilidade(Solucao vetSol[MAX_NOD], list<int> &elite, list<int> &foraElite);
void new_Probability(Solucao vetSol[POP_TOTAL+1], list<int> lista);
int selecao(list<int> lista, Solucao vetSol[MAX_NOD]);
int selec_rand(list<int> b, Solucao vetsol[POP_TOTAL+1], list<int> auxiliar);
bool soluc_is_equal(Solucao first, Solucao second);
bool isOnList(list<int> lista, int numero);
bool compare_nocase (const Solucao first, const Solucao second);
list<Solucao> cruzamento (double percent_cruzamento, int b, list<int>elite, Solucao vetSol[MAX_NOD], Caminho B[MAX_ARC], Rede rede, int n_elite, list<int> &foraElite, list <int> rota_no[MAX_NOD]);
void remove_Equal_Solutions(list<Solucao> lista);
bool campare_nocase(const Solucao first, const Solucao second);
void selecao_pos_cruzamento(Solucao vetSol[MAX_NOD], int pop_total, list<Solucao> filhos, int percentElit, int percentPseudo, int percentRand);
list<Solucao> mutacao(double percent_mutacao,int b, Solucao vetSol[MAX_NOD],double prob_mut_sol, int prob_mut_caminho, Caminho B[MAX_ARC], list <int> sBOD[MAX_NOD][MAX_NOD], Rede rede,double percent_sorteio, list <int> rota_no[MAX_NOD]);
#endif // FUNCAO_TESTE_H_INCLUDED
