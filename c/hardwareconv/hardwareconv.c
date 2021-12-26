#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void convert_file(FILE* infile, FILE* outfile) {
    fputs(
        "#ifndef HARDWARE_INC\n"
        "\n"
        "// hardware.h\n"
        "// Converted from hardware.inc using hardwareconv, by Eievui.\n"
        "\n", outfile);
    unsigned rb = 0;

    while (!feof(infile)) {
        char* next_word = NULL;
        fscanf(infile, "%ms", &next_word);
        if (next_word == NULL)
            break;

        if (strcmp(next_word, "DEF") == 0) {
            fputs("#define ", outfile);
            char* identifier = NULL;
            fscanf(infile, "%ms", &identifier);
            fputs(identifier, outfile);
            fputc(' ', outfile);
            free(identifier);

            char* type = NULL;
            fscanf(infile, "%ms", &type);
            if (strcmp(type, "EQU") == 0) {
                char* value = NULL;
                fscanf(infile, "%ms", &value);
                for (int i = 0; value[i]; i++) {
                    if (value[i] == '$')
                        fputs("0x", outfile);
                    else if (value[i] == '%')
                        fputs("0b", outfile);
                    else
                        fputc(value[i], outfile);
                }
                fputc('\n', outfile);
                free(value);
            } else if (strcmp(type, "RB") == 0) {
                fprintf(outfile, "%u\n", rb);
                unsigned rb_offset;
                fscanf(infile, "%u", &rb_offset);
                rb += rb_offset;
            }
            free(type);
        } else {
            while (getc(infile) != '\n' && !feof(infile));
        }

        free(next_word);
    }

    fputs("\n#endif\n", outfile);
}

int main(int argc, char* argv[]) {
    char* infile_path = "hardware.inc";
    char* outfile_path = "hardware.h";

    if (argc < 2)
        fputs("warn: Expected at least one argument.\nwarn: Defaulting to \"hardware.inc\" as input.\n", stderr);
    else
        infile_path = argv[1];
    if (argc >= 3)
        outfile_path = argv[2];

    FILE* infile = fopen(infile_path, "r");
    FILE* outfile = fopen(outfile_path, "w");

    if (infile == NULL) {
        fprintf(stderr, "error: Failed to open %s for reading.\n", infile_path);
        exit(1);
    }
    if (outfile == NULL) {
        fprintf(stderr, "error: Failed to open %s for writing.\n", outfile_path);
        exit(1);
    }

    convert_file(infile, outfile);
    fclose(infile);
    fclose(outfile);
    return 0;
}
