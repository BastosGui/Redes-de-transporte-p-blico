#include <iostream>
#include "Rede.h"
#include <list>
#include<stdio.h>
#include "Funcao_teste.h"

using namespace std;

//funcao embaralha_vetor
//=========================================================================
void emb_vet(int emb[], int max_pos) // embaralha o vetor de �ndices emb
//=========================================================================
{
    int i, j, k;
    int aux;
    for (i = 1; i <= max_pos; i++)
        emb[i] = i; // emb[i] tera valores de 1 ate max_pos

    for (i = 1; i <= 70*max_pos; i++)
    {
        j = rand()% max_pos + 1; //sorteia uma das jornadas
        k = rand()% max_pos + 1;  //sorteia outra jornada		5

        aux = emb[j]; //aux recebe o valor da jornada j
        emb[j] = emb[k]; // a jornada j recebe o valor da jornada k
        emb[k] = aux; // a jornada k recebe o valor da jornada j
    }
}
//==============================================================================
int randomiza(list <int> lista) //randomiza uma lista qualquer
//==============================================================================
{
    int r;
    list<int>:: iterator itn;
    int aux = 0;
    //for (int k = 1; k < 10000000; k++)
    //aux = (aux + 1);


    r = rand() % lista.size()+1;
    //printf("\nSize %d r = %d ", lista.size(), r);
    itn = lista.begin();
    advance(itn, r-1);
    //printf(" cam %d \n", *itn);
    return(*itn);
}

//============================================================================
void ler_rede (Rede rede1) //faz a leitura da rede
//============================================================================
{
    list<int>::iterator it;
    le_rede(rede1, "rede_Mandl.txt");
    for(it=rede1.lista_saida[3].begin(); it!=rede1.lista_saida[3].end(); it++)
    {
        cout<< " " << *it;
        cout<< " " << rede1.iarc[*it];
        cout<< " -> " << rede1.jarc[*it];
        cout<< " " << rede1.custo[*it]<<endl;
    }
}

//=====================================================================================
void desmarca_rede (Rede &rede1) //desmarca a rede, adicionando 0 em seus nos
//=====================================================================================
{
    for(int i=1; i<=rede1.n; i++)
        rede1.marca[i]=0;
}

//======================================================================================
void pesquisa (Rede &rede) //
//======================================================================================
{
    desmarca_rede (rede);

    list<int>::iterator it;

    rede.marca[rede.src]=1;
    rede.pred[rede.src]=0;
    int next=1;
    int order [MAX_NOD];
    order[next]=rede.src;

    list <int> Lista;
    Lista.push_back(rede.src);
    while (Lista.size()!=0)
    {
        int no = Lista.front();
        Lista.pop_front();
        for (it = rede.lista_saida[no].begin(); it != rede.lista_saida[no].end(); it++)
        {
            if (rede.marca[rede.jarc[*it]] == 0 & rede.na_rede[*it] > 0)
            {
                rede.marca[rede.jarc[*it]] = 1;
                rede.pred[rede.jarc[*it]] = no;
                next = next + 1;
                order[rede.jarc[*it]] = next;
                Lista.push_back(rede.jarc[*it]);
            }

        }
    }
    //printf("\n no | marca ");
    //for(int i = 1; i <= rede.n; i++)
    //    printf("\n %d | %d ", i, rede.marca[i]);
}

//===================================================================================
bool busca_aux(int numero, list<int> lista_de_numeros){
//===================================================================================
    list<int>::iterator iter = lista_de_numeros.begin();
    while(iter != lista_de_numeros.end()){
        if(*iter == numero)
            return true;
        iter++;
    }
    return false;
}

//=============================================================================
int fo_por_Transbordos(Solucao S1, list <int> rota_no[MAX_NOD],  Rede &rede){
//=============================================================================
    
    int peso_do_transbordo = 1;
    int resultado = 0;
    list<int>::iterator iter;
    list<int> aux_rotas;
    list<int> aux_nos;
    /*for(int i =0; i<rede.n ; i++){
        aux_nos.push_back(i);
    }*/
    for(int i = 0; i<rede.n-1; i++){
        aux_rotas.clear();
        aux_nos.clear();
        for(iter = S1.caminho.begin(); iter != S1.caminho.end(); iter++){
            if(busca_aux(*iter, rota_no[i]))
                aux_rotas.push_back(*iter);
        }
        for(int j = i+1; j<rede.n; j++){
            for(list<int>::iterator it = aux_rotas.begin(); it != aux_rotas.end(); it++){
                if(busca_aux(*it, rota_no[j]))
                    aux_nos.push_back(j);
            }
        }
        aux_nos.sort();
        aux_nos.unique();
        resultado+=rede.n-aux_nos.size();
    }

    return (resultado * peso_do_transbordo);
}

int fo_por_frota(Solucao S1){
    int peso_da_frota = 1;
    int tam_frota = 0;
    for(int i = 0; i<S1.caminho.size(); i++){
        tam_frota+= S1.frota_por_rota[i];
    }
    return (tam_frota * peso_da_frota);
}

//=======================================================================================
int sol_viavel (Solucao &S1, Rede &rede, Caminho B[MAX_ARC], list <int> rota_no[MAX_NOD]) //verifica se a solucao e viavel (1) ou se n�o e (0)
//=======================================================================================
{
    list<int>::iterator iter, iter1, iter2;
    int it, z = 0, z1 = 0;
    for (it = 1; it <= rede.m; it++)
        rede.na_rede[it] = 0;

    for (iter = S1.caminho.begin(); iter!=S1.caminho.end(); iter++)
    {

        int caminho=*iter;
        for ( iter1 = B[caminho].list_nos.begin(); iter1 != B[caminho].list_nos.end(); iter1 ++)
        {
            int no = *iter1;
            z += rede.custo[B[caminho].cam_spur[no]];
            rede.na_rede[B[caminho].cam_spur[no]] = 1;
            //printf("\narco na rede: %d -> %d (%d) ", rede.iarc[B[caminho].cam_spur[no]], rede.jarc[B[caminho].cam_spur[no]], B[caminho].cam_spur[no]);
            // incluindo o arco reverso do
            for (iter2 = rede.lista_saida[rede.jarc[B[caminho].cam_spur[no]]].begin(); iter2 != rede.lista_saida[rede.jarc[B[caminho].cam_spur[no]]].end(); iter2++)
            {
                if (rede.jarc[*iter2] ==  rede.iarc[B[caminho].cam_spur[no]])
                {
                    rede.na_rede[*iter2] = 1;
                    //printf("\narco na rede: %d -> %d (%d) ", rede.iarc[*iter2], rede.jarc[*iter2], *iter2);
                }
            }
        }
    }
    pesquisa (rede);
    int viavel=1;
    for(int i = 1; i <= rede.n; i++)
    {
        if (rede.marca[i] == 0)
        {
            viavel = 0;
        }
    }
    /* if(S1.caminho.size()<8)
        viavel=0;*/
    if(viavel == 1){
        S1.caminho.sort();
        S1.fo = z;
        calcula_demandas_maximas_por_rota(rede, S1, B);
        calcula_viagens_por_rota(S1);
        calcula_tempos_de_ciclo(B, S1);
        calcula_frota_por_rota(S1);
        S1.fo += 100*fo_por_frota(S1);
        //S1.fo += fo_por_Transbordos(S1, rota_no, rede);
    }
    /*if (viavel == 0)
    {
        cout<<"\nSolucao nao viavel || Custo: "<< z << endl;
    }
    else
    {
        cout<<"\nSolucao viavel || Custo: "<< z << endl;
    }*/
    //getchar();

    return viavel;
}
//================================================================================================================
void nos_isolados (Rede rede, list <int> sBO[MAX_NOD], Solucao &S1, Caminho B[MAX_ARC], int atendidos [MAX_NOD])
//================================================================================================================
{
    int no;
    list<int>::iterator iter, iter1, it;
    for (no = 1; no <= rede.n; no++)
    {
        if (rede.lista_entrada[no].size ()== 1)
        {
            //cout<< "\nNo isolado: "<< no <<endl;
            if(sBO[no].size()!=0)
            {
                int cam = randomiza(sBO[no]);
                S1.caminho.push_back(cam);
                int arco;
                for (it = B[cam].list_nos.begin(); it != B[cam].list_nos.end(); it++ )
                {
                    arco = B[cam].cam_spur[*it];
                    atendidos [*it] += 1;
                }
            }
        }
    }
}

