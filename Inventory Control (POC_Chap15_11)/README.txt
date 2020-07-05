Write a program to keep track of an inventory of parts and their value. Each part has a description that may be from 1 to 20 characters in length. When a new part is added 
to the inventory, it is assigned the next available part number. The first part  number is 1. The program should store the quantity on hand and the total value for  each part.
The program should take a single argument from the command line, which is  the name of the inventory tile. If the file does not exist, an empty inventory file is  created. 
The program then prompts for transactions and processes them one by one.
Use the following conmand line to compile:
gcc -Wall -c process.c
gcc -Wall -c io.c
gcc -Wall -c POC_Chap15_11.c
gcc -o test POC_Chap15_11.o process.o io.o
./test inventory

Use the following input to process the request:
new description,quantity,cost-each
buy part-number,quantity,cost-each
sell part-number,quantity,price-each
delete part-number
print part-number
print all
total
end