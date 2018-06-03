// piN.c
// D. Thiebaut
// Computes Pi using N processes under MPI
//
// To compile and run:
// mpicc -o piN piN.cpp
// time mpirun -np 2 ./piN 100000000
//
// Output
// Process 1 of 2 started on beowulf2.  N= 50000000
// Process 0 of 2 started on beowulf2.  N= 50000000
//  50000000 iterations: Pi = 3.14159
//
//  real0m1.251s
//  user0m1.240s
//  sys0m0.000s
//


#include<iostream>
#include<gmpxx.h>
#include<gmp.h>
#include<vector>
#include<string>
#include<cstdio>
#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>

using namespace std;
#define MANAGER 0


//--------------------------------------------------------------------
//                         P R O T O T Y P E S
//--------------------------------------------------------------------
void doManager( int, int, int, int, mpz_t, mpz_t, mpz_t );
void doWorker( );

void set_m(mpz_t m, mpz_t p, mpz_t q)
{
  mpz_mul(m, p, q);
}

void set_phi(mpz_t phi, mpz_t p, mpz_t q)
{
  mpz_t first;
  mpz_init(first);
  mpz_sub_ui(first, p, 1);

  mpz_t second;
  mpz_init(second);
  mpz_sub_ui(second, q, 1);

  mpz_mul(phi, first, second);
}

void set_k(mpz_t k, mpz_t phi)
{
  unsigned long seed;
  mpz_t gcd;
  mpz_init(gcd);
  mpz_t n;
  mpz_init(n);
  mpz_set_str(n, "1000000", 10);
  gmp_randstate_t state;
  gmp_randinit_default (state);
  //gmp_randseed_ui(state, seed);
  mpz_urandomm (k, state, n);
  bool abort = false;
//  #pragma omp for
  for(k; mpz_cmp(k, phi) <= 0; mpz_add_ui(k, k, 1)) {
  //  if(!abort){
      mpz_gcd(gcd, k, phi);
      if(mpz_cmp_ui(gcd, 1) == 0) return;
    //}
  }
}

void set_d(mpz_t d, mpz_t phi, mpz_t k)
{
  mpz_t i;
  mpz_init(i);


  bool abort = false;
  int integer_phi = mpz_get_si (phi);
  int integer_i = mpz_get_si (i);
  int x;
  int dummy_variable;

//  #pragma omp parallel for

  //for(integer_i=integer_i;integer_i<integer_phi;integer_i++) {
  for(; mpz_cmp(i, phi) <= 0; mpz_add_ui(i, i, 1)) {
    mpz_add_ui(i, i, 1);
    //mpz_set_si(i, integer_i);
    if(!(mpz_cmp(i, phi) <= 0)) abort = true;
    //cout << "Thread: " << omp_get_thread_num() << endl;
    if(!abort){
      mpz_t j;
      mpz_init(j);
      mpz_set_str(j, "1", 10);

      // j = j+i*phi
      mpz_addmul(j, i, phi);

      mpz_t res;
      mpz_init(res);

      mpz_mod(res, j, k);

      if(mpz_cmp_ui(res, 0) == 0) {

        mpz_divexact(d, j, k);

        abort = true;
        return;
      }
    }
  }
  return;
}

//--------------------------------------------------------------------
//                           M  A  I  N
//--------------------------------------------------------------------
int main(int argc, char *argv[]) {
  int myId, noProcs, nameLen, print;
  char procName[MPI_MAX_PROCESSOR_NAME];
  int n;

  string gen("generate");
  string crack("crack");

  if ( argc<4 ) {
    printf( "Syntax: mpirun -np noProcs piN (generate g |crack k m) \n" );
    return 1;
  }

  if (argv[1] == gen){

    if (!argv[3]) {
      cout << "Se necesita especificar en el último argumento el nivel de impresión. (0/1)" << endl;
      return 0;
    }
    print =  atoi(argv[3]);

    mpz_t p;
    mpz_init(p);
    mpz_set_str(p, argv[2], 10);
    mpz_nextprime(p, p);
    if(print == 1)cout << "P: " << p << endl;

    mpz_t q;
    mpz_init(q);
    mpz_nextprime(q, p);
    if(print == 1)cout << "Q: " << q << endl << endl;

    mpz_t m;
    mpz_init(m);
    set_m(m, p, q);
    if(print == 1)cout << "m: " << m << endl;

    mpz_t phi;
    mpz_init(phi);
    set_phi(phi, p, q);
    if(print == 1)cout << "Ø(m): " << phi << endl << endl;

    mpz_t k;
    mpz_init(k);
    set_k(k, phi);
    if(print == 1)cout << "Llave pública: " << k << " " << m << endl;
    else cout  << k << " " << m << " ";
    mpz_t d;
    mpz_init(d);
    set_d(d, phi, k);
    if(print == 1)cout << "Llave privada: " << d << " " << m << endl;
    else cout << d << endl;
  }else{
  if (!argv[4]) {
    cout << "Se necesita especificar en el último argumento el nivel de impresión. (0/1)" << endl;
    return 0;
  }
  print =  atoi(argv[4]);
  mpz_t k;
  mpz_init_set_str(k, argv[2], 10);

  mpz_t m;
  mpz_init_set_str(m, argv[3], 10);

    int integer_m = atoi(argv[3]);

    mpz_t cracked_phi;
    mpz_init(cracked_phi);

    mpz_t d;
    mpz_init(d);

  //--- start MPI ---
  MPI_Init( &argc, &argv);
  MPI_Comm_rank( MPI_COMM_WORLD, &myId );
  MPI_Comm_size( MPI_COMM_WORLD, &noProcs );
  MPI_Get_processor_name( procName, &nameLen );

  //--- display which process we are, and how many there are ---
  if(print == 1)printf( "Process %d of %d started on %s. m = %d\n",
          myId,  noProcs, procName, integer_m );

  //--- farm out the work: 1 manager, several workers ---
  if ( myId == MANAGER )
    doManager( n, noProcs, print, integer_m, k, cracked_phi, d);
  else
    doWorker( );

  //--- close up MPI ---
  MPI_Finalize();

}
  return 0;
}

