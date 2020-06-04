#include <iostream>
#include "Rede.h"
#include <list>
#include <stdio.h>
#include "Funcao_teste.h"
#include <chrono>
using namespace std::chrono;

//teste do git

void imprime_demandas(Rede &rede){
    cout<<"TEBLA DE DEMANDAS: "<<endl;
    for(int i = 0; i<rede.n; i++){
        for(int j = 0; j<rede.n; j++){
            cout<<rede.demanda[i][j]<<" ";
        }
        cout<<endl;
    }
}

using namespace std;

int main()

{
    int execucoes = 1;
    int b; //tam_pop_total
    list<int> elite;
    list<int>solucao;
    Rede rede;
    list<int>::iterator it;
    char *arq_rede = "rede_Mandl.txt";
    char *arq_demanda = "demandas.txt";
    

    le_rede(rede, arq_rede);

    if(!le_demandas(rede, arq_demanda)){
        exit(1);
    }

    int K, tot_rotas, i, j;
    list<int> cam_min, teste;
    list<int>::iterator iter, iter1;
    Caminho B[MAX_ARC];

    list <int> sBO[MAX_NOD], rota_no[MAX_NOD];

    list <int> sBOD[MAX_NOD][MAX_NOD];
    Solucao S1;
    Solucao S2;
    Solucao vetSol [POP_TOTAL+1];
    list<Solucao> nova_pop;
    list<Solucao> mutante;


    K = 10;                  // total de caminhos m�nimos
    float taxa = 2.0;        // custo m�ximo do caminho em rela��o ao caminho m�nimo
    int tam_min = 4;        // número mínimo de nós em cada linha(rota)
    int tempo_maximo = 40;  //Tempo Máximo do caminho
    //while(execucoes<=15){
        tot_rotas = cria_banco_rotas(rede, B, K, taxa, sBO, sBOD, rota_no, tam_min, tempo_maximo);
        printf("\n  ====  Total de rotas geradas: %d ====\n", tot_rotas);

        cout<<" "<<endl;

        /*
        cout<< "Custo: "<<B[50].custo<<endl;
        for(list<int>::iterator i = B[50].list_nos.begin(); i != B[50].list_nos.end(); i++ ){
            cout<<*i<<" ";
        }
        getchar();*/
        int pop_total = POP_TOTAL;
        int num_max_rotas;
        double percent_elite = 20;
        double calc_elite = (percent_elite/100)*pop_total;
        int n_elite=3;
        //cout<<" oi "<<n_elite; //porcentagem do tamanho do conjunto elite
        num_max_rotas = 5;
        double percent_cruzamento=0.8;
        double percent_mutacao=0.2;
        double prob_mut_sol=0.1;
        double prob_mut_caminho=0.05;
        double valor_minimo_cam=0.01;
        double valor_min_sol=0.1;
        double percent_mut_caminho=0.02;
        double percent_sorteio=30;

        int percent_elite_selecao = 30;
        int percent_pseudo_selecao = 50;
        int percent_rand_selecao = 20;
        int geracoes = 75;
        int limite_Geracoes_sem_melhoras = 10;


        srand (time(NULL));
        for (i = 1; i <= 30; i++){
            int r = (rand() % 10)+1;
            //printf("\nR = %d", r);
        }
        //cout<<"Antes pop"<<endl;
        b  = pop_inicial (vetSol,pop_total, num_max_rotas,rede,sBOD,sBO, rota_no, B);
        //cout<<"Depois pop"<<endl;
        printf("Total de solucoes geradas %d", b);
        /*for (i = 1; i <= b; i++){
            //int x = sol_viavel(vetSol[i],rede,B);
            printf("\nSolucao: %d, fo: %d ", i, vetSol[i].fo);
            for(it = vetSol[i].caminho.begin(); it != vetSol[i].caminho.end(); it++ ){

            //cout<<"Caminho: "<<*it<< "|No: "<<caminho.cam_spur[B[caminho]]<<"|"//<<"|Arco: "<<vetSol[i].rotas.m<<"|"<<endl;
                printf(" caminho: %d", *it);
            }
        }*/
        //getchar();

        //cout<<endl<<"SOLUCOES INICIAIS: "<<endl;
        //imprime(vetSol, pop_total, num_max_rotas,  rede,  sBOD,sBO,rota_no, B, b);

        cout<<endl;
        /*for(int i = 1; i<pop_total; i++){
            for(int j = i+1; j<pop_total; j++){
                if(soluc_is_equal(vetSol[i], vetSol[j])){
                    cout<<"Solucoes iguais: "<< i <<" e "<< i <<endl;
                }
            }
        }

        cout<< "TESTE DA FUNCAO"<<endl;
        cout<<"Valor: "<<soluc_is_equal(vetSol[1], vetSol[1])<<endl;
        */

        int cont_geracoes = 0;
        int cont_geracoes_sem_melhoras = 0;
        int melhor_Solucao = vetSol[i].fo;
        while(cont_geracoes < geracoes && cont_geracoes_sem_melhoras < limite_Geracoes_sem_melhoras){
            cout<<"GERACAO: "<<cont_geracoes<<endl;
            cout<<"ENTROU ELITE"<<endl;
            conjunto_Elite(vetSol,pop_total,num_max_rotas,rede,sBOD,sBO,rota_no,B,b,n_elite,elite);
            cout<<"SAIU ELITE"<<endl;
            //cout<<" tamanhoelite: "<<elite.size()<<endl;
            list<int> foraElite = fora_elite(elite, b);
            cout<<"ENTROU PROBABILIDADE"<<endl;
            probabilidade(vetSol,elite,foraElite);
            cout<<"SAIU PROBABILIDADE"<<endl;

            //int sel = selecao(elite, vetSol);
            
            
            cout<<"ENTROU CRUZAMENTO"<<endl;
            nova_pop= cruzamento(percent_cruzamento,b,elite,vetSol, B, rede, n_elite, foraElite, rota_no);
            cout<<"SAIU CRUZAMENTO"<<endl;
            cout<<"ENTROU SELECAO"<<endl;
            selecao_pos_cruzamento(vetSol, pop_total, nova_pop, percent_elite_selecao, percent_pseudo_selecao, percent_rand_selecao);
            cout<<"SAIU SELECAO"<<endl;
            cout<<"ENTROU MUTACAO"<<endl;
            list<Solucao> nova_pop_mut=mutacao(percent_mutacao, b, vetSol,prob_mut_sol, prob_mut_caminho, B,sBOD,rede,percent_sorteio, rota_no);
            cout<<"SAIU MUTACAO"<<endl;

            //imprime(vetSol, pop_total, num_max_rotas,  rede,  sBOD,sBO,rota_no, B, b);
            //cout<<endl;
            
            if(vetSol[1].fo<melhor_Solucao){
                cont_geracoes_sem_melhoras = 0;
                melhor_Solucao = vetSol[1].fo;
            }
            else
            {
                cont_geracoes_sem_melhoras++;
            }
            
            cont_geracoes++;
        }
        //cout<<"SOLUCOES FINAIS: "<<endl;
        //imprime(vetSol, pop_total, num_max_rotas,  rede,  sBOD,sBO,rota_no, B, b);

        cout<<"-------------------------------------------------------------------------------"<<endl<<"FO: "<<vetSol[1].fo<<endl;
        cout<<"-------------------------------------------------------------------------------"<<endl;
        int rota = 0;
        for(it = vetSol[1].caminho.begin(); it != vetSol[1].caminho.end(); it++ )
            {
                printf("\ncaminho: %d ", *it);
                for ( iter = B[*it].list_nos.begin(); iter != B[*it].list_nos.end(); iter ++)
                {
                    cout << "| no " << *iter << " arco " << B[*it].cam_spur[*iter] << "| ";

                }
                cout<<endl<<"Frota: "<<vetSol[1].frota_por_rota[rota]<<endl;
                cout<<"Headway: "<<vetSol[1].headways[rota]<<endl;
                cout<<"Tempo de Ciclo: "<<vetSol[1].tempo_de_ciclo[rota]<<endl;
                cout<<"Demanda Maxima: "<<vetSol[1].demandas_maximas_por_rota[rota].demanda_maxima<<endl;
                cout<<"Arco: No "<<vetSol[1].demandas_maximas_por_rota[rota].no1<<" para No "<<vetSol[1].demandas_maximas_por_rota[rota].no2<<endl;
                rota++;
            }
            cout<<endl;
        //cout<<endl<<"FROTA_TESTE: "<<vetSol[1].frota_por_rota[0]<<endl;
        cout<<"-------------------------------------------------------------------------------"<<endl<<endl;
        execucoes++;
    //}
  return 0;
}


