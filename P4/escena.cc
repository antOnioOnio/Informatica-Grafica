

#include "aux.h" // includes de OpenGL/glut/glew, windows, y librería std de C++
#include "escena.h"
#include "malla.h" // objetos: Cubo y otros....

//**************************************************************************
// constructor de la escena (no puede usar ordenes de OpenGL)
//**************************************************************************

Escena::Escena()
{
  Front_plane = 0.1;
  Back_plane = 2000.0;
  Observer_distance = 10.0;
  Observer_angle_x = 0.0;
  Observer_angle_y = 0.0;

  ejes.changeAxisSize(5000);

  // crear los objetos de las prácticas: Mallas o Jerárquicos....
  cubo = new Cubo();

  // .......completar: ...
  // .....

  tetraedro = new Tetraedro();

  objetoPLY = new ObjPLY("plys/beethoven.ply");

  objetoREV = new ObjRevolucion("plys/peon.ply", 20);

  cilindro = new Cilindro(40, 20);

  cono = new Cono(10, 20);

  esfera = new Esfera(30, 60);

  objjerar = new ObjJerarquico();


  // blanca direccional apuntando a 1.0
  fuente_1 = new luz(GL_LIGHT0,
                     Tupla4f(1.0,0.0,0.0,0.0),
                     Tupla4f(0.2,0.2,0.2,1.0),
                     Tupla4f(1.0,1.0,1.0,1.0),
                     Tupla4f(1.0,1.0,1.0,1.0));

  fuente_2 = new luz(GL_LIGHT1,
                     Tupla4f(-10.0,0.0,0.0,1.0),
                     Tupla4f(0.2,0.0,0.2,1.0),
                     Tupla4f(1.0,0.0,1.0,1.0),
                     Tupla4f(1.0,0.0,1.0,1.0));

 
  cuadro = new Cuadro();
  skybot = new Skybot();
 

  num_objetos = 10; // se usa al pulsar la tecla 'O' (rotar objeto actual)
  
}

//**************************************************************************
// inicialización de la escena (se ejecuta cuando ya se ha creado la ventana, por
// tanto sí puede ejecutar ordenes de OpenGL)
// Principalmemnte, inicializa OpenGL y la transf. de vista y proyección
//**************************************************************************

void Escena::inicializar(int UI_window_width, int UI_window_height)
{
  glClearColor(1.0, 1.0, 1.0, 1.0); // se indica cual sera el color para limpiar la ventana	(r,v,a,al)

  glEnable(GL_DEPTH_TEST); // se habilita el z-bufer

  redimensionar(UI_window_width, UI_window_height);
}

// **************************************************************************
// Funcion que dibuja el objeto activo actual, usando su método 'draw'
// (llamada desde Escena::dibujar)
// ***************************************************************************