//====================================================================================================
int calc_n_atendidos(int n, int atendidos[], int nao_atendidos[], int emb[])
// calcula o num de n�s n�o atendidos a partir dos atendidos.
// usado para percorrer de forma rand�mica os n�o atendidos, via embaralhamento do ind�ce
//====================================================================================================
{
    int tot = 0;
    for(int j= 1; j <= n; j++)
        nao_atendidos[j] = 0;
    for(int j= 1; j <= n; j++)
    {
        if(atendidos[j]==0)
        {
            tot ++;
            nao_atendidos[tot] = j;
            emb[tot]= tot;
            //cout<< "No nao atendido: "<<j<<endl;
        }
    }
    return(tot);
}

int rota_mais_pesada(Solucao &soluc, list <int> rota_no[MAX_NOD]){
    //cout<<"ENTROU NO MAIS PESADO"<<endl;
    int no1 = soluc.demandas_maximas_por_rota[0].no1;
    int no2 = soluc.demandas_maximas_por_rota[0].no2;
    int aux = soluc.demandas_maximas_por_rota[0].demanda_maxima;
    //cout<<"FOR1"<<endl;
    for(int i = 1; i<soluc.caminho.size(); i++){
        if(soluc.demandas_maximas_por_rota[i].demanda_maxima > aux){
            aux = soluc.demandas_maximas_por_rota[i].demanda_maxima;
            no1 = soluc.demandas_maximas_por_rota[i].no1;
            no2 = soluc.demandas_maximas_por_rota[0].no2;
        }
    }
    //cout<<"FOR1-exit"<<endl;
    int rota;
    list<int> intersect;
    intersect.clear();
    //cout<<"FOR2"<<endl;
    for(list<int>::iterator i = rota_no[no1].begin(); i != rota_no[no1].end(); i++){
        //cout<<"FOR3"<<endl;
        for(list<int>::iterator j = rota_no[no2].begin(); j != rota_no[no2].end(); j++){
            if(*i == *j){
                intersect.push_back(*i);
            }
        }
        //cout<<"FOR3-exit"<<endl;
    }
    //cout<<"FOR2-exit"<<endl;
    if(intersect.size()>0){
        rota = randomiza(intersect);
    }
    else{
        rota = randomiza(rota_no[no2]);
    }
    //cout<<"SAIU NO MAIS PESADO"<<endl;
    return rota;
}

