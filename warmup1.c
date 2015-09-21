#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <time.h>
#include <sys/stat.h>
#include <math.h>
#include "my402list.h"
#include "cs402.h"
#include <locale.h>

struct transaction_info{
    time_t t_date;
    char t_type;
    unsigned int t_amt;
    char t_desc[25];
};


void FreeListObjects(My402List *pList){
    My402ListElem *elt=NULL;
    for (elt=My402ListFirst(pList); elt != NULL; elt=My402ListNext(pList, elt)) {
        struct transaction_info *obj_elt=elt->obj;
        free(obj_elt);
        elt->obj = NULL; 
        My402ListUnlink(pList,elt);       
    }
}

static
void Usage(){
    fprintf(stderr,
            "malformed command\nusage: warmup1 sort [tfile]\n");
    exit(-1);
}

static
void ProcessOptions(int argc, char *argv[]){
  if (argc == 1) Usage();
  else { if (strcmp(argv[1],"sort")!=0) Usage();}
}

static
void BubbleSwap(My402List *pList, My402ListElem **pp_elem1, My402ListElem **pp_elem2)
    /* (*pp_elem1) must be closer to First() than (*pp_elem2) */
{
    My402ListElem *elem1=(*pp_elem1), *elem2=(*pp_elem2);
    void *obj1=elem1->obj, *obj2=elem2->obj;
    My402ListElem *elem1prev=My402ListPrev(pList, elem1);
/*  My402ListElem *elem1next=My402ListNext(pList, elem1); */
/*  My402ListElem *elem2prev=My402ListPrev(pList, elem2); */
    My402ListElem *elem2next=My402ListNext(pList, elem2);

    My402ListUnlink(pList, elem1);
    My402ListUnlink(pList, elem2);
    if (elem1prev == NULL) {
        (void)My402ListPrepend(pList, obj2);
        *pp_elem1 = My402ListFirst(pList);
    } else {
        (void)My402ListInsertAfter(pList, obj2, elem1prev);
        *pp_elem1 = My402ListNext(pList, elem1prev);
    }
    if (elem2next == NULL) {
        (void)My402ListAppend(pList, obj1);
        *pp_elem2 = My402ListLast(pList);
    } else {
        (void)My402ListInsertBefore(pList, obj1, elem2next);
        *pp_elem2 = My402ListPrev(pList, elem2next);
    }
}

static
void BubbleSort(My402List *pList)
{
    My402ListElem *elem=NULL;
    int i=0;
    int num_items = My402ListLength(pList);
    for (i=0; i < num_items; i++) {
        int j=0, something_swapped=FALSE;
        My402ListElem *next_elem=NULL;

        for (elem=My402ListFirst(pList), j=0; j < num_items-i-1; elem=next_elem, j++) {
          struct transaction_info *cur_val=(struct transaction_info *)&elem->obj;
          struct transaction_info *next_val;

            next_elem=My402ListNext(pList, elem);
            next_val = (struct transaction_info *)&next_elem->obj;
            int *a = (int *)cur_val->t_date;
            int *b = (int *)next_val->t_date;
            //printf("%d %d\n",*a,*b );


            if (*a > *b) {
              //printf("ok\n");
              BubbleSwap(pList, &elem, &next_elem);
              something_swapped = TRUE;
            }
        }
        if (!something_swapped) break;
    }
}


static
void addComma (long n,int choice) {
    long temp_n = 0;
    char final[50],buffer[50];
    int i; long order = 1;
    for (i = 0; i < 50; i++) {
        final[i] ='\0';
    }
    if (n < 0) n = -n;
    int rem = n % 100;
    n = n/100;
    while (n >= 1000) {
        temp_n = temp_n + order * (n % 1000);
        n /= 1000;
        order *= 1000;
    }
    sprintf (buffer, "%lu" , n );
    strcat(final,buffer);
    while (order != 1) {
        order /= 1000;
        n = temp_n / order;
        temp_n = temp_n  % order;
        sprintf (buffer,",%03ld", n);
        strcat(final,buffer);
    }
        sprintf (buffer,".%02d", rem);
        //printf ("%s", final);
        //printf (".%02d\n", rem);
        strcat(final,buffer);
        final[strlen(final)]='\0';
        if (choice == 1)
        fprintf(stdout,"| %13s  ",final);
        else if(choice ==2)
        fprintf(stdout,"| %13s  |\n",final);
        else if(choice ==3)
        fprintf(stdout,"| (%12s) ",final);
        else if(choice ==4)
        fprintf(stdout,"| (%12s) |\n",final);

        //*num[strlen(final)]='\0';
        //printf("\nTest :%s--------%s\n",*num,final );
    //*num = final;    
    
}


