/*
Aca debemos implementar la ejecucion de las colas:
1. hacer dos colas, la vip y la normal. Solo poder dequeue la normal cuando la vip este vacia. Se puede hacer enqueue a cualquiera
    Esto lo manejaria asi:
    Queue vip
    Queue regular
    void dequeue():
        if vip.isempty: regular.dequeue
        else: vip.dequeue

2. Puse dos metodos dequeue, uno que elimina el grupo completo, y otro que elimina a la primera persona.
3. Para ingresar personas, se les pregunta si son vips o regulares, y cuantas personas hay en su grupo.
4. Se tiene la capacidad de 'guardar campo': si alguien de un grupo ya esta metido en la cola, los demas de ese mismo grupo pueden
    adelantarse y ponerse todos a la par

*/

#include <iostream>
#include <string>
#include "Queue.h"
#include "VIPQueue.h"
#include "LinkedList.h"
#include <sstream>  // Para usar stringstream
#include <vector>
#include <SFML/Graphics.hpp>

using namespace std;

VIPQueue CVIP;
Queue CRegular;

int AnchoElemento = 100;
int LargoElemento = 50;
int DistanciaEntreElementos = 20;

struct Movimiento {
    bool enMovimiento = false;
    float offset = 0;
};

Movimiento movVIP, movRegular;
float velocidadMovimiento = 0.08f; // Pixeles por frame

void IngresarNombres(vector<string>& names) {

    string input;
    string name;
    stringstream ss; 

    cout << "Ingrese los nombres separados por espacios (presione Enter para terminar): ";
    // Leer toda la línea de entrada
    getline(cin, input);

    // Usamos el stringstream para dividir la línea en palabras
    ss.clear();  // Limpiar cualquier contenido previo del stringstream
    ss.str(input);  // Asignar el nuevo input al stringstream

    // Extraer cada palabra (nombre) del stringstream y agregarla al vector
    while (ss >> name) {
        names.push_back(name);
    }

}

void AgregarElementoACola(bool VIP, const vector<string>& miembros, int Prioridad){
    if (VIP == true)
    {
        CVIP.enqueue(miembros, Prioridad);
    }
    else
    {
        CRegular.enqueue(miembros);
    }
}

void ColaPorPrioridad() {
    if (!CVIP.isEmpty() && !movVIP.enMovimiento) {
        movVIP.enMovimiento = true;
        movVIP.offset = 0;
    } else if (CVIP.isEmpty() && !CRegular.isEmpty() && !movRegular.enMovimiento) {
        movRegular.enMovimiento = true;
        movRegular.offset = 0;
    }
}


void Elementos(sf::RenderWindow &window, Queue &queue, int y, sf::Color color, Movimiento &mov) {
    if (queue.isEmpty()) return;
    
    LinkedList* temp = queue.PrimerElemento();
    int x = 250 - mov.offset; // Aplicar desplazamiento inicial

    while (temp != nullptr) {
        sf::RectangleShape Elemento(sf::Vector2f(AnchoElemento, LargoElemento));
        Elemento.setPosition(sf::Vector2f(x, y));
        Elemento.setFillColor(color);
        window.draw(Elemento);

        x += AnchoElemento + DistanciaEntreElementos;
        temp = temp->next;
    }

    // Movimiento progresivo
    if (mov.enMovimiento) {
        mov.offset += velocidadMovimiento;
        if (mov.offset >= AnchoElemento + DistanciaEntreElementos) {
            mov.enMovimiento = false;
            mov.offset = 0; // Reiniciar desplazamiento para los demás elementos
            queue.dequeue(); // Ahora eliminamos el primer elemento después de la animación
        }
    }
}


// Script de prueba
int main(){
    int option;  // Para elegir las opciones del menú
int salir = 0;

vector<string> names;
int groupID;
int priority;
int SVIP;

// Declaramos el stringstream fuera del switch

while (salir != 9) {
    // Mostrar el menú de opciones
    cout << "\n=== Menú ===\n";
    cout << "1. Agregar grupo\n";
    cout << "2. Salir\n";
    cout << "3. Mostrar Grupo\n";
    cout << "Seleccione una opción: ";
    cin >> option;
    cin.ignore();  // Ignorar el salto de línea del input anterior

    switch (option) {
        case 1:  // Opción para agregar nombres
            IngresarNombres(names);
            cout << "Presione 1 si es VIP o 2 si es regular" << endl;
            cin >> SVIP;
            if (SVIP == 1){
            cout << "Ingrese el número de prioridad (presione Enter para terminar): ";
            cin >> priority;
            AgregarElementoACola(true, names, priority);
            names.clear();  // Limpiar el vector para el siguiente uso
            break;
        }else if (SVIP == 2){
                priority = 0;
                AgregarElementoACola(false, names, priority);
                names.clear();  // Limpiar el vector para el siguiente 
                break;  
            }
        case 2:  // Opción para salir
            salir = 9;
            break;
        case 3: // Opción para salir
            CVIP.showQueue();
            CRegular.showQueue();
            break;

        default:
            cout << "Opción inválida. Intente nuevamente.\n";
            break;
    }
}

    sf::RenderWindow window(sf::VideoMode({900, 600}), "Cola en el parque de Diversiones");
    sf::Clock relog;
    while (window.isOpen()){
        sf::Time actual = relog.getElapsedTime();
        while (const optional event = window.pollEvent()){
            if (event->is<sf::Event::Closed>()){
                window.close();
            }
        }
        if (actual.asSeconds() > 2){
            ColaPorPrioridad();
            relog.restart();
        }
        window.clear();
        Elementos(window, CVIP, 100, sf::Color::Red, movVIP);
        Elementos(window, CRegular, 200, sf::Color::Blue, movRegular);
        window.display();
    }
    return 0;
}
