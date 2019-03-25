#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <signal.h>
#include <sys/types.h>

#include "misemaforo.h"

#define atomic_xchg(A,B)	__asm__ __volatile__ (  \
                                    "lock xchg %1,%0 ;\n"  \
                                    : "=ir" (A)            \
                                    : "m" (B), "ir" (A)    \
                                    );

#define CICLOS 10

char *pais[3] = {"***Peru", "---Bolivia", "   Colombia"};

SEMAFORO *sem;

int *g, *h;

void proceso(int i);

int main(int argc, char const *argv[])
{
	int pid;
	int status;
	int i;
	int idsem;
	int idflg1, idflg2;

	//Solicitar memoria compartida
	idsem = shmget(IPC_PRIVATE, sizeof(SEMAFORO), IPC_CREAT | 0666);
	idflg1 = shmget(IPC_PRIVATE, sizeof(g), IPC_CREAT | 0666);
	idflg2 = shmget(IPC_PRIVATE, sizeof(h), IPC_CREAT | 0666);

	// Conectar la variable a la memoria compartida
	sem = (SEMAFORO*) shmat(idsem, NULL, 0);
	g = shmat(idflg1, NULL, 0);
	h = shmat(idflg2, NULL, 0);

	//Se usa el id como semilla
	srand(getpid());

	//Inicializa el semaforo
	initsem(sem, 1);
	*g = 0;
	*h = 0;

	for( i = 0 ; i < 3 ; i++ )
	{
		//Crea un nuevo proceso hijo que ejecuta la función proceso()
		pid = fork();
		if( pid == 0 )
			proceso(i);
	}

	for( i = 0 ; i < 3 ; i++ )
		pid = wait(&status);

	//Eliminar memoria compartida
	shmdt(g);
	shmdt(h);
	shmdt(sem);
	/* code */
	return 0;
}

void proceso(int i)
{
	int k;
	int l, m;

	for( k = 0 ; k < CICLOS ; k++)
	{
		//Llamada waitsem implementada en la parte 3
		l = m = 1;
		do { atomic_xchg(l, *g); } while (l != 0);
		waitsem(sem);

		//Sección crítica
		printf("%s Entra\n", pais[i]);
		fflush(stdout);
		sleep(rand()%3);
		printf("%s Sale\n", pais[i]);

		//Llamada waitsignal implementada en la parte 3
		*h = 0;
		do { atomic_xchg(m, *h); } while (l != 0);
		
		signalsem(sem);
		*g = 0;

		//Espera aleatoria fuera de la seccón cŕitica
		sleep(rand()%3);
		
		
	}


	exit(0);
}