//================================================================================================================
int pop_inicial (Solucao vetSol [MAX_NOD], int pop_total, int num_max_rotas, Rede rede, list <int> sBOD[MAX_NOD][MAX_NOD], list <int> sBO[MAX_NOD], list <int> rota_no[MAX_NOD], Caminho B[MAX_ARC])
//gera uma populacao inicial
//================================================================================================================
{
    //cout<<"entrou"<<endl;
    int atendidos [MAX_NOD];
    //int max_nos, min_nos;
    //max_nos = 12;
    //min_nos = 5;
    int tentativas = 10;
    for(int j=1; j<=rede.n; j++){
        atendidos[j]=0;
    }
    int i = 1; // primeiro indiv�duo da popula��o
    for (int pop = 1; pop <= pop_total; pop++)
    {
        //srand (pop);//srand (time(NULL));
        //printf("\nPop = %d", pop); //getchar();
        int nao_atendidos [MAX_NOD];
        int emb [MAX_NOD];
        int tot_n_atendidos = 0;
        list<int>::iterator itn;
        int cont = 0;
        while (!sol_viavel(vetSol[i], rede, B, rota_no)& cont < tentativas)
        {
            cont ++;
            //printf("\ncont = %d", cont); //getchar();

            for(int j=1; j<=rede.n; j++)
                atendidos[j]=0;

            vetSol[i].caminho.clear();
            nos_isolados(rede, sBO, vetSol[i], B, atendidos);
            tot_n_atendidos = calc_n_atendidos(rede.n, atendidos, nao_atendidos, emb);
            emb_vet(emb, tot_n_atendidos);
            while (tot_n_atendidos > 0)
            {
                if (tot_n_atendidos > 1)
                {
                    //cout<<"ENTROU NO IF         "<<tot_n_atendidos<<endl;
                    list<int>::iterator it2;
                    //emb_vet(emb, tot_n_atendidos);
                    int emb_i1, emb_j, i1, j;

                    for (int emb_i1 = 1; emb_i1 < tot_n_atendidos; emb_i1++)
                    {
                        i1 = emb[emb_i1];
                        //for (i1=1; i1< tot_n_atendidos; i1++) {
                        for (int emb_j = emb_i1+1; emb_j <= tot_n_atendidos; emb_j++)
                        {
                            //cout<<"ENTROU NO FOR DO EMBJ"<<endl;
                            j = emb[emb_j];
                            //for (j = i1+1; j<= tot_n_atendidos; j++){
                            //printf("\ni1 %d, j %d", nao_atendidos[i1], nao_atendidos[j]);
                            int i11 = nao_atendidos[i1];
                            int j1 = nao_atendidos[j];
                            if (sBOD[i11][j1].size() != 0)
                            {
                                if(atendidos [i11] == 0 && atendidos [j1] == 0){
                                    //cout<<"ENTROU NO IF                INTERNO"<<endl;
                                    int cam = randomiza(sBOD[i11][j1]);
                                    vetSol[i].caminho.push_back(cam);
                                    int arco;
                                    for (it2 = B[cam].list_nos.begin(); it2 != B[cam].list_nos.end(); it2++ )
                                    {
                                        arco = B[cam].cam_spur[*it2];
                                        //cout<<" no "<<*it2<< " arco " << arco << endl;
                                        atendidos[*it2] += 1;
                                    }
                                    j = tot_n_atendidos; // para sair deste la�o pois o i1 j� foi atendido
                                }
                            }
                            else{
                                list<int> intersect;
                                intersect.clear();
                                //cout<<"FOR2"<<endl;
                                for(list<int>::iterator i = rota_no[i11].begin(); i != rota_no[i11].end(); i++){
                                    //cout<<"FOR3"<<endl;
                                    for(list<int>::iterator j = rota_no[j1].begin(); j != rota_no[j1].end(); j++){
                                        if(*i == *j){
                                            intersect.push_back(*i);
                                        }
                                    }
                                    //cout<<"FOR3-exit"<<endl;
                                }
                                //cout<<"FOR2-exit"<<endl;
                                //cout<<"TAMANHO DO INTERSEC:      "<<intersect.size()<<endl;
                                int rota;
                                if(intersect.size()>0){
                                    rota = randomiza(intersect);
                                }
                                else{
                                    rota = randomiza(rota_no[i11]);
                                }
                                //cout<<"ROTA:                      "<<rota<<endl;
                                //cout<<"Tamanho do caminho antes                    "<<vetSol[i].caminho.size()<<endl;
                                vetSol[i].caminho.push_back(rota);
                                vetSol[i].caminho.sort();
                                //cout<<"Tamanho do caminho  depois                   "<<vetSol[i].caminho.size()<<endl;
                                for (it2 = B[rota].list_nos.begin(); it2 != B[rota].list_nos.end(); it2++ )
                                {
                                    atendidos[*it2] += 1;
                                }
                            }
                        }
                    }
                    //cout<<"SAIU DO IF"<<endl;
                }
                else // nao_atendidos == 1
                {
                    //cout<<"ENTROU NO ELSE"<<endl;
                    //printf("\num no nao atendido: %d", nao_atendidos[1]);
                    if (rota_no[nao_atendidos[1]].size() != 0)
                    {
                        int cam = randomiza(rota_no[nao_atendidos[1]]);
                        vetSol[i].caminho.push_back(cam);
                        int arco;
                        for (list<int>::iterator it=B[cam].list_nos.begin(); it!=B[cam].list_nos.end(); it++ )
                        {
                            arco = B[cam].cam_spur[*it];
                            //cout<<" no "<<*it<< " arco " << arco << endl;
                            atendidos [*it] += 1;
                        }
                    }
                    //cout<<"SAIU DO ELSE"<<endl;
                }
                //calc_n_atendidos(atendidos, nao_atendidos, vet_aux, emb, rede, tot_n_atendidos);
                tot_n_atendidos = calc_n_atendidos(rede.n, atendidos, nao_atendidos, emb);
            } // tot_nao_atendidos > 0
            //printf("\nvetor nao-atendidos %d", tot_n_atendidos);
        } // while solu��o n�o for vi�vel
        int x_aux = vetSol[i].caminho.size();
        //cout<<"antes do while no i = "<<i<<endl;
        //int contadordow = 0;
        int min_rotas = 8+rand()%6;
        while(vetSol[i].caminho.size()<min_rotas){
            int route = rota_mais_pesada(vetSol[i], rota_no);
            //int route = 1+rand()%519;
            //cout<<"contador do while: "<<contadordow<<endl;
            vetSol[i].caminho.push_back(route);
            vetSol[i].caminho.sort();
            vetSol[i].caminho.unique();
            sol_viavel(vetSol[i], rede, B, rota_no);
            //contadordow++;
        }
        //cout<<"depois do while no i = "<<i<<endl;
        vetSol[i].caminho.unique();
        if (sol_viavel(vetSol[i], rede, B, rota_no))
        {
            i++;
        }

    } //percorre a popula��o i = 1 at� tot_pop
    //cout<<"saiu"<<endl;
    return (i-1);
}
//==========================================================================================================================================
void imprime(Solucao vetSol[POP_TOTAL+1],int pop_total, int num_max_rotas, Rede rede, list <int> sBOD[MAX_NOD][MAX_NOD], list <int> sBO[MAX_NOD], list <int> rota_no[MAX_NOD], Caminho B[MAX_ARC], int &b)
//imprime o total de solucoes (pop inicial), o valor da fo de cada solucao, e os nos e arcos percorridos por cada caminho
//==========================================================================================================================================
{
    list<int>::iterator it;
    list<int>::iterator iter;
    int i;
    b  = pop_inicial (vetSol,pop_total, num_max_rotas,rede,sBOD,sBO, rota_no, B);
    printf("Total de solucoes %d", b);
    for (i = 1; i <= b; i++)
    {
        cout<<"-------------------------------------------------------------------------------"<<endl;
        printf("\nSolucao: %d, fo: %d ", i, vetSol[i].fo);
        int rota = 0;
        for(it = vetSol[i].caminho.begin(); it != vetSol[i].caminho.end(); it++ )
        {
            printf("\ncaminho: %d ", *it);
            for ( iter = B[*it].list_nos.begin(); iter != B[*it].list_nos.end(); iter ++)
            {
                cout << "| no " << *iter << " arco " << B[*it].cam_spur[*iter] << "| ";

            }
            cout<<endl<<"Frota: "<<vetSol[i].frota_por_rota[rota]<<endl;
            cout<<"Headway: "<<vetSol[i].headways[rota]<<endl;
            cout<<"Tempo de Ciclo: "<<vetSol[i].tempo_de_ciclo[rota]<<endl;
            cout<<"Demanda Maxima: "<<vetSol[i].demandas_maximas_por_rota[rota].demanda_maxima<<endl;
            cout<<"Arco: No "<<vetSol[i].demandas_maximas_por_rota[rota].no1<<" para No "<<vetSol[i].demandas_maximas_por_rota[rota].no2<<endl;
            rota++;
        }
        cout<<endl;
    }
}

