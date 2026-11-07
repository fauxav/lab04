#include <stdio.h>                                  
#include <stdlib.h>                                  

// Node structure definition
typedef struct Node {
    int data;                                        // the value stored in this node
    struct Node* next;                               // pointer to the next node (NULL if last)
} Node;

// Function to insert a node at the end of the list
void insertNode(Node** head, int data) {
    Node* newNode = (Node*)malloc(sizeof(Node));     // make space for the new node
    Node* temp = *head;                              // start walking from the head
    newNode->data = data;                            // store the value
    newNode->next = NULL;                            // its gonna be last so next is NULL

    if (*head == NULL) {
        *head = newNode; // If list is empty, new node becomes the head
    } else {
        while (temp->next != NULL) {                 // keep going til the last node
            temp = temp->next;
        }
        temp->next = newNode; // Add new node at the end
    }
}

// Function to print the linked list
void printList(Node* head) {
    Node* temp = head;                               // start at the front
    while (temp != NULL) {                           // go til we fall off the end
        printf("%d -> ", temp->data);                // print this node's value
        temp = temp->next;                           // move to the next one
    }
    printf("NULL\n");                                // show where the list ends
}

// Function to delete a node by value
void deleteNode(Node** head, int data) {
    Node *temp = *head, *prev = NULL;                // temp = current, prev = the one behind it

    // If the node to be deleted is the head
    if (temp != NULL && temp->data == data) {
        *head = temp->next; // Move head to the next node
        free(temp);                                  // give the old head's memory back
        return;
    }

    // Search for the node to be deleted
    while (temp != NULL && temp->data != data) {
        prev = temp;                                 // remember the node before
        temp = temp->next;                           // step forward
    }

    // If the node was not found
    if (temp == NULL) {
        printf("Node with data %d not found.\n", data);
        return;
    }

    // Unlink the node from the list
    prev->next = temp->next;                         // skip over temp
    free(temp);                                      // free the removed node
}

int main() {
    Node* head = NULL;    // Initialize an empty list

    insertNode(&head, 10);   // Insert nodes
    insertNode(&head, 20);
    insertNode(&head, 30);

    printf("Linked List: ");
    printList(head); 

    deleteNode(&head, 20); // Delete a node
    printf("Linked List after deletion: ");
    printList(head); // Print list after deletion

    return 0;
}