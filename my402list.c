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
 * @(#)$Id: my402list.h,v 1.1 2018/12/26 17:15:59 william Exp $
 */
//Length
#include "my402list.h"
#include <stdio.h>
#include <stdlib.h>
// My402ListElem* head;
// My402ListElem* tail;
// struct object
// {
// 	char* date;
// 	char* description;
// 	double amount;
// 	double balance;
// };

int  My402ListLength(My402List* list){ 
	return list->num_members;
}

//Empty
int  My402ListEmpty(My402List* list){
	if(list->num_members == 0){
		return 1;
	}
	else{
		return 0;
	}
}
//append
int  My402ListAppend(My402List* list, void* newobj){
	My402ListElem* newelem = (My402ListElem*)malloc(sizeof(My402ListElem));
	newelem->obj = newobj;
	newelem->next = NULL;
	newelem->prev = NULL;
	My402ListElem* tail = list->anchor.prev;
	// if(newobj == NULL){
	// 	return 0;
	// }
	if(My402ListEmpty(list) == 1){
		newelem->next = &(list->anchor);
		newelem->prev = &(list->anchor);
		list->anchor.prev = newelem;
		list->anchor.next = newelem;
		list->num_members++;
		return 1;
	}
	else{
		tail->next = newelem;
		newelem->prev = tail;
		newelem->next = &(list->anchor);
		list->anchor.prev = newelem;
		list->num_members++;
		return 1;
	}
}
//prepend
int  My402ListPrepend(My402List* list, void* newobj){
	My402ListElem* newelem = (My402ListElem*)malloc(sizeof(My402ListElem));
	newelem->obj = newobj;
	newelem->next = NULL;
	newelem->prev = NULL;
	My402ListElem* head = list->anchor.next;
	if(My402ListEmpty(list) == 1){
		newelem->next = &(list->anchor);
		newelem->prev = &(list->anchor);
		list->anchor.prev = newelem;
		list->anchor.next = newelem;
		list->num_members++;
		return 1;
	}
	else{
		head->prev = newelem;
		newelem->next = head;
		newelem->prev = &(list->anchor);
		list->anchor.next = newelem;
		list->num_members++;
		return 1;
	}
}
//unlink
void My402ListUnlink(My402List* list, My402ListElem* elem){
	if(elem == NULL){
		return;
	}
	elem->prev->next = elem->next;
	elem->next->prev = elem->prev;
	list->num_members = list->num_members - 1;
	free(elem);
	return;
}
//unlinkall
void My402ListUnlinkAll(My402List* list){
	My402ListElem* head = list->anchor.next;
	My402ListElem* current = head;
	My402ListElem* temp = current;
	while(current != &list->anchor){
		temp = current->next;
		free(current);
		current = temp;
	}
	list->num_members = 0;
	return;
}
//insertafter
int  My402ListInsertAfter(My402List* list, void* newobj, My402ListElem* elem){

	if(elem == NULL){
		My402ListAppend(list,newobj);
		return 1;
	}
	else{
		My402ListElem* newelem = (My402ListElem*)malloc(sizeof(My402ListElem));
		newelem->obj = newobj;
		newelem->prev = elem;
		newelem->next = elem->next;
		elem->next->prev = newelem;
		elem->next = newelem;
		
		list->num_members++;
		return 1;
	}
	return 0;
}

int  My402ListInsertBefore(My402List* list, void* newobj, My402ListElem* elem){

	if(elem == NULL){
		My402ListPrepend(list,newobj);
		return 1;
	}
	else{
		My402ListElem* newelem = (My402ListElem*)malloc(sizeof(My402ListElem));
		newelem->obj = newobj;
		newelem->prev = elem->prev;
		newelem->next = elem;
		elem->prev->next = newelem;
		elem->prev = newelem;
		list->num_members++;
		return 1;
	}
}

My402ListElem *My402ListFirst(My402List* list){
	My402ListElem* head = list->anchor.next;
	return head;
}
My402ListElem *My402ListLast(My402List* list){
	My402ListElem* tail = list->anchor.prev;
	return tail;
}
My402ListElem *My402ListNext(My402List* list, My402ListElem* elem){
	My402ListElem* tail = list->anchor.prev;
	if(elem == tail){
		return NULL;
	}
	return elem->next;
}
My402ListElem *My402ListPrev(My402List* list, My402ListElem* elem){
	My402ListElem* head = list->anchor.next;
	if(elem == head){
		return NULL;
	}
	return elem->prev;
}

My402ListElem *My402ListFind(My402List* list, void* obj_target){
	My402ListElem* head = list->anchor.next;
	My402ListElem* current = head;
	while(current->obj != obj_target && current != &(list->anchor)){
		current = current->next;
	}
	return current;
}

int My402ListInit(My402List* list){
	list->num_members = 0;
	(list->anchor).obj = NULL;
	list->anchor.prev = &(list->anchor);
	list->anchor.next = &(list->anchor);
	return 1;
}