//=================================================================================================================================
void  conjunto_Elite(Solucao vetSol[POP_TOTAL+1],int pop_total, int num_max_rotas, Rede rede, list <int> sBOD[MAX_NOD][MAX_NOD], list <int> sBO[MAX_NOD], list <int> rota_no[MAX_NOD], Caminho B[MAX_ARC], int b, int n_elite, list<int> &elite)
//forma um conjuto elite de solucoes (menore valores de fo)
//=================================================================================================================================
{
    int menor_aux[b];
    int menor_j=0;
    int menor=INFINITY;
    int cont_elite=1;

    for (int i=1; i<=b; i++)
    {
        menor_aux[i]=0;
    }
    for (int i=1; i<=n_elite; i++)
    {

        for (int j=1; j<=b; j++)  //procura o menor valor de fo
        {

            if(vetSol[j].fo < menor && menor_aux[j] !=  1)
            {
                menor=vetSol[j].fo;
                menor_j=j;

            }
        }
        if(cont_elite <= n_elite) //arredondando para baixo
        {
            elite.push_back(menor_j); //add na lista o menor
            //cout<<endl<<"MENOR: "<<menor_j<<endl;
            menor_aux[menor_j] = 1;
        }

        cont_elite++;
        menor = INFINITY;
        menor_j = 0;

    }
    /*cout<<"\n \nConjunto elite: \n"<<endl;
    for (list<int>::iterator x=elite.begin(); x!=elite.end(); x++)
    {
        cout<<"Solucao numero: " <<*x<<" (custo "<<vetSol[*x].fo<<") "<<endl;
    }*/
    int s_star=elite.front();
    //cout<<"S*: "<<s_star<<endl;
}

//=========================================================================================================
list <int> fora_elite(list<int>elite,int b)
//retorna a lista das solucoes nao pertencentes ao elite
//=========================================================================================================
{
    list<int>::iterator it;
    list<int>fora_elite;
    int aux[b];
    for (int i=1; i<=b; i++)
        aux[i]=0;
    for(it=elite.begin(); it!=elite.end(); it++)
        aux[*it]=1;
    for(int j=1; j<=b; j++){
        if(aux[j]==0)
            fora_elite.push_back(j);
    }
    return fora_elite;
}

//==========================================================================
void probabilidade(Solucao vetSol[POP_TOTAL+1], list<int> &elite, list<int> &foraElite)
//==========================================================================
{

    double denominador1=0;
    double denominador2=0;

    //Denominador da Elite
    for(list<int>::iterator it= elite.begin(); it != elite.end(); it++){
        denominador1 += vetSol[*it].fo;
    }

    //Calcula as probabilidades da Elite
    for(list<int>::iterator it= elite.begin(); it != elite.end(); it++)
    {
        vetSol[*it].prob = 1 - vetSol[*it].fo/denominador1;
    }

    //Denominador da foraElite
    for(list<int>::iterator it= foraElite.begin(); it != foraElite.end(); it++){
        denominador2 += vetSol[*it].fo;
    }

    double testeValor = 0;
    for(list<int>::iterator it= foraElite.begin(); it != foraElite.end(); it++)
    {
        vetSol[*it].prob = 1- vetSol[*it].fo/denominador2;
        testeValor+= 1-vetSol[*it].prob;
    }
}

//===========================================================================================
bool compara(const nodo no1, const nodo no2){
//===========================================================================================
    return (no1.fo < no2.fo);
}

//===========================================================================================
void new_Probability(Solucao vetSol[POP_TOTAL+1], list<int> lista){
//===========================================================================================
    list <nodo> nos;
    nodo no_aux;
    for(list<int>::iterator it = lista.begin(); it != lista.end(); it++){
        no_aux.fo = vetSol[*it].fo;
        no_aux.indice = *it;
        nos.push_back(no_aux);
    }

    nos.sort(compara);

    int repete = 0;
    int fo_ant = 0;
    list<nodo>::iterator it_no;
    for(it_no = nos.begin(); it_no != nos.end(); it_no++){
        no_aux = *it_no;
        if (no_aux.fo == fo_ant){
            repete++;
        }
        fo_ant = no_aux.fo;
    }

    double N, S = 0;
    N = nos.size() - repete;

    for(int i = 1; i <= N; i++ )
        S = S + i;

    int cont = 0;
    double prob;
    fo_ant = 0;
    for(it_no = nos.begin(); it_no != nos.end(); it_no++){
        no_aux = *it_no;
        if (no_aux.fo != fo_ant){
            cont++;
        }
        fo_ant = no_aux.fo;
        no_aux.prob = (N - cont + 1)/S;
        vetSol[no_aux.indice].prob = no_aux.prob;
    }
}

//===========================================================================================
int selecao(list<int> lista, Solucao vetSol[POP_TOTAL+1])
//seleciona uma solucao atraves de soteio pseudo
//===========================================================================================
{

    double soma_sorteio = 0;
    int selecionado_1=0;
    double denominador = 0;
    for(list<int>::iterator k = lista.begin(); k != lista.end(); k++){
        denominador+= vetSol[*k].prob;
    }
    double num_rand = (rand()%(int)(denominador*1000));
    num_rand = num_rand/1000;

    for(list<int>::iterator k = lista.begin(); k != lista.end(); k++)
    {
        soma_sorteio = soma_sorteio + vetSol[*k].prob;
        //cout<<"\n soma: "<<soma_sorteio<<" "<<*k<<"Numero rand: "<<num_rand<<endl;
        if(num_rand <= soma_sorteio)
        {
            //cout<<"hello "<< *k;
            selecionado_1 = *k;
            k=lista.end();
            k--;
        }
    }
    //cout<<"SELECIONADO: "<<selecionado_1<<endl;
    return selecionado_1;
}

//======================================================================================
int selecao_rand (list<int> b, Solucao vetSol[POP_TOTAL+1], double p)
//seleciona de forma totalmente randomica uma certa pocentagem da populacao total inicial e desses selecionados devolve-se o melhor (menor valor de fo)
//======================================================================================
{
    srand(time(NULL));
    list<int> lista_sorteados;
    list<int>::iterator auxiliar;
    int random;
    int menor=9999;
    int pos_menor=1;
    for(int i=1; i <= ceil((p/100)*b.size()); i++){
        random = (rand()%b.size())+1;
        auxiliar = b.begin();
        for(int k = 0; k<random; k++){
            auxiliar++;
        }
        lista_sorteados.push_back(*auxiliar);
    }
    for (list<int>::iterator j=lista_sorteados.begin(); j!=lista_sorteados.end(); j++){
        if (vetSol[*j].fo < menor){
            menor = vetSol[*j].fo;
            pos_menor = *j;
        }
    }
    b.remove(pos_menor);
    lista_sorteados.clear();
    return pos_menor;
}

