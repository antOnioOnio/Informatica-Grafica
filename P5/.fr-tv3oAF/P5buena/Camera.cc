#include "Camera.h"

Camera::Camera( Tupla3f _camView, Tupla3f _camAt, Tupla3f _camup,
            float _left, float _right, float _bottom, float _top,  
            float _camNear, float _camFar ) 
{
  camView = _camView;
  camAt = _camAt;
  camUp = _camup;
  camNear = _camNear;
  camFar = _camFar;
  right = _right;
  left = _left;
  top = _top;
  bottom = _bottom;

  radio=sqrt(pow(camView[0]-camAt[0],2)+pow(camView[1]-camAt[1],2)+pow(camView[2]-camAt[2],2));

}

void Camera::setObserver(){
  gluLookAt(camView[0], camView[1], camView[2], camAt[0], camAt[1], camAt[2], camUp[0] , camUp[1], camUp[2] ) ;
}

void Camera::girarCamara(GLfloat x, GLfloat y){

    camView[0] = sin((y*M_PI)/180) * cos((x*M_PI)/180) * radio;
    camView[1]= sin((x*M_PI)/180) * radio; 
    camView[2] = cos((y*M_PI)/180) * cos((x*M_PI)/180) * radio;

    setObserver();

}

void Camera::posicionarCamara(Tupla3f _camView, Tupla3f _camAt, Tupla3f up, float near, float far)
{
    camView[0] = _camView[0];
    camView[1] = _camView[1];
    camView[2] = _camView[2];
 
    camAt[0] = _camAt[0];
    camAt[1] = _camAt[1];
    camAt[2] = _camAt[2];

    camUp[0] = up[0];
    camUp[1] = up[1];
    camUp[2] = up[2];

    //camAperture = 35.0;
    camNear     = near;
    camFar      = far;
    
}
void Camera::setProyeccion(float _left, float _right, float _bottom, float _top, float _camNear, float _camFar, bool Uortho){

    right = _right;
    left  = _left;
    top   = _top;
    bottom = _bottom;
    camNear = _camNear;
    camFar = _camFar;

    if ( Uortho){
      glOrtho(left*20,right*20,bottom*20,top*20,camNear,camFar);
    }else{
      glFrustum(left,right,bottom,top,camNear,camFar);
    }


    
}

void Camera::zoom(GLfloat distance,bool uOrtho){

    std::cout<<" zoomeando" <<std::endl;

    right =right *distance;
    left  = left * distance;
    top  =top *distance;
    bottom =bottom *distance;
   // camNear *=distance;
   // camFar = _camFar;

    glOrtho(left,right,bottom,top,camNear,camFar);

    // setProyeccion(left,right, bottom,top, camNear, camFar,uOrtho);
    // setObserver();
    
}