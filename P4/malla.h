// #############################################################################
//
// Informática Gráfica (Grado Informática)
//
// Archivo: ObjMallaIndexada.h
// -- declaraciones de clase ObjMallaIndexada (mallas indexadas) y derivados
//
// #############################################################################

#ifndef OBJETOS_H_INCLUDED
#define OBJETOS_H_INCLUDED


#include "aux.h"
#include "GL/glut.h"
#include "textura.h"

#if defined(__APPLE__)
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#endif




// ----------------------------------------------------------------------
// NECESARIO PARA CARGAR LAS IMÁGENES DE DISCO
#define cimg_display 0
#include "CImg.h"
using namespace cimg_library;

// *****************************************************************************
//
// clase para objetos 3D (mallas indexadas)
//
// *****************************************************************************
enum ModoVis{Lineas=0,Puntos=1,Solido=2,Ajedrez=3};


struct Material{

  Tupla4f ambiental;
  Tupla4f difusa;
  Tupla4f especular;
  float brillo;

};


class ObjMallaIndexada
{
    public:
      GLuint id_ver = 0,
             id_tri = 0;

      static bool modo_il;

      // dibuja el objeto en modo inmediato
      void draw_ModoInmediato(int materialActual);

      // dibuja el objeto en modo diferido (usando VBOs)
      void draw_ModoDiferido(int materialActual);
      
      //dibuja el objeto en modo ajedrez
      void draw_ModoAjedrez();

     // void draw_iluminacion_plana();
      
      void draw_iluminacion(bool modo_iluminacion);

      GLuint CrearVBO(GLuint tipo_vbo, GLuint tamanio_bytes, GLvoid *puntero_ram);
      
      void asignarMateriales(int materialActual);
      void draw(const ModoVis modo,const bool inmediato, int materialActual);
      


    protected:

      void calcular_normales(); // calcula tabla de normales de vértices (práctica 3)
      void calcular_normales_vertices();
      std::vector<Tupla3f> vertices;   // tabla de coordenadas de vértices (una tupla por vértice, con tres floats)
      std::vector<Tupla3i> triangulos; // una terna de 3 enteros por cada cara o triángulo
      std::vector<Tupla3f> normales_caras;
      std::vector<Tupla3f> normales_vertices;
      bool b_caras = false;
      bool b_vertices = false;

      textura *_textura = NULL;
      char* ruta_textura = nullptr;
      int dimensiones= 0;
      int numero_vertices = 0;
      Material esmeralda, gold, silver, materia_;

      // completar: tabla de colores, tabla de normales de vértices
};

// *****************************************************************************
//
// clases derivadas de ObjMallaIndexada (definen constructores específicos)
//
// *****************************************************************************

// *****************************************************************************
// Cubo con centro en el origen y lado unidad
// (tiene 9 vertices y 6 caras)

class Cubo : public ObjMallaIndexada
{
    public:
      Cubo();
};


class Tetraedro : public ObjMallaIndexada
{
    public:
      Tetraedro();

};

// *****************************************************************************
// objeto leído de un archivo PLY

class ObjPLY : public ObjMallaIndexada
{
    public:
      ObjPLY(const std::string &nombre_archivo);
};

// *****************************************************************************
// objeto de revolución obtenido a partir de un perfil (en un PLY)

class ObjRevolucion : public ObjMallaIndexada
{
    public:
      ObjRevolucion(const std::string &nombre_ply_perfil, int num_instancias_perfil);
      ObjRevolucion(){}
    protected:

      void crearMalla(const std::vector<Tupla3f> & perfil_original, const int num_instancias_perfil, bool tapa_superior, bool tapa_inferior);

};

//********************************************************************************
// clases para el Cilindro, Cono y Esfera

class Cilindro : public ObjRevolucion
{
  public:
  //Cilindro(){}
  Cilindro ( const int num_vert_perfil,const int num_instancias_perfil);
 
};

class Cono : public ObjRevolucion
{
  public:
  //Cono(){}
  Cono ( const int num_vert_perfil,const int num_instancias_perfil);
  
};
class Esfera : public ObjRevolucion
{
  public:
  //Esfera(){}
  Esfera ( const int num_vert_perfil,const int num_instancias_perfil);
  
};

class Cuadro : public ObjMallaIndexada
{
  public:
    Cuadro();
   
  protected:
    std::vector<GLfloat> verticesO ;
    std::vector<GLfloat> texVertices ;


};

class Skybot : public ObjMallaIndexada {
  public:
  Skybot();
     
  protected:
    std::vector<GLfloat> verticesO ;
		
    std::vector<GLfloat> texVertices ;
};


class luz {
    public:
    luz( GLenum p_luz_ind, Tupla4f p_luz_punto, Tupla4f p_luz_ambiente, Tupla4f p_luz_difusa, Tupla4f p_luz_especular);
    void activar();
    void desactivar();
    void transformar(GLenum p_luz_ind,float a, float b, float c, float ang, float x, float y, float z);
    
    protected:
    GLenum luz_ind; //Indice fuente de luz. entre GL_LIGHT0 y 7
    Tupla4f luz_punto; //Posici'on o vector de direci'on de la luz. Se'gun sea el 'ultimo componente 1 o 0.
    Tupla4f luz_ambiente;
    Tupla4f luz_difusa;
    Tupla4f luz_especular;
};


#endif
