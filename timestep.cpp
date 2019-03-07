#include <iostream>
#include "timestep.hpp"

using namespace std;

namespace TopoOsciSim
{

    // Create default TimestepContainer
    TimestepContainer::TimestepContainer(void) : 
        id       {1}, 
        idBefore {0}, 
        idAfter  {2}, 
        phi      {0.}     {}

    // Create TimestepContainer with specific phi
    TimestepContainer::TimestepContainer(double phi) : 
        id       {1}, 
        idBefore {0}, 
        idAfter  {2}, 
        phi      {phi}    {}

    // Create TimestepContainer with specific phi and id
    TimestepContainer::TimestepContainer(int id, double phi) : 
        id       {id}, 
        idBefore {id-1}, 
        idAfter  {id+1}, 
        phi      {phi}    {}

    // Return ostream for TimestepContainer class
    ostream& operator<<(ostream& out, const TimestepContainer &t)
    {
        // out << t.idBefore << " <- " << t.id << " [" << t.phi << "] -> " << t.idAfter;
        out << t.id << " [" << t.phi << "]";

        return out;
        
    }

    void TimestepContainer::mod2Pi()
    {
        // to set phi between -pi and pi
        phi = phi - 2 * M_PI * round(phi / (2 * M_PI));
    }
    
    // Set random timestep angle in [0, 2pi]
    void TimestepContainer::setRandom(mt19937_64& seed)
    {
        
        uniform_real_distribution< > dist( 0 , 2*M_PI );
        phi = dist(seed);
        
    }

    void TimestepContainer::applySymmetrization(int sym, int Nsym)
    {
        phi += 2 * M_PI * sym / Nsym;
    }
    
    // Return projected phi on some angle
    double TimestepContainer::getProjectedAngle(double& angle)
    {
        // double x = cos(phi) - 2 * cos(phi - angle) * cos(angle);
        // double y = sin(phi) - 2 * cos(phi - angle) * sin(angle);
        
        // return atan2(x, y);
        return (M_PI - phi + 2 * angle);
    }

} // TopoOsciSim
