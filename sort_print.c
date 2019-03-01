/******************************************************************************/
/* Important CSCI 402 usage information:                                      */
/*                                                                            */
/* This fils is part of CSCI 402 programming assignments at USC.              */
/*         53616c7465645f5f2e8d450c0c5851acd538befe33744efca0f1c4f9fb5f       */
/*         3c8feabc561a99e53d4d21951738da923cd1c7bbd11b30a1afb11172f80b       */
/*         984b1acfbbf8fae6ea57e0583d2610a618379293cb1de8e1e9d07e6287e8       */
/*         de7e82f3d48866aa2009b599e92c852f7dbf7a6e573f1c7228ca34b9f368       */
/*         faaef0c0fcf294cb                                                   */
/* Please understand that you are NOT permitted to distribute or publically   */
/*         display a copy of this file (or ANY PART of it) for any reason.    */
/* If anyone (including your prospective employer) asks you to post the code, */
/*         you must inform them that you do NOT have permissions to do so.    */
/* You are also NOT permitted to remove or alter this comment block.          */
/* If this comment block is removed or altered in a submitted file, 20 points */
/*         will be deducted.                                                  */
/******************************************************************************/

/*
 * Author:      William Chia-Wei Cheng (bill.cheng@acm.org)
 *
 * @(#)$Id: listtest.c,v 1.1 2018/12/26 17:15:59 william Exp $
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/time.h>

#include "cs402.h"

#include "my402list.h"

char *ctime(const time_t *timep);
struct object{
    char *type;
    char *time;
    char *amount;
    char *description;
};

static
void addnode(My402List *list,char *buf){
    const int length = 1024;
    struct object *newobj = (struct object*)malloc(sizeof(struct object));
    newobj->type = (char*)malloc(sizeof(char)*length);
    newobj->time = (char*)malloc(sizeof(char)*length);
    newobj->amount = (char*)malloc(sizeof(char)*length);
    newobj->description = (char*)malloc(sizeof(char)*length);
    //type process
    strncpy(newobj->type,buf,length);
    char* timeaddr = strchr(buf,'\0') + 1;
    strncpy(newobj->time,timeaddr,length);
    if(strlen(newobj->time) >= 11){
        fprintf(stderr,"error: transaction time exceeds");
        exit(0);
    }
    char* amountaddr = strchr(timeaddr,'\0') + 1;
    strncpy(newobj->amount,amountaddr,length);
    int amount = atoi(newobj->amount);
    if(sizeof(amount)>7){
        fprintf(stderr,"error: trasaction amount exceeds");
        exit(0);
    }
    // printf("%d\n",amount);
    char* descriptionaddr = strchr(amountaddr,'\0') + 1;
    strncpy(newobj->description,descriptionaddr,length);
    if(strlen(newobj->description) == 0){
       fprintf(stderr,"error: description is empty"); 
       exit(0);
    }

    // newobj->type = buf;
    // newobj->time = strchr(buf,'\0') + 1;
    // newobj->amount = strchr(newobj->time,'\0') + 1;
    // newobj->description = strchr(newobj->amount,'\0') + 1;
    My402ListAppend(list,(struct object*) newobj);
}

static
int processfile(My402List *list,char* filename,int m){
    FILE *fp;
    if(m == 0){
        fp = fopen(filename,"r");
    }
    else if(m == 1){
        fp = stdin;
    }
    char buf[1026];
    int count = 0;
    //error message 
    while(1){
        int num_tabs = 0;
        if(fgets(buf,sizeof(buf),fp) == NULL){
            if(strlen(buf) > 1024){
                fprintf(stderr,"error: length of line exceeds");
                fclose(fp);
                exit(0);
            }
            fclose(fp);
            break;
        }
        else{                
            char *start_ptr = buf;
            while(start_ptr != NULL){
                char* tab_ptr = strchr(start_ptr,'\t');
                if(tab_ptr != NULL){
                    *tab_ptr++ = '\0';
                    num_tabs++;
                }
                start_ptr = tab_ptr;               
            }
            if(num_tabs != 3){
                fprintf(stderr,"file format error\n");
                exit(0);
            }
        }
        //add a new node to linked list
        addnode(list,buf);
        count++;
    }

    return count;
}

static
void BubbleForward(My402List *pList, My402ListElem **pp_elem1, My402ListElem **pp_elem2)
    /* (*pp_elem1) must be closer to First() than (*pp_elem2) */
{
    My402ListElem *elem1=(*pp_elem1), *elem2=(*pp_elem2);
    struct object *obj1=elem1->obj, *obj2=elem2->obj;
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
void BubbleSortForwardList(My402List *pList, int num_items)
{
    My402ListElem *elem=NULL;
    int i=0;

    if (My402ListLength(pList) != num_items) {
        fprintf(stderr, "List length is not %1d in BubbleSortForwardList().\n", num_items);
        exit(1);
    }
    for (i=0; i < num_items; i++) {
        int j=0, something_swapped=FALSE;
        My402ListElem *next_elem=NULL;

        for (elem=My402ListFirst(pList), j=0; j < num_items-i-1; elem=next_elem, j++) {
            struct object* tempobj = elem->obj;
            
            unsigned int cur_val = strtoul(tempobj->time,NULL,10), next_val=0;
            next_elem=My402ListNext(pList, elem);
            struct object* nextobj = next_elem->obj;
            next_val = strtoul(nextobj->time,NULL,10);

            if (cur_val > next_val) {
                BubbleForward(pList, &elem, &next_elem);
                something_swapped = TRUE;
            }
        }
        if (!something_swapped) break;
    }
}
void output(My402List * list,int num_items){
    My402ListElem *currentelem = My402ListFirst(list);
    double balance = 0;
    for(int i = 0;i < num_items;i++){
        struct object* currentobj = currentelem->obj;
        double amount = atof(currentobj->amount);
        if(strcmp(currentobj->type,"+") == 0){
            balance = balance + amount;
        }
        else if(strcmp(currentobj->type,"-") == 0){
            balance = balance - amount;
        }
        else{
            fprintf(stderr,"transaction type error");
            exit(0);
        }
        if(i == 0){
            printf("+-----------------+--------------------------+----------------+-----------------+\n");
        }
        time_t unixtime = strtoul(currentobj->time,NULL,10);
        char time[30];
        char description[24];
        strcpy(time,ctime(&unixtime));
        strcpy(description,currentobj->description);

        // str[strcspn(str,"\n")] = 0;
        time[strcspn(time,"\n")] = 0;
        description[strcspn(description,"\n")] = 0;
        if(amount > 10000000 || balance > 10000000){
            fprintf(stdout,"|%15s|%24s|%14s|%14s|\n",time,description,"?,???,???.??","?,???,???.??");
        }
        else if(strcmp(currentobj->type,"+") == 0){
            fprintf(stdout,"|%15s|%24s|%14.2f|%14.2f|\n",time,description,amount,balance);
        }
        else if(strcmp(currentobj->type,"-") == 0){
            fprintf(stdout,"|%15s|%24s|(%12.2f)|%14.2f|\n",time,description,amount,balance);
        }
        if(i == num_items - 1){
            printf("+-----------------+--------------------------+----------------+-----------------+\n");
        }
        currentelem = currentelem->next;
    }
}

/* ----------------------- main() ----------------------- */

int main(int argc, char *argv[])
{
    My402List* list = (My402List*)malloc(sizeof(My402List));
    My402ListInit(list);
    // printf("%s","+-----------------+--------------------------+----------------+----------------+");
    // printf("%s","|       Date      | Description              |         Amount |        Balance |");
    // printf("%s","+-----------------+--------------------------+----------------+----------------+");
    if(argc == 1){
        return 0;
    }
    else if(strcmp(argv[1],"sort") == 0){
        char *iffile = strchr(argv[2],'\t');
        int num_items;
        if(iffile == NULL){
            int m = 0;
            num_items = processfile(list,argv[2],m);
        }
        else{
            int m = 1;
            num_items = processfile(list,argv[2],m);
        }
        BubbleSortForwardList(list,num_items);
        output(list,num_items);
    }
    return 0;
}


