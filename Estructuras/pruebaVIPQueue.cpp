#include <iostream>
#include <sstream>  // Para usar stringstream
#include <vector>
#include "Queue.h"
#include "VIPQueue.h"
using namespace std;

void IngresarNombres(vector<string>& names) {

    string input;
    string name;
    stringstream ss; 

    cout << "Ingrese los nombres separados por espacios (presione Enter para terminar): ";
    // Leer toda la línea de entrada
    getline(cin, input);

    // Se usa stringstream para dividir la línea en palabras
    ss.clear();  // Limpiar cualquier contenido previo del stringstream
    ss.str(input);  // Asignar el nuevo input al stringstream

    // Extraer cada nombre del stringstream y agregarla al vector
    while (ss >> name) {
        names.push_back(name);
    }

}



int main() {
    VIPQueue cola;
    int option;  // Para elegir las opciones del menú

    vector<string> names;
    int groupID;
    int priority;
 

    while (true) {
        // Mostrar el menú de opciones
        cout << "\n=== Menú ===\n";
        cout << "1. Agregar grupo\n";
        cout << "2. Unir a un grupo\n";
        cout << "3. Pasar primer grupo\n";
        cout << "4. Eliminar grupo\n";
        cout << "5. Cambiar prioridad\n";
        cout << "6. Salir\n";
        cout << "Seleccione una opción: ";
        cin >> option;
        cin.ignore();  // Ignorar el salto de línea del input anterior

        switch (option) {
            case 1:  // Opción para agregar nombres
                IngresarNombres(names);
                cout << "Ingrese el número de prioridad (presione Enter para terminar): ";
                cin >> priority;
                cola.enqueue(names, priority);
                names.clear();  // Limpiar el vector para el siguiente uso
                cola.showQueue();
                break;

            case 2:  // Opción para unir a un grupo
                IngresarNombres(names);
                cout << "Ingrese el número de grupo (presione Enter para terminar): ";
                cin >> groupID;
                cola.joinGroup(names, groupID);
                names.clear();  // Limpiar el vector para el siguiente uso
                groupID = 0;  // Limpiar el groupID para el siguiente uso
                cola.showQueue();
                break;

            case 3:  // Opción para pasar primer grupo
                cola.dequeue();
                cola.showQueue();
                break;

            case 4:  // Opción para eliminar grupo
                cout << "Ingrese el número de grupo (presione Enter para terminar): ";
                cin >> groupID;
                cola.deleteGroup(groupID);
                cola.showQueue();
                break;

            case 5:  // Opción para cambiar prioridad
                cout << "Ingrese el número de grupo (presione Enter para terminar): ";
                cin >> groupID;
                cout << "Ingrese el nuevo número de prioridad (presione Enter para terminar): ";
                cin >> priority;
                cola.updatePriority(groupID, priority);
                cola.showQueue();
                break;

            case 6:  // Opción para salir
                cout << "Saliendo del sistema...\n";
                return 0;

            default:
                cout << "Opción inválida. Intente nuevamente.\n";
                break;
        }
    }

    return 0;
}
