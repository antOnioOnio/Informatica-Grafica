#include "aux.h"
#include "ply_reader.h"
#include "malla.h"
#include <algorithm>
// *****************************************************************************
//
// Clase ObjMallaIndexada
//
// *****************************************************************************

// Visualización en modo inmediato con 'glDrawElements'

void ObjMallaIndexada::draw_ModoInmediato()
{
  // visualizar la malla usando glDrawElements,
  // completar (práctica 1)
  // ...
  // habilitar uso de un array de vertices
  glEnableClientState(GL_VERTEX_ARRAY);

  // indicar el formato y la direccion de memoria del array de vertices
  // (son tuplas de 3 valores float, sin espacio entre ellas )
  glVertexPointer(3, GL_FLOAT, 0, vertices.data());

  // visualizar, indicando: tipo de primitiva, numero de indices,
  // tipo de los indices, y direccion de la tabla de indices
  glDrawElements(GL_TRIANGLES, triangulos.size() * 3,
                 GL_UNSIGNED_INT, triangulos.data());

  // deshabilitar array de vertices
  glDisableClientState(GL_VERTEX_ARRAY);
}
// -----------------------------------------------------------------------------
// Visualización en modo diferido con 'glDrawElements' (usando VBOs)

void ObjMallaIndexada::draw_ModoDiferido()
{
  // (la primera vez, se deben crear los VBOs y guardar sus identificadores en el objeto)
  // completar (práctica 1)
  // .....

  if (id_ver == 0)
  {
    id_ver = CrearVBO(GL_ARRAY_BUFFER, vertices.size() * 3 * sizeof(float), vertices.data());
  }

  if (id_tri == 0)
  {
    id_tri = CrearVBO(GL_ELEMENT_ARRAY_BUFFER, triangulos.size() * 3 * sizeof(int), triangulos.data());
  }
  // especificar localización y formato de la tabla de vértices, habilitar tabla
  glBindBuffer(GL_ARRAY_BUFFER, id_ver); // activar VBO de vértices
  glVertexPointer(3, GL_FLOAT, 0, 0);    // especifica formato y offset (=0)
  glBindBuffer(GL_ARRAY_BUFFER, 0);      // desactivar VBO de vértices.
  glEnableClientState(GL_VERTEX_ARRAY);  // habilitar tabla de vértices

  // visualizar triángulos con glDrawElements (puntero a tabla == 0)

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_tri); // activar VBO de triángulos
  glDrawElements(GL_TRIANGLES, 3 * triangulos.size(), GL_UNSIGNED_INT, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); // desactivar VBO de triángulos
  // desactivar uso de array de vértices
  glDisableClientState(GL_VERTEX_ARRAY);
  
}

void ObjMallaIndexada::draw_ModoAjedrez()
{
  std::vector<Tupla3i> pares, impares;

 // glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  glEnableClientState(GL_VERTEX_ARRAY);
  glVertexPointer(3, GL_FLOAT, 0, vertices.data());

  for (int i = 0; i < triangulos.size(); i++)
  {
    if (i % 2 == 0)
    {
      pares.push_back(triangulos.at(i));
    }
    else
    {
      impares.push_back(triangulos.at(i));
    }

  }
    glColor3f(0.5, 0.2, 0);
    glDrawElements(GL_TRIANGLES, pares.size() * 3, GL_UNSIGNED_INT, &(pares.at(0)));

    glColor3f(1, 0.4, 0.5);
    glDrawElements(GL_TRIANGLES, pares.size() * 3, GL_UNSIGNED_INT, &(impares.at(0)));
}

// -----------------------------------------------------------------------------
// Función de visualización de la malla,
// puede llamar a  draw_ModoInmediato o bien a draw_ModoDiferido
void ObjMallaIndexada::draw(const ModoVis modo,const bool inmediato)
{

   switch (modo)
  {
  case 0:
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    break;

  case 1:
    glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
    break;

  case 2:
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    break;

   case 3:
     glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
     draw_ModoAjedrez();
     break;

  }


  if (inmediato){
    draw_ModoInmediato();
  }else{
    draw_ModoDiferido();
  }


 
}

