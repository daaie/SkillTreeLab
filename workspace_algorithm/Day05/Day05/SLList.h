#pragma once

typedef struct tagNode
{
	int Value;
	tagNode *Next;
}Node;

Node *NewNode(int Value);
void AddLastNew(Node* PHead, int Value);
void DeleteNode(Node** PHead, int Value);
void PrintNode(Node *PHead);