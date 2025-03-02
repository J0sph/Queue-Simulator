#include <iostream>
#include <string>
#include "Queue.h"
#include "VIPQueue.h"
#include "LinkedList.h"
#include <sstream>  // Para usar stringstream
#include <vector>
#include <SFML/Graphics.hpp>
#include <optional>


using namespace std;
// Estructura básica para un botón
struct Boton {
    sf::RectangleShape shape;
    sf::Text text;

    Boton(float x, float y, const std::string &texto, sf::Font &font) : text(texto, font) {
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

    sf::Font font;
        if (!font.loadFromFile("C:/Windows/Fonts/arial.ttf")) {
            cout << "No cargó la fuente correctamente" << endl;
            return;
        }

    while (temp != nullptr) {
        sf::RectangleShape Elemento(sf::Vector2f(AnchoElemento, LargoElemento));
        Elemento.setPosition(sf::Vector2f(x, y));
        Elemento.setFillColor(color);
        
        sf::Text numero;
        numero.setFont(font);
        string NG = to_string(temp->groupID);
        numero.setString(NG);
        numero.setCharacterSize(20);
        numero.setFillColor(sf::Color::White);
        numero.setPosition(sf::Vector2f(x+80, y+10));


        sf::Text Grupo;
        Grupo.setFont(font);    
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

    Estado estado = INICIAL;
    string tipoGrupo = "";  // VIP o Regular    
    int numeroGrupo = -1;

    
    while (salir != 9) {
        // Mostrar el menú de opciones
        cout << "\n=== Menú ===\n";
        cout << "1. Agregar grupos a la cola\n";
        cout << "2. Dejar entrar a los grupos\n";
        cout << "Seleccione una opción: ";
        cin >> option;
        cin.ignore();  // Ignorar el salto de línea del input anterior
    
        switch (option) {
            case 1:  {// Opción para agregar nombres
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
          }
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
    if (!font.loadFromFile("C:/Windows/Fonts/arial.ttf")) {
        return -1;
    }

    Boton btnAgregar(100, 500, "Agregar", font);
    Boton btnEliminar(350, 500, "Eliminar", font);
    Boton btnMover(600, 500, "Mover", font);

    Boton btnVIP(200, 600, "VIP", font);
    Boton btnRegular(500, 600, "Regular", font);

    sf::Text textoIngresado;
    textoIngresado.setFont(font);
    textoIngresado.setCharacterSize(24);
    textoIngresado.setPosition({300, 400});
    textoIngresado.setFillColor(sf::Color::White);

    sf::Text MensajeInstruccion;
    MensajeInstruccion.setFont(font);
    MensajeInstruccion.setCharacterSize(24);
    MensajeInstruccion.setPosition({300, 350});
    MensajeInstruccion.setFillColor(sf::Color::White);
    MensajeInstruccion.setString("Ingrese los nombres separados por espacio y presione enter cuando finalice, posteriormente ingrese la prioridad del grupo");

    bool capturandoTexto = false;  // Controla si el usuario está escribiendo
    bool mostrarMensajeInstruccion = false;
    string inputText = "";

    sf::Time actual;
    while (window.isOpen()) {
        actual = relog.getElapsedTime();
        sf::Event event;

        while (window.pollEvent(event)) {
            if (event.type==sf::Event::Closed){
                window.close();
            }
            if (event.type == sf::Event::TextEntered && capturandoTexto) {
                char letra = static_cast<char>(event.text.unicode);
                if (letra == '\b' && !inputText.empty()) {
                       inputText.pop_back(); 
                    
                            
                    } else if (letra == '\r') {  // Enter
                        if (estado == INGRESAR_NOMBRE) {              
                            istringstream stream(inputText);
                            string nombre;
                            while(stream >> nombre){
                                names.push_back(nombre);
                            }
                            if(tipoGrupo == "VIP"){
                                AgregarElementoACola(true, names, 1);
                                
                            } else {
                                AgregarElementoACola(false, names, 1);
                            }
                                names.clear();  
                                capturandoTexto = false;
                                inputText = "";
                                estado = INICIAL;
                                mostrarMensajeInstruccion = false;
                                MensajeInstruccion.setString("");

                        } else if (estado == INGRESAR_GRUPO) {   
                                numeroGrupo = std::stoi(inputText);    
                    
                            if (tipoGrupo == "VIP"){
                            CVIP.deleteGroup(numeroGrupo);
                            } else {
                                CRegular.deleteGroup(numeroGrupo);
                            }

                            capturandoTexto = false;
                            inputText="";
                            estado = INICIAL;  
                            mostrarMensajeInstruccion = false;
                            MensajeInstruccion.setString("");

                        } else if(estado == MOVER_GRUPO) {

                           int numeroGrupo = std::stoi(inputText);
                            vector<string> integrantes = CRegular.deleteGroup(numeroGrupo);
                            if (!integrantes.empty()) {
                                int prioridad = 1;

                                CVIP.enqueue(integrantes, prioridad);

                            }
                            
                            capturandoTexto = false;
                            inputText="";
                            estado = INICIAL;  
                            mostrarMensajeInstruccion = false;
                            MensajeInstruccion.setString("");
                        }
                    
                        } else {
                        inputText += letra;
                        }

                    textoIngresado.setString("Ingresando: " + inputText);
                }
            

            if (event.type == sf::Event::MouseButtonPressed) {
                sf::Vector2i mousePos = sf::Mouse::getPosition(window);

                if (estado == INICIAL) {
                    if (btnAgregar.isClicked(sf::Vector2f(mousePos))) {
                        estado = SELECCIONAR_TIPO;
                        capturandoTexto = false;
                        mostrarMensajeInstruccion = false;
                    
                    } else if (btnEliminar.isClicked(sf::Vector2f(mousePos))) {
                        estado = ELIMINAR_GRUPO;
                        capturandoTexto = false;
                        mostrarMensajeInstruccion = false;
                    
                    } else if (btnMover.isClicked(sf::Vector2f(mousePos))) {
                        estado = MOVER_GRUPO;
                        capturandoTexto = true;
                        mostrarMensajeInstruccion = true;
                        MensajeInstruccion.setString("Ingrese el número de grupo a mover");
                    }
                } else if (estado == SELECCIONAR_TIPO) {
                    if (btnVIP.isClicked(sf::Vector2f(mousePos))) {
                    tipoGrupo = "VIP";
                    estado = INGRESAR_NOMBRE;
                    capturandoTexto = true;
                    mostrarMensajeInstruccion = true;
                    MensajeInstruccion.setString("Ingrese los nombres separados por espacio y presione enter cuando finalice");
                   
                } else if (btnRegular.isClicked(sf::Vector2f(mousePos))) {
                    tipoGrupo = "Regular";
                    estado = INGRESAR_NOMBRE;
                    capturandoTexto = true;
                    mostrarMensajeInstruccion = true;
                    MensajeInstruccion.setString("Ingrese los nombres separados por espacio y presione enter cuando finalice");
                
                }
                
            } else if (estado == ELIMINAR_GRUPO) {
                    if (btnVIP.isClicked(sf::Vector2f(mousePos))) {
                    tipoGrupo = "VIP";
                    estado = INGRESAR_GRUPO;
                    capturandoTexto = true;
                    mostrarMensajeInstruccion = true;
                    MensajeInstruccion.setString("Ingrese el número de grupo a eliminar");
                    } else if (btnRegular.isClicked(sf::Vector2f(mousePos))) {
                    tipoGrupo = "Regular";
                    estado = INGRESAR_GRUPO;
                    capturandoTexto = true;
                    mostrarMensajeInstruccion = true;
                    MensajeInstruccion.setString("Ingrese el número de grupo a eliminar");
                
                }
            
            }else if (estado == MOVER_GRUPO) {
                    if(!capturandoTexto){
                        capturandoTexto = true;
                        mostrarMensajeInstruccion = true;
                        MensajeInstruccion.setString("Ingrese el número de grupo a mover");
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

    if(mostrarMensajeInstruccion){
        window.draw(MensajeInstruccion);
    }
  
    // Mostrar texto en edición
    if (capturandoTexto) {
        window.draw(textoIngresado);
    }

    window.display();
}

return 0;
}

