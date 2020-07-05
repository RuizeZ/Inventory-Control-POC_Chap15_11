/*
ruizeZ
7/5/2020
Write a program to keep track of an inventory of parts and their value. Each part has a description that may be from 1 to 20 characters in length. When a new part is added 
to the inventory, it is assigned the next available part number. The first part  number is 1. The program should store the quantity on hand and the total value for  each part.
The program should take a single argument from the command line, which is  the name of the inventory tile. If the file does not exist, an empty inventory file is  created. 
The program then prompts for transactions and processes them one by  one.  
*/

#include <stdio.h>
#include <stdlib.h>
#include "part.h"
#include "io.h"
#include "process.h"

int main(int ac, char **av)
{
	if (ac != 2)
	{
		printf("invalid argument");
		return 0;
	}

	if (open_file(av[1]))
	{
		while (process_request())
		{
			;
		}
		close_file();
	}
	
	return 1;
}