// #############################################################################
//
// Informática Gráfica (Grado Informática)
//
// Archivo: GrafoParam.h
// -- declaraciones de clase para el objeto jerárquico de la práctica 3
//
// #############################################################################

#ifndef GRAFOPARAM_H_INCLUDED
#define GRAFOPARAM_H_INCLUDED

#include "malla.h" // añadir .h de cualquier objetos malla indexada usados....

constexpr int num_parametros = 6 ; // número de parámetros o grados de libertad
                                   // de este modelo

class GrafoParam
{
   public:

   // crea mallas indexadas (nodos terminales del grafo)
   GrafoParam();

   // función principal de visualización
   void draw( const ModoVis p_modo_vis, const bool p_usar_diferido );

   // actualizar valor efectivo de un parámetro (a partir de su valor no acotado)
   void actualizarValorEfe( const unsigned iparam, const float valor_na );

   // devuelve el número de parámetros
   unsigned numParametros() { return num_parametros ; }

   private:

   // métodos de dibujo de subgrafos
   void cuadrilatero();
   void cilindroCuadrilatero();
   void caraCubos();
   void elementosHorizontal() ;     
   void bolaHelices(float ag);
   void bolaBrazo();
   void ventiladores(float ag);
   void elementosTransversal();

   // objetos tipo malla indexada (nodos terminales)

   Cilindro * cilindro = nullptr ;
   Cubo *     cubo     = nullptr ;
   Esfera *   esfera   = nullptr ;

   // parámetros de la llamada actual (o última) a 'draw'
   ModoVis modo_vis ;      // modo de visualización
   bool    usar_diferido ; // modo de envío (true -> diferido, false -> inmediato)


   // valores efectivos de los parámetros (angulos, distancias, factores de
   // escala, etc.....) calculados a partir de los valores no acotados

   float altura_torre,         // altura de la columna
         ag_rotacion_torre,    // angulo en grados de rotación de todo
         ag_rotacion_vg,       // angulo en grados de los ventiladores grandes
         ag_rotacion_vp,       // angulo en grados de los ventiladores pequeños
         ag_rotacion_brazo_der,// angulo de rotacion de los brazos
         ag_rotacion_seg_brazo;
         

} ;

#endif
