
#include "apc.h"

static node *find_tail(node *head)
{
    if (head == NULL)
        return NULL;
    while (head->next != NULL)
        head = head->next;
    return head;
}

static int is_zero(node *head)
{
    if (head == NULL)
        return 1;

    while (head != NULL)
    {
        if (head->data != 0)
            return 0;
        head = head->next;
    }
    return 1;
}


static void subtract_magnitude_in_place(node **a_head, node *b_head)
{
    node *a_tail = find_tail(*a_head);
    node *b_tail = find_tail(b_head);
    int borrow = 0;

    while (a_tail != NULL)
    {
        int a = a_tail->data - borrow;
        int b = (b_tail != NULL) ? b_tail->data : 0;
        int digit = a - b;

        borrow = 0;
        if (digit < 0)
        {
            digit += 10;
            borrow = 1;
        }

        a_tail->data = digit;
        a_tail = a_tail->prev;
        if (b_tail != NULL)
            b_tail = b_tail->prev;
    }

    remove_pre_zeros(a_head);
}

int division(node *head1, node *head2,
             node **headQ, node **tailQ,
             node **headRem, node **tailRem)
{
    node *current;

    *headQ = NULL;
    *tailQ = NULL;
    *headRem = NULL;
    *tailRem = NULL;

    if (is_zero(head2))
    {
        fprintf(stderr, "Error: division by zero.\n");
        return FAILURE;
    }

    if (insert_last(headRem, tailRem, 0) == FAILURE)
    {
        fprintf(stderr, "Error: memory allocation failed during division.\n");
        return FAILURE;
    }

   
    for (current = head1; current != NULL; current = current->next)
    {
        int quotient_digit = 0;

        if (*headRem == *tailRem && (*headRem)->data == 0)
        {
            (*headRem)->data = current->data;
        }
        else if (insert_last(headRem, tailRem, current->data) == FAILURE)
        {
            fprintf(stderr, "Error: memory allocation failed during division.\n");
            delete_list(headQ, tailQ);
            delete_list(headRem, tailRem);
            return FAILURE;
        }

        remove_pre_zeros(headRem);

        
        while (compare_list(*headRem, head2) != OPERAND2)
        {
            subtract_magnitude_in_place(headRem, head2);
            quotient_digit++;
        }

        if (insert_last(headQ, tailQ, quotient_digit) == FAILURE)
        {
            fprintf(stderr, "Error: memory allocation failed during division.\n");
            delete_list(headQ, tailQ);
            delete_list(headRem, tailRem);
            return FAILURE;
        }
    }

    remove_pre_zeros(headQ);
    remove_pre_zeros(headRem);
    return SUCCESS;
}