GLuint ObjMallaIndexada::CrearVBO(GLuint tipo_vbo, GLuint tamanio_bytes, GLvoid *puntero_ram)
{
  GLuint id_vbo;                  // resultado: identificador de VBO
  glGenBuffers(1, &id_vbo);       // crear nuevo VBO, obtener identificador (nunca 0)
  glBindBuffer(tipo_vbo, id_vbo); // activar el VBO usando su identificador

  // esta instrucción hace la transferencia de datos desde RAM hacia GPU
  glBufferData(tipo_vbo, tamanio_bytes, puntero_ram, GL_STATIC_DRAW);

  glBindBuffer(tipo_vbo, 0); // desactivación del VBO (activar 0)

  return id_vbo; // devolver el identificador resultado
}

// -----------------------------------------------------------------------------
// Recalcula la tabla de normales de vértices (el contenido anterior se pierde)

void ObjMallaIndexada::calcular_normales()
{
  // completar .....
}

// *****************************************************************************
//
// Clase Cubo (práctica 1)
//
// *****************************************************************************

Cubo::Cubo()
{

  // inicializar la tabla de vértices
  vertices = {
      {-0.5, -0.5, -0.5}, // 0
      {-0.5, -0.5, +0.5}, // 1
      {-0.5, +0.5, -0.5}, // 2
      {-0.5, +0.5, +0.5}, // 3
      {+0.5, -0.5, -0.5}, // 4
      {+0.5, -0.5, +0.5}, // 5
      {+0.5, +0.5, -0.5}, // 6
      {+0.5, +0.5, +0.5}  // 7
  };

  

  // inicializar la tabla de caras o triángulos:
  // (es importante en cada cara ordenar los vértices en sentido contrario
  //  de las agujas del reloj, cuando esa cara se observa desde el exterior del cubo)

  triangulos = {
      {0, 2, 4},
      {4, 2, 6},
      {1, 5, 3},
      {3, 5, 7},
      {1, 3, 0},
      {0, 3, 2},
      {5, 4, 7},
      {7, 4, 6},
      {1, 0, 5},
      {5, 0, 4},
      {3, 7, 2},
      {2, 7, 6}};
}

// *****************************************************************************
//
// Clase Tetraedro (práctica 1)
// *****************************************************************************

Tetraedro::Tetraedro()
{
  vertices = {
      {0, 0, 0},  // 0
      {1.0, 0, 0}, // 1
      {0, 1.0, 0}, // 2
      {0, 0, 1.0}, // 3
  };

  triangulos = {
      {0, 1, 2},
      {1, 2, 3},
      {0, 2, 3},
      {3, 1, 0}};
}
//
// *****************************************************************************

// *****************************************************************************
//
// Clase ObjPLY (práctica 2)
//
// *****************************************************************************

ObjPLY::ObjPLY(const std::string &nombre_archivo)
{
  // leer la lista de caras y vértices
  ply::read(nombre_archivo, vertices, triangulos);
  
}

// *****************************************************************************
//
// Clase ObjRevolucion (práctica 2)
//
// *****************************************************************************

// *****************************************************************************
// objeto de revolución obtenido a partir de un perfil (en un PLY)
// *****************************************************************************

ObjRevolucion::ObjRevolucion(const std::string &nombre_ply_perfil, int num_instancias_perfil)
{
  // completar ......(práctica 2)
 std::vector<Tupla3f> perfil_original;
 ply::read_vertices(nombre_ply_perfil,perfil_original);
 crearMalla(perfil_original,num_instancias_perfil,true,true);

}

