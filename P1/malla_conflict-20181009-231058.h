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

// *****************************************************************************
//
// clase para objetos 3D (mallas indexadas)
//
// *****************************************************************************

class ObjMallaIndexada
{
    public:
      GLuint id_ver = 0,
             id_tri = 0;

      // dibuja el objeto en modo inmediato
      void draw_ModoInmediato();

      // dibuja el objeto en modo diferido (usando VBOs)
      void draw_ModoDiferido();
      
      //dibuja el objeto en modo ajedrez
      void draw_ModoAjedrez();

      GLuint CrearVBO(GLuint tipo_vbo, GLuint tamanio_bytes, GLvoid *puntero_ram);

      // función que redibuja el objeto
      // está función llama a 'draw_MI' (modo inmediato)
      // o bien a 'draw_MD' (modo diferido, VBOs)
      void draw(int modo);

    protected:
      void calcular_normales(); // calcula tabla de normales de vértices (práctica 3)

      std::vector<Tupla3f> vertices;   // tabla de coordenadas de vértices (una tupla por vértice, con tres floats)
      std::vector<Tupla3i> triangulos; // una terna de 3 enteros por cada cara o triángulo

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
      ObjRevolucion(const std::string &nombre_ply_perfil, int numero_rotaciones);
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




#endif
