// Autor: Thayse Marques Solis
// Data da última modificação: 13 de março de 2021
// Descrição resumida: Trabalho 3 - Best-Effort Broadcast sobre o VCube

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "smpl.h"
#include "cisj.h"
#include "token_agrupada.h"
#include "int_set.h"

// Vamos definir os EVENTOS
#define test 1
#define fault 2
#define recovery 3
#define no_event 0
#define bebcast 4
#define rcvmsg 5
#define rcvack 6
#define failed 7

#define eh_par(x) ((x)%2==0)
#define ceil_div(x,y) (int)ceil(((double)(x))/(y))

#define BEBCAST
//#define VCUBE

#ifdef VCUBE
#  define V(x) x
#else
#  define V(x) 
#endif

#ifdef BEBCAST
#  define B(x) x
#else
#  define B(x) 
#endif

static int N; // número de processos
float tempo_entre_eventos_vcube = 10.0;
float tempo_entre_eventos_bebcast = 30.0;

typedef struct {
	int id;		// identificador de facility (recurso) do SMPL
	int* State;
	//int cluster_atual;
	int_set PendingACK; //Processos que estão "devendo" ACK ao processo atual
	int remetente; //Processo para o qual o processo atual está "devendo" o ACK
	int origem_broadcast; //Processo origem do broadcast
	int delivered;
	// outras variáveis locais dos processos são declaradas aqui!
} TipoProcesso;

TipoProcesso* processo;

int floorLog2(int x)
{
	int r = 0;
	while (x)
	{
		x = x >> 1;
		r++;
	}
	return r - 1;
}

int ceilLog2(int x)
{
	if (x >= 2)
	{
		return 1 + floorLog2(x - 1);
	}
	else
	{
		return 0;
	}
}

int numero_clusters(int num_processos) {
	
	return ceilLog2(num_processos);
	
}
// Retorna o primeiro processo correto do cluster s do processo i
int FFneighbor(int i, int s)
{
	node_set* processos_cluster_s=cis(i,s);
	for (int y = 0; y < processos_cluster_s->size; y ++)
	{
		int yesimo_vizinho = processos_cluster_s->nodes[y];
		if (yesimo_vizinho<N && eh_par(processo[i].State[yesimo_vizinho]))
		{
			return yesimo_vizinho;
		}
	}
	return -1;	
}

//Retorna a qual cluster de j, o processo i pertence
int Cluster(int i, int j)
{
	for (int num_cluster = 1; num_cluster <= numero_clusters(N); num_cluster++)
	{
		node_set* processos_cluster_s=cis(j,num_cluster);
		for (int y=0; y<processos_cluster_s->size ;y++)
		{
			if (i == processos_cluster_s->nodes[y])
			{
				return num_cluster;
			}
		}
	}
}

