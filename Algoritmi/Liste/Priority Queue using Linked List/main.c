#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int data;
    int priority;
    struct Node *next;
}Node;

Node * newNode(int d, int p) {
    Node * temp = (Node *)malloc(sizeof(Node)); //alocam memorie si cream nodul
    temp->data = d;
    temp->priority = p;
    temp->next = NULL;
    return temp;
}

int peek(Node *head) { //returneaza valoarea din capul cozii
    return head->data;
}

Node *pop(Node *head) { //sterge elem cu cea mai mare prioritate din lista, adica cel din capul listei
    Node *temp = head;
    head = head->next;
    free(temp);
    return head;
}

//Functie pentru adaugarea unui nod in coada in functie de prioritatea sa
Node *push(Node *head, int d, int p) {
    Node *start = head;

    Node *temp = newNode(d, p); //creem un nou nod cu datele primite

    //cazul prioritatea elem e mai mare decat head
    if (head->priority > p) { //inserare in fata
        temp->next = head;
        head = temp;
    }
    else {
        //Parcurgem lista ca sa gasim pozitia curenta
        //unde sa inseram noul nod bazat pe prioritate
        while (start->next != NULL && start->next->priority < p) {
            start = start->next;
        }
        //insereaza nodul la noua pozitie gasita
        temp->next = start->next;
        start->next = temp;
    }
    return head; //returnal capul listei modificate
}

//Functie pentru a verifica daca lista e goala
int isEmpty(Node *head) {
    return (head == NULL);
}

void printList(Node *head) { //afisarea si totodata stergerea si eliberarea listei
    while (!isEmpty(head)) {
        printf("%d ", peek(head));
        head = pop(head);
    }
}

int main(void) {
    Node *q = newNode(4, 1);
    q = push(q, 5, 3);
    q = push(q, 6, 2);
    q = push(q, 7, 0);
    printList(q);

    return 0;
}