void ObjRevolucion::crearMalla(const std::vector<Tupla3f> & perfil_original, const int num_instancias_perfil, bool tapa_superior, bool tapa_inferior)
{

  // alpha = 2*i*M_PI/num_instancias_perfil
  // x = cos(alpha)*px + sin(alpha)pz
  // y = y
  // z = -sin(apha)*px + cos(alpha)pz

    Tupla3f vertice;
    Tupla3i triangulo;
    float coordenada_x, coordenada_y, coordenada_z;

 
    
  // Partimos de la tabla de vertices vacia.
  // Para cada i desde 0 hasta N − 1 (ambos incluidos)
  // Para cada j desde 0 hasta M − 1 (ambos incluidos)
  // Sea v = vertice obtenido rotando oj un angulo igual a 2πi/N radianes. 
  // Anadir val a tabla de vertices(alfinal).
    
    vertices.clear();
    triangulos.clear();
    
    
    float M = perfil_original.size();
    float alpha;
    for (int i = 0 ; i < num_instancias_perfil; i++){

      alpha = 2*i*M_PI/num_instancias_perfil;

      for ( int j = 0; j < M; j++){

        coordenada_x = (perfil_original[j](0) * cos(alpha)) + perfil_original[j](2)*sin(alpha);

        coordenada_y = perfil_original[j](1);

        coordenada_z = perfil_original[j](0) * (-sin(alpha)) + perfil_original[j](2)*cos(alpha);

        vertice=Tupla3f(coordenada_x,coordenada_y,coordenada_z);
        vertices.push_back(vertice);
        
      }

    }   
  
 
//  Partimos de la tabla de triangulos vacia
//  Para cada i desde 0 hasta N − 1 (ambos incluidos)
//  Para cada j desde 0 hasta M − 2 (ambos incluidos)
//  Sea a = Mi+j
//  Sea b = M((i+1)modN)+j
//  Anadir triangulo formado por los indices a,b y b+1.
//  Anadir triangulo formado por los indices a,b+1 y a+1.

    float a,b;

    for ( int i = 0 ; i < num_instancias_perfil; i++){

      for ( int j = 0 ; j< M-1; j++){

        a = M*i+j;
        b = M*((i+1)%num_instancias_perfil) + j;
        
        triangulo = Tupla3i(a, b, b+1);
        triangulos.push_back(triangulo);

        triangulo = Tupla3i(a,b+1,a+1);
        triangulos.push_back(triangulo);
        
      }
    
    }


  // aniadir tapa
  //Finalmente, si se desea anadir las tapas a la malla, se insertaran el vertice en el polo sur (tendra  indice NM)
  // y en el polo norte (que tendra índice NM + 1). Despues se pueden insertar los N triangulos en la tapa inferior, 
  //que conectan el vertice en el polo sur con los N primeros vertices de cada instancia (el primer vertice de la 
  //i-esima instancia tiene indice Mi, para i desde cero hasta N − 1). Finalmente, se insertan los triangulos de la 
  //tapa superior, que conectan el vertice en el polo norte con el ultimo vertice de cada instancia (el ultimo vertice
  // de la i-esima instancia tiene como indiceM(i+1)−1,de nuevo con i entre 0yN −1).

  Tupla3f verticePoloSur, verticePoloNorte;
  

    if ( tapa_inferior)
    {
      // vamos a situar el vertice en el polo sur

      coordenada_x=0.0;
      coordenada_y=perfil_original[0](1);
      coordenada_z=0.0;

     
      verticePoloSur=Tupla3f(coordenada_x,coordenada_y,coordenada_z);
      vertices.push_back(verticePoloSur);

    
      // calculamos triangulos de la tapa inferior

        for ( int i = 0 ; i < num_instancias_perfil ; i++){
          triangulo=Tupla3i(num_instancias_perfil*M , i*M, M*((i+1)%num_instancias_perfil)) ;
          triangulos.push_back(triangulo);
        }
        
    }


    if ( tapa_superior)
    {  

      // situamos vertice en el polo norte
        verticePoloNorte;
        
          coordenada_x=0.0;
          coordenada_y= perfil_original[M-1](1);
          coordenada_z= 0.0;

        verticePoloNorte = Tupla3f(coordenada_x,coordenada_y,coordenada_z);
        vertices.push_back(verticePoloNorte);


        // calculamos triangulos de la tapa superior

          for ( int i = 0 ; i < num_instancias_perfil ; i++){
            triangulo=Tupla3i(num_instancias_perfil*M+1 ,M*(i+1)-1, M*((i+1)%num_instancias_perfil)+M-1 );
            triangulos.push_back(triangulo);
          }
    }
}
//***********************************************************************************************
Cilindro::Cilindro(const int num_vert_perfil,const int num_instancias_perfil)
{
 
  std::vector<Tupla3f> vector_auxiliar;
  float coordenada_x, coordenada_y, coordenada_z;
  Tupla3f vertice;

  // coordenada inicial
    coordenada_x = 1.0;
    coordenada_y = 0.0;
    coordenada_z = 0.0;
  
  vertice = Tupla3f(coordenada_x,coordenada_y,coordenada_z);
  vector_auxiliar.push_back(vertice);

  
  float aumento = 1/((float)num_vert_perfil-1);

  for ( int i = 0 ; i < num_vert_perfil-1; i++){
    
    coordenada_y += aumento;
    vertice = Tupla3f(coordenada_x,coordenada_y,coordenada_z);
    vector_auxiliar.push_back(vertice);
    
  }

  crearMalla(vector_auxiliar,num_instancias_perfil,true,true);

}

