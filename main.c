#include <stdio.h>
#include <math.h>

int validate_parenthesis(char input_str[])
{
    int i = 0, my_char, counter = 0;

    my_char = input_str[0];
    while (my_char != '\0')
    {
        if (my_char == '(')
        {
            counter++;
        }
        else if (my_char == ')')
        {
            counter--;
            if (counter < 0)
            {
                return 0;
            }
        }

        i++;
        my_char = input_str[i];
    }

    if (counter == 0)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

int is_valid(char input_str[], char accepted_symbols[], int len_symbols, int *phas_parenthesis)
{
    int character, flag, i, j;

    *phas_parenthesis = 0;

    if (input_str[0] == '\0')
    {
        return 0;
    }
    else
    {
        i = 0;
        character = input_str[0];
    }

    while (character != '\0')
    {
        if (!(character >= 48 && character <= 57 || character == 32))
        {
            if (character == '(' || character == ')')
            {
                *phas_parenthesis = 1;
            }
            else if (character == '.')
            {
                i++;
                character = input_str[i];
                while (character != '\0'){
                    if (character == '.'){
                        return 0;
                    } else if (character >= '0' && character <= '9') {
                        i++;
                        character = input_str[i];
                    } else {
                        break;;
                    }
                }
                i--; // to prevent accessing out of bounds memory address
            }
            else
            {
                flag = 0;
                for (j = 0; j < len_symbols; j++)
                {
                    if (character == accepted_symbols[j])
                    {
                        flag = 1;
                        break;
                    }
                }

                if (flag == 0)
                    return 0;
            }
        }

        i++;
        character = input_str[i];
    }

    if (!*phas_parenthesis)
    {
        return 1;
    }
    else
    {
        return validate_parenthesis(input_str);
    }
}

double evaluate_expression(char input_str[])
{
    // Should only be used if we don't have parenthesis

    double a = 0;
    char operation;
    int b = 0, i = 0, exponent = 0;
    char my_char = input_str[0];

    while (my_char != '\0')
    {
        if (my_char >= 48 && my_char <= 57)
        {
            b += (my_char - 48) * pow(10, exponent);
            exponent++;
        }
        else
        {
            switch (my_char)
            {
            case '+':
                break;

            case '-':
                break;

            case '*':
                break;

            case '/':
                break;

            case '^':
                break;
            }
        }

        b = 0;
        i++;
        my_char = input_str[i];
    }

    return a;
}

int main()
{
    int max_length = 256;
    int has_parenthesis;
    char accepted_symbols[] = "+-*/^", c;
    // char input_str[max_length];

    // printf("Input your string:\n");
    // fgets(input_str, max_length, stdin);

    char input_str[] = "((1.22 + 1 - 3)*4 / 10.5) ^ 2";

    if (!is_valid(input_str, accepted_symbols, sizeof(accepted_symbols), &has_parenthesis))
    {
        printf("String Invalid");
        return 1;
    }
    else
    {
        printf("String Valid");
    }

    // if parenthesis are present we need to find the smallest possible parenthesis and solve it,
    // By recursion we can recall this function to solve the simplified string
    // We need a function to get the number and initialize to an int variable, similarly to the number next to it
    // We also need to identify the operation that will be used and perform it.
    // 1 + 20 - 3 -> 1 + 2 => a = int('1'); b = int('20'); op = '+' => res = a + b;
    // Now do for b = 3
    // We can imagine a starting as 0 and the first op begin addition, this way we can iterate through the array
    // We get the operation, and then the number; we perform the operation on a with b, and once we are finished we just return a

    // if (!has_parenthesis)
    // {
    //     print(evaluate_expression(input_str));
    // }
    // else
    // {
    // }
    return 0;
}