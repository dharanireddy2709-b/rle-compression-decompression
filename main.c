#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Function prototypes
void compressFile(const char *inputFilename, const char *outputFilename);
void decompressFile(const char *inputFilename, const char *outputFilename);

int main() {
    int choice;
    char inputFile[256], outputFile[256];

    printf("Run-Length Encoding (RLE) Compression/Decompression\n");
    printf("1. Compress\n");
    printf("2. Decompress\n");
    printf("Choose an option (1 or 2): ");
    if (scanf("%d", &choice) != 1) {
        printf("Invalid input. Exiting.\n");
        return 1;
    }

    getchar(); // Clear newline left by scanf

    printf("Enter input filename: ");
    fgets(inputFile, sizeof(inputFile), stdin);
    inputFile[strcspn(inputFile, "\n")] = 0; // Remove trailing newline

    printf("Enter output filename: ");
    fgets(outputFile, sizeof(outputFile), stdin);
    outputFile[strcspn(outputFile, "\n")] = 0; // Remove trailing newline

    if (choice == 1) {
        compressFile(inputFile, outputFile);
    } else if (choice == 2) {
        decompressFile(inputFile, outputFile);
    } else {
        printf("Invalid option. Exiting.\n");
    }

    return 0;
}

void compressFile(const char *inputFilename, const char *outputFilename) {
    FILE *inputFile = fopen(inputFilename, "r");
    if (!inputFile) {
        perror("Error opening input file");
        return;
    }

    FILE *outputFile = fopen(outputFilename, "w");
    if (!outputFile) {
        perror("Error opening output file");
        fclose(inputFile);
        return;
    }

    char currentChar, previousChar;
    int count = 0;

    previousChar = fgetc(inputFile);
    if (previousChar == EOF) {
        printf("Input file is empty.\n");
        fclose(inputFile);
        fclose(outputFile);
        return;
    }

    count = 1;
    while ((currentChar = fgetc(inputFile)) != EOF) {
        if (currentChar == previousChar) {
            count++;
        } else {
            fprintf(outputFile, "%c%d", previousChar, count);
            previousChar = currentChar;
            count = 1;
        }
    }
    // Write the last character and its count
    fprintf(outputFile, "%c%d", previousChar, count);

    printf("Compression complete. Output written to %s\n", outputFilename);

    fclose(inputFile);
    fclose(outputFile);
}

void decompressFile(const char *inputFilename, const char *outputFilename) {
    FILE *inputFile = fopen(inputFilename, "r");
    if (!inputFile) {
        perror("Error opening input file");
        return;
    }

    FILE *outputFile = fopen(outputFilename, "w");
    if (!outputFile) {
        perror("Error opening output file");
        fclose(inputFile);
        return;
    }

    char currentChar;
    int count;

    while (fscanf(inputFile, "%c%d", &currentChar, &count) == 2) {
        for (int i = 0; i < count; i++) {
            fputc(currentChar, outputFile);
        }
    }

    printf("Decompression complete. Output written to %s\n", outputFilename);

    fclose(inputFile);
    fclose(outputFile);
}
