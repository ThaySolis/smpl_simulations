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

#ifndef _TOKEN_AGRUPADA_SMPL_DEFINED
#define _TOKEN_AGRUPADA_SMPL_DEFINED

#define AGRUPAR_TOKEN2(x, y)               _create_token2(x, y)
#define SEPARAR_TOKEN2(tkn, x, y)          _extract_token2(&(tkn), &(x), &(y))
#define AGRUPAR_TOKEN3(x, y, z)            _create_token3(x, y, z)
#define SEPARAR_TOKEN3(tkn, x, y, z)       _extract_token3(&(tkn), &(x), &(y), &(z))
#define AGRUPAR_TOKEN4(x, y, z, k)         _create_token4(x, y, z, k)
#define SEPARAR_TOKEN4(tkn, x, y, z, k)    _extract_token4(&(tkn), &(x), &(y), &(z), &(k))
#define AGRUPAR_TOKEN5(x, y, z, k, w)      _create_token5(x, y, z, k, w)
#define SEPARAR_TOKEN5(tkn, x, y, z, k, w) _extract_token5(&(tkn), &(x), &(y), &(z), &(k), &(w))

int _create_token2(int x, int y);
void _extract_token2(int* tkn, int* x, int* y);
int _create_token3(int x, int y, int z);
void _extract_token3(int* tkn, int* x, int* y, int* z);
int _create_token4(int x, int y, int z, int k);
void _extract_token4(int* tkn, int* x, int* y, int* z, int* k);
int _create_token5(int x, int y, int z, int k, int w);
void _extract_token5(int* tkn, int* x, int* y, int* z, int* k, int* w);

#endif
