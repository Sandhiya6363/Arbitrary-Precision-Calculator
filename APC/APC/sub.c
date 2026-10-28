#include "apc.h"

static node *find_head(node *tail)
{
    if (tail == NULL)
        return NULL;
    while (tail->prev != NULL)
        tail = tail->prev;
    return tail;
}

void subtraction(node *tail1, node *tail2, node **headR, node **tailR)
{
    node *head1 = find_head(tail1);
    node *head2 = find_head(tail2);
    int borrow = 0;

    *headR = NULL;
    *tailR = NULL;

    if (compare_list(head1, head2) == SAME)
    {
        if (insert_last(headR, tailR, 0) == FAILURE)
        {
            fprintf(stderr, "Error: memory allocation failed during subtraction.\n");
            exit(EXIT_FAILURE);
        }
        return;
    }

    
    if (compare_list(head1, head2) == OPERAND2)
    {
        node *tmp_tail = tail1;
        tail1 = tail2;
        tail2 = tmp_tail;
    }

    while (tail1 != NULL)
    {
        int a = tail1->data - borrow;
        int b = (tail2 != NULL) ? tail2->data : 0;
        int digit = a - b;

        borrow = 0;
        if (digit < 0)
        {
            digit += 10;
            borrow = 1;
        }

        if (insert_first(headR, tailR, digit) == FAILURE)
        {
            delete_list(headR, tailR);
            fprintf(stderr, "Error: memory allocation failed during subtraction.\n");
            exit(EXIT_FAILURE);
        }

        tail1 = tail1->prev;
        if (tail2 != NULL)
            tail2 = tail2->prev;
    }

    remove_pre_zeros(headR);
}
