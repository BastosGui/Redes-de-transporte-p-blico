#include "Rede.h"
#include <list>

//------------------------------------------------------------------------------
void Rede::zera_rede()
//------------------------------------------------------------------------------
// limpa todas as informa��es da rede
{
   int i;

   for(i = 0; i < MAX_ARC; i++)
   {
        spur_arc[i] = 0;        // arco de desvio
   }


   for(i = 0; i < MAX_NOD; i++)
   {
        cam_min[i] = 0;         // arc pred no caminho m�nimo
        cam_spur[i] = 0;         // arcos no ultimo caminho minimo
   }
}

//------------------------------------------------------------------------------
void le_rede(Rede &rede, char arquivo[])
//------------------------------------------------------------------------------
// Fun��o que l� o arquivo com as informa��es da rede e carrega nas listas
// de adjac�ncias de entrada e sa�da dos arcos nos respectivos n�s.
{
bool solviavel = true;
char line[150];
int no_aux, b_aux, i, j, k;

    FILE *nt;
    nt = fopen(arquivo, "r");  // abrindo o arquivo rede1.txt

    if (nt == 0)                 // verifica se a abertura deu certo
    {
        printf("\nArquivo de entrada %s nao encontrado\n", arquivo);
        getchar();
        exit(1);
    }

    float velocidade = 16; // Km/h
    float distancia = 0;
    float tempo = 0;
    //printf("\nReading file %s\n", arquivo);
    fscanf(nt, "%d %d %d %d %f\n",&rede.n, &rede.m, &rede.src, &rede.snk, &velocidade);    // reading total of nodes, arcs, source, sink and speed

    printf("\nNos %d Arcos %d Origem %d Destino %d\n", rede.n, rede.m, rede.src, rede.snk);
    
   for (int arc = 1; arc <= rede.m; arc++){
        fscanf(nt, "%d %d %f\n", &rede.iarc[arc], &rede.jarc[arc], &distancia);
        //cout<<"Dist: "<<distancia<<endl<<"Velocidade: "<<velocidade<<endl;
        //tempo = ((distancia/1000)/ velocidade)*60;
        //cout<<"Tempo: "<<tempo<<endl;
        //rede.custo[arc] = ceil(tempo);
        //cout<<"Tempo final: "<<rede.custo[arc]<<endl;
        rede.custo[arc] = distancia;
        rede.custo_orig[arc] = rede.custo[arc];
        rede.spur_arc[arc] = 0;
        //printf("\n arc = %d j = %d l_inf = %d cap = %d custo= %d\n", rede.iarc[arc], rede.jarc[arc], rede.l_inf[arc], rede.cap[arc], rede.custo[arc]);
        rede.lista_saida[rede.iarc[arc]].push_back(arc);
        rede.lista_entrada[rede.jarc[arc]].push_back(arc);
   }

    //getchar();
   fclose(nt);
   //cout<<"LEU A REDE"<<endl;
}

bool le_demandas(Rede &rede, char arquivo[]){
    FILE *arq_demandas;

    if ((arq_demandas = fopen(arquivo, "r")) == NULL)                 // verifica se a abertura deu certo
    {
        printf("\nArquivo de entrada %s nao encontrado\n", arquivo);
        getchar();
        exit(1);
    }

    //cout<<"ABRIU"<<endl;
    int aux;
    //char enter;
    for(int i = 0; i<rede.n; i++){
        for(int j = 0; j<rede.n; j++){
            if(fscanf(arq_demandas, "%d", &aux) == EOF){
                cout<<"ARQUIVO DE DEMANDA COM INFORMACOES INSUFICIENTES"<<endl;
                getchar();
                return false;
            }
            else
            {
                rede.demanda[i][j] = aux;
            }
            
        }
        //fscanf(arq_demandas, "%[\n]", enter);
    }
    return true;
}

//------------------------------------------------------------------------------
void imprime_rede(Rede rede)
//------------------------------------------------------------------------------
{
    list<int>::iterator iter;
    for (int i = 1; i <= rede.m; i++)	{
      //printf("\n i= %d j= %d l= %d u= %d c= %d\n", rede.iarc[i], rede.jarc[i], rede.l_inf[i], rede.cap[i], rede.custo[i]);
      printf("\n i= %d j= %d c= %d\n", rede.iarc[i], rede.jarc[i], rede.custo[i]);
    }
    for (int i = 1; i <= rede.n; i++){
        printf("\n Nos adjacentes saida no %d: ", i);
        for(iter = rede.lista_saida[i].begin(); iter != rede.lista_saida[i].end(); iter++){
            printf(" %d", rede.jarc[*iter]);
        }

        printf("\n Nos adjacentes entrada no %d: ", i);
        for(iter = rede.lista_entrada[i].begin(); iter != rede.lista_entrada[i].end(); iter++){
            printf(" %d", rede.iarc[*iter]);
        }
    }
}
//==============================================================================

