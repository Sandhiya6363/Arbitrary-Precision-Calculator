#include "apc.h"

static int list_is_zero(node *head)
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

static int get_sign(const char *number)
{
    return number[0] == '-' ? -1 : 1;
}

int main(int argc, char *argv[])
{
    node *head1 = NULL, *tail1 = NULL;
    node *head2 = NULL, *tail2 = NULL;
    node *headR = NULL, *tailR = NULL;
    node *headRem = NULL, *tailRem = NULL;
    int sign1;
    int sign2;
    int result_sign = 1;
    char oper;

    if (cla_validation(argc, argv) == FAILURE)
        return EXIT_FAILURE;

    sign1 = get_sign(argv[1]);
    sign2 = get_sign(argv[3]);

    create_list(argv[1], &head1, &tail1);
    create_list(argv[3], &head2, &tail2);
    remove_pre_zeros(&head1);
    remove_pre_zeros(&head2);

    /* Normalize negative zero and positive zero to ordinary zero. */
    if (list_is_zero(head1))
        sign1 = 1;
    if (list_is_zero(head2))
        sign2 = 1;

    oper = argv[2][0];
    if (oper == '/' && list_is_zero(head2))
    {
        fprintf(stderr, "Error: division by zero.\n");
        delete_list(&head1, &tail1);
        delete_list(&head2, &tail2);
        return EXIT_FAILURE;
    }

    switch (oper)
    {
        case '+':
        case '-':
        {
            int effective_sign2 = (oper == '-') ? -sign2 : sign2;

            if (sign1 == effective_sign2)
            {
                addition(tail1, tail2, &headR, &tailR);
                result_sign = sign1;
            }
            else
            {
                int cmp = compare_list(head1, head2);

                if (cmp == SAME)
                {
                    if (insert_last(&headR, &tailR, 0) == FAILURE)
                    {
                        fprintf(stderr, "Error: memory allocation failed.\n");
                        delete_list(&head1, &tail1);
                        delete_list(&head2, &tail2);
                        return EXIT_FAILURE;
                    }
                    result_sign = 1;
                }
                else if (cmp == OPERAND1)
                {
                    subtraction(tail1, tail2, &headR, &tailR);
                    result_sign = sign1;
                }
                else
                {
                    subtraction(tail2, tail1, &headR, &tailR);
                    result_sign = effective_sign2;
                }
            }
            break;
        }

        case '*':
        case 'x':
        case 'X':
            multiplication(tail1, tail2, &headR, &tailR);
            result_sign = sign1 * sign2;
            break;

        case '/':
            if (division(head1, head2, &headR, &tailR,
                         &headRem, &tailRem) == FAILURE)
            {
                delete_list(&head1, &tail1);
                delete_list(&head2, &tail2);
                delete_list(&headR, &tailR);
                delete_list(&headRem, &tailRem);
                return EXIT_FAILURE;
            }

            /* Quotient truncates toward zero; remainder keeps dividend sign. */
            result_sign = sign1 * sign2;
            if (list_is_zero(headR))
                result_sign = 1;

            printf("Quotient: ");
            if (result_sign < 0)
                putchar('-');
            print_list(headR);
            putchar('\n');

            printf("Remainder: ");
            if (sign1 < 0 && !list_is_zero(headRem))
                putchar('-');
            print_list(headRem);
            putchar('\n');

            delete_list(&head1, &tail1);
            delete_list(&head2, &tail2);
            delete_list(&headR, &tailR);
            delete_list(&headRem, &tailRem);
            return EXIT_SUCCESS;

        default:
            /* Operator validation prevents reaching this branch. */
            fprintf(stderr, "Error: invalid operator.\n");
            delete_list(&head1, &tail1);
            delete_list(&head2, &tail2);
            return EXIT_FAILURE;
    }

    if (list_is_zero(headR))
        result_sign = 1;

    printf("Result: ");
    if (result_sign < 0)
        putchar('-');
    print_list(headR);
    putchar('\n');

    delete_list(&head1, &tail1);
    delete_list(&head2, &tail2);
    delete_list(&headR, &tailR);
    return EXIT_SUCCESS;
}
