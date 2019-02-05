

#include "aux.h"
#include "GL/glut.h"

#if defined(__APPLE__)
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#endif

class Camera{

    private:
    
    Tupla3f camView;
    Tupla3f camAt;
    Tupla3f camUp;
 
    // defined as they are used by OpenGL
    // always => positive ; Far > Near (distance from plane to camera origin)
    float camNear;
    float camFar;

    float right, left, top , bottom;
    float radio;

    public:

    Camera();
    Camera( Tupla3f _camView, Tupla3f _camAt, Tupla3f _camup,
            float _left, float _right, float _bottom, float _top,  
            float _camNear, float _camFar );


    void posicionarCamara(Tupla3f _camView, Tupla3f _camAt, Tupla3f up, float near, float farTu);
    void setObserver();
    void girarCamara(GLfloat x, GLfloat y); 
    void zoom(GLfloat distance, bool uOrtho);
    void setProyeccion(float _right, float _left, float _top, float _bottom, float _camNear, float _camFar, bool ortho);
    // dentro de este llamamos a flustum y orto dependiendo de si es ortogonal o perspectiva ( f1 y f2)

    //void girarCamara( float angulo);

    // void Rotar_Latitud( camera *thisCamera, float inc );
    // void Rotar_Longitud( camera *thisCamera, float inc );    


};