//===========================================================================================
//seleciona de forma totalmente randomica uma certa pocentagem da populacao total inicial e 
//desses selecionados devolve-se o melhor (menor valor de fo)
int selec_rand(int percent, Solucao vetSol[POP_TOTAL+1], list<int> auxiliar){
//===========================================================================================
    list<int> escopo;
    for(int i =0; i<(auxiliar.size()*percent/100)+1; i++){
        escopo.push_back(rand()%auxiliar.size());
    }
    list<int>::iterator it = auxiliar.begin();
    int menor = (*it)+escopo.front();

    for(list<int>::iterator iter = escopo.begin(); iter !=escopo.end(); iter++){
        it = auxiliar.begin();
        for(int i = 0; i < (*iter); i++)
            it++;
        if(vetSol[menor].fo>vetSol[*it].fo)
            menor = (*it);
    }
    escopo.clear();
    return menor;
}

//===========================================================================================
bool soluc_is_equal(Solucao first, Solucao second){
//===========================================================================================
    if(first.fo != second.fo || first.caminho.size() != second.caminho.size())
        return false;
    
    first.caminho.sort();
    second.caminho.sort();

    list<int>::iterator primeiro = first.caminho.begin();
    list<int>::iterator segundo = second.caminho.begin();
    
    for(int i = 1; i <= first.caminho.size(); i++){
        if((*primeiro) != (*segundo))
            return false;
        primeiro++;
        segundo++;
    }
    return true;
}

//===========================================================================================
bool isOnList(list<int> lista, int numero){
//===========================================================================================
    bool resultado = false;
    if(lista.size() == 0){
        return false;
    }
    else if(lista.size() == 1){
        if(lista.front() == numero)
            resultado = true;
    }
    else{
        for(list<int>::iterator it = lista.begin(); it != lista.end(); it++){
            if(numero == (*it)){
                resultado = true;
                it = lista.end();
                it--;
            }
        }
    }
    return resultado;
}

//===========================================================================================
bool compare_nocase (const Solucao first, const Solucao second){
//===========================================================================================
  return ( first.fo < second.fo );
}

//======================================================================================
list<Solucao> cruzamento (double percent_cruzamento, int b, list<int>elite, Solucao vetSol[POP_TOTAL+1], Caminho B[MAX_ARC], Rede rede, int n_elite, list<int> &foraElite, list <int> rota_no[MAX_NOD])
//seleciona 2 solucoes, uma dentro do conjunto elite e outra fora, para serem geradoras de 2 solucoes filhas. Cruzamento feito em 'x'
//======================================================================================
{
    //probabilidade(vetSol,elite,foraElite);
    new_Probability(vetSol, elite);
    new_Probability(vetSol, foraElite);

    int tam1, tam2;
    int selecionado_1, selecionado_2;
    int random;
    int selecionou=0;
    list <int> caminho2, caminho3;
    list <int> aux1, aux2;
    list <Solucao> filhos; //nova_pop (percent_cruzamento)
    Solucao S1, S2;
    list<int> randomUtilizados;

    int limiteInteracoes = 1000;
    int interationCounter = 0;

    while (filhos.size()< (b*percent_cruzamento) && interationCounter < limiteInteracoes )
    {
        interationCounter++;
        //cout<<"TAMANHO DE FILHOS: "<<filhos.size()<<endl;
        selecionado_1=selecao(elite, vetSol);
        selecionado_2=selecao(fora_elite(elite,b), vetSol);
        tam1 = vetSol[selecionado_1].caminho.size();
        tam2 = vetSol[selecionado_2].caminho.size();
        
        int menorSolucao;
        if(tam1<tam2)
            menorSolucao = tam1;
        else
            menorSolucao = tam2;
        

        int filhosGerados = 0;
        while(filhosGerados < 2 && randomUtilizados.size() <= (int)(percent_cruzamento*menorSolucao))
        {
            //cout<<"CONTADOR DE ITERAÇAO: "<<interationCounter<<endl;
            
            selecionou = 0;

            //cout<<"TAMANHO DE RANDOMUTILIZADOS ANTES DO WHILE: "<<randomUtilizados.size()<<endl;
            
            while(selecionou!=1)
            {
                random =  rand() % menorSolucao;
                //cout<<"RANDOM: "<<random<<endl;
                if(!isOnList(randomUtilizados, random))
                {
                    selecionou=1;
                    randomUtilizados.push_back(random);
                }
                //cout<<"TRAVOU AK"<<endl;
            }

            //cout<<"TAMANHO DE RANDOMUTILIZADOS DEPOIS DO WHILE: "<<randomUtilizados.size() <<"Limite desejado: "<<(int)(percent_cruzamento*menorSolucao)<<endl;
            //cout<<"SAIU DO RANDOM"<<endl;
            int i=1;
            for(list<int>::iterator x = vetSol[selecionado_1].caminho.begin(); x != vetSol[selecionado_1].caminho.end(); x++)
            {
                if(i<=random)
                    caminho2.push_back(*x);
                else
                    caminho3.push_back(*x);
                i++;
            }

            i=1;
            for(list<int>::iterator y = vetSol[selecionado_2].caminho.begin(); y != vetSol[selecionado_2].caminho.end(); y++)
            {
                if(i<=random)
                    aux1.push_back(*y);
                else
                    aux2.push_back(*y);
                i++;
            }

            caminho2.merge(aux2);
            caminho3.merge(aux1);

            caminho2.sort();
            caminho2.unique();
            caminho3.sort();
            caminho3.unique();
            
            if (filhosGerados < 2)
            {
                //cout<<endl<<"Nova Solucao  "<<cont<<": "<<endl;

                S1.caminho = caminho2;
                int viav1 = sol_viavel(S1, rede, B, rota_no);

                if (viav1==1){
                    if(filhos.size() == 0){
                        filhos.push_back(S1);
                        filhosGerados++;
                    }
                    else if(filhos.size() == 1 && !soluc_is_equal(S1, filhos.front())){
                        filhos.push_back(S1);
                        filhosGerados++;
                    }
                    else{
                        bool aux = true;
                        for(list<Solucao>::iterator iteradorDeFilhos = filhos.begin(); iteradorDeFilhos != filhos.end(); iteradorDeFilhos++){
                            if(soluc_is_equal(S1, *iteradorDeFilhos)){
                                aux = false;
                                iteradorDeFilhos = filhos.end();
                                iteradorDeFilhos--;
                            }
                        }
                        if(aux){
                            filhos.push_back(S1);
                            filhosGerados++;
                        }
                    }
                }
            }
            if (filhosGerados < 2)
            {
                //cout<<endl<<"Nova Solucao  "<<cont<<": "<<endl;

                S2.caminho = caminho3;
                int viav2 = sol_viavel(S2, rede, B, rota_no);
                if (viav2==1){
                    if(filhos.size() == 0){
                        filhos.push_back(S2);
                        filhosGerados++;
                    }
                    else if(filhos.size() == 1 && !soluc_is_equal(S2, filhos.front())){
                        filhos.push_back(S2);
                        filhosGerados++;
                    }
                    else{
                        bool aux = true;
                        for(list<Solucao>::iterator iteradorDeFilhos = filhos.begin(); iteradorDeFilhos != filhos.end(); iteradorDeFilhos++){
                            if(soluc_is_equal(S2, *iteradorDeFilhos)){
                                aux = false;
                                iteradorDeFilhos = filhos.end();
                                iteradorDeFilhos--;
                            }
                        }
                        if(aux){
                            filhos.push_back(S2);
                            filhosGerados++;
                        }
                    }
                }
            }
            caminho2.clear();
            caminho3.clear();
            aux1.clear();
            aux2.clear();
        }
        randomUtilizados.clear();
        //cout<<"TAMANHO DE FILHOS ANTES: "<<filhos.size()<<endl;
        filhos.sort(compare_nocase);
        remove_Equal_Solutions(filhos);
        //ccout<<"TAMANHO DE FILHOS DEPOIS: "<<filhos.size()<<endl;
    }
    //cout<<"TOTAL FILHOS GERADAS: "<<filhos.size()<<endl;
    return filhos;
}

