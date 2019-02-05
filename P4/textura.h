#include <string>
#include <vector>
#include "GL/glut.h"
// #include "GL/gl.h"
// #include "GL/glu.h"

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
#define cimg_use_jpeg
#include "CImg.h"
using namespace cimg_library;



class textura{

public:
//Functions declarations
textura(std::vector<GLfloat> _vertices ,std::vector<GLfloat> _texVertices);

//void Init(int argc, char **argv);
//void OnDraw();

// FUNCIÓN QUE PREPARA LA TEXTURA
void prepara_textura(const char* ruta);

// FUNCIÓN QUE DIBUJA
void dibuja(int dimensiones,int tamanio);

// FUNCIÓN QUE LIBERA LA TEXTURA DE GPU
void libera_textura(void);

//void asignaVectores( );

// IDENTIFICADOR DE TEXTURA
protected:
GLuint textura_id;

std::vector<GLfloat> vertices ;
		
std::vector<GLfloat> texVertices ;

};