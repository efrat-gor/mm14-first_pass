#include "first_pass.h"


/*Kבדוק לגבי השליחה לפונקציות את האינדקס אם לא צריך לשלוח אותו בתור כתוהת כדאי שישתנה ויתקדם אחכ לשאר הפונקציות*/

int empty_or_comment_line(char*line,int index)
{
    index=advance_space(line,index);
    /*empty line */
    if(index>=MAX_LINE_LENGTH){printf("WARNING!.empty line");
    return 1;
    }
    /*if it is a comment line*/
    else if(*(line+index++)=='/'&&*(line+index)=='*')
    {
        index++;
        /*try to find the end of the comment*/
        while(!(*(line+index)=='/'&&*(line+index+1)=='*')&&index<=MAX_LINE_LENGTH-1)
        {
            index++;
        }
        if(index>=MAX_LINE_LENGTH-1)
        {
            printf("WARNING!. in line %d start comment without finish it ",line_counter);
            error_flag=1;
            return 1;
        }
        return 1;/* a comment line*/
    }
    return 0;

}
void discover_line(char *line,int index)
{   int len_op=0,i;
    index=advance_space(line,index);
    if(empty_or_comment_line(line,index))
    {
        /*pass to the next line*/
        return;
    }
    i=index;
    /*remove after*/
    while(*(line+i))
      printf("%c",*(line+ i++));
    printf("\n");
    if(!strncmp(".entry",line+index,6))
    {
        return;
    }
    if(!strncmp(".extern",line+index,7))
    {
       external(line,index+7);
    }
    if(!strncmp(".string",line+index,7))
    {
        string_to_data (line,index+7);
    }
    if(!strncmp(".data",line+index,5))
    {
        data_to_data(line,index+5);
    }
    /*if it a symbol*/
    if(is_label(line,index))
    {
       /*it is a label*/
    }
    else if(len_op=is_operation(line+index))  /*operation*/
    {
        operation(line,index,len_op);       
    }

}
/*This function handles the external guides*/
void external(char* line,int index)
{
	symbol* temp;
	int i;
	index=advance_space(line,index);
	if (*(line+index) == '\0')
	{
		printf("ERROR!! line %d: Label is missing\n", line_counter);
		error_flag = 1;
	}
    /*getting the lenght of the symbol name*/
	for (i = index; *(line+i) != '\0' && *(line+i) != '\n' && *(line+i) != ' '; i++);
	if (i > MAX_LABEL_LENGTH)
	{
		printf("ERROR!! line %d: Label is too long\n", line_counter);
		error_flag =1;
	}
	temp = (symbol*)malloc(sizeof(symbol));
	if (!temp)
	{
		printf("ERORR!! Memory allocation faild\n");
		error_flag = 1;
		return;
	}
	temp->next = head_symbol;
	head_symbol = temp;
	strncpy(head_symbol->symbol_name, line, i);
	head_symbol->is_external = TRUE;
	head_symbol->address = 0;
}
/*End of ext function*/
/*update the rea adress of the synbols in the symbil table*/
void update_symbol_table()
{
    symbol *temp;
    temp=head_symbol;
    print_symbol_table();
    while(temp)
    {
        if(temp->is_code==FALSE&&temp->is_external==FALSE)
           temp->address+=IC;
        temp=temp->next;
    }
    print_symbol_table() ;  
}
/*remove after*/
void print_symbol_table()
{
    symbol *temp;
    temp=head_symbol;
    while ((temp))
    {
        printf(" the name is: %s , adress is %d, iscode?:%d,isexternal %d\n",
        temp->symbol_name,temp->address,temp->is_code,temp->is_external);
        temp=temp->next;
    }
}
void data_to_data(char *line,int index)
{
    /*for negative digit*/
    int flag_negetive;
    int first_entry=1;
    int digit;
    if(!check_after_operation(line,index))
    {
          printf("WARNING . in line %d nissing space after name of operation ",line_counter);
          error_flag=1;
          return;
    }
    index=advance_space(line,index);
    while(*(line+index))
    {  
        digit=0; 
    if(!isdigit(*(line+index))&&(*(line+index)!='-'&&*(line+index)!='+'))
    {
           /*for checking if there is a comma if there are more than 1 digit*/
        if(!first_entry)
        { 
            index=advance_space(line,index);
            printf("%c\n",*(line+index));/*remove after*/
            if(*(line+index)!=',')
            {
                 printf("WARNING . in line %d missing comma between digits  ",line_counter);
                 error_flag=1;
                 return;               
            }
            index++;
                /*if there is no digit after comma*/
            if(*(line+index)==0)
              {
                 printf("WARNING !. in line %d: missing a digit  ",line_counter);
                 error_flag=1;
                 return; 
              }
        }
        if(!isdigit(*(line+index))&&(*(line+index)!='-'&&*(line+index)!='+')){
        printf("WARNING . in line %d not a digit  ",line_counter,'"');
        error_flag=1;
        return;
        }
    }
     /*usually positive*/
    printf("%c\n",*(line+index));/*remove after*/
    flag_negetive=1;
    index=advance_space(line,index);
    /*update the flag for negative number and checking if number after sign*/
    if(*(line+index)=='-'||*(line+index)=='+')
    {
        if(*(line+index)=='-')
        {
            flag_negetive=-1;
        }        
        index++;
        index=advance_space(line,index);
        if(!isdigit(*(line+index)))
        {
          printf("%c\n",*(line+index));/*remove after*/
          printf("WARNING . in line %d not a digit ",line_counter);
          error_flag=1;
          return;
        }
    }
    /*to get the number*/
    while(isdigit(*(line+index)))
    {
        digit=digit*10+(*(line+index)-48);
        index++;
    }
    /*if there are text in the middle of number like 34*6*/
    if(!(*(line+index)==','||isspace(*(line+index))))
    {
         printf("WARNING! . in line %d : extranous tabs in the middle of number ",line_counter);
          error_flag=1;
          return;
    }
    data_table[DC++].ch=digit*flag_negetive;
     printf("%d\n",data_table[DC-1].ch);/*remove after*/
    /*for checking if there is a comma if theer are more than 1 digit*/
    first_entry=0;
    index=advance_space(line,index);
    }
    if(DC==head_symbol->address)
    {
          printf("WARNING . in line %d no digits after \".data \"",line_counter);
          error_flag=1;
          return;
    }
}
/*end function data_to_data*/