void Escena::dibujar_objeto_actual()
{
  using namespace std;

  // (1) configurar OpenGL para el modo actual (puntos/lineas/sólido)
  //    llamar glPolygonMode, glColor... (y alguna cosas más), según dicho modo
  // .........completar (práctica 1)

  // configuracion del color
  // glColor3f(1.0, 0.5, 0.0);

  // Configuracion del sombreado
  //glShadeModel(GL_FLAT);

  // (2) dibujar el objeto actual usando método 'draw' del objeto asociado al
  // valor entero en 'objeto_actual'

  switch (objeto_actual)
  {
      case 0:
        if (cubo != nullptr)
        {
          cubo->draw(modo_actual, modo_dibujo,contadorMaterial);
        }
        break;

      case 1:
        //  ......completar un caso por cada objeto que se haya creado
        if (tetraedro != nullptr)
        {
          tetraedro->draw(modo_actual, modo_dibujo,contadorMaterial);
        }

        break;

      case 2:
        //  ......completar un caso por cada objeto que se haya creado
        if (objetoPLY != nullptr)
        {
          objetoPLY->draw(modo_actual, modo_dibujo,contadorMaterial);
        }

        break;

      case 3:
        if (objetoREV != nullptr)
        {
          objetoREV->draw(modo_actual, modo_dibujo,contadorMaterial);
        }
        break;

      case 4:
        if (cilindro != nullptr)
        {
          cilindro->draw(modo_actual, modo_dibujo,contadorMaterial);
        }
        break;

      case 5:
        if (cono != nullptr)
        {
          cono->draw(modo_actual, modo_dibujo,contadorMaterial);
        }
        break;

      case 6:
        if (esfera != nullptr)
        { 
          esfera->draw(modo_actual, modo_dibujo,contadorMaterial);
        }
        break;

      case 7:

        if (objjerar != nullptr)
        {
          objjerar->draw(modo_actual, modo_dibujo,contadorMaterial);
        }
        break;
      case 8:
      
          if ( cuadro != nullptr){
            cuadro->draw(Solido, modo_dibujo,contadorMaterial);
          }
        break;
      case 9:
        
          if ( skybot != nullptr){
            
            skybot->draw(Solido, modo_dibujo,contadorMaterial);
            
          }
      break;


        case 10:
            default:
            cout << "draw_object: el número de objeto actual (" << objeto_actual << ") es incorrecto." << endl;
        break;
  }
}

// **************************************************************************
//
// función de dibujo de la escena: limpia ventana, fija cámara, dibuja ejes,
// y dibuja los objetos
//
// **************************************************************************

void Escena::dibujar()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Limpiar la pantalla
  
  
 // glPointSize(3.0);
  
  change_observer();
  glEnable(GL_LIGHTING);

  if ( encender_infinito){
    glEnable(GL_LIGHT0);
    fuente_1->activar();

  }

  if ( encender_posicional){
        glPushMatrix();
          glRotatef(angRotacion, 0, 1, 0);
          glEnable(GL_LIGHT1);
          fuente_2->activar();      
        glPopMatrix(); 
  }

  if ( !encender_infinito){
    glDisable(GL_LIGHT0);
  }
  if ( !encender_posicional){
    glDisable(GL_LIGHT1);
  }
  if ( !encender_infinito && !encender_posicional){
    
    glDisable(GL_LIGHTING);
  }

ejes.draw();

  
  dibujar_objeto_actual();
}

//**************************************************************************
//
// función que se invoca cuando se pulsa una tecla
// Devuelve true si se ha pulsado la tecla para terminar el programa (Q),
// devuelve false en otro caso.
//
//**************************************************************************

bool Escena::teclaPulsada(unsigned char tecla, int x, int y)
{
  using namespace std;
  cout << "Tecla pulsada: '" << tecla << "'" << endl;

  if (tecla == 'z' || tecla == 'Z')
  {
    switch (tecla)
    {
    case 'Z':
      objjerar->incrementaParamAct();
      break;

    case 'z':
      objjerar->decrementaParamAct();
      break;
    }
  }
  else
  {
    switch (toupper(tecla))
    {
    case 'Q':

      // salir
      return true;
      break;

    case 'O':
      // activar siguiente objeto
      objeto_actual = (objeto_actual + 1) % num_objetos;
      cout << "objeto actual == " << objeto_actual << endl;
      break;

    case 'M':
      // activar siguiente modo
      if ( objeto_actual != 8){
        modo_actual = (ModoVis)((modo_actual + 1) % 4);
        cout<< " Dibujando en modo ";
        switch (modo_actual){
          //ModoVis{Lineas=0,Puntos=1,Solido=2,Ajedrez=3};
          case 0:
           cout<< "Lineas"<<endl;
          break;
          case 1:
           cout<< "Puntos"<<endl;
          break;
          case 2:
           cout<< "Solido"<<endl;
          break;
          case 3:
           cout<< "Ajedrez"<<endl;
          break;         
        }
        
      }else {
        cout<< " Dibujando texturas " <<endl;
      }
      
      break;

    case 'V':
      modo_dibujo = (modo_dibujo + 1) % 2;
      if (modo_dibujo == 0 ){
        std::cout<<"Dibujando en modo inmediato "<< std::endl;
      }
      std::cout<<"Dibujando en modo diferido "<< std::endl;


    case 'P':
      objjerar->siguienteParametro();
      break;

    case 'A':
       conmutarAnimaciones();
      break;

    case '>':
      objjerar->acelerar();
      break;

    case '<':
      objjerar->decelerar();
      break;
    
    case 'L':
       encender_infinito = !encender_infinito;
      break;

    case 'S':
       encender_posicional = !encender_posicional;
      break;      

    case 'G':
       if (glIsEnabled(GL_LIGHTING) ){
        ObjMallaIndexada::modo_il = ! ObjMallaIndexada::modo_il;
       }
       if (ObjMallaIndexada::modo_il){
        std::cout<<"Iluminacion suave"<< std::endl;
          
       }else {
        std::cout<<"Iluminacion plana"<< std::endl;

       }
      break;

    case 'W':
        angRotacion = angRotacion+10;
      break; 

    case'N':

      contadorMaterial = (contadorMaterial+1)%3;

    break;
          
    }


  }

  return false;
}
//**************************************************************************

