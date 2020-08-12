
#include "first_pass.h"
#define O_source_des(kind)O##kind;
#define M_source_des(kind)M##kind; 
extern symbol* head_symbol;

void free_all(void * s1,int nbites )
{
    char *temp ;
    temp=(char*)malloc(nbites);
    memcpy(temp,s1,nbites);
    free(temp);
}
 int main()
 {
    char *line="                 ";  
    discover_line(line,0);
   
    
     return 0;

 }