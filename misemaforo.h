
//#ifndef _MISEMAFORO_H
//#define _MISEMAFORO_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <signal.h>

#define NPROC 4

typedef struct SEMAFORO
{
	int valor;       //Valor del semaforo
	int cola[NPROC]; //Cola de procesos
	int inicio;
	int final;
} SEMAFORO;

void initsem(SEMAFORO *sem, int i)
{
	//Inicializa el semaforo en 1
	sem->valor = 1;

	//Inicializa la cola de procesos
	sem->inicio = sem->final = 0;
	//sem->final = 0;


	printf("Semaforo inicializado\ninicio: %d\nfinal %d\n", sem->inicio, sem->final);

}

void waitsem(SEMAFORO *sem)
{
	sem->valor--;

	if (sem->valor < 0)
	{
		//Pone el proceso en s.cola_de_bloqueados
		sem->cola[sem->final] = getpid();
		sem->final = (sem->final + 1) % NPROC;

		//Bloquea el proceso;
		kill(getpid(), SIGSTOP);
		
	}
}

void signalsem(SEMAFORO *sem)
{
	sem->valor++;

	if (sem->valor <= 0)
	{
		int tempproc = sem->cola[sem->inicio];
		sem->inicio = (sem->inicio + 1) % NPROC;

		kill(tempproc, SIGCONT);
	}
	//Reanuda el proceso
	//kill(pid, SIGCONT);
}