//***********************************************************************************************
Cono::Cono(const int num_vert_perfil,const int num_instancias_perfil)
{

  std::vector<Tupla3f> vector_auxiliar;
  float coordenada_x, coordenada_y, coordenada_z;
  Tupla3f vertice;

// coordenada inicial
    coordenada_x = 1.0;
    coordenada_y = 0.0;
    coordenada_z = 0.0;

    vertice = Tupla3f(coordenada_x,coordenada_y,coordenada_z);
    vector_auxiliar.push_back(vertice);

  float aumento = 1/((float)num_vert_perfil-1);

  for ( int i = 0 ; i < num_vert_perfil-1; i++){
   
    coordenada_y += aumento;
    coordenada_x = coordenada_x - aumento;

    vertice = Tupla3f(coordenada_x,coordenada_y,coordenada_z);
    vector_auxiliar.push_back(vertice);

  }
 
    crearMalla(vector_auxiliar,num_instancias_perfil,false,true);

}

//***********************************************************************************************
// Funcion para ordenar nuestor perfil de la circunferencia

Esfera::Esfera ( const int num_vert_perfil,const int num_instancias_perfil)
{

  std::vector<Tupla3f> vector_auxiliar;
  float coordenada_x, coordenada_y, coordenada_z;
  Tupla3f vertice;

  float radio = 1.0;
  float alpha = 0;

    //coordenada inicial
    // coordenada_x = 0.0;
    // coordenada_y = -1.0;
     coordenada_z = 0.0;

    // vertice = Tupla3f(coordenada_x,coordenada_y,coordenada_z);
    // vector_auxiliar.push_back(vertice);

    for ( int i = 0 ; i <= num_vert_perfil; i++){

      alpha = -(M_PI/2) + (i*M_PI/(num_vert_perfil+1));

      coordenada_x = radio*cos(alpha);
      coordenada_y = radio*sin(alpha);

      vertice = Tupla3f(coordenada_x,coordenada_y,coordenada_z);
      vector_auxiliar.push_back(vertice);

    }

   // coordenada final;
    coordenada_x = 0.0;
    coordenada_y = 1.0;
    coordenada_z = 0.0;

    vertice = Tupla3f(coordenada_x,coordenada_y,coordenada_z);
    vector_auxiliar.push_back(vertice);

  crearMalla(vector_auxiliar,num_instancias_perfil,false,false);


}