/*the function insert the string to the data_table by asci */
void string_to_data(char*line ,int index)
{  int i=0;/**remve after*/
   if(!check_after_operation(line,index))
    {
          printf("WARNING . in line %d nissing space after name of operation ",line_counter);
          error_flag=1;
          return;
    }
    index=advance_space(line,index);
    if(!*(line+index))
    {
         printf("WARNING . in line %d no string after \".string\"",line_counter);
          error_flag=1;
          return;
    }
    if(*(line+index)!='\"')
    {
        printf("WARNING . in line %d :missing %c before the string ",line_counter,'"');
        error_flag=1;
        return;
    }
    
    else{
        index++;
    /*till the string get finshis  insert the asci to the data table*/
      while(*(line+index)&&*(line+index)!='\"')
         {printf("i am here\n");/**remve after*/
           data_table[DC++].ch=*(line+index++);
         }
      if(*(line+index)!='\"')
       {
        printf("WARNING . in line %d :missing %c after the string ",line_counter,'"');
        error_flag=1;
       }
       data_table[DC++].ch=0;/*last word of every string is 0 (to know that the string get finish)*/
       while (data_table[i].ch)/*remove after*/
       {
       printf("%c",data_table[i++].ch);
       }
    }
    
}
/*end function string_To_data*/

/*the function analized the label and built the right things forthe label(check if  belong to codr/data*/
void label (char * line,int index,char name[MAX_LABEL_LENGTH])
{
    
    symbol* temp;
    index= advance_space(line,index);
    temp = (symbol*)malloc(sizeof(symbol));
	if (!temp)
	{
     printf("no memmory\n");
     error_flag=1;
	 return;
    }
    if(!exsist_symbol(name))
    {
    temp->next=head_symbol;
    head_symbol=temp;
    strcpy(head_symbol->symbol_name,name);
    if (*(line+index) == '.')
			{  
				/*If its guide statement*/
				head_symbol->address = DC;
				head_symbol->is_code = FALSE;
				head_symbol->is_external =FALSE;/*to check why i did that*/
				if (!strncmp(line+index, ".entry", 6)){
					printf("WARNING!! line %d: A label defined at the beginig of entry statement is ignored\n" ,line_counter);
                    error_flag=1;}
				else if (!strncmp(line+index, ".extern", 7)){
					printf("WARNING!! line %d: A label defined at the beginig of extern statement is ignored.\n", line_counter);
                    error_flag=1;}
				else
					/*Sends again to analize to find out if its string or data*/
					discover_line(line,index);  
			}
            
	else if(is_operation(line+index))
			{  
				/*If this is a statement of instruction*/
				head_symbol->address = IC;
				head_symbol->is_code =TRUE;
				head_symbol->is_external = FALSE;
				/*Go again to analize to find out which instruction statement*/
				discover_line(line,index);
			}
            else
            {
                printf("WARNING line %d: there are extranous text after label.\n",line_counter);
                error_flag=1;
            }
    }
     else{
         printf("WARNING line %d: this label is exsisitng in the data already.\n",line_counter);
         error_flag=1;
     }       

}
/*check if a symbol is already exsist in the list ofsymbol-that won't be duplicates symbols*/
 int exsist_symbol(char *name)
{
    symbol *temp;
    temp=head_symbol;
    while(temp)
    {
        if( strcmp(temp->symbol_name,name))
           return 1;
    }
    return 0;
}
int is_label (char *line,int index)
{
    char symbol_name[MAX_LABEL_LENGTH];
    int length=index;
    for(;index< MAX_LINE_LENGTH;index++)
    {
        if(*(line+index)==':')
        {  strncpy(symbol_name,line+length,index-length); 
           label(line,++index,symbol_name);          
           return 1;
        }      
    }
    return 0;
}
/*the function check if the operation needs to get two operands and send them to te function withe the detail i they are sourceor distnation.
*/

