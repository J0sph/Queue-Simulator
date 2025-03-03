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

    Boton(float x, float y, const std::string &texto, sf::Font &font, float textposX, float textposY) : text(texto, font) { //Constructor de la estructura Botón
        shape.setSize({150, 70}); //Se asignan las dimensiones de los botones
        shape.setPosition({x, y}); //Se asigna la posición de cada botón
        shape.setFillColor(sf::Color::Blue); //Se asigna el color del botón

        text.setFont(font); //Se le asigna la fuente al texto de los botones
        text.setString(texto); //Texto que se va a mostrar
        text.setCharacterSize(20);  //Tamaño del texto
        text.setFillColor(sf::Color::White); //Color del texto
        text.setPosition({x + textposX, y + textposY}); //Se asigna la posición que tendrá el texto dentro del botón
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
    MOVER_GRUPO, //Se seleccionó el botón mover
    CAMBIAR_PRIORIDAD // Se seleccionó el botón cambiar prioridad y se le pide al usuario número de grupo y prioridad
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
        if (!font.loadFromFile("assets/arial.ttf")) {
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
   
    vector<string> names; //Vector que contendrá los nombres de los integrantes del grupo
    
    int priority; //Variable para la prioridad de la cola VIP


    Estado estado = INICIAL; //Estado inicial
    string tipoGrupo = "";  // VIP o Regular    
    int numeroGrupo = -1;

    
   
    sf::RenderWindow window(sf::VideoMode({900, 700}), "Cola en el parque de Diversiones"); //Se crea la ventana principal
    sf::Clock relog; //Reloj para controlar cada cuanto avanzan los grupos y para estimar el tiempo de atención
    sf::Font font; //Se carga la fuente
    if (!font.loadFromFile("assets/arial.ttf")) { //Condional por si hay un error al cargar la fuente
        return -1;
    }

    Boton btnAgregar(100, 500, "Agregar", font, 30, 20); //Se agregan los botones agregar, eliminar y mover a la ventana principal
    Boton btnEliminar(280, 500, "Eliminar", font, 30, 20);
    Boton btnMover(460, 500, "Mover", font, 40, 20);
    Boton btnCambioPrioridad (640, 500, "Cambiar\nPrioridad", font, 25, 10);

    Boton btnVIP(200, 600, "VIP", font, 50, 20); //Se agregan los botones VIP y Regular los cuales aparecerán al seleccionar el botón agregar o elimiar
    Boton btnRegular(500, 600, "Regular", font, 30, 20);

    sf::Text textoIngresado; //Texto para que el usuario visualice lo que está dijitando
    textoIngresado.setFont(font); //Se le asignan las características al texto
    textoIngresado.setCharacterSize(14);
    textoIngresado.setPosition({100, 400});
    textoIngresado.setFillColor(sf::Color::White);

    sf::Text MensajeInstruccion; //Mensaje que le indica al usuario lo que debe hacer
    MensajeInstruccion.setFont(font); //Se le asignan las características al texto
    MensajeInstruccion.setCharacterSize(14);
    MensajeInstruccion.setPosition({100, 350});
    MensajeInstruccion.setFillColor(sf::Color::White);
    MensajeInstruccion.setString("Ingrese los nombres separados por espacio y presione enter cuando finalice, posteriormente ingrese la prioridad del grupo");

    bool capturandoTexto = false;  // Controla si el usuario está escribiendo
    bool mostrarMensajeInstruccion = false; //Una vez el usuario no está ingresando texto a traves de la interfaz se deja de monstrar el mensaje de instrucción
    string inputText = ""; //String que contiene el texto que va dijitando el usuario

    sf::Time actual; 
    while (window.isOpen()) {
        actual = relog.getElapsedTime(); //Se obtiene el tiempo actual 
        sf::Event event; //Se crea el evento principal

        while (window.pollEvent(event)) { //Bucle principal
            if (event.type==sf::Event::Closed){ //Condicional para cerrar la ventana al precionar la X
                window.close();
            }
            if (event.type == sf::Event::TextEntered && capturandoTexto) { //Condicional para cuando el usuario esté dijitando texto a través de la interfaz
                char letra = static_cast<char>(event.text.unicode); //
                if (letra == '\b' && !inputText.empty()) {
                       inputText.pop_back(); 
                    
                            
                    } else if (letra == '\r') {  // Condicional para cuando el usuario dijite enter
                        if (estado == INGRESAR_NOMBRE) {  //Si se seleccionó agregar se ingresaran los nombres de los integrantes del grupo           
                            istringstream stream(inputText);
                            string nombre;
                            while(stream >> nombre){
                                names.push_back(nombre);
                            }
                            capturandoTexto = false; //Se desactiva el estado de capturandoTexto cuando el usuario haya dijitado enter 
                            inputText = ""; //Se vacía el texto en inputText para futuras acciones

                            if (tipoGrupo == "VIP"){ //Condicional por si el grupo que se va a agregar es VIP para preguntarle al usuario por la prioridad
                            estado = INGRESAR_PRIORIDAD; //Se activa el estado Ingresar prioridad 
                            capturandoTexto = true; //Se activa capturando texto
                            mostrarMensajeInstruccion = true; //Se muestra el mensaje de instrucción al usuario
                            MensajeInstruccion.setString("Ingrese la prioridad del grupo"); //Mensaje de instrucción
                            } else { //Si el grupo a agregar no es VIP simplemente de agrega sin importar la prioridad
                                AgregarElementoACola(false, names, 0);
                                names.clear(); //Se limpia el vector names
                                capturandoTexto = false; //De nuevo se desactivan los estados para volver al estado inicial y esperar la nueva acción del usuario
                                estado = INICIAL;
                                mostrarMensajeInstruccion = false;
                                MensajeInstruccion.setString("");
                            }
                        } else if (estado == INGRESAR_PRIORIDAD) { //Si el grupo añadido es VIP se agrega tomando en cuenta su proridad la cual es dijitada por el usuario

                            try{
                            priority = stoi(inputText);

                            if(tipoGrupo == "VIP"){
                                AgregarElementoACola(true, names, priority);
                                
                            }
                            names.clear(); //Se limpia el vector names
                            capturandoTexto = false; //Se vuelve al estado inicial para esperar la nueva acción del usuario
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

                        } else if(estado == CAMBIAR_PRIORIDAD){

                            int numgrupo, prioridad;

                            try{
                                istringstream ss(inputText);
                                ss >> numgrupo >> prioridad;

                                CVIP.updatePriority(numgrupo,prioridad);
                            
                                capturandoTexto = false;
                                inputText = "";
                                estado = INICIAL;
                                mostrarMensajeInstruccion = false;
                                MensajeInstruccion.setString("");

                                } catch (const std::invalid_argument& e){
                                    MensajeInstruccion.setString("Error: Ingrese un número entero");
                                    inputText = "";
                                }
                                
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

                    } else if (btnCambioPrioridad.isClicked(sf::Vector2f(mousePos))) {
                        estado = CAMBIAR_PRIORIDAD;
                        capturandoTexto = true;
                        mostrarMensajeInstruccion = true;
                        MensajeInstruccion.setString("Ingrese el número de grupo y la nueva prioridad separados por espacio");
                    }

                    for (int i = 0; i < CVIP.getLength(); i++) {
                        float xPosition = 250 + i * (AnchoElemento + DistanciaEntreElementos) - movVIP.offset;
                        sf::FloatRect cuadroVIP(xPosition, 100, AnchoElemento, LargoElemento);
                        if (cuadroVIP.contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
                           LinkedList* grupo = CVIP.getGroup(i);
                            if (grupo != nullptr) {
                            vector<string> members = grupo->getMembers();
                            int TiempoEstimado = (i+1) * 5;

                            string detalle = "Grupo: " + to_string(grupo->getGroupID()) + "\n";
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

                            string detalle = "Grupo: " + to_string(grupo->getGroupID()) + "\n";
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
                
            } else if (estado == CAMBIAR_PRIORIDAD) {
                    if(!capturandoTexto){
                        capturandoTexto = true;
                        mostrarMensajeInstruccion = true;
                        MensajeInstruccion.setString("Ingrese el número de grupo y la nueva prioridad separados por espacio");
                    } 

            }



        }
    }
    

    if (actual.asSeconds() > 15){ //Si pasó un cierto tiempo se activa la animación de movimiento y se elimina el primer grupo
        ColaPorPrioridad();
        relog.restart(); //Se reinicia el tiempo 
    }

    window.clear(); //Se limplia la pantalla
    Elementos(window, CVIP, 100, sf::Color::Red, movVIP); //Se dibuja la cola VIP
    Elementos(window, CRegular, 200, sf::Color::Blue, movRegular); //Se dinuja la cola Regular
    //Se bibujan los botones iniciales
    btnAgregar.draw(window);
    btnEliminar.draw(window);
    btnMover.draw(window);
    btnCambioPrioridad.draw(window);

    // Mostrar botones VIP/Regular si el usuario presionó los botones Agregar o Eliminar
    if (estado == SELECCIONAR_TIPO || estado == ELIMINAR_GRUPO) {
        btnVIP.draw(window);
        btnRegular.draw(window);
    }
    //Se dibuja el texto de la instrucción que debe seguir el usuario al estar este ingresando texto a través de la interfaz
    if(mostrarMensajeInstruccion){
        window.draw(MensajeInstruccion);
    }
  
    //Se dibuja el texto que va ingresando el usuario a través de la interfaz
    if (capturandoTexto) {
        window.draw(textoIngresado);
    }

    window.display();
}

return 0;
}

