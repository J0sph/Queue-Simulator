#include <iostream>
#include <string>
#include "Queue.h"
#include "VIPQueue.h"
#include "LinkedList.h"
#include <sstream>  // Para usar stringstream
#include <vector>
#include <SFML/Graphics.hpp>

using namespace std;
// Estructura básica para un botón
struct Boton {
    sf::RectangleShape shape;
    sf::Text text;

    Boton(float x, float y, const std::string &texto, sf::Font &font) : text(font) {
        shape.setSize({200, 50});
        shape.setPosition({x, y});
        shape.setFillColor(sf::Color::Blue);

        text.setFont(font);
        text.setString(texto);
        text.setCharacterSize(20);
        text.setFillColor(sf::Color::White);
        text.setPosition({x + 60, y + 10});
    }

    void draw(sf::RenderWindow &window) {
        window.draw(shape);
        window.draw(text);
    }

    bool isClicked(sf::Vector2f mousePos) {
        return shape.getGlobalBounds().contains(mousePos);
    }
};

enum Estado {
    INICIAL,
    SELECCIONAR_TIPO,  // Mostrar VIP/Regular
    INGRESAR_NOMBRE,   // Introducir texto
    ELIMINAR_GRUPO,
    INGRESAR_GRUPO,
    MOVER_GRUPO
};

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
float velocidadMovimiento = 0.1f; // Pixeles por frame

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

