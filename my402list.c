#include <stdlib.h>
#include "cs402.h"
#include "my402list.h"


My402ListElem *My402ListFirst(My402List* ddl) {
	if(ddl->anchor.next == NULL)
		return NULL;
	else
		return ddl->anchor.next;
}

My402ListElem *My402ListLast(My402List* ddl) {
	if(ddl->anchor.prev == NULL)
		return NULL;
	else
		return ddl->anchor.prev;
}

My402ListElem *My402ListNext(My402List* ddl, My402ListElem* elt) {
	if(elt->next==&(ddl->anchor))
		return NULL;
	else
		return elt->next;
}

My402ListElem *My402ListPrev(My402List* ddl, My402ListElem* elt) {
	if(elt->prev==&(ddl->anchor))
		return NULL;
	else
		return elt->prev;
}

My402ListElem *My402ListFind(My402List* ddl, void* obj_data) {
	My402ListElem *elt=NULL;
	for (elt=My402ListFirst(ddl); elt != NULL;) 
	{
        if (elt->obj == obj_data)
        	return elt;
        elt=My402ListNext(ddl, elt);
    }
    return NULL;
}

int My402ListInit(My402List* ddl) {
	ddl->anchor.next=NULL;
	ddl->anchor.prev=NULL;
	ddl->anchor.obj=NULL;
	return TRUE;
}

int My402ListLength(My402List* ddl) {
	
	/*
	int length = 0;
	My402ListElem *elt=NULL;
	
	for (elt=My402ListFirst(ddl); elt != NULL;) 
	{
        length++;
        elt=My402ListNext(ddl, elt);
    }
    return length;
    */
    return ddl->num_members;
}

void My402ListUnlink(My402List* ddl, My402ListElem* elt) {
	if(elt->prev==&(ddl->anchor) && elt->next==&(ddl->anchor)) {
		ddl->anchor.next=NULL;
		ddl->anchor.prev=NULL;
	} else {
		elt->next->prev=elt->prev;
		elt->prev->next=elt->next;
	}
	ddl->num_members--;
	//free(elt->obj);
	free(elt);
}

void My402ListUnlinkAll(My402List* ddl) {
	My402ListElem *elt=NULL,*temp=NULL;
	for (elt=My402ListFirst(ddl); elt != NULL;) 
	{
		
		temp=My402ListNext(ddl, elt);
		My402ListUnlink(ddl, elt);
		elt=temp;
    }
    ddl->anchor.next = NULL;
	ddl->anchor.prev = NULL;
	ddl->num_members =0;
}

int My402ListInsertAfter(My402List* ddl, void *obj_data, My402ListElem *elt) {
	if(elt->prev==&(ddl->anchor) && elt->next==&(ddl->anchor))
		return My402ListAppend(ddl,obj_data);
	else {
		My402ListElem* temp=(My402ListElem*) malloc(sizeof(My402ListElem));
		if(temp==NULL)
			return FALSE; //Malloc issue
		
		temp->obj=obj_data;
		temp->next=elt->next;
		temp->prev=elt;
		elt->next->prev=temp;
		elt->next=temp;
		ddl->num_members++;
		return TRUE;
	}
}

int My402ListInsertBefore(My402List* ddl, void *obj_data, My402ListElem *elt) {
	if(elt->prev==&(ddl->anchor) && elt->next==&(ddl->anchor))
		return My402ListPrepend(ddl,obj_data);
	else {
		My402ListElem* temp=(My402ListElem*) malloc(sizeof(My402ListElem));
		if(temp==NULL)
			return FALSE; //Malloc issue
		
		temp->obj=obj_data;
		temp->prev=elt->prev;
		temp->next=elt;
		elt->prev->next=temp;
		elt->prev=temp;
		ddl->num_members++;
		return TRUE;
	}
}

int My402ListEmpty(My402List* ddl) {
	/*
	int length ;
	length = My402ListLength(ddl);
	if(length == 0)
		return TRUE;
	else
		return FALSE;
	*/

	if(ddl->num_members == 0)
		return TRUE;
	else
		return FALSE;
}

int  My402ListAppend(My402List* ddl, void* obj_data) {

	My402ListElem* temp=(My402ListElem*) malloc(sizeof(My402ListElem));
	if(temp == NULL)
		return FALSE; //malloc issue
	temp->obj = obj_data;

	if (My402ListEmpty(ddl) == TRUE) {
		temp->prev = &(ddl->anchor);
		temp->next = &(ddl->anchor);
		ddl->anchor.prev = temp;
		ddl->anchor.next = temp;
	} else {
		My402ListElem* temp2 = My402ListLast(ddl);
		temp->prev = temp2;
		temp->next = &(ddl->anchor);
		ddl->anchor.prev = temp;
		temp2->next = temp;
	}
	ddl->num_members++;
	return TRUE;
}

int  My402ListPrepend(My402List* ddl, void* obj_data) {

	My402ListElem* temp=(My402ListElem*) malloc(sizeof(My402ListElem));
	if(temp == NULL)
		return FALSE; //malloc issue
	temp->obj = obj_data;

	if (My402ListEmpty(ddl) == TRUE) {
		temp->prev = &(ddl->anchor);
		temp->next = &(ddl->anchor);
		ddl->anchor.prev = temp;
		ddl->anchor.next = temp;
	} else {
		My402ListElem* temp2 = My402ListFirst(ddl);
		temp->prev=&(ddl->anchor);
		temp->next=temp2;
		temp2->prev=temp;
		ddl->anchor.next=temp;	
	}
	ddl->num_members++;
	return TRUE;
}
