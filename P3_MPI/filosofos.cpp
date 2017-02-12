#include <iostream>
#include <time.h>      // incluye "time"
#include <unistd.h>    // incluye "usleep"
#include <stdlib.h>    // incluye "rand" y "srand"
#include <mpi.h>

using namespace std;

void Filosofo( int id, int nprocesos);
void Tenedor ( int id, int nprocesos);

// ---------------------------------------------------------------------

int main( int argc, char** argv )
{
   int rank, size;
   
   srand(time(0));
   MPI_Init( &argc, &argv );
   MPI_Comm_rank( MPI_COMM_WORLD, &rank );
   MPI_Comm_size( MPI_COMM_WORLD, &size );
   
   if( size!=10)
   {
      if( rank == 0) 
         cout<<"El numero de procesos debe ser 10" << endl << flush ;
      MPI_Finalize( ); 
      return 0; 
   }
   
   if ((rank%2) == 0)  
      Filosofo(rank,size); // Los pares son Filosofos 
   else 
      Tenedor(rank,size);  // Los impares son Tenedores 
   
   MPI_Finalize( );
   return 0;
}  
// ---------------------------------------------------------------------

void Filosofo( int id, int nprocesos )
{
   int izq = (id+1) % nprocesos;
   int der = ((id+nprocesos)-1) % nprocesos;   
   
   while(1)
   {
      
      if(id == 0){
      	MPI_Ssend(&id, 1, MPI_INT, der, 0, MPI_COMM_WORLD);
      	cout << "Filosofo "<<id<< " solicita tenedor derecho " << der << endl << flush;
      	
      	MPI_Ssend(&id, 1, MPI_INT, izq, 0, MPI_COMM_WORLD);
      	cout <<"Filosofo "<<id<< " coge tenedor izquierdo " << izq << endl << flush;
      }

      else{
      	
      	MPI_Ssend(&id, 1, MPI_INT, izq, 0, MPI_COMM_WORLD);
      	cout << "Filosofo "<<id<< " solicita tenedor izquierdo " << izq << endl << flush;
      	
      	MPI_Ssend(&id, 1, MPI_INT, der, 0, MPI_COMM_WORLD);
      	cout <<"Filosofo "<<id<< " coge tenedor derecho " << der << endl << flush;
  		}

      cout<<"Filosofo "<<id<< " COMIENDO"<<endl<<flush;
      sleep((rand() % 3)+1);
      
      MPI_Ssend(&id, 1, MPI_INT, izq, 1, MPI_COMM_WORLD);
      cout <<"Filosofo "<<id<< " suelta tenedor izquierdo " << izq << endl << flush;
      
      MPI_Ssend(&id, 1, MPI_INT, der, 1, MPI_COMM_WORLD);
      cout <<"Filosofo "<<id<< " suelta tenedor derecho " << der << endl << flush;

      cout << "Filosofo " << id << " PENSANDO" << endl << flush;
      sleep((rand() % 3)+1);

      usleep( 1000U * (100U+(rand()%900U)) );
 }
}
// ---------------------------------------------------------------------

void Tenedor(int id, int nprocesos)
{
  int buf; 
  MPI_Status status; 
  int Filo;
  
  while( true )
  {
    MPI_Probe(MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);

    MPI_Recv(&buf, 1, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);
    Filo = status.MPI_SOURCE;

    cout << "Tenedor " << id << " recibe petición de " << Filo << endl << flush;
    
    MPI_Recv(&buf, 1, MPI_INT, Filo, 1, MPI_COMM_WORLD, &status);

    cout << "Tenedor " << id << " recibe liberación de " << Filo << endl << flush; 
  }
}
// ---------------------------------------------------------------------
