// Autor: Thayse Marques Solis
// Data da última modificação: 29 de janeiro de 2021
// Descrição resumida: Trabalho 2 - VCube

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "smpl.h"
#include "cisj.h"

// Vamos definir os EVENTOS
#define test 1
#define fault 2
#define recovery 3
#define no_event 0


#define eh_par(x) ((x)%2==0)
#define ceil_div(x,y) (int)ceil(((double)(x))/(y))

// vamos definir o descritor do processo

typedef struct {
	int id;		// identificador de facility (recurso) do SMPL
	int* State;
	int cluster_atual;
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

int main(int argc, char* argv[]) {
	
	printf("=============================================================================\n"); 
	printf("Inicio da execucao: VCube v.1 - Sistemas Distribuídos.\n");
	printf("Thayse Marques Solis\n");
	printf("Data da última modificação: 29 de janeiro de 2021\n");
	printf("Trabalho 2 - VCube v.1\n");
	printf("=============================================================================\n"); 
	
	static int N, // número de processos
		   token, // indica o processo que está sendo executado
		   event, r, i;
	static char fa_name[5];
	double tempo_simulacao; // tempo de simulação
	
	if(argc!=3) {
		puts("Uso correto: VCubeV1 <número de processos> <tempo de simulação>");
		exit(1);
	}
	
	N = atoi(argv[1]);	
	tempo_simulacao = atof(argv[2]);
	
	printf("\nO número de processos para a simulação é %d e o tempo de simulação é %4.1f\n", N, tempo_simulacao);	
	
    smpl(0,"Um Exemplo de Simulação"); // Inicia a simulação
	reset();
	stream(1); // Uma única thread de simulação
	printf("\nSimulação iniciada\n\n");
	
	// inicializar processos
	
	processo = (TipoProcesso*) malloc(sizeof(TipoProcesso)*N);
	
	printf("Número de clusters é: %d\n\n\n", numero_clusters(N));
	
	for (i=0;i<N;i++) {
		processo[i].State = (int*) malloc(sizeof(int)*N); //aloca espaço para o estado de cada processo que é guardado no vetor State
		processo[i].cluster_atual=1; //inicia o cluster atual de cada processo como 1 (primeiro cluster)
		for (int j=0; j < N; j++) {
			if (i==j) {
				processo[i].State[j]=0; //No vetor de estados, o estado do próprio processo que armazena o vetor é 0, ou seja, ele considera que ele próprio começa correto
			}
			else{
				processo[i].State[j]=-1; // inicializa o estado de cada processo como "desconhecido" ou -1
			}
		}
	}
	
	
	for (i=0;i<N;i++) {
		memset(fa_name, '\0', 5); // for (j=0;j<5;j++) fa_name[j] = '\0';
		sprintf(fa_name, "%d", i);
		processo[i].id = facility(fa_name, 1);
		//printf("Foi criada uma facility para o processo #%d. O nome dela é \"%s\" e o ID é %d.\n", i, fa_name, processo[i].id);
	}
	
	//vamos fazer o escalonamento inicial de eventos
	
	// nossos processos vão executar testes em intervalos de testes
	// o intervalo de teste vai ser de 30 unidades de tempo
	// a simulação começa no tempo 0 (zero) e vamos escalonar o primeiro teste de todos os processos 
	//	para o tempo 30.0
	
	for (i = 0; i < N;i++) { // para todos os processos, escalona test para daqui a 30.0 unidades de tempo (i é o token)
		schedule (test, 30.0, i);
		//printf("No tempo %4.1f: AGENDAMENTO: foi agendado um teste para o processo %d no instante 30.0\n", time(), i);
	}
	
	//Log1
	//Usar N = 6
	//Usar tempo de simulação > 640
	schedule(fault, 31.0, 1); // escalona uma falha no tempo 31 para o processo 1
	schedule(fault, 185.0, 2); // escalona uma falha no tempo 185 para o processo 2
	schedule(recovery, 271.0, 2); // escalona uma recuperação no tempo 271 para o processo 2
	schedule(fault, 370.0, 4); // escalona uma falha no tempo 370 para o processo 4
	schedule(recovery, 460.0, 4); // escalona uma recuperação no tempo 460 para o processo 4
	schedule(recovery, 550.0, 1); // escalona uma recuperação no tempo 550 para o processo 1
	
	
	//Log2
	//Usar N = 32
	//Usar tempo de simulação > 1050
	/*schedule(fault, 31.0, 1); // escalona uma falha no tempo 31 para o processo 1
	schedule(fault, 301.0, 2); // escalona uma falha no tempo 301 para o processo 2
	schedule(recovery, 451.0, 2); // escalona uma recuperação no tempo 451 para o processo 2
	schedule(fault, 601.0, 4); // escalona uma falha no tempo 601 para o processo 4
	schedule(recovery, 751.0, 4); // escalona uma recuperação no tempo 751 para o processo 4
	schedule(recovery, 901.0, 1); // escalona uma recuperação no tempo 901 para o processo 1*/
	
	
	//Log3
	//Usar N = 32
	//Usar tempo de simulação > 1050
	/*for (i = 0; i < 32; i += 3)
	{
		schedule(fault, 150.0, i); // escalona uma falha no tempo 150 para o processo i
		printf("No tempo %4.1f: AGENDAMENTO: foi agendada uma falha para o processo %i no instante 150.0\n", time());
	}*/
	
	
	//variáveis das estatísticas dos eventos
	int conta_testes = 0; // número de testes
	int conta_bateria_testes = 0; //conta uma "execução" completa de um processo correto: achou um outro processo correto ou passou por todos os processos falhos
	int monitor_de_evento = no_event; // guarda o evento
	int processo_foco = 0; // numero do processo que falhou/recuperou
	double tempo_inicio_evento;
	int processos_com_falha	= 0; // contador de processos falhos
	
	//agora vem o loop principal do simulador
	while(time()<=tempo_simulacao) {
		cause(&event, &token);
		//o tempo só avança de verdade quando a funcao cause é chamada, entao é necessario verificar se realmente o evento deve ser executado ou se a simulacao deve ser interrompida
		if(time()>tempo_simulacao) {
			printf("\nSimulação finalizada\n");
			break;
		}
		int num_processo_atual = token;
		int indice_dentro_do_cluster;
		node_set* processos_cluster_atual = cis(num_processo_atual, processo[num_processo_atual].cluster_atual);
		int num_processo_testado_correto = num_processo_atual; // se continuar com este valor, quer dizer que nao tem nenhum outro processo correto no cluster atual
		switch(event) {
			case test:
			
				
				// Se o processo atual estiver falho, ele não vai testar nenhum outro processo
				if (status(processo[num_processo_atual].id) != 0) { 
					//printf("Tempo %4.1f: TESTE: o processo %d está falho e portanto, não faz nada\n", time(), token);
					break; 
				}
				printf("\n\n");
				/*printf("O proceso %d vai testar seu cluster %d que tem os seguintes processos: [ ", num_processo_atual, processo[num_processo_atual].cluster_atual);	
				for (i = 0; i < processos_cluster_atual->size; i++) {
					if (processos_cluster_atual->nodes[i] < N) {
						printf("%d ", processos_cluster_atual->nodes[i]);
					}
				}*/
				//printf("]\n");
				
				//Itera pelos processos do cluster atual
				for (indice_dentro_do_cluster = 0; indice_dentro_do_cluster < processos_cluster_atual->size; indice_dentro_do_cluster++) {
					int num_processo_em_teste = processos_cluster_atual->nodes[indice_dentro_do_cluster];
					if (num_processo_em_teste < N) {
						conta_testes++;
						if (status(processo[num_processo_em_teste].id) == 0) {
							//salva o numero do processo
							num_processo_testado_correto = num_processo_em_teste;
							break;
						}
						else {
							printf("Tempo %4.1f: TESTE: o processo %d testou o %d falho.\n", time(), num_processo_atual, num_processo_em_teste);
							if (processo[num_processo_atual].State[num_processo_em_teste]==-1) {//O processo testado está incorreto mas antes seu estado era desconhecido, então, atualiza seu valor em State para 1
								processo[num_processo_atual].State[num_processo_em_teste]=1;
							}
							else if (eh_par(processo[num_processo_atual].State[num_processo_em_teste])) {//O processo testado está incorreto, então atualiza seu valor em State para um número ímpar, somando 1
								processo[num_processo_atual].State[num_processo_em_teste]++;
							}
						}
					} 
					else {
						//o processo #num_processo_em_teste estaria no cluster se ele existisse.
					}
				}
						
				if (num_processo_testado_correto==num_processo_atual){
					printf("Tempo %4.1f: TESTE: o processo %d não encontrou outro processo correto no cluster %d\n", time(), token, processo[num_processo_atual].cluster_atual);
				}
				else {
					printf("Tempo %4.1f: TESTE: o processo %d testou o %d correto.\n", time(), num_processo_atual, num_processo_testado_correto);
					if(!eh_par(processo[num_processo_atual].State[num_processo_testado_correto])) {//O processo testado está correto, então atualiza seu valor em State para um número par, somando 1
						processo[num_processo_atual].State[num_processo_testado_correto]++;
					}
					// Agora o processo atual vai aprender com o num_processo_testado_correto o estado dos processos restantes no cluster atual.
					for (int j = indice_dentro_do_cluster + 1; j < processos_cluster_atual->size; j ++) {
						int num_outro_processo_no_cluster = processos_cluster_atual->nodes[j]; // i é o j-ésimo processo do cluster atual.
						if (num_outro_processo_no_cluster < N)
						{
							if (processo[num_processo_testado_correto].State[num_outro_processo_no_cluster]>processo[num_processo_atual].State[num_outro_processo_no_cluster]) {// Se o processo atual tem estados salvos mais antigos (menores) que o do vetor de estados do processo testado correto, ele aprende com o processo testado correto
								processo[num_processo_atual].State[num_outro_processo_no_cluster]=processo[num_processo_testado_correto].State[num_outro_processo_no_cluster];
								printf("\tO processo %d aprendeu com o %d que o estado do processo %d é %d\n", num_processo_atual, num_processo_testado_correto, num_outro_processo_no_cluster, processo[num_processo_testado_correto].State[num_outro_processo_no_cluster]);
							}
						}
					}
				}
				
				processo[num_processo_atual].cluster_atual++;
				conta_bateria_testes++;
				if (processo[num_processo_atual].cluster_atual > numero_clusters(N))
				{
					processo[num_processo_atual].cluster_atual=1;
				}
				
				// Agora vamos agendar o próximo teste
				schedule(test, 30.0, token);
				//printf("Tempo %4.1f: AGENDAMENTO: foi agendado um teste para o processo %d no tempo %4.1f\n", time(), token, time()+30);
				
				//Mostra o vetor de estados (State) do processo atual

				printf("\tO vetor State do processo %d é: [", num_processo_atual);
				for (i = 0; i < N; i++) {
					printf(" %d", processo[num_processo_atual].State[i]);
				}
				printf(" ]\n");

				int contador_processos=0; //contador de processos que já identificaram o evento
				//verifica se todos os processos detectaram o evento de falha
				if (monitor_de_evento==fault){
					for (i=0;i<N;i++) {
						if (status(processo[i].id) == 0 && !eh_par(processo[i].State[processo_foco])) {
							contador_processos++;
						}
					}
					if (contador_processos == (N-processos_com_falha)) {
						printf("\n####################################################################");
						printf("\n A falha no processo %d foi detectada por todos os outros processos \n após %d testes ou %d rodadas (%4.1f unidades de tempo), no tempo %4.1f\n", processo_foco, conta_testes, ceil_div(conta_bateria_testes,(N-processos_com_falha)),time()-tempo_inicio_evento, time());
						printf("####################################################################\n");
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
						printf("\n#########################################################################");
						printf("\n A recuperação no processo %d foi detectada por todos os outros processos \n após %d testes ou %d rodadas (%4.1f unidades de tempo), no tempo %4.1f\n", processo_foco, conta_testes, ceil_div(conta_bateria_testes,(N-processos_com_falha)), time()-tempo_inicio_evento, time());
						printf("#########################################################################\n");
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
					puts("ERRO: Não foi possível falhar o processo.");
					exit(1);
				}
				printf("\n\nTempo %4.1f: FALHA: o processo %d falhou\n", time(), token);
				for (i=0; i < N; i++) { // Como a falha é crash, zera o vetor de estados do processo que falhou
					if (token==i) {
						processo[token].State[i]=0; //No vetor de estados, o estado do próprio processo que armazena o vetor é "correto" ou 0
					} else{
						processo[token].State[i]=-1; // inicializa o estado de cada processo como "desconhecido" ou -1
					}
				}
				processo[num_processo_atual].cluster_atual=1; // Como a falha é crash, o cluster volta para 1
				processos_com_falha++;
				if (processos_com_falha == N) {
					printf("Tempo %4.1f:####TODOS OS PROCESSOS ESTÃO FALHOS####\n", time());
				}
				break;
			
			case recovery:
				monitor_de_evento = recovery; // o evento que ocorreu foi uma recuperação
				processo_foco = token; // número do processo que recuperou
				conta_testes = 0; // zera o contador de testes
				conta_bateria_testes = 0; // zera o contador de "execuções" completas de cada processo correto
				tempo_inicio_evento = time(); // tempo de início do evento recuperação
				
				release(processo[token].id, token);
				printf("\n\nTempo %4.1f: RECUPERACAO: o processo %d recuperou\n", time(), token);
				schedule(test, 30.0, token);
				//printf("Tempo %4.1f: AGENDAMENTO: foi agendado um teste para o processo %d no tempo %4.1f\n", time(), token, time()+30);
				processos_com_falha--;
				break;
		} // switch
		//set_free(processos_cluster_atual);
	} // while
	return 0;
}

