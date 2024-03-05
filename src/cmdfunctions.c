#include "cmdfunctions.h"

#include <stdlib.h>
#include <stdio.h>

void printUsage()
{
    fprintf(stdout, "Usage: \n    cmdmath \"math expression\"\n    cmdmath --do \"math expression\"\n    cmdmath --file path/to/math.txt\n    cmdmath --help\n");
}

void printHelp()
{
    fprintf(stdout, "Avalible operators: \n    + adds numbers together. Precedence level 1.\n    - subtracts numbers. Precedence level 1.\n    * multiplies numbers. Precedence level 2.\n    / divides one number by another. Precedence level 2.\n    ^ evaluates the left operand to the power of the second. Precedence level 3.\n    () encloses operations to be done first. Precedence level 4.\n");
}

char* combineArgs(int startingIndex, int argc, char* argv[])
{
    char* combined = (char*)malloc(1024 * sizeof(char));

    for (int v = startingIndex; v < argc; v++)
    {
        strcat(combined, argv[v]);
        if (v != argc - 1)
            strcat(combined, " ");
    }
    return combined;
}

char* loadFromFile(const char* filepath)
{
    FILE* file = fopen(filepath, "r");
    if (file == NULL) {
        fprintf(stderr, "Failed to open file \"%s\"\n", filepath);
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    long fileSize = ftell(file);
    rewind(file);

    char* buffer = malloc(fileSize + 1 * sizeof(char));
    if (buffer == NULL) {
        fprintf(stderr, "Failed to allocate memory for file \"%s\"\n", filepath);
        fclose(file);
        return NULL;
    }

    size_t bytesRead = fread(buffer, 1, fileSize, file);
    if (bytesRead < fileSize) {
        fprintf(stderr, "Failed to read file \"%s\"\n", filepath);
        free(buffer);
        fclose(file);
        return NULL;
    }

    buffer[bytesRead] = '\0';

    fclose(file);
    return buffer;
}