#ifndef VIPQUEUE_H  // Preprocesador para evitar múltiples inclusiones
#define VIPQUEUE_H

#include <iostream>
#include <vector>
#include <string>
#include "Queue.h"


using namespace std;


class VIPQueue : public Queue{
    public:
        // Sobrecarga del método enqueue para añadir un nuevo parámetro:
        void enqueue(const vector<string>& members, int priority) { // Recibe un vector con los nombres y la prioridad
            if (members.empty()) return; // Si no se incluye ningun nombre, sale de la función
            // Crea la lista enlazada en el Heap y se le asigna un identificador de grupo.
            LinkedList* newGroup = new LinkedList(nextGroupID); 
            nextGroupID++; // Aumenta el siguiente identificador de grupos para que no se repita
            newGroup->priority = priority; // Le asigna prioridad al grupo.
            for (const string& member : members) { // Añade todos los miembros a la lista enlazada
                newGroup->append(member);  
            }
            if (length == 0) {  // Si la cola está vacía, el nuevo grupo será el primero y el último
                first = last = newGroup;
            } else {
                if(priority < first->priority){ // Si tiene un número de prioridad más bajo es el primero de la cola.
                    newGroup->next = first;
                    first = newGroup; 
                } else if(priority >= last->priority) { // Si tiene un número de prioridad más alto es el último de la cola.
                    last->next = newGroup;
                    last = newGroup;
                } else {
                    LinkedList* temp = first; // Se crea un puntero temporal para recorrer la lista.
                    while(priority >= temp->next->priority){ // Recorre la lista hasta que el grupo siguiente tenga un número de prioridad más alto.
                        temp = temp->next;
                    }
                    newGroup->next = temp->next; // Se conecta el nuevo grupo al grupo siguiente de mayor número de prioridad (temp->next).
                    temp->next = newGroup; // Se conecta el grupo de menor o igual número de prioridad (temp) al nuevo grupo.
                }

            }
            length++;  // Incrementar la longitud de la cola
        }

        // Método para actualizar la prioridad de un grupo en la cola.
        void updatePriority(int groupID, int newPriority){
            vector<string> members;
            members = deleteGroup(groupID);
            enqueue(members, newPriority);

        }

};

#endif // VIPQUEUE_H