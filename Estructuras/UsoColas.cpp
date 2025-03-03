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
// Estructura para realizar los botones
struct Boton {
    sf::RectangleShape shape; //Se hace la forma rectangular de los botones utilizando la biblioteca SFML
    sf::Text text; //Se hace el texto que tendrán los botones utilizando la biblioteca SFML

    Boton(float x, float y, const std::string &texto, sf::Font &font) : text(texto, font) { //Constructor de la estructura Botón
        shape.setSize({200, 50}); //Se asignan las dimensiones de los botones
        shape.setPosition({x, y}); //Se asigna la posición de cada botón
        shape.setFillColor(sf::Color::Blue); //Se asigna el color del botón

        text.setFont(font); //Se le asigna la fuente al texto de los botones
        text.setString(texto); //Texto que se va a mostrar
        text.setCharacterSize(20);  //Tamaño del texto
        text.setFillColor(sf::Color::White); //Color del texto
        text.setPosition({x + 60, y + 10}); //Se asigna la posición que tendrá el texto dentro del botón
    }

    void draw(sf::RenderWindow &window) { //Función para dibujar la forma y texto de los botones en la interfaz 
        window.draw(shape);
        window.draw(text);
    }

    bool isClicked(sf::Vector2f mousePos) { //Función que indica si el botón fue presionado
        return shape.getGlobalBounds().contains(mousePos);
    }
};

enum Estado { //Estados para controlar el flujo de ejecución
    INICIAL, //Para volver al inicio cuando no se ha seleccionado ningún botón
    SELECCIONAR_TIPO,  //Se seleccionó el botón agregar
    INGRESAR_NOMBRE,   //Introduciendo el nombre de los miembros del grupo
    ELIMINAR_GRUPO, //Se seleccionó el botón eliminar
    INGRESAR_GRUPO, //Se seleccionó el botón eliminar y se pidió al usuario que dijite el grupo al que se le desea realizar la acción
    INGRESAR_PRIORIDAD, //Se seleccionó agregar Vip y se le pide al usuario que dijite la prioridad
    MOVER_GRUPO //Se seleccionó el botón mover
};

VIPQueue CVIP; //Se crea la cola VIP
Queue CRegular; //Se crea la cola Regular
//Dimensiones y distancia entre grupos de las colas representados como rectangulos de colores
int AnchoElemento = 100;
int LargoElemento = 50;
int DistanciaEntreElementos = 20;
//Estructura para realizar la animación de que los grupos se desplazan
struct Movimiento {
    bool enMovimiento = false; //Estado que indica si el grupo se está desplazando
    float offset = 0;
};

Movimiento movVIP, movRegular;
float velocidadMovimiento = 0.1f; // Velocidad en pixeles por frame de los grupos

//Función para ingresar los nombres de los integrantes del grupo
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

//Función para agregar los grupos a la cola VIP o regular en base a un parámetro
void AgregarElementoACola(bool VIP, const vector<string>& miembros, int Prioridad){
    if (VIP == true){
        CVIP.enqueue(miembros, Prioridad);
    }else{
        CRegular.enqueue(miembros);
    }
}

//Función para que la cola regular se comience a desplazar hasta que la cola VIP esté vacía
void ColaPorPrioridad() {
    if (!CVIP.isEmpty() && !movVIP.enMovimiento) {
        movVIP.enMovimiento = true;
        movVIP.offset = 0;
    } else if (CVIP.isEmpty() && !CRegular.isEmpty() && !movRegular.enMovimiento) {
        movRegular.enMovimiento = true;
        movRegular.offset = 0;
    }
}