int main(int argc, char* argv[]) {
	
	printf("=============================================================================\n"); 
	printf("Inicio da execucao: Best-Effort Broadcast sobre o VCube - Sistemas Distribuídos.\n");
	printf("Thayse Marques Solis\n");
	printf("Data da última modificação: 13 de março de 2021\n");
	printf("Trabalho 3 - Best-Effort Broadcast sobre o VCube\n");
	printf("=============================================================================\n"); 
	
	//static int N, // número de processos
	static int token, // indica o processo que está sendo executado
		   event, r, i;
	static char fa_name[5];
	float tempo_simulacao; // tempo de simulação
	int origem; // processo que inicia o broadcast
	float tempo_inicio; // tempo de inicio do broadcast
	
	
	if(argc!=2) {
		puts("Uso correto: BEBCVCube <nome do arquivo de entrada>");
		exit(-1);
	}
	
	FILE *entrada  = fopen(argv[1], "r"); // read only 
	
         
	// test for files not existing. 
	if (entrada == NULL) 
	{   
		printf("Erro! Não foi possível abrir o arquivo\n"); 
		exit(-1);
	} 
	
	fscanf(entrada, "nodos %d\n", &N);

	fscanf(entrada, "tempo %f\n", &tempo_simulacao);
	
	printf("\nO número de processos para a simulação é %d e o tempo de simulação é %4.1f\n", N, tempo_simulacao);	
	
    smpl(0,"Um Exemplo de Simulação"); // Inicia a simulação
	reset();
	stream(1); // Uma única thread de simulação
	
	
	// inicializar processos
	processo = (TipoProcesso*) malloc(sizeof(TipoProcesso)*N);
	
	printf("Número de clusters é: %d\n", numero_clusters(N));
	
	for (i=0;i<N;i++) {
		processo[i].State = (int*) malloc(sizeof(int)*N); //aloca espaço para o estado de cada processo que é guardado no vetor State
		//processo[i].cluster_atual=1;
		for (int j=0; j < N; j++) {
			if (i==j) {
				processo[i].State[j]=0; //No vetor de estados, o estado do próprio processo que armazena o vetor é 0, ou seja, ele considera que ele próprio começa correto
			}
			else{
				processo[i].State[j]=-1; // inicializa o estado de cada processo como "desconhecido" ou -1
			}
		}
		processo[i].PendingACK = EMPTY_INT_SET;
		processo[i].delivered = 0;
		processo[i].origem_broadcast = -1;
		processo[i].remetente = -1;
	}
	
	
	for (i=0;i<N;i++) {
		memset(fa_name, '\0', 5); // for (j=0;j<5;j++) fa_name[j] = '\0';
		sprintf(fa_name, "%d", i);
		processo[i].id = facility(fa_name, 1);
		//printf("Foi criada uma facility para o processo #%d. O nome dela é \"%s\" e o ID é %d.\n", i, fa_name, processo[i].id);
	}
		
	fscanf(entrada, "fonte %d %f\n", &origem, &tempo_inicio);
	schedule(bebcast, tempo_inicio, origem);
	
	while(-1)
	{
		char tipo_evento[50];
		int processo_evento;
		float tempo_evento;
		int x = fscanf(entrada, "%s %d %f\n", tipo_evento, &processo_evento, &tempo_evento);

		if (x<2)
		{
			break;
		}
		if (processo_evento<0 || tempo_evento <0)
		{
			if(processo_evento < 0)
			{
				processo_evento=randomic(0,N-1);
			}
			if(tempo_evento < 0)
			{
				tempo_evento=uniform(tempo_inicio,tempo_simulacao);
			}
			if (strcmp("falha", tipo_evento) == 0)
			{
				printf("Falha aleatória: será agendada uma falha para o processo %d no tempo %4.1f\n", processo_evento, tempo_evento);
			}
			else if (strcmp("recuperacao", tipo_evento) == 0)
			{
				printf("Recuperacao aleatória: será agendada uma recuperacao para o processo %d no tempo %4.1f\n", processo_evento, tempo_evento);
			}
		}
		
		if (strcmp("falha", tipo_evento) == 0)
		{
			schedule(fault, tempo_evento, processo_evento);
		}
		else if (strcmp("recuperacao", tipo_evento) == 0)
		{
			schedule(recovery, tempo_evento, processo_evento);
		}
		else
		{
			printf("Evento desconhecido: %s\n", tipo_evento);
		}
	}
	
	
	for (i = 0; i < N;i++) { // para todos os processos, escalona test para daqui a 10.0 unidades de tempo (i é o token)
		schedule (test, 10.0, i);
		//printf("No tempo %4.1f: AGENDAMENTO: foi agendado um teste para o processo %d no instante 30.0\n", time(), i);
	}

	// nossos processos vão executar testes em intervalos de testes
	// o intervalo de teste vai ser de 30 unidades de tempo
	// a simulação começa no tempo 0 (zero) e vamos escalonar o primeiro teste de todos os processos 
	//	para o tempo 30.0


	//variáveis das estatísticas dos eventos
	int conta_testes = 0; // número de testes
	int conta_bateria_testes = 0; //conta uma "execução" completa de um processo correto: achou um outro processo correto ou passou por todos os processos falhos
	int monitor_de_evento = no_event; // guarda o evento
	int processo_foco = 0; // numero do processo que falhou/recuperou
	double tempo_inicio_evento;
	int processos_com_falha	= 0; // contador de processos falhos
	
	printf("\nSimulação iniciada\n\n");
	
	//agora vem o loop principal do simulador
	while(time()<=tempo_simulacao) {
		cause(&event, &token);
		//o tempo só avança de verdade quando a funcao cause é chamada, entao é necessario verificar se realmente o evento deve ser executado ou se a simulacao deve ser interrompida
		if(time()>tempo_simulacao) {
			printf("\n\nSimulação finalizada\n");
			break;
		}
		int processo_atual = token;
		int indice_dentro_do_cluster;
		int proximo_processo = processo_atual; // se continuar com este valor, quer dizer que nao tem nenhum outro nodo ok.
			

		switch(event) {
			case test:
			//printf("#####O processo %d vai testar no cluster %d\n", processo_atual, processo[processo_atual].cluster_atual);
				// Se o processo atual estiver falho, ele não vai testar nenhum outro processo
				if (status(processo[processo_atual].id) != 0) { 
					//printf("Tempo %4.1f: TESTE: o processo %d está falho e portanto, não faz nada\n", time(), token);
					break; 
				}
				V(printf("\n\n"));
				for (int s = 1; s <=numero_clusters(N); s++)
				{
					//Quando o processo processo_atual vai testar:
					for (int y = 0; y < N; y ++) //Para cada processo "y"
					{
						node_set* testadores_de_y;
						testadores_de_y = cis(y, s);
						
						//Verifica se processo_atual está em testadores_de_y.
						for (i = 0; i < testadores_de_y->size; i++)
						{
							if (testadores_de_y->nodes[i]==processo_atual)
							{
								conta_testes++;
								//printf("#####O processo %d vai testar o processo %d\n", processo_atual, y);
								if (status(processo[y].id) != 0) { 
									V(printf("Tempo %4.1f: TESTE: o processo %d testou %d do cluster %d que está falho.\n", time(), processo_atual, y, s));
									// o processo processo_atual acabou de testar o processo y, agora ele vai atualizar o vetor State dele próprio (o processo_atual).
									if (processo[processo_atual].State[y]==-1)
									{
										processo[processo_atual].State[y]=1;
										schedule(failed,0.0, AGRUPAR_TOKEN2(processo_atual, y));
									}
									if (eh_par(processo[processo_atual].State[y]))
									{
										processo[processo_atual].State[y]++;
										schedule(failed,0.0, AGRUPAR_TOKEN2(processo_atual, y));
									}
								}
								else{
									V(printf("Tempo %4.1f: TESTE: o processo %d testou %d do cluster %d que está correto.\n", time(), processo_atual, y, s));
									if (!eh_par(processo[processo_atual].State[y]))
									{
										processo[processo_atual].State[y]++;
									}
									// Agora o processo processo_atual tem que aprender o estado global com o processo y
									for (int outro_processo = 0; outro_processo < N; outro_processo++) {
										if (outro_processo!=processo_atual)
										{
											if (processo[y].State[outro_processo]>processo[processo_atual].State[outro_processo]) {// Se o processo atual tem estados salvos mais antigos (menores) que o do vetor de estados do processo testado correto, ele aprende com o processo testado correto
												processo[processo_atual].State[outro_processo]=processo[y].State[outro_processo];
												V(printf("\tO processo %d aprendeu com o processo %d que o estado do processo %d é %d\n", processo_atual, y, outro_processo, processo[y].State[outro_processo]));
												if (!eh_par(processo[processo_atual].State[outro_processo]))
												{
													schedule(failed, 0.0, AGRUPAR_TOKEN2(processo_atual, outro_processo));
												}
												
											}
										}
									}
								}
								break;
							}
							else if (testadores_de_y->nodes[i] < N && eh_par(processo[processo_atual].State[testadores_de_y->nodes[i]])) 
							{
								//o processo atual tem um valor par (correto) no State para um processo que vem antes na lista de testadores de y, então ele não vai testar o processo y
								break;
							}
						}
						set_free(testadores_de_y);
					}
				}
				//processo[processo_atual].cluster_atual++;
				conta_bateria_testes++;
				//if (processo[processo_atual].cluster_atual > numero_clusters(N))
				//{
					//processo[processo_atual].cluster_atual=1;
				//}					
									
				// Agora vamos agendar o próximo teste
				schedule(test, tempo_entre_eventos_vcube, token);
				//printf("Tempo %4.1f: AGENDAMENTO: foi agendado um teste para o processo %d no tempo %4.1f\n", time(), token, time()+30);
				
				//Mostra o vetor de estados (State) do processo atual
				V(printf("\tO vetor State do processo %d é: [", processo_atual));
				for (i = 0; i < N; i++) {
					V(printf(" %d", processo[processo_atual].State[i]));
				}
				V(printf(" ]\n"));

				int contador_processos=0; //contador de processos que já identificaram o evento
				//verifica se todos os processos detectaram o evento de falha
				if (monitor_de_evento==fault){
					for (i=0;i<N;i++) {
						if (status(processo[i].id) == 0 && !eh_par(processo[i].State[processo_foco])) {
							contador_processos++;
						}
					}
					if (contador_processos == (N-processos_com_falha)) {
						V(printf("\n####################################################################"));
						V(printf("\n A falha no processo %d foi detectada por todos os outros processos \n após %d testes ou %d rodadas (%4.1f unidades de tempo), no tempo %4.1f\n", processo_foco, conta_testes, ceil_div(conta_bateria_testes,(N-processos_com_falha)),time()-tempo_inicio_evento, time()));
						V(printf("####################################################################\n"));
						monitor_de_evento = no_event;
						conta_testes = 0;
						tempo_inicio_evento = time();
					}
				}
		
				//verifica se todos os processos detectaram o evento de recuperação
				if (monitor_de_evento==recovery){
					for (i=0;i<N;i++) {
						if (status(processo[i].id) == 0 && eh_par(processo[i].State[processo_foco])) {
							contador_processos++;
						}
					}
					//printf("O valor do contador é: %d\n", contador);
					if (contador_processos == (N-processos_com_falha)) {
						V(printf("\n#########################################################################"));
						V(printf("\n A recuperação no processo %d foi detectada por todos os outros processos \n após %d testes ou %d rodadas (%4.1f unidades de tempo), no tempo %4.1f\n", processo_foco, conta_testes, ceil_div(conta_bateria_testes,(N-processos_com_falha)), time()-tempo_inicio_evento, time()));
						V(printf("#########################################################################\n"));
						monitor_de_evento = no_event;
						conta_testes = 0;
						tempo_inicio_evento = time();
					}
				}
		
				break;
			
			case fault:
				monitor_de_evento = fault; // o evento que ocorreu foi uma falha
				processo_foco = token; // número do processo que falhou
				conta_testes = 0; // zera o contador de testes
				conta_bateria_testes = 0; // zera o contador de "execuções" completas de cada processo correto
				tempo_inicio_evento = time(); // tempo de início do evento falha
				
				r = request(processo[token].id, token, 0); // request reserva a facility e faz status != 0
				if(r!=0) {
					V(printf("ERRO: Não foi possível falhar o processo."));
					exit(1);
				}
				printf("\nTempo %4.1f: FALHA: o processo %d falhou\n", time(), token);
				for (i=0; i < N; i++) { // Como a falha é crash, zera o vetor de estados do processo que falhou
					if (token==i) {
						processo[token].State[i]=0; //No vetor de estados, o estado do próprio processo que armazena o vetor é "correto" ou 0
					} else{
						processo[token].State[i]=-1; // inicializa o estado de cada processo como "desconhecido" ou -1
					}
				}
				int_set__clear(&(processo[processo_atual].PendingACK));
				//processo[processo_atual].cluster_atual=1; // Como a falha é crash, o cluster volta para 1
				processos_com_falha++;
				if (processos_com_falha == N) {
					V(printf("Tempo %4.1f:####TODOS OS PROCESSOS ESTÃO FALHOS####\n", time()));
				}
				break;
			
			case recovery:
				monitor_de_evento = recovery; // o evento que ocorreu foi uma recuperação
				processo_foco = token; // número do processo que recuperou
				conta_testes = 0; // zera o contador de testes
				conta_bateria_testes = 0; // zera o contador de "execuções" completas de cada processo correto
				tempo_inicio_evento = time(); // tempo de início do evento recuperação
				
				release(processo[token].id, token);
				V(printf("\n\nTempo %4.1f: RECUPERACAO: o processo %d recuperou\n", time(), token));
				schedule(test, tempo_entre_eventos_vcube, token);
				//printf("Tempo %4.1f: AGENDAMENTO: foi agendado um teste para o processo %d no tempo %4.1f\n", time(), token, time()+30);
				processos_com_falha--;
				break;
				
			case bebcast:
			{
				B(printf("\nTempo %4.1f: o broadcast iniciou!\n\n", time()));
				int processo_atual = token;
				int origem_broadcast = token;
				//Confere se o processo atual está correto
				if (status(processo[processo_atual].id) != 0) { 
					//printf("Tempo %4.1f: TESTE: o processo %d está falho e portanto, não faz nada\n", time(), token);
					break; 
				}
				//O processo que inicia o broadcast não precisa mandar ACK para ninguém
				processo[processo_atual].remetente=-1;
				processo[processo_atual].origem_broadcast=origem_broadcast;
				B(printf("Tempo %4.1f: processo %d vai enviar uma mensagem de broadcast:\n", time(), processo_atual));
				//Se ainda não tiver entregado a mensagem
				if (processo[processo_atual].delivered == 0)
				{
					B(printf("\tProcesso %d fez o deliver da mensagem\n", processo_atual));
					//Marca a mensagem atual como "delivered" (entregue)
					processo[processo_atual].delivered = 1;
				}
				//Para cada cluster 
				for (int s = 1; s <= numero_clusters(N); s ++)
				{
					//Determina-se qual o processo que deve receber a mensagem de bebcast
					int processo_destino = FFneighbor(processo_atual, s);
					//Se for -1, é porque não há processo no cluster em questão que deve receber a mensagem
					if (processo_destino != -1) { 
						B(printf("\tProcesso %d enviou a mensagem para %d (cluster %d).\n", processo_atual, processo_destino, s));
						//Agenda o recebimento da mensagem pelo processo destino
						schedule(rcvmsg, tempo_entre_eventos_bebcast, AGRUPAR_TOKEN3(origem_broadcast, processo_atual, processo_destino));
						//Adiciona o processo destino na lista de PendingACK do processo atual
						int_set__add(&(processo[processo_atual].PendingACK), processo_destino);
					}
				}
				//Imprime a lista de ACKs pendentes do processo atual
				B(printf("\tPending ACK[%d]: [ ", processo_atual));
				for (int i = 0; i < int_set__lenght(&(processo[processo_atual].PendingACK)); i++)
				{	
					B(printf("%d ", int_set__get(&(processo[processo_atual].PendingACK), i)));
				}
				B(printf("]\n"));
			}				
				break;
				
			case rcvmsg:
			{
				int processo_origem;
				int processo_atual;
				int origem_broadcast;
				//Separa o token em origem_broadcast, processo_origem da mensagem e processo_atual e atribui os devidos valores às variáveis correspondentes
				SEPARAR_TOKEN3(token, origem_broadcast, processo_origem, processo_atual);
				//Determina o cluster a que o processo atual pertence em relação ao processo de origem da mensagem
				int s_origem = Cluster(processo_origem, processo_atual);
				//Confere se o processo atual está correto
				if (status(processo[processo_atual].id) != 0) { 
					break; 
				}
				//Guarda o remetente do broadcast
				processo[processo_atual].remetente=processo_origem;
				//Guarda a origem do broadcast
				processo[processo_atual].origem_broadcast=origem_broadcast;
				B(printf("\nTempo %4.1f: processo %d recebeu uma mensagem do processo %d no cluster %d.\n", time(), processo_atual, processo_origem, s_origem));
				//Se ainda não tiver entregado a mensagem
				if (processo[processo_atual].delivered == 0)
				{
					B(printf("\tProcesso %d fez o deliver da mensagem\n", processo_atual));
					//Marca a mensagem atual como "delivered" (entregue)
					processo[processo_atual].delivered = 1;
				}
				if (eh_par(processo[processo_atual].State[processo_origem]) && eh_par(processo[processo_atual].State[origem_broadcast]))
				{
					//Para cada cluster cujo número seja menor que o cluster de origem - 1
					for (int s = s_origem-1; s >=1; s--)
					{
						//Determina-se qual o processo que deve receber a mensagem de bebcast
						int processo_destino=FFneighbor(processo_atual,s);
						////Se for -1, é porque não há processo no cluster em questão que deve receber a mensagem
						if (processo_destino != -1) { 
							B(printf("\tProcesso %d enviou a mensagem para %d (cluster %d).\n", processo_atual, processo_destino, s));
							//Agenda o recebimento da mensagem pelo processo destino
							schedule (rcvmsg, tempo_entre_eventos_bebcast, AGRUPAR_TOKEN3(origem_broadcast, processo_atual, processo_destino));
							//Adiciona o processo destino na lista de PendingACK do processo atual
							int_set__add(&(processo[processo_atual].PendingACK), processo_destino);
						}
					}
					//Imprime a lista de ACKs pendentes do processo atual
					B(printf("\tPending ACK[%d]: [ ", processo_atual));
					for (int i = 0; i < int_set__lenght(&(processo[processo_atual].PendingACK)); i++)
					{	
						B(printf("%d ", int_set__get(&(processo[processo_atual].PendingACK), i)));
					}
					B(printf("]\n"));
					//Verifica se o tamanho da lista de ACKs pendentes é igual a 0
					if (int_set__lenght(&(processo[processo_atual].PendingACK)) == 0)
					{
						//Manda a mensagem de ACK para o processo de quem recebeu a mensagem
						//Lembrando que o ACK faz o caminho inverso!
						B(printf("\tProcesso %d enviou o ACK para o processo %d.\n", processo_atual, processo_origem));
						schedule(rcvack, tempo_entre_eventos_bebcast, AGRUPAR_TOKEN2(processo_atual, processo_origem));
					}
				}
				else
				{
					if (!eh_par(processo[processo_atual].State[processo_origem]))
					{
						printf("\tMensagem não encaminhada pois o processo %d percebeu que seu remetente %d falhou.\n", processo_atual, processo_origem);
					}
					if (!eh_par(processo[processo_atual].State[origem_broadcast]))
					{
						printf("\tMensagem não encaminhada pois o processo %d percebeu que %d (origem do broadcast) falhou.\n", processo_atual, origem_broadcast);
					}
					//Imprime a lista de ACKs pendentes do processo atual
					B(printf("\tPending ACK[%d]: [ ", processo_atual));
					for (int i = 0; i < int_set__lenght(&(processo[processo_atual].PendingACK)); i++)
					{	
						B(printf("%d ", int_set__get(&(processo[processo_atual].PendingACK), i)));
					}
					B(printf("]\n"));
				}
				

			}	
				break;

			case rcvack:
			{
				int processo_origem;
				int processo_atual;
				//Separa o token em processo_origem da mensagem e processo_atual e atribui os devidos valores às variáveis correspondentes
				SEPARAR_TOKEN2(token, processo_origem, processo_atual);
				//Confere se o processo atual está correto
				if (status(processo[processo_atual].id) != 0) { 
					//printf("Tempo %4.1f: TESTE: o processo %d está falho e portanto, não faz nada\n", time(), token);
					break; 
				}
				B(printf("\nTempo %4.1f: processo %d recebeu ACK de %d\n", time(), processo_atual, processo_origem));
				//Remove o processo que mandou o ACK da lista de ACKs pendentes
				int_set__remove(&(processo[processo_atual].PendingACK), processo_origem);
				//Imprime a lista de ACKs pendentes do processo atual
				B(printf("\tPending ACK[%d]: [ ", processo_atual));
				for (int i = 0; i < int_set__lenght(&(processo[processo_atual].PendingACK)); i++)
				{	
					B(printf("%d ", int_set__get(&(processo[processo_atual].PendingACK), i)));
				}
				B(printf("]\n"));
				//Verifica se o tamanho da lista de ACKs pendentes é igual a 0
				if (int_set__lenght(&(processo[processo_atual].PendingACK)) == 0)
				{
					//Se o processo atual não for a origem do broadcast: remetente != -1)
					if (processo[processo_atual].remetente != -1) {
						B(printf("\tProcesso %d enviou o ACK para o processo %d.\n", processo_atual, processo[processo_atual].remetente));
						//Manda a mensagem de ACK para o processo de quem recebeu a mensagem
						schedule(rcvack, tempo_entre_eventos_bebcast, AGRUPAR_TOKEN2(processo_atual, processo[processo_atual].remetente));
						//Senão, é a origem, e recebeu todos os ACKs que estava esperando: termina o broadcast
						processo[processo_atual].remetente = -1;
					} 
					else if (processo[processo_atual].origem_broadcast == processo_atual)
					{
						B(printf("\nO broadcast terminou!"));
						processo[processo_atual].origem_broadcast = -1;
					}
				}
			}
				break;
			case failed:
			{
				int processo_atual;
				int processo_falho;
				//Separa o token em processo_atual e processo_falho e atribui os devidos valores às variáveis correspondentes
				SEPARAR_TOKEN2(token, processo_atual, processo_falho);
				//Se a lista de ACKs pendentes do processo atual não estiver vazia, este processo está participando do broadcast
				if (int_set__lenght(&(processo[processo_atual].PendingACK))>0)
				{
					//Se for o processo origem a falhar, limpa a lista de ACKs pendentes do processo atual
					if (processo_falho == processo[processo_atual].origem_broadcast)
					{
						int_set__clear(&(processo[processo_atual].PendingACK));
						B(printf("\nTempo %4.1f: processo %d percebeu que a origem do broadcast falhou.\n", time(), processo_atual));
						//Imprime a lista de ACKs pendentes do processo atual (que é vazia)
						B(printf("\tPending ACK[%d]: [ ", processo_atual));
						for (int i = 0; i < int_set__lenght(&(processo[processo_atual].PendingACK)); i++)
						{	
							B(printf("%d ", int_set__get(&(processo[processo_atual].PendingACK), i)));
						}
						B(printf("]\n"));
					}
					//Se for o processo remetente a falhar, limpa a lista de ACKs pendentes do processo atual
					else if (processo_falho == processo[processo_atual].remetente)
					{
						int_set__clear(&(processo[processo_atual].PendingACK));
						//O processo que inicia o broadcast não precisa mandar ACK para ninguém
						processo[processo_atual].remetente=-1;
						B(printf("\nTempo %4.1f: processo %d percebeu que seu remetente (processo %d) falhou.\n", time(), processo_atual, processo_falho));
						//Imprime a lista de ACKs pendentes do processo atual (que é vazia)
						B(printf("\tPending ACK[%d]: [ ", processo_atual));
						for (int i = 0; i < int_set__lenght(&(processo[processo_atual].PendingACK)); i++)
						{	
							B(printf("%d ", int_set__get(&(processo[processo_atual].PendingACK), i)));
						}
						B(printf("]\n"));
					}
				}
				//Se a lista de ACKS pendentes do processo atual contiver o processo que falhou:
				if (int_set__contains(&(processo[processo_atual].PendingACK), processo_falho))
				{
					B(printf("\nTempo %4.1f: processo %d percebeu que o processo %d falhou e estava esperando um ACK dele.\n", time(), processo_atual, processo_falho));
					//Remove o processo falho da lista de ACKs pendentes
					int_set__remove(&(processo[processo_atual].PendingACK), processo_falho);
					B(printf("\tProcesso %d removeu %d da sua lista de ACKs pendentes.\n", processo_atual, processo_falho));
					//Se for possível, reenvia a mensagem para outro processo no mesmo cluster do processo que falhou
					int processo_destino = FFneighbor(processo_atual, Cluster(processo_falho, processo_atual));
					//Se houver outro processo correto no mesmo cluster do processo falho
					if (processo_destino != -1) { 
						B(printf("\tProcesso %d enviou a mensagem para %d (cluster %d).\n", processo_atual, processo_destino, Cluster(processo_falho, processo_atual)));
						//Envia a mensagem para o outro processo
						schedule (rcvmsg, tempo_entre_eventos_bebcast, AGRUPAR_TOKEN3(processo[processo_atual].origem_broadcast, processo_atual, processo_destino));
						//Adiciona o novo processo que vai receber a mensagem na lista de ACKs pendentes
						int_set__add(&(processo[processo_atual].PendingACK), processo_destino);
					}
					else
					{
						//Não há outro processo correto no cluster em que o processo falhou
						B(printf("\tNão há outro processo correto no cluster %d do processo %d\n", Cluster(processo_falho, processo_atual), processo_atual));
						//Se não houver para quem enviar as mensagens e não estiver esperando nenhum ACK
						if (int_set__lenght(&(processo[processo_atual].PendingACK)) == 0)
						{
							//Se o processo atual não for a origem do broadcast: remetente != -1)
							if (processo[processo_atual].remetente != -1) {
								B(printf("\tProcesso %d enviou o ACK para o processo %d.\n", processo_atual, processo[processo_atual].remetente));
								//Manda a mensagem de ACK para o processo de quem recebeu a mensagem
								//Lembrando que o ACK faz o caminho inverso!
								schedule(rcvack, tempo_entre_eventos_bebcast, AGRUPAR_TOKEN2(processo_atual, processo[processo_atual].remetente));
								processo[processo_atual].remetente = -1;
							} 
							else if (processo[processo_atual].origem_broadcast == processo_atual)
							{
								//Senão, é a origem, e recebeu todos os ACKs que estava esperando: termina o broadcast
								B(printf("\nO broadcast terminou!"));
								processo[processo_atual].origem_broadcast = -1;
							}
						}
					}
					//Imprime a lista de ACKs pendentes do processo atual
					B(printf("\tPending ACK[%d]: [ ", processo_atual));
					for (int i = 0; i < int_set__lenght(&(processo[processo_atual].PendingACK)); i++)
					{	
						B(printf("%d ", int_set__get(&(processo[processo_atual].PendingACK), i)));
					}
					B(printf("]\n"));
				}

			}
				break;
			
			
			/*	printf("Testando a função Cluster\n");
				printf("\n Cluster(0,1) = %d\n", Cluster(0,1));
				printf("\n Cluster(0,2) = %d\n", Cluster(0,2));
				printf("\n Cluster(0,3) = %d\n", Cluster(0,3));
				printf("\n Cluster(0,4) = %d\n", Cluster(0,4));
				printf("\n Cluster(0,5) = %d\n", Cluster(0,5));
				printf("\n Cluster(0,6) = %d\n", Cluster(0,6));
				printf("\n Cluster(0,7) = %d\n", Cluster(0,7));
				printf("\n Cluster(1,0) = %d\n", Cluster(1,0));
				printf("\n Cluster(1,2) = %d\n", Cluster(1,2));
				printf("\n Cluster(1,3) = %d\n", Cluster(1,3));
				printf("\n Cluster(1,4) = %d\n", Cluster(1,4));
				printf("\n Cluster(1,5) = %d\n", Cluster(1,5));
				printf("\n Cluster(1,6) = %d\n", Cluster(1,6));
				printf("\n Cluster(1,7) = %d\n", Cluster(1,7));
				printf("\n Cluster(2,0) = %d\n", Cluster(2,0));
				printf("\n Cluster(2,1) = %d\n", Cluster(2,1));
				printf("\n Cluster(2,3) = %d\n", Cluster(2,3));
				printf("\n Cluster(2,4) = %d\n", Cluster(2,4));
				printf("\n Cluster(2,5) = %d\n", Cluster(2,5));
				printf("\n Cluster(2,6) = %d\n", Cluster(2,6));
				printf("\n Cluster(2,7) = %d\n", Cluster(2,7));
				printf("\n Cluster(3,0) = %d\n", Cluster(3,0));
				printf("\n Cluster(3,1) = %d\n", Cluster(3,1));
				printf("\n Cluster(3,2) = %d\n", Cluster(3,2));
				printf("\n Cluster(3,4) = %d\n", Cluster(3,4));
				printf("\n Cluster(3,5) = %d\n", Cluster(3,5));
				printf("\n Cluster(3,6) = %d\n", Cluster(3,6));
				printf("\n Cluster(3,7) = %d\n", Cluster(3,7));
				printf("\n Cluster(4,0) = %d\n", Cluster(4,0));
				printf("\n Cluster(4,1) = %d\n", Cluster(4,1));
				printf("\n Cluster(4,2) = %d\n", Cluster(4,2));
				printf("\n Cluster(4,3) = %d\n", Cluster(4,3));
				printf("\n Cluster(4,5) = %d\n", Cluster(4,5));
				printf("\n Cluster(4,6) = %d\n", Cluster(4,6));
				printf("\n Cluster(4,7) = %d\n", Cluster(4,7));
				printf("\n Cluster(5,0) = %d\n", Cluster(5,0));
				printf("\n Cluster(5,1) = %d\n", Cluster(5,1));
				printf("\n Cluster(5,2) = %d\n", Cluster(5,2));
				printf("\n Cluster(5,3) = %d\n", Cluster(5,3));
				printf("\n Cluster(5,4) = %d\n", Cluster(5,4));
				printf("\n Cluster(5,6) = %d\n", Cluster(5,6));
				printf("\n Cluster(5,7) = %d\n", Cluster(5,7));
				printf("\n Cluster(6,0) = %d\n", Cluster(6,0));
				printf("\n Cluster(6,1) = %d\n", Cluster(6,1));
				printf("\n Cluster(6,2) = %d\n", Cluster(6,2));
				printf("\n Cluster(6,3) = %d\n", Cluster(6,3));
				printf("\n Cluster(6,4) = %d\n", Cluster(6,4));
				printf("\n Cluster(6,5) = %d\n", Cluster(6,5));
				printf("\n Cluster(6,7) = %d\n", Cluster(6,7));
				printf("\n Cluster(7,0) = %d\n", Cluster(7,0));
				printf("\n Cluster(7,1) = %d\n", Cluster(7,1));
				printf("\n Cluster(7,2) = %d\n", Cluster(7,2));
				printf("\n Cluster(7,3) = %d\n", Cluster(7,3));
				printf("\n Cluster(7,4) = %d\n", Cluster(7,4));
				printf("\n Cluster(7,5) = %d\n", Cluster(7,5));
				printf("\n Cluster(7,6) = %d\n", Cluster(7,6));

					
				printf("Testando a função FFneighbor\n");
				printf("\n FFneighbor(0,1) = %d\n", FFneighbor(0,1));
				printf("\n FFneighbor(0,2) = %d\n", FFneighbor(0,2));
				printf("\n FFneighbor(0,3) = %d\n", FFneighbor(0,3));
				printf("\n FFneighbor(1,1) = %d\n", FFneighbor(1,1));
				printf("\n FFneighbor(1,2) = %d\n", FFneighbor(1,2));
				printf("\n FFneighbor(1,3) = %d\n", FFneighbor(1,3));
				printf("\n FFneighbor(2,1) = %d\n", FFneighbor(2,1));
				printf("\n FFneighbor(2,2) = %d\n", FFneighbor(2,2));
				printf("\n FFneighbor(2,3) = %d\n", FFneighbor(2,3));
				printf("\n FFneighbor(3,1) = %d\n", FFneighbor(3,1));
				printf("\n FFneighbor(3,2) = %d\n", FFneighbor(3,2));
				printf("\n FFneighbor(3,3) = %d\n", FFneighbor(3,3));
				printf("\n FFneighbor(4,1) = %d\n", FFneighbor(4,1));
				printf("\n FFneighbor(4,2) = %d\n", FFneighbor(4,2));
				printf("\n FFneighbor(4,3) = %d\n", FFneighbor(4,3));
				printf("\n FFneighbor(5,1) = %d\n", FFneighbor(5,1));
				printf("\n FFneighbor(5,2) = %d\n", FFneighbor(5,2));
				printf("\n FFneighbor(5,3) = %d\n", FFneighbor(5,3));
				printf("\n FFneighbor(6,1) = %d\n", FFneighbor(6,1));
				printf("\n FFneighbor(6,2) = %d\n", FFneighbor(6,2));
				printf("\n FFneighbor(6,3) = %d\n", FFneighbor(6,3));
				printf("\n FFneighbor(7,1) = %d\n", FFneighbor(7,1));
				printf("\n FFneighbor(7,2) = %d\n", FFneighbor(7,2));
				printf("\n FFneighbor(7,3) = %d\n", FFneighbor(7,3));*/
		} // switch
	} // while
	return 0;
}