/*
void BellmanFord(Rede rede)//list vertices, list edges, vertex source)
   ::distance[],predecessor[]

   // This implementation takes in a graph, represented as
   // lists of vertices and edges, and fills two arrays
   // (distance and predecessor) with shortest-path
   // (less cost/distance/metric) information

   // Step 1: initialize graph
   for each vertex v in vertices:
       if v is source then distance[v] := 0
       else distance[v] := inf
       predecessor[v] := null

   // Step 2: relax edges repeatedly
   for i from 1 to size(vertices)-1:
       for each edge (u, v) in Graph with weight w in edges:
           if distance[u] + w < distance[v]:
               distance[v] := distance[u] + w
               predecessor[v] := u

   // Step 3: check for negative-weight cycles
   for each edge (u, v) in Graph with weight w in edges:
       if distance[u] + w < distance[v]:
           error "Graph contains a negative-weight cycle"
   return distance[], predecessor[]
*/

//------------------------------------------------------------------------------
list<int> Rot_Modif(Rede &rede)     //Agoritmo de Dijkstra
//------------------------------------------------------------------------------
// retorna em uma lista os arcos do ciclo ou lista vazia.
{
   list<int> Lista, arc_ciclo, ciclo;   // lista dos nos ainda n�o rotulados
   list<int>::iterator lst, lst01;
   int no, no1, i, arco;
   int d[MAX_NOD];            // limitante da dist�ncia at� o n� origem
   int pred[MAX_NOD];         // n� predecessor no caminho m�nimo
   int naLista[MAX_NOD];      // marca se o n� est� na lista ( = 1) ou n�o ( = 0)
   int janaLista[MAX_NOD];    // se o no esteve antes na lista
   int CamMin[MAX_NOD];       // guarda os carcos do caminho m�nimo
   bool continua;

   //sprintf(str,"Algoritmo do rotumento modificado com dequeue.\n");
   for (i= 1; i <= rede.n; i++)
   {
      d[i] = INFINITY;
      naLista[i] = 0;
      janaLista[i] = 0;
      pred[i] = 0;
      CamMin[i] = 0;
   }

   continua = true;
   d[rede.src] = 0;
   naLista[rede.src] = 1;
   janaLista[rede.src] = 1;
   Lista.push_back(rede.src);
   pred[rede.src] = rede.src;
   arc_ciclo.clear();
//double cpuTime() {
//    static struct rusage usage;
  //  getrusage(RUSAGE_SELF, &usage);
  //  return ((double) usage.ru_utime.tv_sec)+(((double) usage.ru_utime.tv_usec) / ((double) 1000000));
//}

   while (Lista.size()!= 0)
   {
      lst01 = Lista.begin();
      Lista.pop_front();
      no = *lst01;
      janaLista[no] = 1;
      naLista[no] = 0;
//      printf("\n pesquisando no %d", no);
      for (lst = rede.lista_saida[no].begin(); lst != rede.lista_saida[no].end(); lst++)
      {
         //*lst � o indice do arco que sai do no
         if (d[rede.jarc[*lst]] > d[rede.iarc[*lst]] + rede.custo[*lst])
         {
            d[rede.jarc[*lst]] = d[rede.iarc[*lst]] + rede.custo[*lst];
            pred[rede.jarc[*lst]] = rede.iarc[*lst];
            CamMin[rede.jarc[*lst]] = *lst;
            int no_pesq = rede.jarc[*lst];
            int no_rot = no_pesq;
            ciclo.clear();
            ciclo.push_front(no_pesq);
            // pesquisando na arvore
            while(pred[no_pesq] != no_pesq && continua){
                if(pred[no_pesq] == no_rot){
    //                printf("\n\n achou ciclo");
                    continua = false;
      //              printf("\nCiclo: ");
                    for(lst01 = ciclo.begin(); lst01 != ciclo.end(); lst01++){
 //                       printf(" no %d - ", *lst01);
 //                       printf(" arco %d ", CamMin[*lst01]);
                        arc_ciclo.push_back(CamMin[*lst01]);
                    }
 //                   getchar();
                    return(arc_ciclo);
                }
                no_pesq = pred[no_pesq];
                ciclo.push_front(no_pesq);
            }
            //continua = 1;

            if (!naLista[rede.jarc[*lst]])
            {
                naLista[rede.jarc[*lst]] = 1;  // esta entrando na lista agora
                if(janaLista[rede.jarc[*lst]]){
                    Lista.push_front(rede.jarc[*lst]);
//                    printf("\n no entrando na frente");
                }
                else{
                    Lista.push_back(rede.jarc[*lst]);
 //                   printf("\n no entrando atras");
                }
            }
         }
      }
   }
   return(arc_ciclo);
}


