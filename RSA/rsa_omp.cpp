#include<iostream>
#include<gmpxx.h>
#include<gmp.h>
#include<vector>
#include<string>
#include<cstdio>
#include <omp.h>
using namespace std;

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

//  bool abort = false;
  int x;
  int integer_m = mpz_get_si (m);

  //omp_set_num_threads();
  int sum = 0;
  //
  #pragma omp parallel for reduction(+: sum)
  for(x = 1; x < integer_m; x++) {
  //for(mpz_set_ui(p, 1); mpz_cmp(p, m) < 0; mpz_add_ui(p, p, 1)) {
    //if(x == 1) mpz_set_ui(p, 1);
    //else mpz_add_ui(p, p, 1);

    mpz_set_si(p, x);
    int result = GCD(x, integer_m);
    //if(x !=  mpz_get_si (p)) cout << "x " << x << " p " << p << endl;
    //if (!(mpz_cmp(p, m) < 0)) abort = true;
    //if(!abort) {
    mpz_gcd(gcd, p, m);

      //if (mpz_cmp_ui(gcd, 1) == 0) {
      if(result == 1){
        //cout << "Cracked phi: " << cracked_phi  << endl;
        //#pragma omp critical
        sum++;
        //mpz_add_ui(cracked_phi, cracked_phi, 1);
      }
    //}
  }
  //cout << "Coprimes " << cracked_phi << endl;
  //cout << "Coprimes " << sum << endl;



  // std::string out_string;
  // std::stringstream ss;
  // ss << sum;
  // out_string = ss.str();


  //mpz_set_str(cracked_phi, out_string, 10);
  mpz_set_si(cracked_phi, sum);
  //cout << "Coprimes " << cracked_phi << endl;

  // for(mpz_set_ui(p, 1); mpz_cmp(p, m) < 0; mpz_add_ui(p, p, 1)) {
  //   mpz_gcd(gcd, p, m);
  //
  //   if (mpz_cmp_ui(gcd, 1) == 0) {
  //     mpz_add_ui(cracked_phi, cracked_phi, 1);
  //   }
  // }
}

int main(int argc, char *argv[10])
{

  string gen("generate");
  string crack("crack");




int print;


//   cout << "Threads " << omp_get_num_threads() << endl;
//    #pragma omp parallel for
//      for(int r = 0; r < 10; r++){
//        cout << "Iteration/Thread " << r << "/" << omp_get_thread_num() << endl;
//        cout << "Threads !!!" << omp_get_num_threads() << endl;
//      }
// cout << "Threads " << omp_get_num_threads() << endl;
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

    int nProcessors = omp_get_max_threads();
    int threads =  atoi(argv[4]);


    omp_set_num_threads(nProcessors);
    // cout << "Hilos a lanzar: " << nProcessors << endl;
    // omp_set_num_threads(nProcessors);
    if(print == 1)cout << "Hilos a lanzar: " << threads << endl;
    omp_set_num_threads(threads);




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
