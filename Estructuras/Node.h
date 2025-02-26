// Node.h
#ifndef NODE_H  // Preprocesador para evitar múltiples inclusiones.
#define NODE_H
#include <string>

using namespace std;

class Node {
public:
    string name;    // Almacena el valor del nodo.
    Node* next;   // Puntero al siguiente nodo en la lista enlazada.

    // Constructor que inicializa el nodo con un valor y establece el puntero next en nullptr.
    // Indicar el valor de priority es opiconal, por defecto es 0.
    // La idea es llamarlo como Node(nombre) en la cola normal y Node(nombre, prioridad) en la cola VIP.
    Node(string name) 
        : name(name), next(nullptr) {}
};

#endif // NODE_H