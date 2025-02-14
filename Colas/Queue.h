// Codigo con la clase Queue, basado en el repositorio del curso

#include <iostream>
#include <string>
#include "Node.h"
using namespace std;

class Queue{
    private:
        Node *first;
        Node *last; // primer y ultimo de la fila
        int length;

    public:
        Queue(){// Por defecto, crea fila vacia
            first = nullptr;
            last = nullptr;
            length = 0;
        }

        Queue(string name){ // Crea fila con un elemento
            Node *newNode = new Node(name, 1); // el primer elemento es el grupo 1
            first = newNode;
            last = newNode;
            length = 1;
        }

        // Metodo que agrega un grupo de elementos al final de la fila
        void enqueue(string name, int groupSize){
            if (length == 0){ // Si la lista esta vacia, el primer nodo se hace a mano, con grp = 1
                Node *newNode = new Node(name, 1);
                first = newNode;
                last = newNode;

                for (int i = 0; i < groupSize - 1; i++){
                    Node *newNode2 = new Node(name, 1);
                    last->next = newNode2;
                    last = newNode2;
                }

            } else {  
                int currGroup = last->grp + 1; // Los numeros de grupo son consecutivos
                for (int i = 0; i < groupSize; i++){ // Crea los nodos correspondientes al grupo
                    Node *newNode = new Node (name, currGroup);
                    last->next = newNode;
                    last = newNode;
                }
            }
            length += groupSize; // Se aumenta el largo de la fila
        }

        // Poner metodo para aumentar el tamaño de grupos ya existentes

        // Metodo que se disminuye por 1 el numero de grupo de todos los elementos
        // Se usa en dequeue
        void updateGroupNum(){
            Node *temp = first;

            while (temp){  // Recorre la cola y hace el cambio un nodo a la vez
                temp->grp --;
                temp = temp->next;
            }
        }

       // Metodo que elimina el primer ELEMENTO (no grupo) de la fila
        void dequeueSingle(){ 
            if (length == 0){// Si la lista esta vacia
                cout << "La fila esta vacia, nada para eliminar" << endl;
                return;
            } 

            Node *temp = first;
            if (length == 1){ // Si la fila solo tiene un elemento
                first = nullptr;
                last = nullptr;
                cout << "La fila quedo vacia" << endl;
            }
            else{
                first = first->next;
                delete temp;
            }
             // Se elimina el nodo
            length --;

            // Actualizar numeros de grupo, si es necesario
            if (length != 0 && first->grp != 1) updateGroupNum();
        }

        // Metodo que elimina el primer GRUPO de la fila
        void dequeueGroup(){
            if (length == 0){// Si la lista esta vacia
                cout << "La fila esta vacia, nada para eliminar" << endl;
                return;
            } 
            Node *temp2 = first;

            // Se determina de que tamaño es el grupo
            int groupSize = 1;
            while(temp2->next->grp == 1){
                groupSize ++;
                temp2 = temp2->next;
                if (temp2->next == nullptr) break;
            }
            
            // Se corre dequeueSingle la cantidad de veces requerida
            for (int i = 0; i < groupSize; i++){
                dequeueSingle();
            }

        }

        // Metodo para imprimir la cola
        void printQueue() {
            if (length == 0){
                cout << "La fila esta vacia, nada para imprimir" << endl;
            }else{
                Node* temp = first;
                while (temp) {  
                    cout <<"Name: " << temp->name <<", group: " << temp->grp << endl;  
                    temp = temp->next; 
                }
            }
        }

};

// Node: string name, int grp, node next