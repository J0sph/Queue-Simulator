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
#include <SFML/Graphics.hpp>

using namespace std;

Queue CVIP;
Queue CRegular;

int AnchoElemento = 100;
int LargoElemento = 50;
int DistanciaEntreElementos = 20;

struct Movimiento {
    bool enMovimiento = false;
    float offset = 0;
};

Movimiento movVIP, movRegular;
float velocidadMovimiento = 0.1f; // Pixeles por frame

void AgregarElementoACola(bool VIP, string nombre, int tamañoGrupo){
    if (VIP == true)
    {
        CVIP.enqueue(nombre, tamañoGrupo);
    }
    else
    {
        CRegular.enqueue(nombre, tamañoGrupo);
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
    
    Node* temp = queue.PrimerElemento();
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
            queue.dequeueSingle(); // Ahora eliminamos el primer elemento después de la animación
        }
    }
}


// Script de prueba
int main(){
    sf::RenderWindow window(sf::VideoMode({900, 600}), "Cola en el parque de Diversiones");
    sf::Clock relog;
    AgregarElementoACola(true, "Rodrigo", 1);
    AgregarElementoACola(false, "Roberto", 1);
    AgregarElementoACola(true, "Ramiro", 1);
    AgregarElementoACola(false, "Roy", 1);
    AgregarElementoACola(true, "Rodolfo", 1);
    AgregarElementoACola(false, "Rafael", 2);
    AgregarElementoACola(true, "Ricardo", 1);
    AgregarElementoACola(true, "ReyMundo", 1);
    while (window.isOpen()){
        sf::Time actual = relog.getElapsedTime();
        while (const optional event = window.pollEvent()){
            if (event->is<sf::Event::Closed>()){
                window.close();
            }
            if (event->is<sf::Event::MouseButtonPressed>()){
                ColaPorPrioridad();
                relog.restart();
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
