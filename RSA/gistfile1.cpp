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
  mpz_t gcd;
  mpz_init(gcd);

  for(mpz_set_str(k, "832912", 10); mpz_cmp(k, phi) <= 0; mpz_add_ui(k, k, 1)) {
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

void text_to_numbers(string text, mpz_t *numbers)
{
  size_t i = 0;
  for(string::const_iterator it = text.begin(); it != text.end(); ++it, ++i) {
    mpz_t n;
    mpz_init(n);
    mpz_set_ui(n, (int) *it);

    mpz_set(numbers[i], n);
  }
}

void encrypt(mpz_t *numbers, mpz_t k, mpz_t m, int length)
{
  mpz_t res;
  mpz_init(res);

  for(size_t i = 0; i < length; ++i) {
    mpz_powm(res, numbers[i], k, m);
    mpz_set(numbers[i], res);
  }
}

void decrypt(mpz_t *numbers, mpz_t d, mpz_t m, int length)
{
  mpz_t res;
  mpz_init(res);

  for(size_t i = 0; i < length; ++i) {
    mpz_powm(res, numbers[i], d, m);
    mpz_set(numbers[i], res);
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

  if (!argv[1]) {
    cout << "no action: crack <public>, decrypt <public>, encrypt <private>, generate" << endl;
    return 0;
  }

  if (argv[1] == gen){
    mpz_t p;
    mpz_init(p);
    mpz_set_str(p, argv[2], 10);
    mpz_nextprime(p, p);
    cout << "P: " << p << endl;

    mpz_t q;
    mpz_init(q);
    mpz_nextprime(q, p);
    cout << "Q: " << q << endl << endl;

    mpz_t m;
    mpz_init(m);
    set_m(m, p, q);
    cout << "m: " << m << endl;

    mpz_t phi;
    mpz_init(phi);
    set_phi(phi, p, q);
    cout << "Ø(m): " << phi << endl << endl;

    mpz_t k;
    mpz_init(k);
    set_k(k, phi);
    cout << "Offentlig nøgle: " << k << " " << m << endl;

    mpz_t d;
    mpz_init(d);
    set_d(d, phi, k);
    cout << "Privat nøgle: " << d << " " << m << endl;
  } else if (argv[1] == en) {
    mpz_t k;
    mpz_init_set_str(k, argv[2], 10);

    mpz_t m;
    mpz_init_set_str(m, argv[3], 10);

    string text((std::istreambuf_iterator<char>(cin) ),
        (std::istreambuf_iterator<char>()));

    mpz_t length;
    mpz_init(length);
    mpz_set_ui(length, text.length());

    mpz_t numbers[100];
    mpz_array_init(numbers[0], 100, 512);
    text_to_numbers(text, numbers);
    encrypt(numbers, k, m, text.length());

    cout << endl << "Krypteret tekst:" << endl;
    for(size_t i = 0; i < text.length(); ++i)
      cout << numbers[i] << " ";
    
    cout << endl;

  } else if (argv[1] == de) {
    mpz_t numbers[100];
    mpz_array_init(numbers[0], 100, 512);

    string number;
    size_t i = 0;
    for(; cin >> number; ++i)
      mpz_set_str(numbers[i], number.c_str(), 10);

    mpz_t d;
    mpz_init_set_str(d, argv[2], 10);

    mpz_t m;
    mpz_init_set_str(m, argv[3], 10);

    cout << endl << endl << "Decrypted text:" << endl;
    decrypt(numbers, d, m, i);

    for(size_t j = 0; j < i; ++j)
      cout << (char)mpz_get_ui(numbers[j]);
    cout << endl << endl;

  } else if (argv[1] == crack) {
    cout << endl << "Cracking d.." << endl;

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

    cout << d << endl;
  }

  return 0;
}
