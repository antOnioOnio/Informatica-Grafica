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
bool ObjMallaIndexada::modo_il = false;



void ObjMallaIndexada::draw_ModoInmediato(int materialActual)
{
  // visualizar la malla usando glDrawElements,
  // completar (práctica 1)
  // ...
  // habilitar uso de un array de vertices
  glEnableClientState(GL_VERTEX_ARRAY);
  
  // indicar el formato y la direccion de memoria del array de vertices
  // (son tuplas de 3 valores float, sin espacio entre ellas )
  glVertexPointer(3, GL_FLOAT, 0, vertices.data());

  if (glIsEnabled(GL_LIGHTING) ){

    asignarMateriales(materialActual);
    draw_iluminacion(modo_il);
  } 
  // visualizar, indicando: tipo de primitiva, numero de indices,
  // tipo de los indices, y direccion de la tabla de indices

  glDrawElements(GL_TRIANGLES, triangulos.size() * 3,
                 GL_UNSIGNED_INT, triangulos.data());

  // deshabilitar array de vertices
  glDisableClientState(GL_VERTEX_ARRAY);

  if ( glIsEnabled(GL_LIGHTING)){
      glDisableClientState(GL_NORMAL_ARRAY);
  }

}

// -----------------------------------------------------------------------------
// Visualización en modo diferido con 'glDrawElements' (usando VBOs)

void ObjMallaIndexada::draw_ModoDiferido(int materialActual)
{
  // (la primera vez, se deben crear los VBOs y guardar sus identificadores en el objeto)
  // completar (práctica 1)
  // .....
  if (glIsEnabled(GL_LIGHTING) ){
      asignarMateriales(materialActual);
      draw_iluminacion(modo_il);
    } 
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

// -----------------------------------------------------------------------------
// Visualización en modo ajedrez

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

void ObjMallaIndexada::draw(const ModoVis modo,const bool inmediato,int materialActual)
{
  
   //glEnable(GL_CULL_FACE);
   if ( !b_caras){
     calcular_normales();
     b_caras = true;
   }
   
   switch (modo)
  {
  case 0:
    
    //std::cout << "Modo lineas"<<std::endl;
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    break;

  case 1:
   // std::cout << "Modo puntos"<<std::endl;
    glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
    break;

  case 2:
  //  std::cout << "Modo relleno"<<std::endl;
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    break;

   case 3:
    // std::cout << "Modo ajedrez"<<std::endl;      
     glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
     if ( !glIsEnabled(GL_LIGHTING)){
       draw_ModoAjedrez();
     }
     
     break; 
  

  }
 
 
    if (!inmediato){ 
      draw_ModoInmediato(materialActual);
    }else{
      draw_ModoDiferido(materialActual);
    }

    if (_textura!=NULL){
        glColor3f(1.0,1.0,1.0);
        _textura->prepara_textura(ruta_textura);
        _textura->dibuja(dimensiones, numero_vertices);
        _textura->libera_textura();   
    }


 
}

// -----------------------------------------------------------------------------
// Creacion VBO

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
// Calculamos normales a los triangulos

void ObjMallaIndexada::calcular_normales()
{
 
      int v0, v1, v2;
    
    Tupla3f v0v1, v1v2, normal;
    for ( int i = 0 ; i < triangulos.size() ; i ++){
  
        v0 = triangulos[i](0);
        v1 = triangulos[i](1);
        v2 = triangulos[i](2);
        
      
          v0v1 = vertices[v1] - vertices[v0];
          v1v2 = vertices[v2] - vertices[v0];
        
  
          // vector normal
          normal = v0v1.cross(v1v2);
          normales_caras.push_back(normal);
            
    }

 calcular_normales_vertices();
  
}

// -----------------------------------------------------------------------------
// Calculamos normales a los vertices

void ObjMallaIndexada::calcular_normales_vertices(){

    Tupla3f inicial = Tupla3f( 0.0, 0.0, 0.0);
    float m;

 
        normales_vertices.assign(vertices.size(),inicial);

      // inicializamos 

      for ( int i = 0 ; i < triangulos.size() ; i++){

        normales_vertices[triangulos[i](0)] = normales_vertices[triangulos[i](0)] + normales_caras[i];
        normales_vertices[triangulos[i](1)] = normales_vertices[triangulos[i](1)] + normales_caras[i];
        normales_vertices[triangulos[i](2)] = normales_vertices[triangulos[i](2)] + normales_caras[i];  
        
      }
      for ( int i = 0 ; i < vertices.size() ; i++){
          m=sqrt(normales_vertices[i](0) * normales_vertices[i](0) +
                 normales_vertices[i](1) * normales_vertices[i](1) +
                 normales_vertices[i](2)* normales_vertices[i](2)  ) ;
          normales_vertices[i](0) /=m;
          normales_vertices[i](1) /=m;
          normales_vertices[i](2) /=m;
      }

 
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
      {3, 1, 2 },
      {2, 1, 0 },
      {2, 0, 3 },
      {3, 0, 1 }};
}


