
/*About the program: This program simulates an assembler. It gets a list of files names, (using the arguments in command line). Each file contains an assembly language code. It reads and analyzes the code line by line from each file, so at the end of reading proccess, the program prints detailed error messages, if any, or alternatively export files as required: object file (=contains the memory image of the machine code), entries file, externals file, (as detailed later in the program).
Assumptions: *Source files names with '.as' extension. *Each source program provided as input has a possible maximum size. *Excess "white spaces" are ignored in an assembly language input file. *Lowercase and upper case letters are considered different in the assembly language. *Assembly language supports the representation of integers on a decimal base only. *There must be a white character separating (one or more) between command/label and operands, except commas.*/

#include "assembler.h"
#include "auxiliary.h"



int main(int argc, char** argv)
{
	handle_input(argc, argv);
	return 0;
}

/*This function gets the command line arguments as a parameter and checks if the input is vaild, if vaild it opens each file and send its content to a function that analize it and if no errors are found in the first pass on the file, another pass is made on the file, that also call a function to analize it. And if no errors were detected, the functions for exporting appropriate output is called. one file is open each round.
Algorithm: creating a "file descriptor"-file pointer for operations on files: fopen(), fscanf(), feof()- in order to know when the file ends, fclose() and more. argc- stores the amount of input parameters in command line. argv[i]- holds each time a file name.*/
void handle_input(int argc, char** argv)
{
    /*Array to get line by line*/
    char line[MAX_LINE_LENGTH]; 
    char* file_name;
    FILE* fd;
	int i;
	if (argc == 1)
	{
		printf("ERROR!! You must send files.\n");
		error_flag =1;
		exit(0);
	}
	/* Loop for all the files the user input */
	for (i = 1; i < argc; i++)  
	{
		file_name = (char*)malloc(strlen(argv[i]) + 4);
		if (!file_name)
		{
			printf("ERROR!! Memory allocation faild\n");
			error_flag = 1;
			exit(0);
		}
		/*Opening the file in read permission.*/
		strcpy(file_name, argv[i]);
		/*Using strcat()- because the user sends the file name without extension*/
		strcat(file_name, INPUT_FILE_EXTENSION);
		/*Opening the file in read permission.*/
		fd = fopen(file_name, "r");
		if (!fd)
			printf("ERROR!! File %s cannot be opened. It does not exist or you do not have the appropriate access permission.\n", argv[i]);
		else
		{
			DC = DC_INITIAL_VALUE;
			IC = IC_INITIAL_VALUE;
			line_counter = 0;
			error_flag=0;
			/*First pass*/
			while (!feof(fd))
			{
				/*First analize*/
				discover_line(line,0);/*in the begining the index is 0-<ןndicates the beginning of the line */
				line_counter++;
				/*Get one line from the file V */
				fgets(line, MAX_LINE_LENGTH, fd); 
			}
			/*Checking memory limit exceeded.*/
			if (IC + DC > MEMORY_SIZE)
			{
				printf("ERROR!! The program has exceeded the memory limits.\n");
				error_flag= 1;
			}
			if(!error_flag)
			{
				/*Tmpty the first element of the array*/
				line[0] = '\0';
				/*Return fd to point on the begining of the file.*/
				rewind(fd);
				/*Zero the parameters before the next analize*/
				line_counter = 0;
				/*initialize_line(line);*/
				ICF=IC-100;
                DCF=DC;
				/*Update the address of the guide labels in the symbal table*/
				 update_symbol_table(); 
				/*Second pass*/
				/*
				while (!feof(fd))
				{
					
					analize_2_second_pass(line);
					line_counter++;
					fgets(line, MAX_LINE_LENGTH, fd);
				}
				
				fclose(fd); 
				
				if (!error_flag)
				{
					create_object_file();
					create_entry_file();
					create_external_file();
					printf("The file %s has been successfully compiled\n", argv[i]);
				}
				
				free(file_name);
				free_data_table();
				free_code_table(); 
				free_symbol_table(); 
				free_entries_list();
				free_externals_list();*/
			}
		}
	}/*End of for loop to open the files which givven from CMD by their names*/
}/*End of function*/