//===============================================================================
list<int> BFCT(Rede rede) //
//===============================================================================
{
        int d[MAX_NOD];
        int p[MAX_NOD];
        int arc_p[MAX_NOD];
        int nalista[MAX_NOD];   // para verificar se esta na lista
        int janalista[MAX_NOD]; // para verificar se ja passou alguma vez pela lista
        list <int> lista, ciclo;
        ciclo.clear();
        list<int>:: iterator iter, iter1;
        inicializarDistancias(rede, d, nalista, janalista);
        d[rede.src] = 0;
        p[rede.src] = rede.src;
        lista.push_back(rede.src);
        nalista[rede.src] = 1; // esta na lista
        janalista[rede.src] = 1; // ja passou pela lista
        while(lista.size()!= 0)
        {
            iter = lista.begin();
            lista.pop_front();
            int u = *iter;
            nalista[u] = 0;
            printf("\n\n === Pesquisando no u %d", u);
            for(iter = rede.lista_saida[u].begin(); iter != rede.lista_saida[u].end(); iter++)
            {
                int v = rede.jarc[*iter];
                printf("\n v = %d, d[v] = %d", v, d[v]);
                if(d[v] > d[u] + rede.custo[*iter])
                {
                    printf("\nRotulando %d", v);
                    d[v] = d[u] + rede.custo[*iter];
                    p[v] = u;
                    arc_p[v] = *iter;
                    if (nalista[v]==0){   // nao esta na lista no momento
                        nalista[v] = 1;
                        if(janalista[v])  // ja esteve antes na lista
                            lista.push_front(v);
                        else{             // nunca esteve na lista antes
                            lista.push_back(v);
                            janalista[v] = 1;
                        }
                    }
                    int w = v;
                    while(w != p[w]) // quando for igual significa que est� na raiz
                    {
                        printf("\n w = %d", w);
                        w = p[w];
                        if(w == v)
                        {
                            printf("\n encontrei um ciclo iniciando em %d", w);
                            int x1 = p[v];
                            ciclo.push_back(arc_p[x1]);
                            while(x1 != v){
                                x1 = p[x1];
                                ciclo.push_back(arc_p[x1]);
                            }
                            ciclo.reverse();
                            return(ciclo);
                            exit(0);
                        }
                    }
                }
            }
        }
        return(ciclo);
}
//------------------------------------------------------------------------------
list<int> Tarjan1 (Rede rede)//(int n, int s, int l[], ArrayList<Integer>[] Succ, int destino[])
//------------------------------------------------------------------------------
{
    int d[MAX_NOD]; // = new int[n+1];
    int ant[MAX_NOD]; // = new int[n+1];
    int suc[MAX_NOD]; // = new int[n+1];
    int arc_pred[MAX_NOD];
    int nalista[MAX_NOD];
    int janalista[MAX_NOD];

    for (int i = 0; i < MAX_NOD; ++i) d[i] = 99999;  // inicializa��o das dist�ncias

    for (int i = 0; i < MAX_NOD; ++i){
        janalista[i] = nalista[i] = ant[i] = suc[i] = 0;
    }
    d[rede.src] = 0;
    janalista[rede.src] = 1;
    list <int> Q, ciclo;
    list <int>::iterator iter;
    Q.push_back(rede.src);

    while(Q.size() != 0)
    {
        iter = Q.begin();
        Q.pop_front();
        int i = *iter;
        printf("\n\ni = %d\n", i);

        for(iter = rede.lista_saida[i].begin(); iter != rede.lista_saida[i].end(); ++iter)
        {
            int j = rede.jarc[*iter];
            printf("\ni = %d, j = %d", i, j);
            printf("\nd[%d] = %d, d[%d] = %d, c(%d,%d) = %d", j, d[j], i, d[i], i, j, rede.custo[*iter]);
            if(d[j] > d[i] + rede.custo[*iter])
            {
                int w = suc[j];
                //int w = j;
                //printf("\n suc[%d] = %d",j, suc[j]);
                d[j] = d[i] + rede.custo[*iter];
                ant[j] = i;
                suc[i] = j;
                arc_pred[j] = *iter;
                printf("\n rotulando no: %d, ant: %d, suc: %d", j, ant[j], suc[j]);
                Q.push_back(j);

                while(w != 0)
                {
                    printf("\n w = %d", w);
                    if(w == i)
                    {
                        printf("\n iniciando ciclo. w: %d, v: %d, arcp v %d, arcp w %d", i, j, arc_pred[j], arc_pred[w]);
                        getchar();
                        ant[j] = w;
                        ciclo = caminhoCiclo(ant, arc_pred, j, w);
                        return(ciclo);
                        exit(0);
                    }
                    w = suc[w];
                    //w = ant[w];
                }
            }
        }
    }
    return(ciclo);
}

