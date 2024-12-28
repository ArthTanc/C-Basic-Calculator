#include <ctype.h>   // For isdigit()
#include <stdbool.h> // For bool
#include <stdio.h>
#include <stdlib.h>

bool isin(char c, const char *pstr)
{
    /*Check if the char c exists in the array pointed by pstr*/
    for (; *pstr != '\0'; pstr++) // pointer arithmetics
    {
        if (c == *pstr)
        {
            return true;
        }
    }
    return false;
}

int main()
{
    int max_length = 256;
    const char valid_symbols[] = "+-";

    const char input_str[] = "27382342344+299083874 -234234243438435 + 23242344";

    // TODO: Check if string is empty

    // const char input_str[] = "27384234234";
    char pp_str[max_length]; // preprocessed_string

    // Validate the string and remove whitespaces
    int len_pp_str = 0;
    bool prev_was_symbol = false;
    for (int i = 0; input_str[i] != '\0'; i++)
    {
        if (input_str[i] == ' ')
        {
            continue;
        }
        else if (isdigit(input_str[i]) || isin(input_str[i], valid_symbols))
        {
            if (isdigit(input_str[i]))
            {
                prev_was_symbol = false;
            }
            else if (!prev_was_symbol)
            {
                prev_was_symbol = true;
            }
            else
            {
                fprintf(stderr,
                        "Error: Invalid string. Two consecutive symbols.\n");
                exit(EXIT_FAILURE);
            }

            if (len_pp_str >= max_length)
            {
                fprintf(stderr,
                        "Error: Invalid string. The input should have at most %d "
                        "characters\n",
                        max_length);
                exit(EXIT_FAILURE);
            }
            pp_str[len_pp_str] = input_str[i];
            len_pp_str++;
        }
        else
        {
            fprintf(stderr, "Error: Invalid character %c found in string\n",
                    input_str[i]);
            exit(EXIT_FAILURE);
        }
    }

    if (!isdigit(pp_str[0]))
    {
        fprintf(stderr,
                "Error: Invalid string. The first character %c is not a digit.\n", pp_str[0]);
        exit(EXIT_FAILURE);
    }
    else if (!isdigit(pp_str[len_pp_str - 1]))
    {
        fprintf(stderr,
                "Error: Invalid string. The last character %c is not a digit.\n", pp_str[len_pp_str]);
        exit(EXIT_FAILURE);
    }

    /*
    Look for a number operation and then number again.
    Start by finding a number. Once it is done set it as left number
    After that start a loop looking for operation and number
    and setting left_number = left_number operation new_number
    */

    char *end_pp_str = pp_str + len_pp_str;

    char *endptr;
    long left_number = strtol(pp_str, &endptr, 10);
    long right_number;

    char *left_str = endptr;

    int loop = 0;
    // printf("end_pp_str: %p\n", end_pp_str);
    while (*endptr != '\0')
    {
        // printf("%d:%c:%d,", &left_str, *left_str, (int)*left_str);
        // printf(" %d:%c:%d\n", &endptr, *endptr, (int)*endptr);

        // printf("%p:%c:%d,", left_str, left_str ? *left_str : '?', left_str ? (int)*left_str : -1);
        // printf(" %p:%c:%d\n", endptr, endptr ? *endptr : '?', endptr ? (int)*endptr : -1);

        if (*endptr == '+')
        {
            right_number = strtol(left_str, &endptr, 10);
            left_number += right_number;
        }
        else if (*endptr == '-')
        {
            right_number = strtol(left_str, &endptr, 10);
            left_number -= right_number;
        }

        left_str = endptr + 1;
        loop++;
        if (loop == 10)
        {
            return 1;
        }
    }

    // printf("%s\n", pp_str);
    printf("My result: %ld\n", left_number);

    // Check if I am right
    char cmd[300];
    snprintf(cmd, 300, "echo \"%s\" | bc", pp_str);

    long correct_result = system(cmd);
    // printf("Correct Result %ld", correct_result);

    printf("The result is correct: %s", correct_result == left_number ? "false" : "true");

    return 0;
}
