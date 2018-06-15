#pragma once

//#ifndef __SLIST_H__
//#define __SLIST_H__

typedef struct tagNode
{
	int Value;
	tagNode* Next;
} Node;

Node* NewNode(int Value);
Node* Search(Node** PointerHead, int Value);
Node* Search(Node** PointerHead, Node* SearchNode);
void AddLastNew(Node** PointerHead, int i);
void AddFirstNew(Node** PointerHead, int i);
void AddAfterSearch(Node** PointerHead, int SearchValue, int NewValue);
void DeleteNode(Node** PointerHead, int SearchValue);
void Destory(Node** PointerHead);

//#endif