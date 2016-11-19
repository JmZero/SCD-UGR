// *********************************************************************
// SCD. Ejemplos del seminario 1.
//
// Plantilla para el ejercicio de cálculo de PI
// *********************************************************************

#include <iostream>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include "fun_tiempos.h"

using namespace std ;

// ---------------------------------------------------------------------
unsigned long m = 999;
unsigned long n = 4;

double resultado_parcial[4];
// ---------------------------------------------------------------------
// implementa función $f$

double f( double x )       
{ 
   return 4.0/(1+x*x) ;     // $~~~~f(x)\,=\,4/(1+x^2)$
}
// ---------------------------------------------------------------------
// cálculo secuencial

double calcular_integral_secuencial(){
   double suma = 0.0 ;                      // inicializar suma
   for( unsigned long i = 0 ; i < m ; i++ ) // para cada $i$ entre $0$ y $m-1$
      suma += f( (i+0.5)/m );               // $~~~~~$ añadir $f(x_i)$ a la suma actual
   return suma/m ;                          // devolver valor promedio de $f$
}
// ---------------------------------------------------------------------
// función que ejecuta cada hebra

void * funcion_hebra( void * ih_void ) 
{  
   unsigned long ih = (unsigned long) ih_void ; // número o índice de esta hebra
   double sumap = 0 ;
   
   // calcular suma parcial en "sumap"
   for( unsigned long i = ih ; i < m; i+=n)
     sumap += f( (i+0.5)/m );
   
   resultado_parcial[ih] = sumap/m ; // guardar suma parcial en vector.
   return NULL ;
}
// ---------------------------------------------------------------------
// cálculo concurrente

double calcular_integral_concurrente()
{  
	pthread_t id_hebra[n];
	double res = 0.0;
	for (unsigned long i = 0; i < n; i++){
		void* arg_ptr = (void*) i;
		pthread_create(&(id_hebra[i]), NULL, funcion_hebra, arg_ptr);
	}

	for( unsigned i = 0 ; i < n; i++){
		pthread_join( id_hebra[i], NULL);
	}

	for (int i = 0; i < n; i++)
		res += resultado_parcial[i];
 
   return res ;
}

// ---------------------------------------------------------------------

int main(int argc, char* argv[]){
	
	n = atoi(argv[1]);
	m = atoi(argv[2]);

   cout << "Ejemplo 4 (cálculo de PI)" << endl ;
   double pi_sec = 0.0, pi_conc = 0.0 ;
   
	struct timespec inicio=ahora();
   	pi_sec  = calcular_integral_secuencial() ;
	struct timespec fin=ahora();

	struct timespec inicioc=ahora();
   	pi_conc = calcular_integral_concurrente();
	struct timespec finc=ahora();
   
   cout << "valor de PI (calculado secuencialmente)  == " << pi_sec  << endl << "Duración: " << duracion(&inicio, &fin) << " segundos" << endl 
        << "valor de PI (calculado concurrentemente) == " << pi_conc << endl << "Duración: " << duracion(&inicioc, &finc) << " segundos" << endl; 
    
   return 0 ;
}
// ---------------------------------------------------------------------



// ----------------------------------------------------------------------------
