// Autor: Thayse Marques Solis
// Data da última modificação: 09 de dezembro de 2020
// Descrição resumida: Trabalho 1 - VRing

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "smpl.h"

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
	// outras variáveis locais dos processos são declaradas aqui!
} TipoProcesso;

TipoProcesso* processo;

int main(int argc, char* argv[]) {
	
	printf("=============================================================================\n"); 
	printf("Inicio da execucao: VRing - Sistemas Distribuídos.\n");
	printf("Thayse Marques Solis\n");
	printf("Data da última modificação: 09 de dezembro de 2020\n");
	printf("Trabalho 1 - VRing\n");
	printf("=============================================================================\n"); 
	
	static int N, // número de processos
		   token, // indica o processo que está sendo executado
		   event, r, i;
	static char fa_name[5];
	double tempo_simulacao; // tempo de simulação
	
	if(argc!=3) {
		puts("Uso correto: VRing <número de processos> <tempo de simulação>");
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
	
	for (i=0;i<N;i++) {
		processo[i].State = (int*) malloc(sizeof(int)*N); //aloca espaço para o estado de cada processo que é guardado no vetor State
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
		printf("No tempo %4.1f: AGENDAMENTO: foi agendado um teste para o processo %d no instante 30.0\n", time(), i);
	}
	
	//Log1
	//Usar N = 3
	//Usar tempo de simulação > 151
	schedule(fault, 40.0, 1); // escalona uma falha no tempo 40 para o processo 1
	printf("No tempo %4.1f: AGENDAMENTO: foi agendada uma falha para o processo 1 no instante 40.0\n", time());
	schedule(fault, 89.0, 2); // escalona uma falha no tempo 89 para o processo 2
	printf("No tempo %4.1f: AGENDAMENTO: foi agendada uma falha para o processo 2 no instante 89.0\n", time());	
	schedule(recovery, 91.0, 2); // escalona uma recuperção no tempo 91 para o processo 2
	printf("No tempo %4.1f: AGENDAMENTO: foi agendada uma recuperação para o processo 2 no instante 91.0\n", time());
	schedule(recovery, 130.0, 1); // escalona uma recuperção no tempo 130 para o processo 1
	printf("No tempo %4.1f: AGENDAMENTO: foi agendada uma recuperação para o processo 1 no instante 130.0\n", time());

	
	//Log2
	//Usar N = 4
	//Usar tempo de simulação > 330
	/*schedule(fault, 31.0, 1); // escalona uma falha no tempo 31 para o processo 1
	printf("No tempo %4.1f: AGENDAMENTO: foi agendada uma falha para o processo 1 no instante 31.0\n", time());
	schedule(fault, 91.0, 2); // escalona uma falha no tempo 91 para o processo 2
	printf("No tempo %4.1f: AGENDAMENTO: foi agendada uma falha para o processo 2 no instante 91.0\n", time());
	schedule(fault, 121.0, 3); // escalona uma falha no tempo 121 para o processo 3
	printf("No tempo %4.1f: AGENDAMENTO: foi agendada uma falha para o processo 3 no instante 121.0\n", time());
	schedule(recovery, 181.0, 1); // escalona uma recuperação no tempo 181 para o processo 1
	printf("No tempo %4.1f: AGENDAMENTO: foi agendada uma recuperação para o processo 1 no instante 181.0\n", time());
	schedule(recovery, 241.0, 2); // escalona uma recuperação no tempo 241 para o processo 2
	printf("No tempo %4.1f: AGENDAMENTO: foi agendada uma recuperação para o processo 2 no instante 241.0\n", time());
	schedule(recovery, 301.0, 3); // escalona uma recuperação no tempo 301 para o processo 3
	printf("No tempo %4.1f: AGENDAMENTO: foi agendada uma recuperação para o processo 3 no instante 301.0\n", time());*/
	
	//Log3
	//Usar N = 6
	//Usar tempo de simulação > 511
	/*schedule(fault, 31.0, 1); // escalona uma falha no tempo 31 para o processo 1
	printf("No tempo %4.1f: AGENDAMENTO: foi agendada uma falha para o processo 1 no instante 31.0\n", time());
	schedule(fault, 151.0, 2); // escalona uma falha no tempo 151 para o processo 2
	printf("No tempo %4.1f: AGENDAMENTO: foi agendada uma falha para o processo 2 no instante 151.0\n", time());
	schedule(recovery, 241.0, 2); // escalona uma recuperação no tempo 241 para o processo 2
	printf("No tempo %4.1f: AGENDAMENTO: foi agendada uma recuperação para o processo 2 no instante 241.0\n", time());
	schedule(fault, 331.0, 4); // escalona uma falha no tempo 331 para o processo 4
	printf("No tempo %4.1f: AGENDAMENTO: foi agendada uma falha para o processo 4 no instante 331.0\n", time());
	schedule(recovery, 391.0, 4); // escalona uma recuperação no tempo 391 para o processo 4
	printf("No tempo %4.1f: AGENDAMENTO: foi agendada uma recuperação para o processo 4 no instante 391.0\n", time());
	schedule(recovery, 451.0, 1); // escalona uma recuperação no tempo 451 para o processo 1
	printf("No tempo %4.1f: AGENDAMENTO: foi agendada uma recuperação para o processo 1 no instante 451.0\n", time());*/
	
	
	


	//variáveis das estatísticas dos eventos
	int conta_testes = 0; // número de testes
	int conta_bateria_testes = 0; //conta uma "execução" completa de um processo correto: achou um outro processo correto ou passou por todos os processos falhos
	int monitor_de_evento = no_event; // guarda o evento
	int processo_foco; // numero do processo que falhou/recuperou
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
		int num_proximo_processo = (token+1)%N;

		switch(event) {
			case test:
				
				printf("\n\n");
				// Se o processo atual estiver falho, ele não vai testar o processo seguinte
				if (status(processo[num_processo_atual].id) != 0) { 
					printf("No tempo %4.1f: TESTE: o processo %d está falho e portanto, não faz nada\n", time(), token);
					break; 
				}
				// Agora o processo atual vai testar os processos seguintes até encontrar um correto.
				
				while (status(processo[num_proximo_processo].id) != 0) {
					conta_testes++;
					printf("No tempo %4.1f: TESTE: o processo %d testou o processo %d que está falho.\n", time(), num_processo_atual, num_proximo_processo);
					if (processo[num_processo_atual].State[num_proximo_processo]==-1) {//O processo testado está incorreto mas antes seu estado era desconhecido, então, atualiza seu valor em State para 1
						processo[num_processo_atual].State[num_proximo_processo]=1;
					}
					else if (eh_par(processo[num_processo_atual].State[num_proximo_processo])) {//O processo testado está incorreto, então atualiza seu valor em State para um número ímpar, somando 1
						processo[num_processo_atual].State[num_proximo_processo]++;
					}
					num_proximo_processo = (num_proximo_processo+1)%N;
				}
				if (num_proximo_processo==num_processo_atual) {
					conta_bateria_testes++;
					printf("No tempo %4.1f: TESTE: o único processo correto é o %d\n", time(), token);
				}
				else {
					conta_testes++;
					conta_bateria_testes++;
					printf("No tempo %4.1f: TESTE: o processo %d testou o processo %d que está correto.\n", time(), num_processo_atual, num_proximo_processo);
					if(!eh_par(processo[num_processo_atual].State[num_proximo_processo])) {//O processo testado está correto, então atualiza seu valor em State para um número par, somando 1
						processo[num_processo_atual].State[num_proximo_processo]++;
					}
					int limite;
					if (num_proximo_processo<num_processo_atual) {
						limite = num_processo_atual;
					} else {
						limite = num_processo_atual+N;
					}
					for (i=num_proximo_processo+1;i<limite;i++) {
						if (processo[num_proximo_processo].State[i%N]>processo[num_processo_atual].State[i%N]) {// Se o processo atual tem estados salvos mais antigos (menores) que o do vetor de estados do processo que ele está testando, ele os obtém do vetor de dados do processo que está testando
							processo[num_processo_atual].State[i%N]=processo[num_proximo_processo].State[i%N];
							printf("\tO processo %d aprendeu com o processo %d que o estado do processo %d é %d\n", num_processo_atual, num_proximo_processo, i%N, processo[num_proximo_processo].State[i%N]);
						}
					}
				}	
				
				// Agora vamos agendar o próximo teste
				schedule(test, 30.0, token);
				printf("No tempo %4.1f: AGENDAMENTO: foi agendado um teste para o processo %d no tempo %4.1f\n", time(), token, time()+30);
				
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
				printf("\n\nNo tempo %4.1f: FALHA: o processo %d falhou\n", time(), token);
				for (i=0; i < N; i++) { // Como a falha é crash, zera o vetor de estados do processo que falhou
					if (token==i) {
						processo[token].State[i]=0; //No vetor de estados, o estado do próprio processo que armazena o vetor é "correto" ou 0
					} else{
						processo[token].State[i]=-1; // inicializa o estado de cada processo como "desconhecido" ou -1
					}
				}
				processos_com_falha++;
				if (processos_com_falha == N) {
					printf("No tempo %4.1f:####TODOS OS PROCESSOS ESTÃO FALHOS####\n", time());
				}
				break;
			
			case recovery:
				monitor_de_evento = recovery; // o evento que ocorreu foi uma recuperação
				processo_foco = token; // número do processo que recuperou
				conta_testes = 0; // zera o contador de testes
				conta_bateria_testes = 0; // zera o contador de "execuções" completas de cada processo correto
				tempo_inicio_evento = time(); // tempo de início do evento recuperação
				
				release(processo[token].id, token);
				printf("\n\nNo tempo %4.1f: RECUPERACAO: o processo %d recuperou\n", time(), token);
				schedule(test, 30.0, token);
				printf("No tempo %4.1f: AGENDAMENTO: foi agendado um teste para o processo %d no tempo %4.1f\n", time(), token, time()+30);
				processos_com_falha--;
				break;
		} // switch
	} // while
	return 0;
}

