#include "apc.h"

static int valid_signed_digits(const char *s)
{
    size_t i = 0;

    if (s == NULL || s[0] == '\0')
        return FAILURE;

    if (s[i] == '+' || s[i] == '-')
        i++;

    /* A sign by itself is not a number. */
    if (s[i] == '\0')
        return FAILURE;

    for (; s[i] != '\0'; i++)
    {
        if (s[i] < '0' || s[i] > '9')
            return FAILURE;
    }

    return SUCCESS;
}

int cla_validation(int argc, char *argv[])
{
    const char *op;

    if (argc != 4)
    {
        fprintf(stderr,
                "Usage: %s <integer1> <operator> <integer2>\n"
                "Operators: +  -  *  x  X  /\n",
                (argc > 0 && argv[0] != NULL) ? argv[0] : "./apc");
        return FAILURE;
    }

    if (!valid_signed_digits(argv[1]) || !valid_signed_digits(argv[3]))
    {
        fprintf(stderr,
                "Error: operands must be integers with optional leading + or - signs.\n");
        return FAILURE;
    }

    op = argv[2];
    if (op[0] == '\0' || op[1] != '\0' ||
        (op[0] != '+' && op[0] != '-' && op[0] != '*' &&
         op[0] != 'x' && op[0] != 'X' && op[0] != '/'))
    {
        fprintf(stderr, "Error: invalid operator. Use +, -, *, x, X, or /.\n");
        return FAILURE;
    }

    return SUCCESS;
}

int insert_first(node **head, node **tail, int data)
{
    node *new_node = malloc(sizeof(*new_node));

    if (new_node == NULL)
        return FAILURE;

    new_node->data = data;
    new_node->prev = NULL;
    new_node->next = *head;

    if (*head != NULL)
        (*head)->prev = new_node;
    else
        *tail = new_node;

    *head = new_node;
    return SUCCESS;
}

int insert_last(node **head, node **tail, int data)
{
    node *new_node = malloc(sizeof(*new_node));

    if (new_node == NULL)
        return FAILURE;

    new_node->data = data;
    new_node->next = NULL;
    new_node->prev = *tail;

    if (*tail != NULL)
        (*tail)->next = new_node;
    else
        *head = new_node;

    *tail = new_node;
    return SUCCESS;
}

int delete_list(node **head, node **tail)
{
    node *current = *head;

    while (current != NULL)
    {
        node *next = current->next;
        free(current);
        current = next;
    }

    *head = NULL;
    *tail = NULL;
    return SUCCESS;
}

void create_list(char *operand, node **head, node **tail)
{
    size_t i = 0;

    *head = NULL;
    *tail = NULL;

    /* The sign is tracked separately in main.c, not stored as a digit. */
    if (operand[i] == '+' || operand[i] == '-')
        i++;

    for (; operand[i] != '\0'; i++)
    {
        if (insert_last(head, tail, operand[i] - '0') == FAILURE)
        {
            delete_list(head, tail);
            fprintf(stderr, "Error: memory allocation failed while reading an operand.\n");
            exit(EXIT_FAILURE);
        }
    }
}

void print_list(node *head)
{
    if (head == NULL)
    {
        putchar('0');
        return;
    }

    while (head != NULL)
    {
        printf("%d", head->data);
        head = head->next;
    }
}

int list_len(node *head)
{
    int length = 0;

    while (head != NULL)
    {
        length++;
        head = head->next;
    }
    return length;
}

int compare_list(node *head1, node *head2)
{
    int len1 = list_len(head1);
    int len2 = list_len(head2);

    if (len1 > len2)
        return OPERAND1;
    if (len1 < len2)
        return OPERAND2;

    while (head1 != NULL && head2 != NULL)
    {
        if (head1->data > head2->data)
            return OPERAND1;
        if (head1->data < head2->data)
            return OPERAND2;
        head1 = head1->next;
        head2 = head2->next;
    }

    return SAME;
}

void remove_pre_zeros(node **head)
{
    while (*head != NULL && (*head)->data == 0 && (*head)->next != NULL)
    {
        node *old_head = *head;
        *head = old_head->next;
        (*head)->prev = NULL;
        free(old_head);
    }
}
