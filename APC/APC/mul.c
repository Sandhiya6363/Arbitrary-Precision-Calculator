#include "apc.h"


static int is_single_zero(node *tail)
{
    return tail != NULL && tail->prev == NULL && tail->data == 0;
}

void multiplication(node *tail1, node *tail2, node **headR, node **tailR)
{
    int len1 = 0;
    int len2 = 0;
    node *p;
    node *row_start;

    *headR = NULL;
    *tailR = NULL;

    if (tail1 == NULL || tail2 == NULL || is_single_zero(tail1) || is_single_zero(tail2))
    {
        if (insert_last(headR, tailR, 0) == FAILURE)
        {
            fprintf(stderr, "Error: memory allocation failed during multiplication.\n");
            exit(EXIT_FAILURE);
        }
        return;
    }

    for (p = tail1; p != NULL; p = p->prev)
        len1++;
    for (p = tail2; p != NULL; p = p->prev)
        len2++;


    for (int i = 0; i < len1 + len2; i++)
    {
        if (insert_last(headR, tailR, 0) == FAILURE)
        {
            delete_list(headR, tailR);
            fprintf(stderr, "Error: memory allocation failed during multiplication.\n");
            exit(EXIT_FAILURE);
        }
    }

  
    row_start = *tailR;
    for (node *a = tail1; a != NULL; a = a->prev)
    {
        int carry = 0;
        node *slot = row_start;

        for (node *b = tail2; b != NULL; b = b->prev)
        {
            int product = slot->data + a->data * b->data + carry;
            slot->data = product % 10;
            carry = product / 10;
            slot = slot->prev;
        }

   
        if (slot != NULL)
            slot->data += carry;

        row_start = row_start->prev;
    }

    remove_pre_zeros(headR);
}