/*  IMPRIME UMA POPULA��O DE SOLU��O COM B ELEMENTOS

    for (i = 1; i <= b; i++){
        printf("\nSolucao: %d, fo: %d ", i, vetSol[i].fo);
        for(it = vetSol[i].numero_rota.begin(); it != vetSol[i].numero_rota.end(); it++ ){
            printf(" linha: %d", *it);
        }
    }
    */


/*
    //solucao viavel
    S2.numero_rota.push_back(5);
    S2.numero_rota.push_back(262);
    S2.numero_rota.push_back(76);
    S2.numero_rota.push_back(319);
    S2.numero_rota.push_back(362);
    S2.numero_rota.push_back(463);
*/




/*
    for(iter = S2.numero_rota.begin(); iter != S2.numero_rota.end(); iter++ )
    {
        printf("\nRota: %d > ", *iter);
        for(iter1 = B[*iter].list_nos.begin(); iter1 != B[*iter].list_nos.end(); iter1++)
        {
            printf(" : %d ", *iter1);
        }
            printf("\n");

    }


    printf("\nTestando viabilidade");
    int b = sol_viavel(S2, rede, B);
    getchar();

    vector <int> nao_atendidos;
    vector<int> aux;
    nao_atendidos.push_back(2);
    aux.push_back(0);
    nao_atendidos.push_back(4);
    aux.push_back(0);
    nao_atendidos.push_back(6);
    aux.push_back(0);
    nao_atendidos.push_back(8);
    aux.push_back(0);
    nao_atendidos.push_back(10);
    aux.push_back(0);
    nao_atendidos.push_back(12);
    aux.push_back(0);
    for (vector<int>::iterator a = nao_atendidos.begin() ; a != nao_atendidos.end()-1; ++a)
    {
        for (vector<int>::iterator b = nao_atendidos.begin() ; b != nao_atendidos.end(); ++b)
        {
            if (aux[*a==0] && aux[*b==0])
            {
                if (sBOD[*a][*b].size()!=0)
                {
                    int k;
                    k = randomiza(sBOD[*a][*b]);
                    vetSol[i].numero_rota.push_back(k);
                    aux[*a]=aux[*b]=1;
                }
            }
        }
    }

    for(iter=S1.numero_rota.begin(); iter!=S1.numero_rota.end(); iter++)
    {
        cout<<" "<<*iter<<endl;

        int rota=*iter;
        for ( iter1 = B[rota].list_nos.begin(); iter1 != B[rota].list_nos.end(); iter1 ++)
        {


            cout << " no " << *iter1 << " arco " << B[rota].cam_spur[*iter1] << endl;
            rede.na_rede[B[rota].cam_spur[*iter1]]=1;
        }
    }

    pesquisa (rede);


    cout << " ======== "<< endl;
    for ( iter = B[12].list_nos.begin(); iter != B[12].list_nos.end(); iter ++)
    {
        cout << " no " << *iter << " arco " << B[12].cam_spur[*iter] << endl;
        rede.na_rede[B[12].cam_spur[*iter]]=1;
    }

    cout << " ======== "<< endl;
    for ( iter = B[11].list_nos.begin(); iter != B[11].list_nos.end(); iter ++)
    {
        cout << " no " << *iter << " arco " << B[11].cam_spur[*iter] << endl;
        rede.na_rede[B[11].cam_spur[*iter]]=1;
    }

    cout<<"------------"<<endl;

    */

    /*
    for(it=rede.lista_saida[3].begin(); it!=rede.lista_saida[3].end(); it++)
    {
        cout<< " " << *it;
        cout<< " " << rede.iarc[*it];
        cout<< " -> " << rede.jarc[*it];
        cout<< " " << rede.custo[*it]<<endl;
    }

    for (int i=1; i<= rede.m; i++)
        rede.na_rede[i]=1;

    pesquisa (rede);

    for (int i=1; i<= rede.n; i ++)
        cout<< " "<< i << " "<< rede.pred[i]<< endl;
    int no=5;
    while (no!=0){
        cout<< " "<< no;
        no=rede.pred[no];
    }
    for(int i=1; i<=rede1.n; i++)
        rede1.marca[i]=0;

    rede1.marca[rede1.src]=1;
    rede1.pred[rede1.src]=0;
    int next=1;
    int order [MAX_NOD];
    order[next]=rede1.src;

    list <int> Lista;
    Lista.push_back(rede1.src);
    while (Lista.size()!=0)
    {
        int no=Lista.front();
        Lista.pop_front();
        for (it=rede1.lista_saida[no].begin(); it!=rede1.lista_saida[no].end(); it++)
        {

            if (rede1.marca[rede1.jarc[*it]]==0)
            {
                rede1.marca[rede1.jarc[*it]]=1;
                rede1.pred[rede1.jarc[*it]]=no;
                next=next+1;
                order[rede1.jarc[*it]]=next;
                Lista.push_back(rede1.jarc[*it]);
            }

        }
    }
    for(int i=1; i<=rede1.n; i++)
        cout<< " "<< rede1.marca[i]<<endl;
    */
