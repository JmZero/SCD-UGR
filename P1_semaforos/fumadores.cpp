// *****************************************************************************
//
// Prácticas de SCD. Práctica 1.
// Plantilla de código para el ejercicio de los fumadores
//
// *****************************************************************************

#include <iostream>
#include <cassert>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>      // incluye "time(....)"
#include <unistd.h>    // incluye "usleep(...)"
#include <stdlib.h>    // incluye "rand(...)" y "srand"

using namespace std ;

sem_t estanquero;
sem_t fumador[3];
// ---------------------------------------------------------------------
// introduce un retraso aleatorio de duración comprendida entre
// 'smin' y 'smax' (dados en segundos)

void retraso_aleatorio( const float smin, const float smax )
{
  static bool primera = true ;
  if ( primera )        // si es la primera vez:
  {  srand(time(NULL)); //   inicializar la semilla del generador
     primera = false ;  //   no repetir la inicialización
  }
  // calcular un número de segundos aleatorio, entre {\ttbf smin} y {\ttbf smax}
  const float tsec = smin+(smax-smin)*((float)random()/(float)RAND_MAX);
  // dormir la hebra (los segundos se pasan a microsegundos, multiplicándos por 1 millón)
  usleep( (useconds_t) (tsec*1000000.0)  );
}

// ----------------------------------------------------------------------------
// función que simula la acción de fumar, como un retardo aleatorio de la hebra.
// recibe como parámetro el numero de fumador
// el tiempo que tarda en fumar está entre dos y ocho décimas de segundo.

void fumar( int num_fumador )
{
   cout << "Fumador número " << num_fumador << ": comienza a fumar." << endl << flush ;
   retraso_aleatorio( 0.2, 0.8 );
   cout << "Fumador número " << num_fumador << ": termina de fumar." << endl << flush ;
}
	
unsigned long ingrediente_estanquero(){
	unsigned long ingrediente = rand() % 3;

	return ingrediente;
}

void * funcion_estanquero(void *)
{
	while(true){

		sem_wait(&estanquero);

		unsigned long ingrediente = ingrediente_estanquero();

		if (ingrediente == 0)
			cout << "El estanquero pone papel" << endl << flush;	

		else if (ingrediente == 1)

			cout << "El estanquero pone tabaco" << endl << flush;

		else
			cout << "El estanquero pone cerillas" << endl << flush;
		
		sem_post(&fumador[ingrediente]);
	}
}

void * funcion_fumador (void * num_fum_void)
{
	while(true){
		unsigned long num_fum = (unsigned long)num_fum_void;
		
		sem_wait(&fumador[num_fum]);
		sem_post(&estanquero);
		fumar(num_fum);
	}
}

// ----------------------------------------------------------------------------

int main()
{
   srand( time(NULL) ); // inicializa semilla aleatoria para selección aleatoria de fumador
	
	// Inicialización de los semáforos

	sem_init(&estanquero, 0, 1);
	for(unsigned long i = 0; i < 3; ++i)
		sem_init(&fumador[i], 0, 0);
	
	// Creación de hebras
	
	pthread_t id_estanquero;
   pthread_t id_fumador[3];
	
	pthread_create(&id_estanquero, NULL, funcion_estanquero, NULL);
	for(unsigned long i = 0; i < 3; ++i){
		void * puntero = (void *) i;
		pthread_create(&(id_fumador[i]), NULL, funcion_fumador, puntero);
	}

	// Unión de hebras

	pthread_join(id_estanquero, NULL);
	for(int i = 0; i < 3; ++i)
		pthread_join(id_fumador[i], NULL);
	
	// Destrucción de semáforos

	sem_destroy(&estanquero);
	for(int i = 0; i < 3; ++i)
		sem_destroy(&fumador[i]);

  return 0 ;
}