//------------------------------------------------------------------------------
    list<int> caminhoCiclo(int ant[], int arc_pred[], int j, int w)
//------------------------------------------------------------------------------
    {
        list <int> caminho, caminho_arco;
        list<int> ::iterator iter;
        int x = w;
        caminho.push_front(x);
        printf("\n fora x: %d", x);
        x = ant[x];
        caminho_arco.push_front(arc_pred[x]);
        while(x != w)
        {
           caminho.push_front(x);
           x = ant[x];
           printf("\nx: %d", x);
           caminho_arco.push_front(arc_pred[x]);
        }
/*
        for(iter = caminho.begin(); iter != caminho.end(); iter++)
        {
            printf("\n caminho = %d arco %d", *iter, arc_pred[*iter]);
            //caminho_arco.push_front(arc_pred[*iter]);
        }
        */
        return(caminho_arco);
    }


//------------------------------------------------------------------------------
void imprimirDados(int n, int d[], int ant[], int suc[])
    {
        printf("\nDIST�NCIAS");
        for(int i = 1; i <= n; ++i)
            printf("d[%d] = %d - ", i, d[i]);

        //printf("\nANTECESSORES");printf("\n Nos adjacentes ao no %d: ", i);

        for(int i = 1; i <= n; ++i)
            printf("antecessor[%d] = %d - ", i, ant[i]);

        printf("\nSUCESSORES");
        for(int i = 1; i <= n; ++i)
            printf("sucessor[%d] = %d - ", i, suc[i]);
    }

//------------------------------------------------------------------------------
void inicializarDistancias(Rede rede, int d[], int nalista[], int janalista[])
//------------------------------------------------------------------------------
{
    for (int i = 0; i <=rede.n; ++i)
    {
            d[i] = 99999;
            nalista[i] = 0;
            janalista[i] = 0;
//            p[i] = -1;
    }
}

//------------------------------------------------------------------------------
void Dijkstra(Rede &rede)  //Agoritmo de Dijkstra
//------------------------------------------------------------------------------
// retorna em uma lista os arcos do ciclo ou lista vazia.
{
   list<int> ciclo;   // lista dos nos ainda n�o rotulados
   list<int> Lista;
   list<int>::iterator lst, lst01;
   int no, i;
   int janaLista[MAX_NOD];
   int naLista[MAX_NOD];
   // inicializa��o
   for (i= 1; i <= rede.n; i++)
   {
      rede.d[i] = INFINITY;
      rede.s_barra[i] = 1;
      rede.pred[i] = -1;
      rede.cam_min[i] = 0;
      janaLista[i] = 0;
      naLista[i] = 0;
   }

   int continua = 1;
   rede.d[rede.src] = 0;
   rede.pred[rede.src] = 0;
   Lista.empty();

   while (Lista.size()!= rede.n && continua)
   {
     /* lst01 = Lista.begin();
      no = *lst01;
      janaLista[no]++;
      if(janaLista[no] > rede.n - 1)
      {
               no1 = rede.pred[no];
               continua = 0;
               break;
      }
      Lista.pop_front();
      naLista[no] = 0;
*/
		int minimo = INFINITY;
		for (i = 1; i <= rede.n; i++){  // encontrando o no em S barra com menor d
			if( rede.s_barra[i] & rede.d[i] < minimo){
				no = i;
				minimo = rede.d[i];
			}
		}
		//printf("\n no pesquisado %d", no);
		rede.s_barra[no] = 0;
		Lista.push_back(no);
		continua = (no == rede.snk)? 0 : 1;
		printf("\n continua %d", continua);

		if(continua){
         for (lst = rede.lista_saida[no].begin(); lst != rede.lista_saida[no].end(); lst++)
         {
            //*lst � o indice do arco que sai do no
            if (rede.d[rede.jarc[*lst]] > rede.d[rede.iarc[*lst]] + rede.custo[*lst])
            {
               rede.d[rede.jarc[*lst]] = rede.d[rede.iarc[*lst]] + rede.custo[*lst];
               rede.pred[rede.jarc[*lst]] = rede.iarc[*lst];
               rede.cam_min[rede.jarc[*lst]] = *lst;
                   /*if (!naLista[rede.jarc[*lst]])
                   {
                       naLista[rede.jarc[*lst]] = 1;  // esta entrando na lista agora
                       Lista.push_back(rede.jarc[*lst]);
                   }*/
            }
         }
		}
	}
	printf("\nDistancias min\n");
	for( i = 1; i <= rede.n; i++){
		printf("i:%d\t", i);
		printf("d: %d\t", rede.d[i]);
		printf("pred[%d]: %d\n",i, rede.pred[i]);
	}
	printf("\nCaminho min\n");
	no = rede.snk;
	while (rede.pred[no] != 0){
      printf("no %d, arc %d, no pred %d\n", no, rede.cam_min[no], rede.pred[no]);
      no = rede.pred[no];
	}
}



