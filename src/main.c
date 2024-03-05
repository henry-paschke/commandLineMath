#include <stdio.h>
#include <string.h>

#include "cmdfunctions.h"
#include "token.h"
#include <stdlib.h>


int main(int argc, char* argv[])
{

    if (argc == 1)
    {
        printUsage();
    }
    else
    {
        if (strncmp(argv[1], "--", 2) == 0)
        {
            if ((strcmp(argv[1], "--do") == 0)  && argc > 2)
            {
                char* equation = combineArgs(2, argc, argv);

                evalEquation(equation);
                free(equation);

            }
            else if (strcmp(argv[1], "--file") == 0 && argc > 2)
            {
                char* equation = loadFromFile(argv[2]);
                if (equation == NULL)
                {
                    fprintf(stdout, "Failed to read file.\n");
                }

                evalEquation(equation);
                free(equation);
            }
            else if (strcmp(argv[1], "--help") == 0)
            {
                printHelp();
            }
            else
            {
                printUsage();
            }
        }
        else
        {
            char* equation = combineArgs(1, argc, argv);

            evalEquation(equation);

            free(equation);
        }
    }

    return 0;
}