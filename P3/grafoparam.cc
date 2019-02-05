 // #############################################################################
//
// Informática Gráfica (Grado Informática)
//
// Archivo: GrafoParam.cc
// -- implementación del objeto jerárquico de la práctica 3
//
// #############################################################################


#include "grafoparam.h"

// -----------------------------------------------------------------------------
// constructor: crea mallas indexadas en los nodos terminales del grafo

GrafoParam::GrafoParam()
{
   cilindro = new Cilindro( 4, 16 );
   cubo     = new Cubo();
   esfera   = new Esfera(20,20);
}
// -----------------------------------------------------------------------------
// actualizar valor efectivo de un parámetro (a partir de su valor no acotado)

void GrafoParam::actualizarValorEfe( const unsigned iparam, const float valor_na )
{
   assert( iparam < num_parametros );

   using namespace std ;
   //cout << "GrafoParam::actualizarValorEfe( " << iparam << ", " << valor_na << " )" << endl ;

   constexpr float vp = 2.5 ;

   switch( iparam )
   {

        case 0:
            //altura de la torre
            altura_torre= 7.5 + 3*sin( 0.7*(2.0*M_PI*valor_na) ); 
        break;

        case 1:
            //angulo rotacion de la toore
            ag_rotacion_torre = 150.0*valor_na ;
        break;
            
        case 2:
            //angulo rotacion parte derecha
            ag_rotacion_brazo_der = 5 + 15*sin( 1.5* (2.0*M_PI*valor_na) );
        break;
        case 3:
            // rotacion ventiladores chicos
            ag_rotacion_vp = 1 + 4000.0*valor_na ;
        break;
        case 4:
            // rotacion ventilador grande
            ag_rotacion_vg = 1 + 1000*valor_na ;
        break;
        case 5:
            // rotacion segundo brazo
            ag_rotacion_seg_brazo = 10 + 30*sin( 1.5* (2.0*M_PI*valor_na) );
   }
}

// -----------------------------------------------------------------------------
// visualización del objeto Jerárquico con OpenGL,
// mediante llamadas a los métodos 'draw' de los sub-objetos

void GrafoParam::draw( const ModoVis p_modo_vis, const bool p_usar_diferido )
{
   // asegurarnos que está inicializado ok

   assert( cubo     != nullptr );
   assert( cilindro != nullptr );
   assert ( esfera  != nullptr );

   // guardar parametros de visualización para esta llamada a 'draw'
   // (modo de visualización, modo de envío)

   modo_vis      = p_modo_vis ;
   usar_diferido = p_usar_diferido ;

   // dibujar objetos

   constexpr float
      altura       = 1,  // separación entre centros de las columnas
      radio_cil = 0.5 , // radio del cilindro más fino
      radio_cil2 = radio_cil+0.1 ; // radio del cilindro más grueso

    
    elementosHorizontal();

    elementosTransversal();
    // columna 
    glPushMatrix();
        glScalef(0.5,altura_torre -1,0.5);
        glColor3f(0,0,1);
        cilindro->draw(modo_vis,usar_diferido);
    glPopMatrix();
    // base
    glPushMatrix();
        glTranslatef(0,-0.2,0);
        glScalef(5,0.2,5);
        glColor3f(0,0,0);
        cilindro->draw(modo_vis,usar_diferido);
    glPopMatrix();

}
// -----------------------------------------------------------------------------
// dibujamos los elementos transversales a nuestra torre, 

 void GrafoParam::elementosTransversal(){
     
     glPushMatrix();
        glRotatef(90,0,1,0);
        glPushMatrix();
            glTranslatef(0,altura_torre,0);         // altura de todos los elementos
            glRotatef(ag_rotacion_torre,0,1,0) ;     // rotacion de todos los elementos
            glPushMatrix(); 
                glTranslatef(-10,0,0);          
                bolaHelices(ag_rotacion_vg);        //rotacion de los ventiladores grandes
                bolaBrazo();
                glRotatef(ag_rotacion_brazo_der,0,0,1);
                glPushMatrix();
                    glTranslatef(5,0,0);
                    glRotatef(90,0,0,1); 
                    glPushMatrix();
                        glScalef(0.3,4,0.3);
                        glColor3f(0,0,1);
                        cilindro->draw(modo_vis,usar_diferido);
                    glPopMatrix();
                glPopMatrix();
                glPushMatrix();
                     glTranslatef(6,0,0);
                     glColor3f(1,0.5,0);
                     esfera->draw(modo_vis,usar_diferido);
                    glPushMatrix();
                        glRotatef(ag_rotacion_seg_brazo,1,0,1);
                        glTranslatef(5,0,0);
                        glPushMatrix();
                            glRotatef(90,0,0,1); 
                            glScalef(0.3,4,0.3);
                            glColor3f(0,0,1);
                            cilindro->draw(modo_vis,usar_diferido);
                        glPopMatrix();
                        
                        glPushMatrix();
                            glTranslatef(-9,0,0);   // traslado porque en cuadrilatero desplazamos 9
                            cuadrilatero();
                        glPopMatrix();
                    glPopMatrix();
                glPopMatrix();

            glPopMatrix();
        glPopMatrix();
     glPopMatrix();

 }
