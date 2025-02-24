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
        void enqueue(string name, int groupSize) {
            // Si la cola está vacía, agregamos el primer nodo
            if (length == 0) {
                Node *newNode = new Node(name, 1); // El primer nodo siempre tendrá grupo 1
                first = newNode;
                last = newNode;
                length = 1;
        
                // Agregamos el resto de los nodos si groupSize > 1
                for (int i = 1; i < groupSize; i++) {
                    Node *newNode2 = new Node(name, 1); // Todos los nodos tienen el mismo nombre y grupo
                    last->next = newNode2;
                    last = newNode2;
                }
                length += groupSize - 1; // Aumentamos la longitud de la cola
            } else {
                Node *temp = first;
                bool groupFound = false;
        
                // Buscamos si ya existe el grupo, si es así agregamos al final del mismo
                while (temp) {
                    if (temp->name == name) {
                        // Caso: el grupo ya existe
                        for (int i = 0; i < groupSize; i++) {
                            Node *newNode = new Node(name, temp->grp); // Los nuevos nodos se agregan con el mismo grupo
                            last->next = newNode;
                            last = newNode;
                        }
                        length += groupSize; // Aumentamos la longitud de la cola
                        groupFound = true;
                        break;
                    }
                    temp = temp->next;
                }
        
                // Si no encontramos el grupo, lo agregamos al final de la cola con un nuevo grupo
                if (!groupFound) {
                    int currGroup = last->grp + 1; // Incrementamos el grupo
                    for (int i = 0; i < groupSize; i++) {
                        Node *newNode = new Node(name, currGroup);
                        last->next = newNode;
                        last = newNode;
                    }
                    length += groupSize; // Aumentamos la longitud de la cola
                }
            }
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
            Node* temp = first;
            first = first->next;
            if (first == nullptr){
                last = nullptr;
            }
            delete temp;
            // Se elimina el nodo
            length--;

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

                // Metodo que devuelve el primer ELEMENTO (no grupo) de la fila
        Node* PrimerElemento(){ 
            if (length == 0){
                return last;
            }else {
                return first;
            }
            }

        int longitud(){
            return length;
        }

};