//Función para calcular el tiempo de espera en base a la posición del grupo en la cola y el tiempo que tarda cada grupo en avanzar
int CalcularTiempoEspera(int posicion){
    int TiempoPorGrupo = 5;
    return posicion * TiempoPorGrupo;
}
//Función para visualizar desde la interfaz la información de cada grupo
void DetallesGrupos(int groupID, bool isVIP, sf::RenderWindow& window, sf:: Font& font){
    Queue& cola = isVIP ? CVIP : CRegular;
    LinkedList* grupo = cola.getGroup(groupID);

    if (grupo != nullptr) { 
    vector<string> members = grupo->getMembers(); //Se obtionen los miembros del grupo seleccionado 
    int TiempoEstimado = CalcularTiempoEspera(groupID); //Se llama la función que calcula el tiempo de espera
    
    sf::Text DetallesGrupo; //Se crea el texto que se verá en la interfaz con la información
    DetallesGrupo.setFont(font); //Se le asignan las caracteristicas al texto
    DetallesGrupo.setCharacterSize(21);
    DetallesGrupo.setFillColor(sf::Color::White);
    DetallesGrupo.setPosition(300, 150);

    string texto = "Grupo: " + to_string(groupID) + "\n"; //String con la información
    texto += "Tiempo estimado de espera: " + to_string(TiempoEstimado) + " minutos\n";
    texto += "Integrantes: \n";
    for (const string& member : members) { //Se agregan los miembros
        texto += member + "\n";
    }
    DetallesGrupo.setString(texto);

    window.clear(); //Se limpia la pantalla
    window.draw(DetallesGrupo); //Se dibuja el texto 
    window.display(); //Se muestra en pantalla

    sf::Event event; //Se crea el evento para mostrar la infomación
    while (window.waitEvent(event)) {
        if (event.type == sf::Event::KeyPressed) {
            break;
        }
    }  

    }
}