//------------------------------------------------------------------------------
//list <int> Dijkstra_reverso(Rede &rede, int origem, int destino)  //Agoritmo de Dijkstra
list <int> Dijkstra_reverso(Rede &rede, int origem, int destino, list <int> root)
//------------------------------------------------------------------------------
// retorna em uma lista os arcos do ciclo ou lista vazia.
{
   list<int> Lista;
   list<int>::iterator lst, lst01;
   int no, i;
   int janaLista[MAX_NOD];
   int naLista[MAX_NOD];

   for (i = 1; i <= rede.n; i++)
   {
      rede.d[i] = INFINITY;
      rede.s_barra[i] = 1;
      rede.pred[i] = -1;
      rede.cam_min[i] = 0;
      janaLista[i] = 0;
      naLista[i] = 0;
   }
   for (lst = root.begin(); lst != root.end(); lst++)
        rede.s_barra[*lst] = 0;

   rede.d[destino] = 0;
   rede.pred[destino] = 0;
   Lista.empty();
   while (Lista.size()!= rede.n)
   {
		int minimo = INFINITY;
		for (i = 1; i <= rede.n; i++){  // encontrando o no em S barra com menor d
			if(( rede.s_barra[i]) & (rede.d[i] < minimo)){
				minimo = rede.d[i];
				no = i;
			}
		}
		//printf("\n no pesquisado %d", no);
		rede.s_barra[no] = 0;
		Lista.push_back(no);
		for (lst = rede.lista_entrada[no].begin(); lst != rede.lista_entrada[no].end(); lst++){
            //*lst � o indice do arco que sai do no
            if (rede.d[rede.iarc[*lst]] > rede.d[rede.jarc[*lst]] + rede.custo[*lst]){
               rede.d[rede.iarc[*lst]] = rede.d[rede.jarc[*lst]] + rede.custo[*lst];
               rede.pred[rede.iarc[*lst]] = rede.jarc[*lst];
               rede.cam_min[rede.iarc[*lst]] = *lst;
            }
        }
    }
    Lista.resize(0);
	if (rede.d[origem] < INFINITY){
        //printf("\nCaminho minimo: %d \n", rede.d[origem]);
        no = origem;
        while (rede.pred[no] != 0){
            //printf("\nno %d, arc %d, no pred %d", no, rede.cam_min[no], rede.pred[no]);
            Lista.push_back(no);
            no = rede.pred[no];
        }
        Lista.push_back(no);
	}
	return(Lista);
}
//------------------------------------------------------------------------------
void copia_vetor(int vet_orig[], int vet_dest[], int dim){
//------------------------------------------------------------------------------
for (int i = 1; i <= dim; i++)
    vet_dest[i] = vet_orig[i];
}