static
void PrintTestList(My402List *pList)
{
    My402ListElem *elem=NULL;
    long bal =0;
    time_t prev = 0,temp;
    for (elem=My402ListFirst(pList); elem != NULL; elem=My402ListNext(pList, elem)) {
      struct transaction_info *ival=elem->obj;
      temp = ival->t_date;
      if ((long)temp - (long)prev == 0) { fprintf(stderr, "input file is not in the right format : Duplicate transaction time\n");exit(-1);}
      prev = temp;
    }
    //char *num1,*num2;
    fprintf(stdout, "+-----------------+--------------------------+----------------+----------------+\n");
    fprintf(stdout, "|       Date      | Description              |         Amount |        Balance |\n");
    fprintf(stdout, "+-----------------+--------------------------+----------------+----------------+\n");
    for (elem=My402ListFirst(pList); elem != NULL; elem=My402ListNext(pList, elem)) {
        struct transaction_info *ival=elem->obj;
        temp = ival->t_date;

        char date[16];
    char buf[26];
    strncpy(buf, ctime(&temp), sizeof(buf));
    // OLD: Mon Jul 13 20:33:53 2009
    // NEW: Mon Jul 13 2009 
    int i=0,j=20;
    while(i<11) {
      date[i]=buf[i];i++;
    }
    while(i<15) {
      date[i]=buf[j];i++;j++;
    }
    date[15] = '\0';
    //setlocale(LC_NUMERIC,"");

    if(ival->t_type == '+') {
      bal+=ival->t_amt;
      //addcomma(&num1,(long double)ival->t_amt/100.00);
      //addcomma(&num2,(long double)bal/100.00);
          fprintf(stdout, "| %s | %-24s ", date,ival->t_desc);
      addComma((long)ival->t_amt,1);


      if(bal>0)
      addComma((long)bal,2);
    else
      addComma((long)(bal*-1),4);
    } else{
      bal-=ival->t_amt;
          fprintf(stdout, "| %s | %-24s ", date,ival->t_desc);
      addComma((long)ival->t_amt,3);


      if(bal>0)
      addComma((long)bal,2);
    else
      addComma((long)(bal*-1),4);
    }
    //free(num1);
    //free(num2);
        
  }
  fprintf(stdout, "+-----------------+--------------------------+----------------+----------------+\n");
}

