#ifndef TIMESTEP_H
#define TIMESTEP_H

#include <random>

using namespace std;

namespace TopoOsciSim
{

  class TimestepContainer
  {
      
  public:

      // Timestep id
      int id;
      
      // Left neighbor
      int idBefore;

      // Right neighbor
      int idAfter;
      
      // Timestep angle
      double phi;

      // Create default TimestepContainer
      TimestepContainer();

      /**
         Create TimestepContainer with specific phi

         @param phi Timetep angle phi
       */
      TimestepContainer(double phi);

      /**
         Create TimestepContainer with specific phi and id

         @param id  Timestep id
         @param phi Timetep angle
       */      
      TimestepContainer(int id, double phi);

      /**
         Return ostream for TimestepContainer class
         
         @param out Ostream where output goes
         @param t   This class
         @return    Ostream including t
      */
      friend ostream& operator<<(ostream& out, const TimestepContainer &t);

      // set phi to phi modulo 2 pi
      void mod2Pi();
      
      /**
         Set random timestep angle in [0, 2pi]

         @param seed Seed number
       */
      void setRandom(mt19937_64& seed);

      /**
         Apply Symmetrization to angle

         @param sym  Symmetrization number
         @param Nsym Number of symmetrizations
      */
      void applySymmetrization(int sym, int Nsym);
      
      /**
         Return projected phi on some angle

         @param angle Projection angle
         @return Projected angle
      */
      double getProjectedAngle(double& angle);

  };

} // TopoOsciSim

#endif // TIMESTEP_H
