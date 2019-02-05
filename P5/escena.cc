

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
  Observer_distance = 1.0;
  Observer_angle_x = 0.0;
  Observer_angle_y = 0.0;

  cubo = new Cubo();
  
  ejes.changeAxisSize(5000);

  objjerar = new ObjJerarquico();

  GLfloat ratio = float(Width)/float(Height);
  GLfloat wy = 0.84 * Front_plane;
  GLfloat wx = ratio * wy;

  frustrum = new Camera(Tupla3f(0,0,10), Tupla3f(0,0,0),Tupla3f(0,1,0), 
                        -wx, +wx, -wy, +wy, Front_plane, Back_plane);

                        // left, right, bottom, top, nearVal, farVal
  // orthogonal = new Camera(Tupla3f(0,0,5), Tupla3f(0,0,0),Tupla3f(0,1,0), 
  //                       -Width*15, +Width*15, -Height*15, +Height*15, Front_plane, Back_plane);

  num_objetos = 1; // se usa al pulsar la tecla 'O' (rotar objeto actual)

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
  glColor3f(1.0, 0.5, 0.0);

  // Configuracion del sombreado
  glShadeModel(GL_FLAT);

  draw_cubos();
  
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
  change_observer();
  ejes.draw();
  glPointSize(3.0);
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
  
    switch (toupper(tecla))
    {
    case 'Q':

      // salir
      return true;
      break;
    
    case 'O':
      orthog = !orthog;
    break;

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
  
  frustrum->setProyeccion(-wx*Observer_distance, +wx*Observer_distance, -wy*Observer_distance, +wy*Observer_distance, Front_plane, Back_plane, orthog);

  //glFrustum(-wx, +wx, -wy, +wy, Front_plane, Back_plane);
             // left, right, bottom, top, nearVal, farVal
}
//**************************************************************************
// Funcion que se invoca cuando cambia el tamaño de la ventana
//***************************************************************************

void Escena::redimensionar(int newWidth, int newHeight)
{
  Width = newWidth;
  Height = newHeight;
  change_projection(float(Width) / float(Height));
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
  change_projection(float(Width) / float(Height));
  frustrum->girarCamara(Observer_angle_x,Observer_angle_y );


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

void Escena::setCamara(GLfloat x, GLfloat y){
  Observer_angle_x += y;
  Observer_angle_y += x;


}

//**************************************************************************
// funcion que realiza un zoom sobre la escena
//**************************************************************************


void Escena::zoom(GLfloat x, GLfloat y){

  Observer_distance *= x;
  Observer_distance /= y;

 // frustrum->zoom(Observer_distance,false);

  glutPostRedisplay();
}

//**************************************************************************

//**************************************************************************
void Escena::pick(int x, int y)
{
    GLuint selectBuf[100]={0}, *names; 
    GLint viewport[4], hits=0;


    // Declarar buffer de selección
    glSelectBuffer(100, selectBuf);
    // Obtener los parámetros del viewport
    glGetIntegerv (GL_VIEWPORT, viewport);


    // Pasar OpenGL a modo selección
    glRenderMode (GL_SELECT);
    glInitNames();
    glPushName(0);
    //  Fijar la transformación de proyección para la seleccion
    glMatrixMode (GL_PROJECTION);
    glLoadIdentity ();
    gluPickMatrix (x,(viewport[3] - y),5.0, 5.0, viewport);
    glFrustum( -Width, +Width, -Height, +Height, Front_plane, Back_plane);
    // Dibujar la escena
    dibujar();
    // Pasar OpenGL a modo render
    hits = glRenderMode (GL_RENDER);
    // Restablecer la transformación de proyección (sin gluPickMatrix)
    glMatrixMode (GL_PROJECTION);
    glLoadIdentity();
    glFrustum( -Width, +Width, -Height, +Height, Front_plane, Back_plane);

    // Analizar el contenido del buffer de selección
    // procesar_hits(hits);
    names=selectBuf;

    // Dibujar la escena
    dibujar();
}

//**************************************************************************

//**************************************************************************

void Escena::pick_color(int x, int y){
  GLint viewport[4];
  unsigned char pixel[3];

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Limpiar la pantalla
  draw_seleccion();

  glGetIntegerv(GL_VIEWPORT, viewport);
  glReadPixels(x,viewport[3]-y,1,1,GL_RGB,GL_UNSIGNED_BYTE,(GLubyte *) &pixel[0]);
  printf(" valor x %d, valor y %d, color %d, %d, %d \n",x,y,pixel[0],pixel[1],pixel[2]);
  procesar_hits(pixel);
  glutPostRedisplay();
}

//**************************************************************************
// funcion para activar las figuras correspondientes
//**************************************************************************
void Escena::procesar_hits(unsigned char pixel[3]){

  switch(pixel[0]){
    case 0:
      figurasActivas[0]= !figurasActivas[0];
    break;
    case 1:
      figurasActivas[1]= !figurasActivas[1];
    break;

    case 2:
      figurasActivas[2]= !figurasActivas[2];
    break;

  }

}

//**************************************************************************
// funciones para dibujar las figuras
//**************************************************************************

void Escena::draw_seleccion(){

    glPushMatrix();
        glScalef(1,5,1);
        glTranslatef(-5,0.5,0);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glColor3ub(0,100,0);
        cubo->draw_ModoInmediato();
    glPopMatrix();
  
    
    glPushMatrix();
        glScalef(1,5,1);
        glTranslatef(5,0.5,0);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glColor3ub(1,100,0);
        cubo->draw_ModoInmediato();
    glPopMatrix();

    glPushMatrix();
        glScalef(1,5,1);
        glTranslatef(0,0.5,-5);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glColor3ub(2,100,0);
        cubo->draw_ModoInmediato();
    glPopMatrix();
  
}

void Escena::draw_cubos(){
    
    glPushMatrix();
        glScalef(1,5,1);
        glTranslatef(-5,0.5,0);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        if (figurasActivas[0]){
          glColor3f(1.0,0,0);
        }else{
          glColor3f(0,0.5,1);
        }
        
        cubo->draw_ModoInmediato();
    glPopMatrix();
  
    glPushMatrix();
        glScalef(1,5,1);
        glTranslatef(5,0.5,0);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        if (figurasActivas[1]){
          glColor3f(1.0,0,0);
        }else{
          glColor3f(0,0.5,1);
        }
        cubo->draw_ModoInmediato();
    glPopMatrix();
    

    glPushMatrix();
        glScalef(1,5,1);
        glTranslatef(0,0.5,-5);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        if (figurasActivas[2]){
          glColor3f(1.0,0,0);
        }else{
          glColor3f(0,0.5,1);
        }
        cubo->draw_ModoInmediato();
    glPopMatrix();

}
