#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include <iostream>
#include "Node.h"  // Incluir la definición de la clase Node

using namespace std;


class LinkedList {
    public:
        int groupID;  // Identificador de grupo.
        Node* head;   // Puntero al primer nodo de la lista
        Node* tail;   // Puntero al último nodo de la lista
        LinkedList* next; // Puntero a la siguiente lista enlazada (para conectar listas dentro de la cola)
        int length;   // Longitud de la lista enlazada
        
        // Constructor
        // Recibe como parámetro un identificador de grupo
        LinkedList(int groupID) 
            : groupID(groupID), head(nullptr), tail(nullptr), next(nullptr), length(0) {}

    
        // Destructor que elimina todos los nodos de la lista para liberar memoria
        ~LinkedList() {
            while (head) {
                Node* temp = head;
                head = head->next;
                delete temp;
            }
            tail = nullptr;  
            length = 0;      
        }

        // Método para agregar un nodo al final de la lista
        void append(const string& name) { // Solo se lee el nombre
            Node* newNode = new Node(name); // Crea un nuevo nodo con el nombre indicado
            if (length == 0) { // Si está vacía, head y tail son el nuevo nodo
                head = tail =newNode;
            } else { // Si no, se agrega después de tail
                tail->next = newNode;
                tail = newNode; // Ahora tail es el nodo agregado
            }
            length++; // Se incrementa la longitud
        }

        //Método para mostrar integrantes del grupo
        void showGroup() {
            Node* temp = head;
            cout << "Grupo " << groupID << ": ";
            while (temp) {
                cout << temp->name << " ";
                temp = temp->next;
            }
        }



        
   
};

#endif // LINKEDLIST_H