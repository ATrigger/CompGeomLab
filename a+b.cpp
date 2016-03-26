//
// Created by kamenev on 26.03.16.
//
#include <stddef.h>
#include <fstream>
#include <gmpxx.h>
#include "tests.h"

int main(){
    auto fin = new std::fstream("input.txt", std::fstream::in);
    auto fout = new std::fstream("output.txt", std::fstream::out | std::fstream::trunc);
    int num;
    fin->operator>>(num);
    auto input = genTest(num);
    mpz_class a = mpz_class(input.first);

    mpz_class b = mpz_class(input.second);
    a+=b;
    *fout << a.get_str(10) << std::endl;
    delete fout;
    delete fin;
}