//===========================================================================================
void remove_Equal_Solutions(list<Solucao> lista){
//===========================================================================================
    if(lista.size()<2)
        return;
    
    vector<Solucao> auxiliar;
    for(list<Solucao>::iterator it1 = lista.begin(); it1 != lista.end(); it1++){
        auxiliar.push_back(*it1);
    }

    for(int i = 0; i<auxiliar.size()-1; i++){
        for(int j = i+1; j<auxiliar.size(); j++){
            if(soluc_is_equal(auxiliar[i], auxiliar[j])){
                auxiliar.erase(auxiliar.begin()+j);
            }
        }
    }

    lista.clear();

    for(int i = 0; i<auxiliar.size(); i++){
        lista.push_back(auxiliar[i]);
    }

    auxiliar.clear();
}

//===========================================================================================
void selecao_pos_cruzamento(Solucao vetSol[POP_TOTAL+1], int pop_total, list<Solucao> filhos, int percentElit, int percentPseudo, int percentRand){
//===========================================================================================
    list<Solucao> novapopulacao;
    int nElite, nPseudo, nRand;
    //Adiciona na novapopulaçao as solucoes em vetsol
    for(int i = 1; i<= pop_total; i++){
        novapopulacao.push_back(vetSol[i]);
    }
    //Adiciona na novapopulaçao as solucoes geradas a partir do cruzamento
    for(list<Solucao>::iterator i = filhos.begin(); i != filhos.end(); i++){
        novapopulacao.push_back(*i);
    }

    nElite = pop_total*percentElit/100;
    nPseudo = pop_total*percentPseudo/100;
    nRand = pop_total*percentRand/100;

    nPseudo += pop_total - (nElite+nPseudo+nRand);

    novapopulacao.sort(compare_nocase);


    //Lista de soluções sobreviventes após a seleção
    list<Solucao> sobreviventes;
    list<Solucao>::iterator it;

    //Adiciona a elite de toda a população na lista de sobreviventes
    for(int i= 1; i<=nElite; i++){
        it = novapopulacao.begin();
        sobreviventes.push_back(*it);
        novapopulacao.pop_front();
    }

    //Lista auxiliar para a indiciação do vetSol
    list<int> aux;
    it = novapopulacao.begin();
    //Vetor para armazenamento temporario das soluçoes
    Solucao vetPopulacao[novapopulacao.size()];
    //Adiciona as Soluções pós seleção de elite ao vetPopulacao
    for(int i = 0; i<novapopulacao.size(); i++){
        aux.push_back(i);
        vetPopulacao[i] = (*it);
        it++;
    }

    //Seleção Pseudo-Randomica de sobreviventes
    for(int i = 0; i<nPseudo; i++){
        new_Probability(vetPopulacao, aux);
        int selecionado = selecao(aux, vetPopulacao);
        sobreviventes.push_back(vetPopulacao[selecionado]);
        aux.remove(selecionado);
    }

    //Seleção Randomica de sobreviventes
    for(int i = 0; i<nRand; i++){
        int selecionado = selec_rand(percentElit, vetPopulacao, aux);
        sobreviventes.push_back(vetPopulacao[selecionado]);
        aux.remove(selecionado);
    }

    int contador = 1;
    sobreviventes.sort(compare_nocase);
    //Transfere o conteudo da lista de sobreviventes para o vetSol
    for(list<Solucao>::iterator it = sobreviventes.begin(); it != sobreviventes.end(); it++){
        //cout<<"FO dos sobreviventes: "<< (*it).fo<<endl;
        vetSol[contador]=(*it);
        contador++;
    }

    sobreviventes.clear();
    novapopulacao.clear();
    //cout<<"MENOR FO:"<<vetSol[1].fo<<endl;
}


