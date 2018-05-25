#pragma once

typedef struct tagNode
{
	int Value;
	tagNode *Next;
}Node;


Node *NewNode(int Value);

void PrintNode(Node *head);
void AddLastNode(Node **head, int i);
void AddHead(Node** head, int i);
void AddAfter(Node **head, int i, int value);
void AddBefore(Node **head, int i, int value);
void DelNode(Node **head, int value);
void DestroyNode(Node **Head);