Cuadro::Cuadro()
{

  verticesO = {        0.0, 0.0,
                       0.0, 5.0,
                       5.0, 5.0,  
                       5.0, 0.0,

                       0.0, 0,0,
                       2.5, 0.0,
                       5.0, 0.0,
                       0.0, 2.5,
                       0.0, 5.0
                       };


  texVertices = {    0.0,1.0,
                     0.0,0.0,
                     1.0,0.0,
                     1.0,1.0 } ;
  dimensiones = 2;
  numero_vertices = 4;
  this->ruta_textura = "./imagenes/meme.jpg";

  _textura = new textura(verticesO, texVertices);
}

Skybot::Skybot()
{
  texVertices.clear();
  verticesO.clear();

      
texVertices = {       0.0  , 0.25 , // primera
                      0.0  , 0.5  , 
                      0.25  , 0.5  , 
                      0.25 , 0.25 , 

                      0.25 , 0.25 , // segunda
                      0.25 , 0.5  , 
                      0.5  , 0.5  , 
                      0.5   , 0.25 , 
                     
                      0.50 , 0.25 , // tercera
                      0.5  , 0.5  , 
                      0.75 , 0.5  , 
                      0.75  , 0.25 ,
                      
                      0.75 , 0.25 , // cuarta
                      0.75 , 0.5  , 
                      0.5  , 0.5  , 
                      1.00 , 0.25 , 
                     

                      0.25 , 0.0  , // quinta
                      0.25 , 0.25 , 
                      0.5  , 0.25 , 
                      0.5  , 0.0  , 

                      0.25 , 0.5  , // sexta
                      0.25 , 0.75 , 
                      0.5   , 0.75 ,
                      0.5  , 0.5  };
                           
                            
 verticesO = {
                        -0.5, 0.5, 0.5,
                        -0.5, -0.5, 0.5,
                        0.5, -0.5, 0.5,
                        0.5, 0.5, 0.5,

                        0.5, 0.5, 0.5,
                        0.5, -0.5,0.5,
                        0.5, -0.5,-0.5,
                        0.5, 0.5, -0.5,

                        0.5, 0.5,-0.5,
                        0.5,-0.5,-0.5,
                        -0.5, -0.5,-0.5,
                        -0.5, 0.5,-0.5,

                        -0.5, 0.5,-0.5,
                        -0.5, -0.5, -0.5,
                        -0.5, -0.5, 0.5,
                        -0.5, 0.5,0.5,

                        -0.5, 0.5, 0.5,
                        0.5, 0.5, 0.5,
                        0.5, 0.5,-0.5,
                        -0.5, 0.5,-0.5,

                        0.5, -0.5, 0.5,
                        -0.5, -0.5, 0.5,
                        -0.5, -0.5,-0.5,
                        0.5, -0.5,-0.5 };

  dimensiones = 3;
  numero_vertices = 24;
  this->ruta_textura = "./imagenes/skybot.jpg";
  
  _textura = new textura(verticesO, texVertices);
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
// -----------------------------------------------------------------------------
// Creamos la malla de revolucion
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
          triangulo=Tupla3i(num_instancias_perfil*M , M*((i+1)%num_instancias_perfil) ,i*M ) ;
          triangulos.push_back(triangulo);
        }
        
    }


    if ( tapa_superior)
    {  

      // situamos vertice en el polo norte
       //() verticePoloNorte;
        
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
//CONSTRUCTOR CILINDRO
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
//CONSTRUCTOR CONO
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
//CONSTRUCTOR ESFERA
//***********************************************************************************************

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

    //coordenada final
    coordenada_x = 0.0;
    coordenada_y = 1.0;
    
    coordenada_z = 0.0;

    vertice = Tupla3f(coordenada_x,coordenada_y,coordenada_z);
    vector_auxiliar.push_back(vertice);

  crearMalla(vector_auxiliar,num_instancias_perfil,false,false);


}

//***********************************************************************************************
//CONSTRUCTOR LUZ
//***********************************************************************************************

