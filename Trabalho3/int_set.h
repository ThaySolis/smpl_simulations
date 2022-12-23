/*
MIT License

Copyright (c) 2021 Felipe Michels Fontoura

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#ifndef _INT_SET_DEFINED
#define _INT_SET_DEFINED

#include <stdlib.h>

// estrutura que representa um conjunto de inteiros.
// esta estrutura é opaca: nunca acesse os dados dela diretamente, apenas usando as funcoes abaixo.
typedef struct
{
    void* first;
} int_set;

// valor constante que repsenta um conjunto de inteiros vazio.
#define EMPTY_INT_SET (int_set){first: 0}

// este "define" habilita algumas checagens no codigo para alertar alguns erros de programacao.
#define SAFETY_CHECKS_ON

// adiciona um elemento no conjunto de inteiros.
// OBS: se o valor ja estiver no conjunto, esta funcao nao faz nada.
// set - o ponteiro para o conjunto
// value - o valor
int int_set__add(int_set* set, int value);

// remove um elemento do conjunto de inteiros.
// OBS: se o valor nao estiver no conjunto, esta funcao nao faz nada.
// set - o ponteiro para o conjunto
// value - o valor
int int_set__remove(int_set* set, int value);

// verifica se um valor esta no conjunto de inteiros.
// set - o ponteiro para o conjunto
// value - o valor
// retorna true (-1) caso o valor esteja, e false (0) caseo nao esteja.
int int_set__contains(int_set* set, int value);

// obtem o n-esimo elemento de um conjunto de inteiros.
// OBS: se o valor n for invalido, e SAFETY_CHECKS_ON estiver definido, sera exibida uma mensagem de erro!
// set - o ponteiro para o conjunto
// n - o indice do elemento
int int_set__get(int_set* set, int n);

// obtem o tamanho de um conjunto de inteiros.
// set - o ponteiro para o conjunto
// retorna o tamanho do conjunto.
int int_set__lenght(int_set* set);

// limpa o conjunto.
// retorna o numero de elementos removidos.
int int_set__clear(int_set* self);

#endif
