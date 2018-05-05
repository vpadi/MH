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
   Definimos el operador de cruce como una clase funcional.
   En este caso utilizaremos la variante de posición. Esto
   es que se conservan los elementos que esten en la misma posición
   en los padres. Los demas elementos los asignamos aleatoriamente.
*/

class ProblemaCruce : public eoQuadOp<Problema> {
public:
   /* El operador "()" hace del cruce entre los dos individuos */
   bool operator()(Problema & _problema1, Problema & _problema2){
      int i;
      int random, temp;
      int pos_no_asignadas[TAM];
      int elem_restantes[TAM];
      int j = 0;

      /* 1º Encontrar los elementos asignados a diferentes posiciones para los dos padres */
      for (i = 0 ; i < TAM ; i++){
         if (_problema1.solucion[i] != _problema2.solucion[i]){
            pos_no_asignadas[j] = i;
            elem_restantes[j] = _problema1.solucion[i];
            j++;
         }
      }

      /* 2º Mezclamos los elementos restantes para asegurar que los elementos
            restantes sean asignados e posiciones aleatorias */

      for (i = 0; i < j; i++){
         random = eo::rng.rand()%(j-i) + i;
         temp = elem_restantes[i];
         elem_restantes[i] = elem_restantes[random];
         elem_restantes[random] = temp;
      }

      /* 3º Copiamos los elementos mezclados restantes a posiciones sin asignar */

      for (i = 0; i < j ; i++)
         _problema1.solucion[pos_no_asignadas[i]] = elem_restantes[i];

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
