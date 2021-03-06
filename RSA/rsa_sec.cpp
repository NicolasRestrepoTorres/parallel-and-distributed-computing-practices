#include<iostream>
#include<gmpxx.h>
#include<gmp.h>
#include<vector>
#include<string>
#include<cstdio>
using namespace std;

void set_prime(mpz_t rop)
{
  mpz_set_str(rop, "17", 10);
  mpz_nextprime(rop, rop);
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
  //gmp_randseed_ui(state, seed);
  mpz_urandomm (k, state, n);

  for(k; mpz_cmp(k, phi) <= 0; mpz_add_ui(k, k, 1)) {
    mpz_gcd(gcd, k, phi);
    if(mpz_cmp_ui(gcd, 1) == 0) return;
  }
}

void set_d(mpz_t d, mpz_t phi, mpz_t k)
{
  mpz_t i;
  mpz_init(i);


  for(; mpz_cmp(i, phi) <= 0; mpz_add_ui(i, i, 1)) {
    mpz_t j;
    mpz_init(j);
    mpz_set_str(j, "1", 10);

    mpz_addmul(j, i, phi);

    mpz_t res;
    mpz_init(res);

    mpz_mod(res, j, k);

    if(mpz_cmp_ui(res, 0) == 0) {
      mpz_divexact(d, j, k);
      return;
    }
  }
}

void crack_phi(mpz_t cracked_phi, mpz_t m)
{
  mpz_t p;
  mpz_init(p);

  mpz_t gcd;
  mpz_init(gcd);

  for(mpz_set_ui(p, 1); mpz_cmp(p, m) < 0; mpz_add_ui(p, p, 1)) {
    mpz_gcd(gcd, p, m);

    if (mpz_cmp_ui(gcd, 1) == 0) {
      mpz_add_ui(cracked_phi, cracked_phi, 1);
    }
  }
}

int main(int argc, char *argv[10])
{
  string en("encrypt");
  string de("decrypt");
  string gen("generate");
  string crack("crack");

  int print;
  if (!argv[1]) {
    cout << "no action: crack <public>, decrypt <public>, encrypt <private>, generate" << endl;
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
    if(print == 1) cout << "Llave privada: " << d << " " << m << endl;
    else cout << d << endl;
  } else if (argv[1] == crack) {

    if (!argv[4]) {
      cout << "Se necesita especificar en el último argumento el nivel de impresión. (0/1)" << endl;
      return 0;
    }
    print =  atoi(argv[4]);
    if(print == 1)cout << endl << "Cracking d.." << endl;

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

    if(print == 1)cout << "Result: " << d << endl;
    else cout << d << endl;
  }

  return 0;
}