//------------------------------------------------------------------------------
int k_shortest_path1(Rede &rede, int origem, int destino, int K, Caminho A1[]){
//------------------------------------------------------------------------------
    list<int> totalPath, spurPath, rootPath;   // lista dos nos ainda n�o rotulados
    list<int> ::iterator lst_arc, lst, lst_i, lst_aux, fim;
    int cont, custo1, custo2, spurNode, spurArc;
    int tot_cam_min = 0;
    Caminho B[MAX_NOD];
    rootPath.empty();
    A1[0].list_nos = Dijkstra_reverso(rede, origem, destino, rootPath);
    A1[0].custo = rede.d[origem];
    if(rede.d[origem] < INFINITY){// n�s origem de destino s�o conectados
        tot_cam_min++;
        for (int i = 1; i <= rede.n; i++){
            A1[0].cam_spur[i] = rede.cam_min[i];  // s� vale para a primeira itera��o
            //printf("\nCusto min: %d", A1[0].custo);
        }
        for (int k = 1; k < K; k++){
            cont = 0;
            fim = A1[k-1].list_nos.end();
            fim--;
            custo1 = 0;
            for (lst_i = A1[k-1].list_nos.begin(); lst_i != fim; lst_i++){
                spurNode = *lst_i;
                spurArc = A1[k-1].cam_spur[spurNode];
                lst_aux = lst_i; lst_aux--;
                if(lst_i != A1[k-1].list_nos.begin()){
                    copia_vetor(A1[k-1].cam_spur, B[cont].cam_spur, rede.n);
                    //printf("\n atualizando custo1 %d, arco %d, i %d, j %d", custo1, *lst_aux, rede.iarc[*lst_aux],rede.jarc[*lst_aux] );
                }
                rootPath.assign(A1[k-1].list_nos.begin(), lst_i);
                rede.spur_arc[spurArc] = 1;
                for(lst_aux = rede.lista_saida[spurNode].begin(); lst_aux != rede.lista_saida[spurNode].end(); lst_aux++){
                    if(rede.spur_arc[*lst_aux] == 1){
                        rede.custo[*lst_aux] = INFINITY;
                    // printf("\ncusto do  arco %d recebendo infinito", *lst_aux);
                    }
                }
                //spurPath = Dijkstra_reverso(rede, spurNode, destino);
                spurPath = Dijkstra_reverso(rede, spurNode, destino, rootPath);
                custo2 = rede.d[spurNode];  // distancia calculada pelo Dijkstra
                for(lst = spurPath.begin(); lst != spurPath.end(); lst++){
                    B[cont].cam_spur[*lst] = rede.cam_min[*lst];
                }
                totalPath = rootPath;
                totalPath.insert(totalPath.end(), spurPath.begin(),spurPath.end());
                B[cont].list_nos = totalPath;
                B[cont].custo = custo1 + custo2;
                custo1 += rede.custo_orig[A1[k-1].cam_spur[spurNode]];
                cont++;
                retorna_custo_arco(rede);
            }
            int min_cam = INFINITY;
            int pos_min = -1;
            for(int i = 0; i < cont; i++){
                if (B[i].custo <= min_cam){
                    min_cam = B[i].custo;
                    pos_min = i;
                }
            }
            if (min_cam < INFINITY/2){
                tot_cam_min++;
                A1[k] = B[pos_min];
                /*printf("\n A[%d].custo = %d\n nos, arc_spur >> ", k, A1[k].custo);
                for(lst = A1[k].list_nos.begin(); lst != A1[k].list_nos.end(); lst++)
                    printf(" no: %d, arc %d || ", *lst, A1[k].cam_spur[*lst]);
                */
            }
        }
    }/*
    else{
        printf("\nNos %d e %d sao desconectados", origem, destino);
        //getchar();
    }*/
    //printf("\nTotal cam_min entre %d e %d = %d", origem, destino, tot_cam_min);
    //getchar();
    return(tot_cam_min);
}