void Elementos(sf::RenderWindow &window, Queue &queue, float y, sf::Color color, Movimiento &mov) {
    if (queue.isEmpty()) return;
    
    LinkedList* temp = queue.PrimerElemento();
    float x = 250 - mov.offset; // Aplicar desplazamiento inicial

    while (temp != nullptr) {
        sf::RectangleShape Elemento(sf::Vector2f(AnchoElemento, LargoElemento));
        sf::Font font;
        if (!font.openFromFile("C:\\Users\\josep\\OneDrive\\Escritorio\\Cosas de Joseph\\Prueba2\\Fonts\\arial.ttf")) {
            cout << "No cargó la fuente correctamente" << endl;
        }
        sf::Text numero(font);
        sf::Text Grupo(font);
        Elemento.setPosition(sf::Vector2f(x, y));
        Elemento.setFillColor(color);
        string NG = to_string(temp->groupID);
        numero.setString(NG);
        numero.setCharacterSize(20);
        numero.setFillColor(sf::Color::White);
        numero.setPosition(sf::Vector2f(x+80, y+10));
        string G = "Grupo: ";
        Grupo.setString(G);
        Grupo.setCharacterSize(20);
        Grupo.setFillColor(sf::Color::White);
        Grupo.setPosition(sf::Vector2f(x+10, y+10));
        window.draw(Elemento);
        window.draw(numero);
        window.draw(Grupo);

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

// Metodo que permite que un grupo regular haga 'upgrade' a vip
void upgradeToVip(int groupID, int prioridad){
    // Borra el grupo de la cola regular
    vector<string> integrantes = CRegular.deleteGroup(groupID);
   
    // Ingresa el grupo en la VIP
    CVIP.enqueue(integrantes, prioridad);
    
}


int main() {
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
        cout << "1. Agregar grupos a la cola\n";
        cout << "2. Dejar entrar a los grupos\n";
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
        default:
            cout << "Opción inválida. Intente nuevamente.\n";
            break;
    }
}

    sf::RenderWindow window(sf::VideoMode({900, 700}), "Cola en el parque de Diversiones");
    sf::Clock relog;
    sf::Font font;
    if (!font.openFromFile("C:\\Users\\josep\\OneDrive\\Escritorio\\Cosas de Joseph\\Prueba2\\Fonts\\arial.ttf")) {
        return -1;
    }

    Boton btnAgregar(100, 500, "Agregar", font);
    Boton btnEliminar(350, 500, "Eliminar", font);
    Boton btnMover(600, 500, "Mover", font);

    Boton btnVIP(200, 600, "VIP", font);
    Boton btnRegular(500, 600, "Regular", font);

    Estado estado = INICIAL;
    string inputText = "";  // Almacena texto o número ingresado
    bool capturandoTexto = false;  // Controla si el usuario está escribiendo

    string tipoGrupo = "";  // VIP o Regular
    int numeroGrupo = -1;  // Para eliminar o mover

    sf::Text textoIngresado(font);
    textoIngresado.setString("");
    textoIngresado.setCharacterSize(24);
    textoIngresado.setPosition({300, 400});
    textoIngresado.setFillColor(sf::Color::White);

    while (window.isOpen()) {
        sf::Time actual = relog.getElapsedTime();
        while (const optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()){
                window.close();
            }
            if (event->is<sf::Event::TextEntered>() && capturandoTexto) {
                const auto* textEvent = event->getIf<sf::Event::TextEntered>();
                if (textEvent->unicode < 128) {
                    char letra = static_cast<char>(textEvent->unicode);
                    if (letra == '\b') {
                        if (!inputText.empty()) {
                            inputText.pop_back(); 
                        }
                    } else if (letra == '\r') {  // Enter
                        if (estado == INGRESAR_NOMBRE) {
                            istringstream stream(inputText);
                            string nombre;
                            while(stream >> nombre){
                                names.push_back(nombre);
                            }
                            if(tipoGrupo == "VIP"){
                            AgregarElementoACola(true, names, 1);
                            names.clear();  // Limpiar el vector para el siguiente uso
                            }else {
                                AgregarElementoACola(false, names, 1);
                                names.clear();  // Limpiar el vector para el siguiente uso
                            }
                            std::cout << "Nombre ingresado para grupo " << tipoGrupo << ": " << inputText << std::endl;
                        } else if (estado == INGRESAR_GRUPO) {
                            numeroGrupo = std::stoi(inputText);
                            if (tipoGrupo == "VIP"){
                            CVIP.deleteGroup(numeroGrupo);
                            }else {
                                CRegular.deleteGroup(numeroGrupo);
                            }
                            std::cout << "Número de grupo seleccionado: " << numeroGrupo << std::endl;
                        }else if(estado == MOVER_GRUPO){
                            numeroGrupo = std::stoi(inputText);
                            std::cout << "Número de grupo seleccionado: " << numeroGrupo << std::endl;
                        }
                        capturandoTexto = false;
                        inputText = "";
                        estado = INICIAL;  // Volver al menú inicial
                    } else {
                        inputText += letra;
                    }
                    textoIngresado.setString("Ingresando: " + inputText);
                }
            }

            if (event->is<sf::Event::MouseButtonPressed>()) {
                sf::Vector2i mousePos = sf::Mouse::getPosition(window);

                if (estado == INICIAL) {
                    if (btnAgregar.isClicked(sf::Vector2f(mousePos))) {
                        estado = SELECCIONAR_TIPO;
                    } else if (btnEliminar.isClicked(sf::Vector2f(mousePos))) {
                        estado = ELIMINAR_GRUPO;
                        capturandoTexto = true;
                    } else if (btnMover.isClicked(sf::Vector2f(mousePos))) {
                        estado = MOVER_GRUPO;
                        capturandoTexto = true;
                    }
                } else if (estado == SELECCIONAR_TIPO) {if (btnVIP.isClicked(sf::Vector2f(mousePos))) {
                    tipoGrupo = "VIP";
                    estado = INGRESAR_NOMBRE;
                    capturandoTexto = true;
                } else if (btnRegular.isClicked(sf::Vector2f(mousePos))) {
                    tipoGrupo = "Regular";
                    estado = INGRESAR_NOMBRE;
                    capturandoTexto = true;
                }
                }else if (estado == ELIMINAR_GRUPO) {if (btnVIP.isClicked(sf::Vector2f(mousePos))) {
                    tipoGrupo = "VIP";
                    estado = INGRESAR_GRUPO;
                    capturandoTexto = true;
                } else if (btnRegular.isClicked(sf::Vector2f(mousePos))) {
                    tipoGrupo = "Regular";
                    estado = INGRESAR_GRUPO;
                    capturandoTexto = true;
                }
            }
        }
    }
    if (actual.asSeconds() > 5){
        ColaPorPrioridad();
        relog.restart();
    }

    window.clear();
    Elementos(window, CVIP, 100, sf::Color::Red, movVIP);
    Elementos(window, CRegular, 200, sf::Color::Blue, movRegular);
    // Mostrar botones iniciales
    btnAgregar.draw(window);
    btnEliminar.draw(window);
    btnMover.draw(window);

    // Mostrar botones VIP/Regular si está en ese estado
    if (estado == SELECCIONAR_TIPO || estado == ELIMINAR_GRUPO) {
        btnVIP.draw(window);
        btnRegular.draw(window);
    }

    // Mostrar texto en edición
    if (capturandoTexto) {
        window.draw(textoIngresado);
    }

    window.display();
}

return 0;
}
