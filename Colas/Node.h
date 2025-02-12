// Se define la clase nodo
// Basado en el codigo del repositorio del curso

#include <string>
using namespace std;
class Node{
    public:
        string name; // El nombre del grupo
        int grp; // numero de grupo
        Node *next;

        Node(string name, int group){
            this->name = name;
            this->grp = group;  
            this->next = nullptr;
        }
};