//------------------------------------------------------------------------------
void retorna_custo_arco(Rede &rede){
//------------------------------------------------------------------------------
    for (int i = 1; i <= rede.m; i++)
        rede.custo[i] = rede.custo_orig[i];
}
/*------------------------------------------------------------------------------
void limpa_B(Caminho B[], int pos_min, int cont){
//------------------------------------------------------------------------------
    list<int>::iterator lst_cont;
    list<int> LA;
    for(int i = 0; i < cont; i++){
        if((i!= pos_min) && (B[i].custo == B[pos_min].custo)&&(B[i].list_nos.size() == B[pos_min].list_nos.size())) {
            LA = B[i].list_nos;
            LA.insert(LA.end(), B[pos_min].list_nos.begin(), B[pos_min].list_nos.end());
            LA.sort(); LA.unique();
            if(LA.size() == B[i].list_nos.size())
                B[i].custo = INFINITY + 1; // retirei do vetor
                B[i].list_nos.resize(0);
        }
    }
    B[pos_min].custo = INFINITY + 1; // retirei do vetor
    B[pos_min].list_nos.resize(0);
}
*/
//------------------------------------------------------------------------------
int cria_banco_rotas(Rede rede, Caminho B[MAX_ARC], int K, float taxa, list <int> sBO[MAX_NOD], list <int> sBOD[MAX_NOD][MAX_NOD], list <int> rota_no[MAX_NOD], int tam_min, int tempo_max){
//------------------------------------------------------------------------------
    list<int>::iterator lst;
    int origem, destino, k1;
    int tot_rotas = 0;
    int menorTempo;
    for (origem = 1; origem <= rede.n; origem ++){
        for (destino = 1; destino <= rede.n; destino ++){
            int tam_min_prov = tam_min+rand()%5;
            //printf("\n origem %d, destino %d", origem, destino);
            if (origem != destino) {
                Caminho A[MAX_NOD];
                //printf("\n origem %d, destino %d", origem, destino);
                k1 = k_shortest_path1(rede, origem, destino, K, A);
                //printf("\nk = %d", k1);
                if (k1 > 0){
                    if (A[0].list_nos.size() >= tam_min_prov && A[0].custo <= tempo_max) {
                        tot_rotas++;
                        B[tot_rotas] = A[0]; // caminho min entre i e j
                        menorTempo = A[0].custo;
                        sBO[origem].push_back(tot_rotas);
                        sBOD[origem][destino].push_back(tot_rotas);
                        for(lst = B[tot_rotas].list_nos.begin(); lst != B[tot_rotas].list_nos.end(); lst++){
                            //printf(" %d - %d | ", *lst, B[tot_rotas].cam_spur[*lst]);
                            rota_no[*lst].push_back(tot_rotas);
                        }
                    }
                    for(int i = 1; i < k1; i++){
                        //printf("\ncusto %d", A[i].custo);
                        if(((float)A[i].custo/menorTempo <= taxa)&& (A[i].list_nos.size() >= tam_min_prov && A[i].custo <= tempo_max)) {
                            tot_rotas++;
                            B[tot_rotas] = A[i];
                            sBO[origem].push_back(tot_rotas);
                            sBOD[origem][destino].push_back(tot_rotas);
                            //printf("\nCusto do %d-esimo caminho minimo: %d, size %d nos >> ", tot_rotas, B[tot_rotas].custo, B[tot_rotas].list_nos.size());
                            for(lst = B[tot_rotas].list_nos.begin(); lst != B[tot_rotas].list_nos.end(); lst++){
                                //printf(" %d - %d | ", *lst, B[tot_rotas].cam_spur[*lst]);
                                rota_no[*lst].push_back(tot_rotas);
                            }
                        }
                    }
                }
                //getchar();
                rede.zera_rede();
            }
        }
    }
    return(tot_rotas);
}
void zera_rpa(Solucao &solucao){
    for(int i = 0; i<NOS_NA_REDE; i++){
        for(int j = 0; j<NOS_NA_REDE; j++){
            solucao.rotas_por_arco[i][j] = 0;
        }
    }
}

void calcula_rotas_por_arco(Solucao &solucao, Caminho B[MAX_ARC]){
    zera_rpa(solucao);
    for(list<int>:: iterator it = solucao.caminho.begin(); it != solucao.caminho.end(); it++){
        list<int>:: iterator aux1 = B[*it].list_nos.begin();
        list<int>:: iterator aux2 = B[*it].list_nos.begin();
        aux2++;
        //cout<<"ROTA "<<*it<<endl;
        for(int i = 0; i < B[*it].list_nos.size()-1; i++){
            solucao.rotas_por_arco[*aux1-1][*aux2-1]++;
            //cout<<"TESTE DAS ROTAS POR ARCO:"<<endl;
            //cout<<"ARCO DO NO "<<*aux1<<" PARA O NO "<<*aux2<<":  "<<solucao.rotas_por_arco[*aux1-1][*aux2-1]<<endl;
            aux1++;
            aux2++;
        }
    }
}
/*
void calcula_demandas_soluc(Rede rede, Solucao &solucao, Caminho B[MAX_ARC]){
    for(list<int>:: iterator rota = solucao.caminho.begin(); rota != solucao.caminho.end(); rota++){
        list<int>:: iterator fno = B[*rota].list_nos.begin();
        list<int>:: iterator sno = B[*rota].list_nos.begin();
        sno++;
        list<int>:: iterator aux1 = fno;
        list<int>:: iterator aux1 = sno;
        for(int i = 0; i < B[*rota].list_nos.size()-2; i++){

            } 
            fno++;
            sno++;
        }
}
*/

void zera_dpa(Solucao &solucao){
    for(int i = 0; i<NOS_NA_REDE; i++){
        for(int j = 0; j<NOS_NA_REDE; j++){
            solucao.demandas[i][j] = 0;
        }
    }
}

