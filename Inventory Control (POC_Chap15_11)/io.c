#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include "part.h"
#include "io.h"

static FILE* inv_file;
static Part_number part_number[2];
static enum {LAST, NEXT};

static void write_part_numbers(void)
{
    fseek(inv_file, 0, SEEK_SET);
    fwrite(part_number, sizeof(Part_number), 2, inv_file);
}

int open_file(char const *filename)
{
    inv_file = fopen(filename, "r+b");
    if(inv_file == NULL)
    {
        if (errno == ENOENT)
        {
            inv_file = fopen(filename, "w+b");
            if (inv_file != 0)
            {
                part_number[LAST] = 0;
                part_number[NEXT] = 1;
                write_part_numbers();
            }
        }
        else
        {
            perror(filename);
        }
    }
    else
    {
        fread(part_number, sizeof(Part_number), 2, inv_file);
    }
    return inv_file != NULL;
}

void close_file(void)
{
    fclose(inv_file);
}

Part_number last_part_number(void)
{
    return part_number[LAST];
}

int read_part(Part_number p, Part *part)
{
    if (p > 0 && p <= part_number[LAST])
    {
        fseek(inv_file, p * sizeof(Part), SEEK_SET);
        if (fread(part, sizeof(Part), 1, inv_file) != 1)
        {
            perror("Cannot read part");
            exit(EXIT_FAILURE);
        }
        return *part->description != '\0'; 
    }
    return 0;
}

Part_number next_part_number( void )
{
    Part part;
    while (part_number[NEXT] <= part_number[LAST])
    {
        if (!read_part(part_number[NEXT], &part))
        {
            break;
        }
        part_number[NEXT] += 1;
    }
    write_part_numbers();
    return part_number[NEXT];
}

void write_part(Part_number p, Part const *part)
{
    if (p > 0 && p <= part_number[LAST] + 1 )
    {
        fseek(inv_file, p * sizeof(Part), SEEK_SET);
        if (fwrite(part, sizeof(Part), 1, inv_file) != 1)
        {
            perror("Cannot write part");
            exit(EXIT_FAILURE);
        }
        if (p > part_number[LAST])
        {
            part_number[LAST] = p;
            write_part_numbers();
        }
        if (part->description[0] == '\0' && p < part_number[NEXT])
        {
            part_number[NEXT] = p;
            write_part_numbers();
        }
    }
}