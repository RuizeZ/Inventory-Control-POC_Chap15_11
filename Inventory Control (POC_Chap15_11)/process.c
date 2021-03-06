#include <stdio.h>
#include <string.h>
#include "part.h"
#include "io.h"
#include "process.h"

//total
static void total(void)
{
    Part_number p;
    Part part;
    Value total_value;
    total_value = 0;
    p = last_part_number();
    for ( p = last_part_number(); p > 0; p-=1)
    {
        if (read_part(p, &part))
        {
            total_value += part.total_value;
        }  
    }
    printf("Total value of inventory = %.2f\n", total_value);
    
}

//new part
static void new_part(char const*description, Quantity quantity, Value price_each)
{
    Part part;
    Part_number part_number;

    strcpy(part.description, description);
    part.quantity = quantity;
    part.total_value = quantity * price_each;
    
    part_number = next_part_number();
    write_part(part_number, &part);
    
    printf("%s is part number %lu\n", description, part_number);
}

//buy and sell
static void buy_sell(char request_type, Part_number part_number, Quantity quantity, Value price_each)
{
    Part part;
    if(!read_part(part_number, &part))
    {
        fprintf(stderr, "No such part\n");
    }

    else
    {
        if (request_type == 'b')
        {
            part.quantity += quantity;
            part.total_value += quantity * price_each;
        }
        else
        {
            Value unit_value;
            if (quantity > part.quantity)
            {
                printf("Sorry, only %hu in stock\n", part.quantity);
                return;
            }
            unit_value = part.total_value / part.quantity;
            part.total_value -= quantity * unit_value;
            part.quantity -= quantity;
            printf("Total profit: $%.2f\n", quantity * (price_each - unit_value));
        }
        write_part(part_number, &part);
    }
}

static void delete(Part_number part_number)
{
    Part part;
    if(!read_part(part_number, &part))
    {
        fprintf(stderr, "No such part\n");
    }
    else
    {
        part.description[0] = '\0';
        write_part(part_number, &part);
    }
}

static void print(Part_number part_number)
{
    Part part;
    if(!read_part(part_number, &part))
    {
        fprintf(stderr, "No such part\n");
    }
    else
    {
        printf("Part number %lu\n", part_number);
        printf("Description: %s\n", part.description);
        printf( "Quantity on hand: %hu\n", part.quantity );
        printf( "Total value: %.2f\n", part.total_value );
    }  
}

static void print_all(void)
{
    Part_number p;
    Part part;
    printf("Part number Description Quantity " "Total value\n" );
    for (p = 1; p <= last_part_number(); p += 1)
    {
        if (read_part(p, &part))
        {
            printf( "%11lu %s %10hu %11.2f\n", p, part.description, part.quantity, part.total_value );
        }
    }
    
}



int process_request(void)
{
    char request[MAX_REQUEST];
    char request_type[10];
    char description[DESCRIPTION_FIELD_LEN];
    Part_number part_number;
    Quantity quantity;
    Value price_each;
    char left_over[2];

    // read the request
    fputs("\nNext request? ", stdout);
    if (fgets(request, MAX_REQUEST, stdin) == NULL)
    {
        return 0;
    }
    
    if (sscanf(request, "%10s %1s", request_type, left_over) == 1 && (strcmp(request_type, "end") == 0 || strcmp(request_type, "total") == 0))
    {
        if (strcmp(request_type, "end") == 0)
        {
            return 0;
        }

        else
        {
            total();
        }
        
    }

    else if (sscanf(request, "new %20[^,],%hu,%lf %s", description, &quantity, &price_each, left_over) == 3)
    {
            new_part(description, quantity, price_each);
    }

    else if (sscanf(request, "%10s %lu,%hu,%lf %ls", request_type, &part_number, &quantity, &price_each, left_over) == 4
            && (strcmp(request_type, "buy") == 0 || strcmp(request_type, "sell") == 0))
    {
        buy_sell(request_type[0], part_number, quantity, price_each);
    }

    else if (sscanf(request, "%10s %lu %ls", request_type, &part_number, left_over) == 2
            && (strcmp(request_type, "delete") == 0 || strcmp(request_type, "print") == 0))
    {
        if (request_type[0] == 'd')
        {
            delete(part_number);
        }
        else
        {
            print(part_number);
        } 
    }

    else if (sscanf(request, " print %10s %ls", request_type, left_over) == 1
            && (strcmp(request_type, "all") == 0))
    {
        print_all();
    }
    else
    {
        if( sscanf( request, "%10s", request_type ) == 1);
    }
    return 1;  
}