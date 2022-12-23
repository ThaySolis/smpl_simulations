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

#include <stdlib.h>
#include <stdio.h>

#include "int_set.h"

typedef struct
{
    int value;
    void* next;
#ifdef SAFETY_CHECKS_ON
    void* _glob_prev;
    void* _glob_next;
#endif
} int_set_node;


#ifdef SAFETY_CHECKS_ON
static int_set_node* glob_node_list = NULL;
#endif

static int_set_node* _create_node(int value)
{
    int_set_node* node;

    node = (int_set_node*) malloc(sizeof(int_set_node));
    node->value = value;
    node->next = NULL;

#ifdef SAFETY_CHECKS_ON
    node->_glob_prev = NULL;
    node->_glob_next = glob_node_list;
    if (glob_node_list != NULL)
    {
        glob_node_list->_glob_prev = node;
    }
    glob_node_list = node;
#endif

    return node;
}

static void _delete_node(int_set_node* node)
{
#ifdef SAFETY_CHECKS_ON
    if (node->_glob_prev != NULL)
    {
        int_set_node* prev = (int_set_node*) node->_glob_prev;
        prev->_glob_next = node->_glob_next;
    }
    else
    {
        glob_node_list = (int_set_node*) node->_glob_next;
    }
    if (node->_glob_next != NULL)
    {
        int_set_node* next = (int_set_node*) node->_glob_next;
        next->_glob_prev = node->_glob_prev;
    }
#endif

    free((void*) node);
}

#ifdef SAFETY_CHECKS_ON
static void _check_node(int_set_node* node)
{
    if (node != NULL)
    {
        int_set_node* cur = glob_node_list;
        while (cur != NULL)
        {
            if (cur == node)
            {
                return;
            }

            cur = cur->_glob_next;
        }

        fprintf(stderr, "\r\n## ERRO - voce esqueceu de inicializar um int_set com o valor EMPTY_INT_SET ###\r\n");

        exit(-1);
    }
}
#endif

int int_set__add(int_set* self, int value)
{
#ifdef SAFETY_CHECKS_ON
    _check_node((int_set_node*) self->first);
#endif

    int_set_node* prev = NULL;
    int_set_node* cur = (int_set_node*) (self->first);

    while (cur != NULL)
    {
        if (cur->value == value)
        {
            return 0;
        }
        prev = cur;
        cur = (int_set_node*) (cur->next);
    }

    cur = _create_node(value);

    if (prev != NULL)
    {
        prev->next = (void*) cur;
    }
    else
    {
        self->first = (void*) cur;
    }

    return -1;
}

int int_set__contains(int_set* self, int value)
{
#ifdef SAFETY_CHECKS_ON
    _check_node((int_set_node*) self->first);
#endif

    int_set_node* cur = (int_set_node*) (self->first);

    while (cur != NULL)
    {
        if (cur->value == value)
        {
            return -1;
        }
        cur = (int_set_node*) (cur->next);
    }

    return 0;
}

int int_set__remove(int_set* self, int value)
{
#ifdef SAFETY_CHECKS_ON
    _check_node((int_set_node*) self->first);
#endif

    int_set_node* prev = NULL;
    int_set_node* cur = (int_set_node*) (self->first);

    while (cur != NULL)
    {
        if (cur->value == value)
        {
            if (prev != NULL)
            {
                prev->next = cur->next;
            }
            else
            {
                self->first = cur->next;
            }

            _delete_node(cur);

            return -1;
        }

        prev = cur;
        cur = (int_set_node*) (cur->next);
    }

    return 0;
}

int int_set__get(int_set* self, int index)
{
#ifdef SAFETY_CHECKS_ON
    _check_node((int_set_node*) self->first);
#endif

    int_set_node* cur = (int_set_node*) (self->first);
    int cntdwn = index;
    while (cntdwn > 0 && cur != NULL)
    {
        cntdwn --;
        cur = (int_set_node*) (cur->next);
    }

#ifdef SAFETY_CHECKS_ON
    if (cntdwn != 0 || cur == NULL)
    {
        fprintf(stderr, "\r\n## ERRO - o indice %d esta fora do range ###\r\n", index);

        exit(-1);
    }
#endif

    return cur->value;
}

int int_set__lenght(int_set* self)
{
#ifdef SAFETY_CHECKS_ON
    _check_node((int_set_node*) self->first);
#endif

    int_set_node* cur = (int_set_node*) (self->first);
    int len = 0;
    while (cur != NULL)
    {
        len ++;
        cur = (int_set_node*) (cur->next);
    }

    return len;
}

int int_set__clear(int_set* self)
{
#ifdef SAFETY_CHECKS_ON
    _check_node((int_set_node*) self->first);
#endif

    int len = 0;

    int_set_node* cur = (int_set_node*) (self->first);
    while (cur != NULL)
    {
        int_set_node* next = (int_set_node*) (cur->next);

        len ++;

        _delete_node(cur);

        cur = next;
    }

    self->first = NULL;

    return len;
}