void Escena::teclaEspecial(int Tecla1, int x, int y)
{
  switch (Tecla1)
  {
  case GLUT_KEY_LEFT:
    Observer_angle_y--;
    break;
  case GLUT_KEY_RIGHT:
    Observer_angle_y++;
    break;
  case GLUT_KEY_UP:
    Observer_angle_x--;
    break;
  case GLUT_KEY_DOWN:
    Observer_angle_x++;
    break;
  case GLUT_KEY_PAGE_UP:
    Observer_distance *= 1.2;
    break;
  case GLUT_KEY_PAGE_DOWN:
    Observer_distance /= 1.2;
    break;
  }

  //std::cout << Observer_distance << std::endl;
}

//**************************************************************************
// Funcion para definir la transformación de proyeccion
//
// ratio_xy : relacción de aspecto del viewport ( == ancho(X) / alto(Y) )
//
//***************************************************************************

void Escena::change_projection(const float ratio_xy)
{
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  const float wy = 0.84 * Front_plane,
              wx = ratio_xy * wy;
  glFrustum(-wx, +wx, -wy, +wy, Front_plane, Back_plane);
}
//**************************************************************************
// Funcion que se invoca cuando cambia el tamaño de la ventana
//***************************************************************************

void Escena::redimensionar(int newWidth, int newHeight)
{
  Width = newWidth;
  Height = newHeight;
  change_projection(float(Width) / float(Height));
  //glutPostRedisplay();
  glViewport(0, 0, Width, Height);
}

//**************************************************************************
// Funcion para definir la transformación de vista (posicionar la camara)
//***************************************************************************

void Escena::change_observer()
{
  // posicion del observador
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glTranslatef(0.0, 0.0, -Observer_distance);
  glRotatef(Observer_angle_x, 1.0, 0.0, 0.0);
  glRotatef(Observer_angle_y, 0.0, 1.0, 0.0);
}
//**************************************************************************

void Escena::mgeDesocupado()
{
  objjerar->actualizarEstado();
  glutPostRedisplay();
}

//**************************************************************************

void Escena::conmutarAnimaciones()
{

  if (objeto_actual != 7)
  {
    animaciones_activas = true;
    std::cout << " No es el objeto jerarquico " << std::endl;
  }
  else
  {
    animaciones_activas = !animaciones_activas;
    if (animaciones_activas)
    {

      objjerar->inicioAnimaciones();
      glutIdleFunc( funcion_desocupado );
    }
    else
    {

      glutIdleFunc(nullptr);
    }
  }
  
}

// void Escena::asignaMaterial(Tupla4f _ambiental,Tupla4f _difusa,
//                                       Tupla4f _especular, float _brillo){
//   material.ambiental = _ambiental;
//   material.difusa =   _difusa;
//   material.especular = _especular;
//   material.brillo = _brillo;

  
// }