void calcula_demandas_maximas_por_rota(Rede rede, Solucao &solucao, Caminho B[MAX_ARC]){
    calcula_rotas_por_arco(solucao, B);
    zera_dpa(solucao);
    int rota_da_solucao = 0;
    for(list<int>:: iterator rota = solucao.caminho.begin(); rota != solucao.caminho.end(); rota++){
        list<int>:: iterator fno = B[*rota].list_nos.begin();
        list<int>:: iterator sno = B[*rota].list_nos.begin();
        sno++;
        for(sno; sno != B[*rota].list_nos.end(); sno++, fno++){
            list<int>:: iterator runner = sno;
            for(runner; runner != B[*rota].list_nos.end(); runner++){
                solucao.demandas[*fno-1][*sno-1] += ceil(rede.demanda[*fno-1][*runner-1]/(solucao.rotas_por_arco[*fno-1][*sno-1]*solucao.rotas_por_arco[*fno-1][*sno-1]));
                //solucao.demandas[*fno-1][*sno-1] += ceil(rede.demanda[*fno-1][*runner-1]/(solucao.rotas_por_arco[*fno-1][*sno-1]));
            }
        }
    }

    for(list<int>:: iterator rota = solucao.caminho.begin(); rota != solucao.caminho.end(); rota++){
        list<int>:: iterator fno = B[*rota].list_nos.begin();
        list<int>:: iterator sno = B[*rota].list_nos.begin();
        sno++;
        int maior_demanda_da_rota = solucao.demandas[*fno-1][*sno-1];
        int rotas;
        int primeiro_no = *fno;
        int segundo_no = *sno;
        fno++;
        sno++;
        //cout<<"Rota: "<<*rota<<endl;
        for(sno; sno != B[*rota].list_nos.end(); sno++, fno++){
            if(maior_demanda_da_rota < solucao.demandas[*fno-1][*sno-1]){
                maior_demanda_da_rota = solucao.demandas[*fno-1][*sno-1];
                primeiro_no = *fno;
                segundo_no = *sno;
            }
        }
        //cout<<"MAIOR POS FOR: "<<maior_demanda_da_rota<<"  Rotas p/ arco: "<<rotas<<endl;
        solucao.demandas_maximas_por_rota[rota_da_solucao].demanda_maxima = maior_demanda_da_rota;
        solucao.demandas_maximas_por_rota[rota_da_solucao].no1 = primeiro_no;
        solucao.demandas_maximas_por_rota[rota_da_solucao].no2 = segundo_no;
        rota_da_solucao++;
    }

}

void calcula_tempos_de_ciclo(Caminho B[MAX_ARC], Solucao &solucao){
    //cout<<"Entrou tempo de ciclo"<<endl;
    list<int>::iterator caminho = solucao.caminho.begin();
    for(int rota = 0; rota<solucao.caminho.size(); rota++){
        solucao.tempo_de_ciclo[rota] = 2*B[*caminho].custo;
        caminho++;
    }
    //cout<<"Saiu tempo de ciclo"<<endl;
}

void calcula_frota_por_rota(Solucao &solucao){
    //cout<<"Entrou frota por rota"<<endl;
    for(int rota = 0; rota<solucao.caminho.size(); rota++){
        //cout<<"Viagens por rota: "<<solucao.viagens_por_rota[rota]<<endl;
        //cout<<"Headways: "<<solucao.headways[rota]<<endl;
        solucao.frota_por_rota[rota] = ceil((double)solucao.tempo_de_ciclo[rota]/(double)solucao.headways[rota]);
        if(solucao.frota_por_rota[rota] == 0) solucao.frota_por_rota[rota]++;
    }
    //cout<<"Saiu frota por rota"<<endl;
}

void calcula_viagens_por_rota(Solucao &solucao){
    //cout<<"Entrou viagens por rota"<<endl;
    for(int  rota = 0; rota < solucao.caminho.size(); rota++){
        //cout<<"VALOR DA DEMANDA: "<<solucao.demandas_maximas_por_rota[rota].demanda_maxima<<endl;
        solucao.viagens_por_rota[rota] = ceil((double)solucao.demandas_maximas_por_rota[rota].demanda_maxima /(double)(FC * BUSS_CAPACITY));
        if(solucao.viagens_por_rota[rota] == 0) solucao.viagens_por_rota[rota]++;
        solucao.headways[rota] = ceil(60.0 / (double)solucao.viagens_por_rota[rota]);
        //cout<<"FROTA_GERADA: "<<solucao.frota_por_rota[rota]<<endl;
    }
    //cout<<"Saiu viagens por rota"<<endl;
}