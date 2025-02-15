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

        // Metodo que determina si la cola esta vacia
        bool isEmpty(){
            if (length == 0) return true;
            else return false;
        }

        // Metodo que agrega un grupo de elementos en cualquier parte de la fila
        void enqueue(string name, int groupSize){
            if (length == 0){ // Si la lista esta vacia, el primer nodo se hace a mano, con grp = 1
                Node *newNode = new Node(name, 1);
                first = newNode;
                last = newNode;

                for (int i = 0; i < groupSize - 1; i++){ // Se agregan el resto de nodos
                    Node *newNode2 = new Node(name, 1);
                    last->next = newNode2;
                    last = newNode2;
                }
                length += groupSize;

            } else {  
                Node *temp = first;
                while (temp){  // Se recorre la lista para ver si el grupo ya existe

                    if ((temp->name == name) && (temp == last)){ // Caso de que el grupo existente es el ultimo de la fila
                        for (int i = 0; i < groupSize; i++){ // Crea los nodos correspondientes al grupo y los pone al final del mismo
                            Node *newNode = new Node (name, temp->grp);
                            last->next = newNode;
                            last = newNode;
                        }
                    length += groupSize;
                    return; // Despues de agregar al grupo, se termina la funcion
                    
                    } else if ((temp->name == name) && (temp->next->name != name)){ // Caso de que el grupo existente NO es el ultimo
                        for (int i = 0; i < groupSize; i++){ 
                            Node *newNode = new Node (name, temp->grp);
                            newNode->next = temp->next; // No se modifica last, solo temp
                            temp->next = newNode;

                            temp = newNode;
                        }
                    length += groupSize;
                    return; 

                    } else temp = temp->next; // Si el grupo no existe, se llega al final y termina el while
                }
                
                // Si el grupo no existe, se agrega al final de la lista
                int currGroup = last->grp + 1; // Los numeros de grupo son consecutivos
                for (int i = 0; i < groupSize; i++){
                    Node *newNode = new Node (name, currGroup);
                    last->next = newNode; // Modifica last, usa nuevo numero de grupo
                    last = newNode;
                }
            }
            length += groupSize; // Se aumenta el largo de la fila
        }

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
                cout << "La fila esta vacia, ningun elemento para eliminar" << endl;
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
                cout << "La fila esta vacia, ningun grupo para eliminar" << endl;
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