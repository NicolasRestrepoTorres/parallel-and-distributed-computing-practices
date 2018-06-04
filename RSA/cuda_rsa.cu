#include <thrust/host_vector.h>
#include <thrust/device_vector.h>
#include <thrust/generate.h>
#include <thrust/reduce.h>
#include <thrust/functional.h>
#include<iostream>
#include<gmpxx.h>
#include<gmp.h>
#include<vector>
#include<string>
#include<cstdio>
#include <stdio.h>

using namespace std;

__global__ void gcd_vector(int * d_out){
  int idx = threadIdx.x;
  int u = idx, v = blockDim.x;
  while ( v != 0) {
    int r = u % v;
    u = v;
    v = r;
  }
  if(u == 1){
    d_out[idx]=1;
  }
}

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
  mpz_urandomm (k, state, n);
  bool abort = false;
  for(k; mpz_cmp(k, phi) <= 0; mpz_add_ui(k, k, 1)) {
      mpz_gcd(gcd, k, phi);
      if(mpz_cmp_ui(gcd, 1) == 0) return;
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

  for(; mpz_cmp(i, phi) <= 0; mpz_add_ui(i, i, 1)) {
    mpz_add_ui(i, i, 1);
    if(!(mpz_cmp(i, phi) <= 0)) abort = true;
    if(!abort){
      mpz_t j;
      mpz_init(j);
      mpz_set_str(j, "1", 10);

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
unsigned GCD(unsigned u, unsigned v) {
    while ( v != 0) {
        unsigned r = u % v;
        u = v;
        v = r;
    }
    return u;
}

void crack_phi(mpz_t cracked_phi, mpz_t m)
{

  mpz_t p;
  mpz_init(p);

  mpz_t gcd;
  mpz_init(gcd);

  int sum;
  int integer_m = mpz_get_si (m);

  int h_out[integer_m];
  int * d_out;

  cudaMalloc((void **) &d_out, integer_m*sizeof(int));


  //launch the kernel
  gcd_vector<<<1,integer_m>>>(d_out);

  // transfer data back to host
  cudaMemcpy(h_out, d_out, integer_m*sizeof(int), cudaMemcpyDeviceToHost);

  for(int i=0; i<integer_m; i++){
    sum += h_out[i];
  }
  mpz_set_si(cracked_phi, sum);

}

int main(int argc, char *argv[10])
{

  string gen("generate");
  string crack("crack");




int print;

  if (!argv[1]) {
    cout << "Uso: crack <public>, decrypt <public>, encrypt <private>, generate" << endl;
    return 0;
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
  } else if (argv[1] == crack) {

    if (!argv[4]) {
      cout << "Se necesita el número de hilos, abortando." << endl;
      return 0;
    }
    if (!argv[5]) {
      cout << "Se necesita especificar en el último argumento el nivel de impresión. (0/1)" << endl;
      return 0;
    }
    print =  atoi(argv[5]);

    int nProcessors = 8; //TODO
    int threads =  atoi(argv[4]);



    if(print == 1)cout << "Hilos a lanzar: " << threads << endl;


    if(print == 1)cout << endl << "Rompiendo la llave privada..." << endl;

    mpz_t k;
    mpz_init_set_str(k, argv[2], 10);

    mpz_t m;
    mpz_init_set_str(m, argv[3], 10);

    mpz_t cracked_phi;
    mpz_init(cracked_phi);

    mpz_t d;
    mpz_init(d);


    crack_phi(cracked_phi, m);

    set_d(d, cracked_phi, k);

    if(print == 1)cout << "Resultado: " << d << endl;
    else cout << d << endl;
  }

  return 0;
}
