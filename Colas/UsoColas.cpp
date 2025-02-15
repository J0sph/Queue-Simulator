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

// Script de prueba
int main(){
    Queue prueba;

    prueba.enqueue("Ale", 1);
    prueba.enqueue("Cami", 1);

    prueba.printQueue();
    cout << "---------------" << endl;

    prueba.enqueue("Ale",3);
    prueba.printQueue();
    cout << "---------------" << endl;

    prueba.enqueue("Cami", 2);
    prueba.printQueue();
    cout << "---------------" << endl;
    prueba.enqueue("Fer", 2);
    prueba.printQueue();
    cout << "---------------" << endl;
    prueba.enqueue("Ale", 4);
    prueba.printQueue();
    cout << "---------------" << endl;
    cout << "Se comienza a borrar elementos" << endl;

    prueba.dequeueSingle();
    prueba.dequeueSingle();
    prueba.dequeueSingle();
    prueba.printQueue();
    cout << "---------------" << endl;
    cout << "Se comienza a borrar grupos" << endl;

    prueba.dequeueGroup();
    prueba.printQueue();
    cout << "---------------" << endl;

    return 0;
}