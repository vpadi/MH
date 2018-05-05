#ifndef _QAPGA_h
#define _QAPGA_h

#include "QAP.hpp"

class ProblemaInit : public eoInit<Problema> {
public:

   void operator()(Problema & _problema) {
      _problema.create();
   }
};

class ProblemaEvalFunc : public eoEvalFunc<Problema> {
public:

   void operator()(Problema & _problema) {
      _problema.fitness(_problema.evaluate());

   }
};

/*
   Clase representacion del operador de cruce por orden (OX)
*/
class ProblemaCruce : public eoQuadOp<Problema> {
public:
   /*
      Los dos parametros de entrada son los padres. A su vez, los padres hacen de hijos
   */
   bool operator()(Problema & _problema1, Problema & _problema2){
      const unsigned SIZE = _problema1.solucion.size();
      std::vector<int> h1(SIZE), h2(SIZE);
      bool en_hijo1[SIZE] = {false}, en_hijo2[SIZE] = {false};

      /*
         Creamos dos indices aleatorios que delimiten una subcadena
         en los cromosomas padres que heredaran los hijos
      */
      unsigned int n1 = eo::rng.rand() % (SIZE-1),
                   n2 = eo::rng.rand() % SIZE;

      unsigned int izq = std::min(n1, n2),
                   dcha = std::max(n1, n2);

      /* Marcamos que tales valores se han heredado en los hijos */

      for (size_t i = izq; i <= dcha; ++i) {
         h1[i] = _problema1.solucion[i];
         en_hijo1[h1[i]] = true;

         h2[i] = _problema2.solucion[i];
         en_hijo2[h2[i]] = true;
      }

      /*
         Aquí la parte importante del algoritmo. Lo que realiza es un desplazamiento
         a la izquierda de los valores del padre del que no ha heredado que no esten
         ya insertados. Para ello creamos dos indices suplementarios que indiquen
         en la posicion real que hay que insertar en los cromosomas hijos.

         Si no estaba el valor actual que indica "i", se introduce en el hijo y
         se avanza una posicion el indice real.
      */

      unsigned ind_real_h1 = 0,
               ind_real_h2 = 0;

      for (int i = 0; i < SIZE; ++i) {
         if(ind_real_h1 == izq)
            ind_real_h1 = dcha + 1;
         if(ind_real_h2 == izq)
            ind_real_h2 = dcha + 1;

         if(!en_hijo1[_problema2.solucion[i]]) {
            h1[ind_real_h1] = _problema2.solucion[i];
            ++ind_real_h1;
         }

         if(!en_hijo2[_problema1.solucion[i]]) {
            h2[ind_real_h2] = _problema1.solucion[i];
            ++ind_real_h2;
         }
      }

      /* Finalmente copiamos los hijos en los padres */

      _problema1.solucion = h1;
      _problema2.solucion = h2;

      // El cruce entre dos soluciones siempre es posible
      return true;
   }
};

class ProblemaSwapMutacion: public eoMonOp<Problema> {
public:

   bool operator()(Problema& _problema)  {
      int i,j;
      int temp;

      // Generamos dos índices distintos aleatorios que sean distintos el uno del otro
      i=eo::rng.rand()%TAM;

      do
         j = eo::rng.rand()%TAM;
      while (i == j);

      // Realizamos el swap

      std::swap(_problema[i],_problema.solucion[j]);

      return true;
   }
};

#endif