//======================================================================================================
list<Solucao> mutacao(double percent_mutacao,int b, Solucao vetSol[POP_TOTAL+1],double prob_mut_sol, int prob_mut_caminho, Caminho B[MAX_ARC], list <int> sBOD[MAX_NOD][MAX_NOD], Rede rede, double percent_sorteio, list <int> rota_no[MAX_NOD])
//Faz mutacoes nos caminhos de cada solucao
//======================================================================================================
{
    Solucao mutante;
    list<Solucao>sol_mutada;
    list<int>sol_ja_sorteadas;
    list<int>cam_ja_mutados;
    list<int>cam_ja_sorteados_sBOD;
    list<int>::iterator i;
    list<int>::iterator k;
    list<int> auxiliar;
    for(int i=1; i<POP_TOTAL+1;i++){
        auxiliar.push_back(i);
    }
    int cont_cam_mutados=0;
    int origem;
    int destino;
    int teste_mutacao;
    double random;
    double percent_mut_caminho = percent_mutacao;
    double chance_de_mutar_caminho = 1;
    int tries=0;
    while (sol_mutada.size() < POP_TOTAL*percent_mutacao && auxiliar.size() >= POP_TOTAL/3) //20%
    {
        //cout<<1<<endl;
        int mut=selecao_rand(auxiliar,vetSol,percent_sorteio); //seleciona 1 solucao a partir da selecao randomica
        //i=find(sol_ja_sorteadas.begin(),sol_ja_sorteadas.end(), mut); //verifica se a solucao selecionada ja esta na lista de sorteadas
        //if(i == sol_ja_sorteadas.end())//se for verdade, significa que nao encontrou, entao podera ocorrer mutacao
        //{
            cout<<"SOLUCAO A SER MUTADA: "<<mut<<endl;
            sol_ja_sorteadas.push_back(mut); //add a atual na lista para que nao seja sorteada novamente
            //cout<<"TESTE 1"<<endl;
            //cout<<"Mute: "<<mut<<endl;
            mutante=vetSol[mut];
            mutante.caminho.sort();
            //cout<<"TESTE 2"<<endl;
            //cont_cam_mutados=0; //conta a quantidade de caminhos mutados
            //cout<<"TESTE 3"<<endl;
            cam_ja_mutados.clear(); //lista de caminhos ja mutados para que nao sejam mutados novamente
            //cout<<"TESTE 4"<<endl;
            cam_ja_sorteados_sBOD.clear(); //lista de caminhos ja sorteados dentro do sub banco sBOD
            //cout<<"TESTE POS 2"<<endl;
            //while (cont_cam_mutados < percent_mut_caminho*mutante.caminho.size() && tries < 3) // para que uma porcentagem 'x' de caminhos sejam mutados dentre o total
            //{
                //cout<<3<<endl;
                for(list<int>::iterator j = mutante.caminho.begin(); j != mutante.caminho.end(); j++) //percorre todos os caminhos de uma solucao
                {
                    random=(rand()%100); //sorteia um numero de 0 a 99
                    //cout<<"CAMINHO A SER MUTADO: "<<*j<<endl;
                    //cout<<*mutante.caminho.begin()<<" ----------- "<<*mutante.caminho.end()<<endl;
                    if (random < chance_de_mutar_caminho)//colocar numero gerado<prob sort caminho &&
                    {
                        cout<<"ENTROU NO "<<4<<endl;
                        origem = B[*j].list_nos.front(); //primeiro no do caminho (origem)
                        destino = B[*j].list_nos.back(); //ultimo no do caminho (destino)
                        teste_mutacao=0;
                        if(sBOD[origem][destino].size() >= 2){
                            while(teste_mutacao == 0 &&  cam_ja_sorteados_sBOD.size()<sBOD[origem][destino].size()) //enquanto nao mutar ou s� existir um caminho com essa origem e destino ou testar todos possiveis caminhos
                            {
                                cout<<"AK"<<endl;
                                int x = randomiza(sBOD[origem][destino]); //randomiza a lista sBOD
                                cam_ja_sorteados_sBOD.push_back(*j); //caminho ja sorteado
                                k=find(cam_ja_sorteados_sBOD.begin(), cam_ja_sorteados_sBOD.end(), x); //verifica se o randomizado esta na lista se caminhos ja sorteados
                                if (k== cam_ja_sorteados_sBOD.end()) //se verdadeiro, pode trocar
                                {
                                    *j=x; //faz a troca dos camihos
                                    if(sol_viavel(mutante, rede, B, rota_no) == 1 && sol_mutada.size() < b*percent_mutacao ) //verifica a viabilidade da solucao ja mutada (verdadeiro)
                                    {
                                        sol_mutada.push_back(mutante); //add a solucao mutada a lista
                                            //cout<<"TAMANHO : "<<sol_mutada.size()<<endl;
                                        mutante.caminho.sort();
                                        vetSol[mut] = mutante;
                                        teste_mutacao = 1; //indica que ocorreu mutacao
                                        cam_ja_mutados.push_back(x); // add o caminho mutado a lista
                                        cont_cam_mutados ++; //incrementa o contador de caminhos mutados
                                    }
                                                                        
                                    cam_ja_sorteados_sBOD.push_back(x); //ja testou o caminho randomizado x
                                    
                                }
                                //cout<<"1"<<endl;
                            }
                            cout<<"SAIU"<<endl;
                        }
                            cam_ja_sorteados_sBOD.clear();
                    }

                }
                tries++;
                //cout<<"2"<<endl;
            //}



        //}
        //cout<<"3"<<endl;
        tries=0;
    }
return sol_mutada; //retorna  a lista ja mutada
}


