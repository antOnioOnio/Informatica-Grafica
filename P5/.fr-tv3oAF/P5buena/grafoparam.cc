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
   cilindro = new Cilindro(4, 16 );
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

//    assert( cubo     != nullptr );
//    assert( cilindro != nullptr );
//    assert ( esfera  != nullptr );

//    // guardar parametros de visualización para esta llamada a 'draw'
//    // (modo de visualización, modo de envío)

//    modo_vis      = p_modo_vis ;
//    usar_diferido = p_usar_diferido ;

//    // dibujar objetos

//    constexpr float
//       altura       = 1,  // separación entre centros de las columnas
//       radio_cil = 0.5 , // radio del cilindro más fino
//       radio_cil2 = radio_cil+0.1 ; // radio del cilindro más grueso

//     // int count = 1;

//     glPushMatrix();
//         // glLoadName(count);
//         glScalef(1,5,1);
//         glTranslatef(-5,0.5,0);
//         cubo->draw(modo_vis,usar_diferido);
//     glPopMatrix();
//     count++;
    
//     glPushMatrix();
//         // glLoadName(count);
//         glScalef(1,5,1);
//         glTranslatef(5,0.5,0);
//         cubo->draw(modo_vis,usar_diferido);
//     glPopMatrix();
//     count++;

//     glPushMatrix();
//         // glLoadName(count);
//         glScalef(1,5,1);
//         glTranslatef(0,0.5,-5);
//         cubo->draw(modo_vis,usar_diferido);
//     glPopMatrix();
//     count++;
}

void GrafoParam::draw_seleccion(){
    // // int count=1;
    // glPushMatrix();
    //     // glLoadName(count);
    //     glScalef(1,5,1);
    //     glTranslatef(-5,0.5,0);
        
    //     glColor3vub(0,100,0);
    //     cubo->draw(modo_vis,usar_diferido);
    // glPopMatrix();
    // count++;
    
    // glPushMatrix();
    //     // glLoadName(count);
    //     glScalef(1,5,1);
    //     glTranslatef(5,0.5,0);
    //             glColor3vub(1,100,0);
    //     cubo->draw(modo_vis,usar_diferido);
    // glPopMatrix();
    // count++;

    // glPushMatrix();
    //     glLoadName(count);
    //     glScalef(1,5,1);
    //     glTranslatef(0,0.5,-5);
    //             glColor3vub(2,100,0);
    //     cubo->draw(modo_vis,usar_diferido);
    // glPopMatrix();
    // count++;
}
