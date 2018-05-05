#ifndef QAP_H
#define QAP_H

#include <eo>
#include <iostream>
#include <stdlib.h>
#include <vector>

/* global variables */
extern unsigned int TAM; // size
extern std::vector<std::vector<int> > flux, dist;

class Problema : public EO<eoMinimizingFitness> {

public:

   std::vector<int> solucion;

   Problema () {
      solucion.resize(TAM);
      create();
   }

   Problema (const Problema & _problema){ // copy constructor
      solucion.resize(TAM);

      for (int i = 0; i < TAM ; i++)
         solucion[i] = _problema.solucion[i];

      if (!_problema.invalid()) // if the solucion has already been evaluated
         fitness(_problema.fitness()); // copy the fitness
   }

   ~Problema(){ // destructor
   }

   void operator= (const Problema & _problema){ // copy assignment operator
      for (int i = 0; i < TAM ; i++)
         solucion[i] = _problema.solucion[i];

      fitness(_problema.fitness()); // copy the fitness
   }

   int& operator[] (unsigned i) {
      return solucion[i];
   }


   void create(){ // create and initialize a solucion
      int random, temp;

      for (int i=0; i< TAM; i++)
         solucion[i]=i;

      // we want a random permutation so we shuffle
      for (int i = 0; i < TAM; i++){
         random = eo::rng.rand()%(TAM-i) + i;
         temp = solucion[i];
         solucion[i] = solucion[random];
         solucion[random] = temp;
      }
   }

   int evaluate() const{ // evaluate the solucion
      int cost=0;

      for (int i=0; i<TAM; i++)
         for (int j=0; j<TAM; j++)
            cost += flux[i][j] * dist[solucion[i]][solucion[j]];

      return cost;
   }

   friend std::ostream& operator<<(std::ostream &os, const Problema& prob);
};

std::ostream& operator<<(std::ostream &os, const Problema& prob){
   for (int i = 0; i < TAM; i++)
      os << prob.solucion[i] << " " ;

   os << "\n\nCoste de la solución: " << prob.evaluate() << std::endl;
}
#endif