//Función para dibujar los rectángulos(Grupos)
void Elementos(sf::RenderWindow &window, Queue &queue, float y, sf::Color color, Movimiento &mov) {
    if (queue.isEmpty()) return; //Comprueba si las colas están vacías
    
    LinkedList* temp = queue.PrimerElemento(); //Seleccionada el primer elemento de la lista enlazada (Primer grupo)
    float x = 250 - mov.offset; // Se aplica el desplazamiento con mov.offset cuando enMovimiento sea activado pasado un tiempo seleccionado

    sf::Font font; //Se carga la fuente a utilizar 
        if (!font.loadFromFile("C:/Windows/Fonts/arial.ttf")) {
            cout << "No cargó la fuente correctamente" << endl;
            return;
        }

    while (temp != nullptr) { //Se recorren los nodos de la lista enlazada para dibujar cada grupo
        sf::RectangleShape Elemento(sf::Vector2f(AnchoElemento, LargoElemento)); //Se asignan las caracteristicas de los rectangulos
        Elemento.setPosition(sf::Vector2f(x, y));
        Elemento.setFillColor(color);
        //Se asignan las características del texto correspondiente al número de grupo de cada rectángulo
        sf::Text numero;
        numero.setFont(font);
        string NG = to_string(temp->groupID);
        numero.setString(NG);
        numero.setCharacterSize(20);
        numero.setFillColor(sf::Color::White);
        numero.setPosition(sf::Vector2f(x+80, y+10));

        //Se asignan las características del texto correspondiente a "Grupo: " Mostrado en cada rectángulo
        sf::Text Grupo;
        Grupo.setFont(font);    
        string G = "Grupo: ";
        Grupo.setString(G);
        Grupo.setCharacterSize(20);
        Grupo.setFillColor(sf::Color::White);
        Grupo.setPosition(sf::Vector2f(x+10, y+10));

        window.draw(Elemento); //Se dinuja el rectángulo
        window.draw(numero); //Se dibuja el níumero
        window.draw(Grupo); //Se dibula la string

        x += AnchoElemento + DistanciaEntreElementos;
        temp = temp->next;
    }

    // Se realiza la animación del movimiento progresivo de los grupos
    if (mov.enMovimiento) {
        mov.offset += velocidadMovimiento; //Cada frame mov.offset aumenta
        if (mov.offset >= AnchoElemento + DistanciaEntreElementos) { //Al llegar a una cierta distancia se elimina el grupo simulando que entró al parque o a una atracción
            mov.enMovimiento = false; //Se desactiva el movimiento para el siguiente grupo, se activa hasta pasado un tiempo
            mov.offset = 0; // Se reinicia el desplazamiento para los demás elementos
            queue.dequeue(); // Ahora eliminamos el primer elemento después de acabada la animación
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
    int salir = 0; //Variable para salir del menú
    
    vector<string> names; //Vector que contendrá los nombres de los integrantes del grupo
    int groupID; //Variable para saber el número de grupo
    int priority; //Variable para la prioridad de la cola VIP
    int SVIP; //Variable para indicar por consola si el grupo es VIP o no

    Estado estado = INICIAL; //Estado inicial
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
                            capturandoTexto = false;
                            inputText = "";

                            if (tipoGrupo == "VIP"){
                            estado = INGRESAR_PRIORIDAD;
                            capturandoTexto = true;
                            mostrarMensajeInstruccion = true;
                            MensajeInstruccion.setString("Ingrese la prioridad del grupo");
                            } else {
                                AgregarElementoACola(false, names, 0);
                                names.clear();
                                capturandoTexto = false;
                                estado = INICIAL;
                                mostrarMensajeInstruccion = false;
                                MensajeInstruccion.setString("");
                            }
                        } else if (estado == INGRESAR_PRIORIDAD) {

                            try{
                            priority = stoi(inputText);

                            if(tipoGrupo == "VIP"){
                                AgregarElementoACola(true, names, priority  );
                                
                            }
                            names.clear();
                            capturandoTexto = false;
                            inputText = "";
                            estado = INICIAL;
                            mostrarMensajeInstruccion = false;
                            MensajeInstruccion.setString("");
                            } catch (const std::invalid_argument& e){
                                MensajeInstruccion.setString("Error: Ingrese un número entero");
                                inputText = "";
                            }
                            
                        } else if (estado == INGRESAR_GRUPO){
                            numeroGrupo = std::stoi(inputText);

                            if (tipoGrupo == "VIP"){
                                CVIP.deleteGroup(numeroGrupo);
                            } else {
                                CRegular.deleteGroup(numeroGrupo);
                            }
                            capturandoTexto = false;
                            inputText = "";
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

                    for (int i = 0; i < CVIP.getLength(); i++) {
                        float xPosition = 250 + i * (AnchoElemento + DistanciaEntreElementos) - movVIP.offset;
                        sf::FloatRect cuadroVIP(xPosition, 100, AnchoElemento, LargoElemento);
                        if (cuadroVIP.contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
                           LinkedList* grupo = CVIP.getGroup(i);
                            if (grupo != nullptr) {
                            vector<string> members = grupo->getMembers();
                            int TiempoEstimado = (i+1) * 5;

                            string detalle = "Grupo: " + to_string(i + 1) + "\n";
                            detalle += "Tiempo estimado de espera: " + to_string(TiempoEstimado) + " minutos\n";
                            detalle += "Integrantes: \n";
                            for (const auto& member : members) {
                                detalle +="-" + member + "\n";                                                       
                            }
                            MensajeInstruccion.setString(detalle);
                            mostrarMensajeInstruccion = true;
                            }
                            break;
                        }
                    }

                    for (int i = 0; i < CRegular.getLength(); i++) {
                        float xPosition = 250 + i * (AnchoElemento + DistanciaEntreElementos) - movRegular.offset;
                        sf::FloatRect cuadroRegular(xPosition, 200, AnchoElemento, LargoElemento);

                        if (cuadroRegular.contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
                            LinkedList* grupo = CRegular.getGroup(i);
                            if (grupo != nullptr) {
                            vector<string> members = grupo->getMembers();
                            int TiempoEstimado = (i+1)*5;

                            string detalle = "Grupo: " + to_string(i + 1) + "\n";
                            detalle += "Tiempo estimado de espera: " + to_string(TiempoEstimado) + " minutos\n";
                            detalle += "Integrantes: \n";
                            for (const auto& member : members) {
                                detalle +="-" + member + "\n";                                                       
                            }
                            MensajeInstruccion.setString(detalle);
                            mostrarMensajeInstruccion = true;
                            }

                            break;
                        }
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
    

    if (actual.asSeconds() > 15){
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

