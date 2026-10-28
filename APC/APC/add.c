#include "apc.h"

void addition(node *tail1, node *tail2, node **headR, node **tailR)
{
    int carry = 0;

    *headR = NULL;
    *tailR = NULL;

    while (tail1 != NULL || tail2 != NULL || carry != 0)
    {
        int sum = carry;

        if (tail1 != NULL)
        {
            sum += tail1->data;
            tail1 = tail1->prev;
        }
        if (tail2 != NULL)
        {
            sum += tail2->data;
            tail2 = tail2->prev;
        }

        if (insert_first(headR, tailR, sum % 10) == FAILURE)
        {
            delete_list(headR, tailR);
            fprintf(stderr, "Error: memory allocation failed during addition.\n");
            exit(EXIT_FAILURE);
        }
        carry = sum / 10;
    }

    if (*headR == NULL && insert_last(headR, tailR, 0) == FAILURE)
    {
        fprintf(stderr, "Error: memory allocation failed during addition.\n");
        exit(EXIT_FAILURE);
    }
    remove_pre_zeros(headR);
}