/* //=================================================================================================================================
list <int>  f_Elite(Solucao vetSol[POP_TOTAL+1],int pop_total, int num_max_rotas, Rede rede, list <int> sBOD[MAX_NOD][MAX_NOD], list <int> sBO[MAX_NOD], list <int> rota_no[MAX_NOD], Caminho B[MAX_ARC], int b, int n_elite    )
//=================================================================================================================================
{
    list<int>::iterator x;
    list<int>::iterator y;
    list <int> elite;
    list<double> calculo;
    list<int> caminho2;
    list<int> caminho3;
    list<int> aux1;
    list<int> aux2;
    list<int> fora_elite;
    int selecionado_1, selecionado_2;
    int menor_aux[b];
    int indices[6];
    double soma=0;
    int menor=9999;
    int menor_j=0; //guarda o indice do menor valor
    int selecionou=0;
    int tam1, tam2;
    int cont=1, cont_elite = 0;
    Solucao S1,S2;
    list <Solucao> elite_viavel;
    double r [n_elite], r1[n_elite];
    int contt=1, u;
    double soma_ab=0, soma_bc=0;
    double probb;

    for (int i=1; i<=b; i++)
    {
        menor_aux[i]=0;
    }
    for (int i=1; i<=b; i++)
    {

        for (int j=1; j<=b; j++)  //procura o menor valor de fo
        {

            if(vetSol[j].fo < menor && menor_aux[j] !=  1)
            {
                menor=vetSol[j].fo;
                menor_j=j;

            }
        }
        if(cont_elite < n_elite)
        {
            elite.push_back(menor_j); //add na lista o menor
            menor_aux[menor_j] = 1;
        }
        else
        {
            fora_elite.push_back(menor_j);
        }
        cont_elite++;
        menor = 9999;
        menor_j = 0;

    }
    for (x=elite.begin(); x!=elite.end(); x++)
    {
        int i=1;
        cout<<"\n Conjunto elite " <<*x <<" (custo "<<vetSol[*x].fo<<") "<<endl;
        i++;
    }

    for (int u=1; u<= b-n_elite; u++)
    {
        r1[u]=(b -n_elite)-u+1;
        soma_bc = soma_bc + r1[u];

    }
    cout<<"Soma BC: "<<soma_bc<<endl;

    list<int>::iterator p1;
    p1 = fora_elite.begin();
    for(int w=1; w<=b - n_elite; w++)
    {
        vetSol[*p1].prob = r1[w]/soma_bc;
        cout<<" Probabilidade: "<< vetSol[*p1].prob<<endl;

        p1++;
    }


    for (list<int>::iterator y=elite.begin(); y!=elite.end(); y++ )
        soma=soma+vetSol[*y].fo;

    cout<<"soma: "<<soma<<endl;

    double soma_a=0;





    for (u=1; u<= n_elite; u++)
    {
        r[u]=n_elite-u+1;
        soma_ab = soma_ab + r[u];

    }
    cout<<"Soma AB: "<<soma_ab<<endl;

    list<int>::iterator p;
    p = elite.begin();
    for(int w=1; w<=n_elite; w++)
    {
        vetSol[*p].prob = r[w]/soma_ab;
        cout<<" Probabilidade: "<< vetSol[*p].prob<<endl;

        p++;
    }


    for (y=elite.begin(); y!=elite.end(); y++ )
    {
        soma_a=soma_a + (vetSol[*y].fo/soma);
        calculo.push_back(vetSol[*y].fo / soma);
        vetSol[*y].prob=vetSol[*y].fo / soma;
    }

    cout<<"Soma_a: "<<soma_a<<endl;

    double soma_sorteio = 0;
    while (elite_viavel.size()<b)
    {
        double num_rand = (rand()%1001);
        num_rand = num_rand/1000;
        for(list<int>::iterator k = elite.begin(); k != elite.end(); k++)
        {
            soma_sorteio = soma_sorteio + vetSol[*k ].prob;
            if(num_rand <= soma_sorteio){
                selecionado_1 = *k;
                selecionou=1;
            }
        }

        //Solu��o fora do elite
        int q= 0;
        soma_sorteio = 0;
        num_rand = (rand() % 1001)/1000;
        for(list<int>::iterator k = fora_elite.begin(); k != fora_elite.end(); k++)
        {
            soma_sorteio = soma_sorteio + vetSol[*k ].prob;
            if(num_rand <= soma_sorteio){
                selecionado_2 = *k;
                selecionou=1;
            }
        }

        tam1 = vetSol[selecionado_1].caminho.size();
        tam2 = vetSol[selecionado_2].caminho.size();
        int random; //sorteia uma posi��o aleatoria do vetor "selecionado"
        selecionou = 0;
        if(tam1 < tam2)  //pegar menor vetor
        {
            while(selecionou!=1)
            {
                random = (rand() % tam1);
                if(random != vetSol[selecionado_2].caminho.front() && random != vetSol[selecionado_2].caminho.back())
                {
                    selecionou=1;
                    cout<<"random: "<<random<<endl;
                }
            }
            int i=1;
            for(list<int>::iterator x = vetSol[selecionado_1].caminho.begin(); x != vetSol[selecionado_1].caminho.end(); x++)
            {
                if(i<=random)
                    caminho2.push_back(*x);
                else
                    caminho3.push_back(*x);
                i++;
            }
            i=1;
            for(list<int>::iterator y = vetSol[selecionado_2].caminho.begin(); y != vetSol[selecionado_2].caminho.end(); y++)
            {
                if(i<=random)
                    aux1.push_back(*y);
                else
                    aux2.push_back(*y);
                i++;

            }
            caminho2.merge(aux2);
            caminho3.merge(aux1);

        }

        else
        {
            while(selecionou!=1)
            {
                random = (rand() % tam1);
                if(random != vetSol[selecionado_2].caminho.front() && random != vetSol[selecionado_2].caminho.back())
                {
                    selecionou=1;
                    cout<<"random: "<<random<<endl;
                }
            }
            int i=1;
            for(list<int>::iterator x = vetSol[selecionado_2].caminho.begin(); x != vetSol[selecionado_2].caminho.end(); x++)
            {
                if(i<=random)
                    caminho2.push_back(*x);
                else
                    caminho3.push_back(*x);
                i++;
            }
            i=1;
            for(list<int>::iterator y = vetSol[selecionado_1].caminho.begin(); y != vetSol[selecionado_1].caminho.end(); y++)
            {
                if(i<=random)
                    aux1.push_back(*y);
                else
                    aux2.push_back(*y);
                i++;

            }
            caminho2.merge(aux2);
            caminho3.merge(aux1);
        }
        if (elite_viavel.size()<b)
        {
            cout<<endl<<"Nova Solucao  "<<cont<<": "<<endl;
            for(list<int>::iterator z = caminho2.begin(); z != caminho2.end(); z++)
            {
                //cout<<endl<<" Caminho: "<<*z;
                for ( list<int>::iterator iter = B[*z].list_nos.begin(); iter != B[*z].list_nos.end(); iter ++)
                {
                    //    cout << "| no " << *iter << " arco " << B[*z].cam_spur[*iter] << "| ";

                }
            }
            S1.caminho = caminho2;
            int viav1 = sol_viavel(S1, rede, B);
            if (viav1==1)
                elite_viavel.push_back(S1);
            cont++;
        }
        if (elite_viavel.size()<b)
        {
            cout<<endl<<"Nova Solucao  "<<cont<<": "<<endl;

            for(list<int>::iterator z = caminho3.begin(); z != caminho3.end(); z++)
            {
                //printf("\n caminho: %d ", *z);
                for ( list<int>::iterator iter = B[*z].list_nos.begin(); iter != B[*z].list_nos.end(); iter ++)
                {
                    //cout << "| no " << *iter << " arco " << B[*z].cam_spur[*iter] << "| ";

                }
            }
            S2.caminho = caminho3;
            int viav2 = sol_viavel(S2, rede, B);
            if (viav2==1)
                elite_viavel.push_back(S1);
        }
        cont++;
        caminho2.clear();
        caminho3.clear();
        aux1.clear();
        aux2.clear();
    }

    //sorteio
    Solucao mutante;
    list<Solucao> sol_mutada;
    int teste_mutacao = 0, teste_caminho = 0, cont_mut = 0;
    list<int>::iterator aa;
    list<Solucao>::iterator sol;
    sol = elite_viavel.begin();
    int origem, destino, novo_caminho, sorteio, contador = 0;
    cout<<"/n ======================================== \n                 MUTACOES "<<endl;
    while(sol_mutada.size() < b )
    {   teste_mutacao = 0;
        mutante = *sol;
        while(teste_mutacao == 0 || contador < mutante.caminho.size())
        {
            sorteio = randomiza(mutante.caminho);
            cout<<"\nsorteio: "<<sorteio<<endl;
            origem = B[sorteio].list_nos.front();
            destino = B[sorteio].list_nos.back();   //no lugar do 3 colocar o sorteado
            aa = find(mutante.caminho.begin(),mutante.caminho.end(),sorteio);
            while(teste_mutacao == 0)
            {
                *aa = randomiza(sBOD[origem][destino]);
                cout<<"novo caminho: "<<*aa<<endl;
                if(sol_viavel(mutante, rede, B) == 1)
                {
                    teste_mutacao = 1;
                    sol_mutada.push_back(mutante);
                    for(list<int>::iterator z = mutante.caminho.begin(); z != mutante.caminho.end(); z++)
                    {
                        printf("\n caminho: %d ", *z);
                        for ( list<int>::iterator iter = B[*z].list_nos.begin(); iter != B[*z].list_nos.end(); iter ++)
                        {
                            cout << "| no " << *iter << " arco " << B[*z].cam_spur[*iter] << "| ";

                        }
                    }
                }
            }
            contador++;
        }
        sol++; //tirar essa parte e mudar para o sorteio da solu��o
    }

    cout<<"\nTamanho vetor de mutacao: "<<sol_mutada.size()<<endl;


    return elite;


}
*/
