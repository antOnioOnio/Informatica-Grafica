#ifndef _ESCENA_H
#define _ESCENA_H

#include "ejes.h"
#include "malla.h"
#include "jerarquico.h"


#include <string>
#include <vector>
#include "GL/glut.h"
#include "Camera.h"


#if defined(__APPLE__)
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#endif
class Escena
{

  private:
    Ejes ejes;

    // variables que definen la posicion de la camara en coordenadas polares
    GLfloat Observer_distance;
    GLfloat Observer_angle_x;
    GLfloat Observer_angle_y;

    // variables que controlan la ventana y la transformacion de perspectiva
    GLfloat Width, Height, Front_plane, Back_plane;

    void clear_window();
    void dibujar_objeto_actual();

    // Transformación de cámara
    void change_projection(const float ratio_xy);
    void change_observer();

    int objeto_actual = 0, // objeto actual (el que se visualiza)
        num_objetos = 0,   // número de objetos (actualizado al crear los objetos en el constructor)
        modo_dibujo = 0;   // Inmediato o diferido

    bool animaciones_activas = false;
    bool orthog = false;
    bool figurasActivas[3]= {false,false,false};
    ModoVis modo_actual;
    
    ObjJerarquico *objjerar = nullptr;
    Cubo *cubo = nullptr;
    Camera *orthogonal = nullptr;
    Camera *frustrum = nullptr;

    int modo[3], cambio=0;

    
    void conmutarAnimaciones();
 

    //           y a un objeto jerarquico (práctica 3).
    // ......

  public:
    Escena();
    void inicializar(int UI_window_width, int UI_window_height);
    void redimensionar(int newWidth, int newHeight);
    // Dibujar
    void dibujar();
    void setCamara(GLfloat x, GLfloat y);
    //void getCamara(GLfloat *x, GLfloat *y);
    void zoom(GLfloat x, GLfloat y);

    // Interacción con la escena
    bool teclaPulsada(unsigned char Tecla1, int x, int y);
    void teclaEspecial(int Tecla1, int x, int y);
    void mgeDesocupado();
    void pick(int x, int y);
    void pick_color(int x, int y);
    void procesar_hits(unsigned char pixel[3]);
    void draw_seleccion();
    void draw_cubos();
};

#endif
