#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char* programName;
const char* fileName = "/sys/class/backlight/intel_backlight/brightness";
const int minValue = 1;
const int maxValue = 852;

int storeNewValue(const char* file_name, int value)
{
    FILE* wfile = fopen(file_name, "w");
    if(wfile == NULL)
    {
        printf("%s: %s: Permission denied\n", programName, file_name);
        return 1;
    }
    fprintf(wfile, "%d\n", value);
    fclose(wfile);
    return 0;
}

void StoreAndExit(int i)
{
    exit(storeNewValue(fileName, i));
}

int readCurrentValue(const char* file_name)
{
  FILE* file = fopen (file_name, "r");
  int i = 0;
  fscanf (file, "%d", &i);    
  fclose (file);        
  return i;

}

typedef struct 
{
    int isValid;
    char* errorMessage;
} validationResult;


int validateChangeValue(int newValue,  int i)
{
    if(i > 0) 
    {
        // i is positive
        if(newValue <= maxValue)
        {
            return newValue;
        } else
        {
            return maxValue;
        }
    }

    // i is negative
    if(newValue >= minValue)
    {
        return newValue;
    }
    return minValue;
}

int main(int argc, char* argv[])
{
    programName = argv[0];
    if(argc == 2 && (strcmp(argv[1], "--help") == 0 || strcmp(argv[1], "-h") == 0))
    {
        printf("Usage: %s [change|set] [VALUE|min|max]\n", programName);
        printf("Increments or decrements screen brightness by value\n");

        printf("Screen brightness file: %s\n", fileName);
        printf("Min brightness value: %d\n", minValue);
        printf("Max brightness value: %d\n", maxValue);
        exit(0);
    }
    if(argc < 3)
    {
        printf("%s: Too few arguments\n", programName);
        exit(1);
    }
    if(argc > 3)
    {
        printf("%s: Too many arguments\n", programName);
        exit(1);
    }

    int currentValue = readCurrentValue(fileName);
    int inputValue = atoi(argv[2]);

    if(strcmp(argv[1], "change") == 0)
    {
        int potentialNewValue = currentValue + inputValue;
        int validNewValue = validateChangeValue(potentialNewValue, inputValue);
        StoreAndExit(validNewValue);
    }
    
    if(strcmp(argv[1], "set") == 0)
    {
        if(inputValue <= maxValue && inputValue >= minValue)
        {
            StoreAndExit(inputValue);
        }
        if(strcmp(argv[2], "max") == 0)
        {
            StoreAndExit(maxValue);
        }
        if(strcmp(argv[2], "min") == 0)
        {
            StoreAndExit(minValue);
        }
        printf("%s: Invalid set value\n", programName);
        exit(1);
    }

    printf("%s: Command missing!\n", programName);
}