//--------------------------------------------------------------------
// The function to be evaluated
//--------------------------------------------------------------------
unsigned GCD(unsigned u, unsigned v) {
    while ( v != 0) {
        unsigned r = u % v;
        u = v;
        v = r;
    }
    return u;
}

int crack_phi(int begin, int end, int m)
{

  int x;

  int sum = 0;

  for(x = begin; x < end; x++) {

    int result = GCD(x, m);

      if(result == 1){
        sum++;
      }
  }
  return sum;

}

//--------------------------------------------------------------------
// The manager's main work function.  Note that the function
// can and should be made more efficient (and faster) by sending
// an array of 3 ints rather than 3 separate ints to each worker.
// However the current method is explicit and better highlights the
// communication pattern between Manager and Workers.
//--------------------------------------------------------------------
void doManager( int n, int noProcs, int print, int integer_m , mpz_t k, mpz_t cracked_phi, mpz_t d) {


  if(print == 1)cout << endl << "Rompiendo la llave privada..." << endl;




  mpz_t m;
  mpz_init(m);
  mpz_set_si(m, integer_m);



  double sum0 = 0, sum1;
  int i, begin, end;

  MPI_Status status;

  //--- first send n and bounds of series to all workers ---
  end = integer_m/noProcs;
  for ( i=1; i<noProcs; i++ ) {
    begin = end;
    end   = (i+1) * integer_m / noProcs;
    //cout << "begin/end " << begin << " "  << end << " " << noProcs << endl;


    MPI_Send( &begin, 1, MPI_INT, i /*node i*/, 0, MPI_COMM_WORLD );
    MPI_Send( &end, 1, MPI_INT, i /*node i*/, 0, MPI_COMM_WORLD );
    MPI_Send( &integer_m, 1, MPI_INT, i /*node i*/, 0, MPI_COMM_WORLD );
  }

  //--- perform summation over 1st interval of the series ---
  begin = 0;
  end   = integer_m/noProcs;

  //cout << "begin/end " << begin << " "  << end << " " << noProcs << endl;
  sum0 += crack_phi( begin, end, integer_m  );

  //--- wait for other half from worker ---
  for ( i=1; i<noProcs; i++ ) {
    MPI_Recv( &sum1, 1, MPI_DOUBLE, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status );
    sum0 += sum1;
  }
  int total_sum = (int) sum0;
  //cout << "Coprimes :" << total_sum << endl;
  mpz_set_si(cracked_phi, total_sum);

  set_d(d, cracked_phi, k);

  if(print == 1)cout << "Resultado: " << d << endl;
  else cout << d << endl;

  //--- output result ---

}

//--------------------------------------------------------------------
// The worker's main work function.  Same comment as for the
// Manager.  The 3 ints would benefit from being sent in an array.
//--------------------------------------------------------------------
void doWorker( ) {
  int begin, end, m, i;

  //--- get n and bounds for summation from manager ---
  MPI_Status status;
  MPI_Recv( &begin, 1, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status );
  MPI_Recv( &end, 1, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status );
  MPI_Recv( &m, 1, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status );

  //--- sum over boundaries received ---
  double sum = 0;


  sum += crack_phi( begin, end, m  );

  //-- send result to manager ---
  MPI_Send( &sum, 1, MPI_DOUBLE, MANAGER, 0, MPI_COMM_WORLD );
}