// -----------------------------------------------------------------------------
// giramos nuestros elementos transversales 90 grados
void GrafoParam::elementosHorizontal()
{
    glPushMatrix();
        glRotatef(90,0,1,0);
        elementosTransversal();
    glPopMatrix();
}
// -----------------------------------------------------------------------------
// dibujamos 5 cubos con sus cables
void GrafoParam::caraCubos()
{
    glPushMatrix();
   //  cubos a la mitad
        glColor3f(1,0,0);
        glPushMatrix(); 
            cubo->draw(modo_vis,usar_diferido);
        glPopMatrix();  

        glPushMatrix();
            glTranslatef(0,3,3);
            cubo->draw(modo_vis,usar_diferido);
        glPopMatrix(); 

        glPushMatrix();
            glTranslatef(0,3,-3);
            cubo->draw(modo_vis,usar_diferido);
        glPopMatrix(); 

        glPushMatrix();
            glTranslatef(0,-3,3);
            cubo->draw(modo_vis,usar_diferido);
        glPopMatrix(); 

        glPushMatrix();
            glTranslatef(0,-3,-3);
            cubo->draw(modo_vis,usar_diferido);
        glPopMatrix(); 


        glColor3f(0,0,1);
        glPushMatrix();
            glRotatef(45,1,0,0);
            glScalef(0.05,4.5,0.05);
            cilindro->draw(modo_vis,usar_diferido);
        glPopMatrix();

       glPushMatrix();
            glRotatef(-45,1,0,0);
            glScalef(0.05,4.5,0.05);
            cilindro->draw(modo_vis,usar_diferido);
        glPopMatrix();

        glPushMatrix();
            glRotatef(135,1,0,0);
            glScalef(0.05,4.5,0.05);
            cilindro->draw(modo_vis,usar_diferido);
        glPopMatrix();    

        glPushMatrix();
            glRotatef(-135,1,0,0);
            glScalef(0.05,4.5,0.05);
            cilindro->draw(modo_vis,usar_diferido);
        glPopMatrix();


    glPopMatrix(); 
}
// -----------------------------------------------------------------------------
// nuestro ventilador
void GrafoParam::bolaHelices(float ag){

 glRotatef(ag, 1,0,0);

        glPushMatrix();

            // esfera
            glPushMatrix();
            
                glColor3f(1,0.5,0);
                esfera->draw(modo_vis,usar_diferido);

            glPopMatrix();
            
            // helices sin rotar
            glColor3f(0,0,0);
            glPushMatrix();
            
                glTranslatef(0,2,0);
                glScalef(0.2, 2, 0.2);
                cubo->draw(modo_vis, usar_diferido); 
            
            glPopMatrix();

            
            glPushMatrix();
                glTranslatef(0,-2,0);
                glScalef(0.2, 2, 0.2);
                cubo->draw(modo_vis, usar_diferido); 
            
            glPopMatrix();  

            // helices rotadas

            glPushMatrix();
                glRotatef(90,1, 0 , 0 );
                glTranslatef(0,-2,0);
                glScalef(0.2, 2, 0.2);
                cubo->draw(modo_vis, usar_diferido); 
            
            glPopMatrix();  

            glPushMatrix();
                glRotatef(90,1, 0 , 0 );
                glTranslatef(0,2,0);
                glScalef(0.2, 2, 0.2);
                cubo->draw(modo_vis, usar_diferido); 
            
            glPopMatrix();  

            glPopMatrix();    
        glPopMatrix();
}

// -----------------------------------------------------------------------------
// giramos 5 veces caracubos para formar nuestra figura
void GrafoParam::cuadrilatero(){
        glPushMatrix();
            glTranslatef(9,0,0);
            glScalef(0.5,0.5,0.5);
            caraCubos();

            glPushMatrix();
               
                glRotatef(90,0,1,0);
                glTranslatef(3,0,3);
                caraCubos();

            glPopMatrix();

            glPushMatrix();
               
                glRotatef(90,0,1,0);
                glTranslatef(-3,0,3);
                caraCubos();
            glPopMatrix();

            glPushMatrix();
                glTranslatef(6,0,0);
                caraCubos();
            glPopMatrix();

            glPushMatrix();
               
                glRotatef(90,0,0,1);
                glTranslatef(3,-3,0);
                caraCubos();
            glPopMatrix();
            glPushMatrix();
               
                glRotatef(90,0,0,1);
                glTranslatef(-3,-3,0);
                caraCubos();
            glPopMatrix();

         ventiladores(ag_rotacion_vp);
        glPopMatrix();

         
}
// -----------------------------------------------------------------------------
// aniadimos ventiladores
void GrafoParam::ventiladores(float ag){
    glPushMatrix();
        glPushMatrix();

            glPushMatrix();
                glTranslatef(7,0,0);
                glScalef(0.5,0.5,0.5);
                bolaHelices(ag);
            glPopMatrix();

            glPushMatrix();  
                glTranslatef(3,0,4);
                glScalef(0.5,0.5,0.5);
                glRotatef(90,0,1,0);
                bolaHelices(ag);
            glPopMatrix();

            glPushMatrix();  
                glTranslatef(3,0,-4);
                glScalef(0.5,0.5,0.5);
                glRotatef(90,0,1,0);
                bolaHelices(ag);
            glPopMatrix();

        glPopMatrix();
    glPopMatrix();

}

// -----------------------------------------------------------------------------

void GrafoParam::cilindroCuadrilatero(){
            glPushMatrix();
                glTranslatef(9,0,0);
                glRotatef(90,0,0,1); 
                glScalef(0.3,8,0.3);
                glColor3f(0,0,1);
                cilindro->draw(modo_vis,usar_diferido);
            glPopMatrix();
          
            cuadrilatero();
    
}
// -----------------------------------------------------------------------------

void GrafoParam::bolaBrazo()
{
    glPushMatrix();
        glColor3f(1,0.5,0);
        esfera->draw(modo_vis,usar_diferido);

        glPushMatrix();
            glTranslatef(-1,0,0);
            glRotatef(90,0,0,1);        
            glScalef(0.3,8,0.3);
            glColor3f(0,0,1);
            cilindro->draw(modo_vis,usar_diferido);
        glPopMatrix();
       
    glPopMatrix();
}
// -----------------------------------------------------------------------------