void operation (char * line,int index,int len_op)
{
    if(operation_table[operation_index].code>=0&&operation_table[operation_index].code<=13)
    {
        index=operands(line,index+len_op,"source");/*send for the first time*/
        index++;
    }
    /*destenation -2 operands*/
    if(operation_table[operation_index].code>=0&&operation_table[operation_index].code<=4)
    {
        index=advance_space(line, index);
        if(*(line+ index)!=',')
        {
            printf("WARNING! in line %d : missing comma.\n",line_counter);
            error_flag=1;
        }
        else
        {
           index++;
           index= operands(line,index,"destenation");/*if there are 2 operands*/
        }
    }
}
/*the function check and count how many words the operatiom take */
int operands(char *line, int index, char * kind)
{
   
    index=advance_space(line,index);
   
    if(*(line+index)=='r')
    {   printf("%c\n",*(line+ index));/*remove this ine after*/
        index++;
        printf("%c\n",*(line+index));/*remove this ine after*/
        if(!( (*(line+index)-48) >0 && (*(line+index)-48) <= 7  ))
        {
           printf("%c\n",*(line+index));/*remove this ine after*/
           printf("WARNING in line %d: not exsisting such a register.\n",line_counter);
           error_flag=1;
        }

    }
    /*need another word*/
    else if(*(line+index)=='#')
    {    printf("%c\n",*(line+index));/*remove after*/
        IC++;/*two lines of instruction are needded*/
        index++;
        index=advance_space(line,index);
        if(*(line+index)=='+'&& !(isdigit(*(line+index+1)))&& !(isdigit(*(line+index)))&& (*(line+ index)=='-'&& !(isdigit(*(line+index+1)))))
         { 
            printf("WARNNIG !in line %d : not a number after #.\n",line_counter);
            error_flag=1;                  
         }
         printf("%c\n",*(line+index));/*remove after*/
        
    }
    
    else if(*(line+index)=='&')/*second system*/
    {
        IC++;
        if(operation_table[operation_index].code!=9)/*this system fit only for operation that their opcode is 9*/
        {
            printf("WARNING ! in line %d: cant prefore & after such a operation.\n",line_counter);
            error_flag=1;
            
        }
        
        /*code_table[IC].number==/*only in the second pass we will be able to complete this because we still dont know if this label is exsist already*/
 
    }
    
    else if(isalpha(*(line+index)))/*label*/
    { 
       IC++;
       /*for increasing the the index (for the rest of the checking)*/
        while (isalpha(*(line+index)))
       {
           index++;
       }
       index--;
    }
    else/*could be extranous tabs after operation or variables*/
    { if(!strncmp(kind,"source",6)){
        printf("WARNING ! in line %d : extranous text after operation\n",line_counter);}
      else /*destenation*/
       { printf("WARNING ! in line %d : extranous text after variable\n",line_counter);}
      error_flag=1;
       /*advanse the index to the rest of the line that we want meet any more the extarnous text for countinue checking the rest of the line*/
       while (!isalpha(*(line+index)))
       {
           index++;
       }
      index=operands(line,index,kind);/*send to checking again the same line(check if it is label or register and more...*/
    }
    return index;
}
/* the function check if the order is from the table_operation*/
int is_operation(char * str)
{ 
    int i;
    for(i=0;i<15;i++)
    {
        if(!strncmp(str,operation_table[i].name_operation,3))
        {
            operation_index=i;
            return 3;

        }

    }
    /*if it "stop"*/
    if(!strncmp(str,operation_table[i].name_operation,4))
    {
         operation_index=i;
         return 4;
    }

  return 0;

}
int advance_space (char *line,int index)
{
    while(isspace(*(line+index)))
    {
        index++;
    }
    return index;
}
/*the function check if after operation name there is a space and not a tab*/
int check_after_operation(char* line,int index)
{
    /*if ther is a sapce after operation */
    if(isspace(*(line+index)) )
      return 1;
    return 0;
}
/*void free_all(void * s1,int nbites )
{

}*/