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

#include <stdio.h>
#include <stdlib.h>

#include "token_agrupada.h"

typedef struct {
    int id;
    void* value;
    void* next;
} composite_token_node;

static composite_token_node* _first_composite = NULL;
static int _composite_next_id = -1;

static int add_composite_token(void* value)
{
    int id = (_composite_next_id--);
    
    composite_token_node* node = (composite_token_node*) malloc(sizeof(composite_token_node));
    node->value = value;
    node->id = id;
    
    node->next = (void*) _first_composite;
    _first_composite = node;
    
    return id;
}

static void* remove_composite_token(int id)
{
    composite_token_node* prev = NULL;
    composite_token_node* cur = _first_composite;
    while (cur != NULL)
    {
        if (cur->id == id)
        {
            void* result = cur->value;
            
            if (prev != NULL)
            {
                prev->next = cur->next;
            }
            else
            {
                _first_composite = (composite_token_node*) cur->next;
            }
            
            free((void*) cur);
            
            return result;
        }
        
		prev = cur;
        cur = (composite_token_node*) cur->next;
    }
    
    return NULL;
}

int _create_token2(int x, int y)
{
    int* ptr = (int*) malloc(sizeof(int) * 2);

    ptr[0] = x;
    ptr[1] = y;

    return add_composite_token(ptr);
}

void _extract_token2(int* tkn, int* x, int* y)
{
    if (*tkn == 0)
    {
        fprintf(stderr, "\r\n## ERRO - uma token somente pode ser separada uma vez! ###\r\n");
    }
    
    int* ptr = remove_composite_token(*tkn);

    if (ptr == NULL)
    {
        fprintf(stderr, "\r\n## ERRO - esta token nao corresponde a um valor agrupado! ###\r\n");
    }

    *x = ptr[0];
    *y = ptr[1];

    free((void*) ptr);

    *tkn = 0;
}

int _create_token3(int x, int y, int z)
{
    int* ptr = (int*) malloc(sizeof(int) * 3);

    ptr[0] = x;
    ptr[1] = y;
    ptr[2] = z;

    return add_composite_token(ptr);
}

void _extract_token3(int* tkn, int* x, int* y, int* z)
{
    if (*tkn == 0)
    {
        fprintf(stderr, "\r\n## ERRO - uma token somente pode ser separada uma vez! ###\r\n");
    }
    
    int* ptr = remove_composite_token(*tkn);

    if (ptr == NULL)
    {
        fprintf(stderr, "\r\n## ERRO - esta token nao corresponde a um valor agrupado! ###\r\n");
    }

    *x = ptr[0];
    *y = ptr[1];
    *z = ptr[2];

    free((void*) ptr);

    *tkn = 0;
}

int _create_token4(int x, int y, int z, int k)
{
    int* ptr = (int*) malloc(sizeof(int) * 4);

    ptr[0] = x;
    ptr[1] = y;
    ptr[2] = z;
    ptr[3] = k;

    return add_composite_token(ptr);
}

void _extract_token4(int* tkn, int* x, int* y, int* z, int* k)
{
    if (*tkn == 0)
    {
        fprintf(stderr, "\r\n## ERRO - uma token somente pode ser separada uma vez! ###\r\n");
    }
    
    int* ptr = remove_composite_token(*tkn);

    if (ptr == NULL)
    {
        fprintf(stderr, "\r\n## ERRO - esta token nao corresponde a um valor agrupado! ###\r\n");
    }

    *x = ptr[0];
    *y = ptr[1];
    *z = ptr[2];
    *k = ptr[3];

    free((void*) ptr);

    *tkn = 0;
}

int _create_token5(int x, int y, int z, int k, int w)
{
    int* ptr = (int*) malloc(sizeof(int) * 4);

    ptr[0] = x;
    ptr[1] = y;
    ptr[2] = z;
    ptr[3] = k;
    ptr[4] = w;

    return add_composite_token(ptr);
}

void _extract_token5(int* tkn, int* x, int* y, int* z, int* k, int* w)
{
    if (*tkn == 0)
    {
        fprintf(stderr, "\r\n## ERRO - uma token somente pode ser separada uma vez! ###\r\n");
    }
    
    int* ptr = remove_composite_token(*tkn);

    if (ptr == NULL)
    {
        fprintf(stderr, "\r\n## ERRO - esta token nao corresponde a um valor agrupado! ###\r\n");
    }

    *x = ptr[0];
    *y = ptr[1];
    *z = ptr[2];
    *k = ptr[3];
    *w = ptr[4];

    free((void*) ptr);

    *tkn = 0;
}
