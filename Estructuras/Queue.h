#ifndef QUEUE_H  // Preprocesador para evitar múltiples inclusiones
#define QUEUE_H

#include <iostream>
#include <vector>
#include <string>
#include "LinkedList.h" // Incluir la definición de la clase LinkedList


using namespace std;

class Queue {
    protected:
        LinkedList* first;  // Puntero al primer nodo de la cola
        LinkedList* last;   // Puntero al último nodo de la cola
        int length;   // Longitud de la cola (número de grupos)
        int nextGroupID; // Identificador de grupo a asignar

    public:
        // Constructor por defecto: inicializa una cola vacía
        Queue() : first(nullptr), last(nullptr), length(0), nextGroupID(1) {}


        // Metodo que determina si la cola esta vacia
        bool isEmpty(){
            if (length == 0) return true;
            else return false;
        }

        // Método para agregar un nuevo elemento al final de la cola
        virtual void enqueue(const vector<string>& members) { // Recibe un vector con los nombres y los lee
            if (members.empty()) return; // Si no se incluye ningun nombre, sale de la función
            // Crea la lista enlazada en el Heap
            LinkedList* newGroup = new LinkedList(nextGroupID); // Se le asigna un ID
            nextGroupID++; // Aumenta el siguiente identificador de grupos para que no se repita
            for (const string& member : members) { // Añade todos los miembros a la lista enlazada
                newGroup->append(member);  
            }
            if (length == 0) {  // Si la cola está vacía, el nuevo grupo será el primero y el último
                first = last = newGroup;
            } else {  // Si no está vacía, agrega el nuevo grupo después de last
                last->next = newGroup;
                last = newGroup;  // El nuevo grupo ahora es el último
            }
            length++;  // Incrementar la longitud de la cola
        }

        // Método para agregar miembros a un grupo existente
        bool joinGroup(const vector<string>& members, int groupID){ // SOlo lee el vector y recibe el identificador a buscar
            LinkedList* temp = first; // Inicia en el primer grupo
            while (temp) { // Mientras esté en la cola:
                if (temp->groupID == groupID){ // Si el grupo actual es el que se está buscando:
                    for (const string& member : members){
                        temp->append(member); //Añade los nuevos miembros al grupo
                    }
                    cout << "Se agregó correctamente" << endl;
                    return true; // Inidca que sí se encontró el grupo
                }
                temp = temp->next; // Si no es el grupo a buscar, pasa al siguiente
            }
            cout << "No existe el grupo" << endl;
            return false; // Indica que el grupo no existe
        }

        // Método para eliminar e imprimir en consola el primer grupo
        void dequeue(){
            if (length == 0) return; // Si no hay grupos sale de la función
            LinkedList* temp = first; // Selecciona el primer grupo
            first = first->next; // Ahora el primero va a ser el grupo siguiente (nullptr si no hay nadie)
            cout << "\nAtendiendo a: \n"; // Se imprime en la consola los nombres de quienes ingresan
            temp->showGroup(); // Muestra los integrantes del grupo
            cout << endl;
            delete temp; //Elimina el grupo de la fila
            length --; //Se reduce la longitud de la cola
            if (!first) last = nullptr; //Si la lista queda vacía el último apunta a nullptr
            if(length==0){
                nextGroupID = 1;
            }
        }

        // Método para verificar si se encontró y se eliminó un grupo con cierto ID.
        vector<string> deleteGroup(int groupID){
            if(length == 0){ // Si la lista está vacía no hace nada.
                return vector<string>(); // No se eliminó ningún Grupo.
            }

            if(groupID == first->groupID){ // Si el ID se encuetra en el primer grupo, se elimina similar al método dequeue.
                LinkedList* temp = first; // Selecciona el primer grupo
                first = first->next; // Ahora el primero va a ser el grupo siguiente (nullptr si no hay nadie)
                if(!first) last = nullptr; //Si la lista queda vacía el último apunta a nullptr
                length --; //Se reduce la longitud de la cola
                
                vector<string> members = temp->getMembers(); 
                delete temp; //Elimina el grupo de la fila
                return members; // Se elimina el grupo.

            } else{ // Si no, se recorre la lista hasta encontrar el valor.
                LinkedList* temp = first;
                while(temp->next){ // El ciclo continúa hasta el final de la lista.
                    if(temp->next->groupID == groupID){ // Verifica si el valor a eliminar es el del grupo siguiente a temp. 
                        LinkedList* delGroup = temp->next; // Crea un puntero para ubicar el grupo a eliminar.
                        temp->next = temp->next->next; // Se conecta el grupo temp, al grupo siguiente al que se va a eliminar.
                        if(delGroup == last){ // Si el grupo a eliminar es el último, se actualiza el puntero last.
                            last = temp;
                        }
                        length--; // Se reduce la longitud de la cola.
                        
                        vector<string> members = delGroup->getMembers(); 
                        delete delGroup; //Elimina el grupo de la fila
                        return members; // Se elimina el grupo.
                      
                    }
                    temp = temp->next; // Si no encuntra el valor, pasa al siguiente grupo.
                }
                return vector<string>(); // No se eliminó ningún nodo.
            }
        }

        LinkedList* getGroup(int groupID){
            LinkedList* temp = first; 
            while (temp) { 
                if (temp->groupID == groupID){ 
                    return temp; 
                }
                temp = temp->next; 
            }
            return nullptr;
        }

        // Mostrar la cola de grupos
        void showQueue() {
            if (length == 0) return;
            cout << "\nEstado de la fila: "<< endl;
            LinkedList* temp = first;
            while (temp) {
                temp->showGroup();
                cout<<endl;
                temp = temp->next;
            }
            cout << endl;
        }

        // Método que devuelve el primer grupo
        LinkedList* PrimerElemento()
        {
            if (length == 0){
                return last;                   // Si no hay grupos sale de la función
            }else{
            return first;
            }
        }

        // Metodo para obtener la longitud de la cola
        int getLength(){
            return length;
        }


};

#endif // QUEUE_H