luz::luz(GLenum p_luz_ind, Tupla4f p_luz_punto, Tupla4f p_luz_ambiente, Tupla4f p_luz_difusa, Tupla4f p_luz_especular)
{
    luz_ind = p_luz_ind;
    luz_punto = p_luz_punto;
    luz_ambiente = p_luz_ambiente;
    luz_difusa = p_luz_difusa;
    luz_especular = p_luz_especular;
}

//-------------------------------------------------------------
// FUNCION ACTIVAR LUZ
//-------------------------------------------------------------

void luz::activar()
{
    glEnable(luz_ind);

    glLightfv(luz_ind, GL_AMBIENT, (GLfloat*)&luz_ambiente);
    glLightfv(luz_ind, GL_DIFFUSE, (GLfloat*)&luz_difusa);
    glLightfv(luz_ind, GL_SPECULAR, (GLfloat*)&luz_especular);
    glPushMatrix();
    glLightfv(luz_ind, GL_POSITION, (GLfloat*)&luz_punto);
    glPopMatrix();
}

//-------------------------------------------------------------
// FUNCION DESACTIVAR LUZ
//-------------------------------------------------------------

void luz::desactivar(){
  glDisable(luz_ind);
  
}

//-------------------------------------------------------------
// FUNCION TRANSFORMAR LUZ
//-------------------------------------------------------------

void luz::transformar(GLenum p_luz_ind, float a, float b, float c, float ang, float x, float y, float z)
{
    glPushMatrix();
    glRotatef(ang, a, b, c);
    glTranslatef(x, y, z);
    glLightfv(p_luz_ind, GL_POSITION, (GLfloat*)&luz_punto);
    glPopMatrix();

}

//-------------------------------------------------------------
// FUNCION ILUMINACION
//-------------------------------------------------------------
void  ObjMallaIndexada::draw_iluminacion(bool modo_iluminacion)
{
      std::cout<<"Iluminacion suave"<< std::endl;

  glEnableClientState(GL_NORMAL_ARRAY);
  glNormalPointer(GL_FLOAT, 0, normales_vertices.data() );

  if ( modo_iluminacion){
    
    glShadeModel(GL_SMOOTH);
  }else {
    glShadeModel(GL_FLAT);

  }
  
   
  glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT,(GLfloat *) & materia_.ambiental);
  glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,(GLfloat *) &materia_.difusa);
  glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,(GLfloat *)&materia_.especular);
  glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,materia_.brillo);

  glEnable(GL_NORMALIZE);
  
}

//-------------------------------------------------------------
// FUNCION PARA ASIGNAR MATERIALES
//-------------------------------------------------------------
void ObjMallaIndexada::asignarMateriales(int materialActual){

  Tupla4f ambiental1=  Tupla4f(0.0215, 0.1745, 0.0215, 1.0);       // ambiental
  Tupla4f difusa1 =    Tupla4f(0.07568, 0.61424, 0.07568, 1.0);      // difusa
  Tupla4f especular1 = Tupla4f(0.633, 0.727811, 0.633, 1.0);        // especular
  float brillo1 =      128*0.6;
  
  esmeralda.ambiental = ambiental1;
  esmeralda.difusa = difusa1;
  esmeralda.especular = especular1;
  esmeralda.brillo = brillo1;  


  Tupla4f ambiental2=  Tupla4f(0.24725, 0.1995, 0.0745, 1.0);       // ambiental
  Tupla4f difusa2 =   Tupla4f(0.75164, 0.60648, 0.22648, 1.0);      // difusa
  Tupla4f especular2 = Tupla4f(0.628281, 0.555802, 0.366065, 1.0);        // especular
  float brillo2 =      128*0.4;

  gold.ambiental = ambiental2;
  gold.difusa = difusa2;
  gold.especular = especular2;
  gold.brillo = brillo2;  



  Tupla4f ambiental3=  Tupla4f(0.19225,	0.19225	,0.19225, 1.0);       // ambiental
  Tupla4f difusa3 =   Tupla4f(0.50754	,0.50754	,0.50754, 1.0);      // difusa
  Tupla4f especular3 = Tupla4f(	0.508273,	0.508273,	0.508273, 1.0);        // especular
  float brillo3 =      128*0.4;

  silver.ambiental = ambiental3;
  silver.difusa = difusa3;
  silver.especular = especular3;
  silver.brillo = brillo3;  

      switch (materialActual){
        
        case 0:
          materia_ = gold;
        break;
        case 1:
          materia_ = silver;
        break;
        case 2:
          materia_ = esmeralda;
        break;

      }
}
