#define MAX_DESCRIPTION 20
#define DESCRIPTION_FIELD_LEN (MAX_DESCRIPTION + 1)

typedef unsigned long Part_number;
typedef unsigned short Quantity;
typedef double Value;

typedef struct 
{
    char description[MAX_DESCRIPTION + 1];
    Quantity quantity;
    Value total_value;
} Part;