void convert_file_to_data(My402List *pList,FILE *fp) {
  char buffer[1030],buffer2[1030]; float t_amount;unsigned int t_time;int length,count,i,j;time_t curr_time;
  time(&curr_time);
  while ((fgets(buffer,sizeof(buffer),fp)!=NULL)) {
    struct transaction_info *temp = (struct transaction_info *)malloc(sizeof(struct transaction_info));
    length = strlen(buffer);
    if(length>1024) { fprintf(stderr, "input file is not in the right format : Line size greater than 1024 characters\n");exit(-1);}
    for(count=0,i=0;i<length;i++) {
      if(count>3) break;
      if(buffer[i]=='\t')
        count++;
    }
    if(count != 3) { fprintf(stderr, "input file is not in the right format : Incorrect number of fields\n");exit(-1);}
    buffer[length-1] = ' ';
    buffer[length++] = '\n';
    //strncpy(buffer2,strtok(buffer,"\t"),sizeof(buffer2));
    strcpy(buffer2,strtok(buffer,"\t"));
    if(strlen(buffer2) != 1) { fprintf(stderr, "input file is not in the right format : Incorrect transaction type\n");exit(-1);}

    if (!(buffer2[0] == '+' || buffer2[0] == '-')) {fprintf(stderr, "input file is not in the right format : Incorrect transaction type\n"); exit(-1);}
    temp->t_type = buffer2[0];
    strncpy(buffer2,strtok(NULL,"\t"),sizeof(buffer2));
    //strcpy(buffer2,strtok(NULL,"\t"));
    count = strlen(buffer2);
    for(i=0;i<count;i++) {
      if(!(buffer2[i]>= '0' && buffer2[i]<= '9')) { count =-1 ; break;} 
    }
    //printf("%s\n", buffer2);
    t_time = atoi(buffer2);
    if (count == -1 || count >= 11 || curr_time < t_time){ fprintf(stderr, "input file is not in the right format : Incorrect transaction tiime\n");exit(-1);}
    t_time = atoi(buffer2);
    temp->t_date = t_time;
    strncpy(buffer2,strtok(NULL,"\t"),sizeof(buffer2));
    //strcpy(buffer2,strtok(NULL,"\t"));
    count = strlen(buffer2);
    for(i=0,j=0;i<count;i++) {
      if(!(buffer2[i]>= '0' && buffer2[i]<= '9') ) { if (buffer2[i] == '.' && j == 1) {count =-1 ; break;} else j++;} 
    }
    if (count == -1 ){ fprintf(stderr, "input file is not in the right format : Incorrect amount type\n");exit(-1);}
    count = strlen(buffer2);
    j=0;i=0;
    while(i< count && buffer2[i]!='.') i++;
    if(i>7) { fprintf(stderr, "input file is not in the right format : Incorrect amount value\n");exit(-1);}
    //printf("%d\n", i);
    //printf("%d\n", (count-i));
    if(count-i > 3) { fprintf(stderr, "input file is not in the right format : Incorrect amount value\n");exit(-1);}
    for(i=0,j=0;i<count;i++) {
      if(!(buffer2[i]>= '0' && buffer2[i]<= '9') ) { if (buffer2[i] == '.' && j == 1) {count =-1 ; break;} else j++;} 
    }
    if (count == -1 ){ fprintf(stderr, "input file is not in the right format: Incorrect amount value\n");exit(-1);}
    t_amount = atof(buffer2)*100;
    temp->t_amt = t_amount;
    strncpy(buffer2,strtok(NULL,"\n"),sizeof(buffer2));
    //strcpy(buffer2,strtok(NULL,"\t"));
    count = strlen(buffer2);
    //printf("Desc is : %s\n", buffer2);
    if (count == 0 ){ fprintf(stderr, "input file is not in the right format : Empty Description\n");exit(-1);}
    i=0;j=0;
    while(buffer2[i]==' ') i++;
    while(i<count) buffer2[j++] = buffer2[i++];
    buffer2[j]='\0';
    count = strlen(buffer2);
    i = count-1;
    while(buffer2[i]==' ') { buffer2[i]='\0';i--;}
    count = strlen(buffer2);    
    if (count == 0 ){ fprintf(stderr, "input file is not in the right format : Empty Description\n");exit(-1);}
    if (count > 24 ) count = 24;
    strncpy(temp->t_desc,buffer2,sizeof(char) * count);  
    (void)My402ListAppend(pList, temp);
  }
    fclose(fp);
}


int main(int argc, char *argv[])
{

    My402List list;
    char file_path_name[1024];
    ProcessOptions(argc, argv);
    memset(&list, 0, sizeof(My402List));
    (void)My402ListInit(&list);
    if (argc > 2) {
      FILE *fp;
      strncpy(file_path_name,argv[2],sizeof(file_path_name));
      fp = fopen(file_path_name,"r"); 
      if( fp == NULL ) {
        //printf("Path is : %s\n",file_path_name );
        //fprintf(stderr, "malformed command or input file \"%s\" does not exist",file_path_name );
        switch(errno) {
            case EACCES:
            case EINTR:
                        fprintf(stderr, "Error with input file \"%s\" : %s\n",file_path_name,strerror(errno));
                        break;
            case EISDIR: fprintf(stderr, "Error with input file \"%s\" : is a directory",file_path_name ); break;

            default: fprintf(stderr, "Error with input file \"%s\" : %s\n",file_path_name,strerror(errno));
                        break;
        

        }
        //fprintf(stderr, "Error with input file \"%s\" : %s\nAAbc: %d\n",file_path_name,strerror(errno),errno );
        //perror("");
        exit(-1);
      }
      convert_file_to_data(&list,fp);
    } else {
      convert_file_to_data(&list,stdin);
    }
    BubbleSort(&list);
    PrintTestList(&list);
    FreeListObjects(&list);
    return 0;
}

