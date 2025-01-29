
#ifndef MODELCREATOR_H
#define MODELCREATOR_H

#include <BTCSet.h>
#include <Vector.h>

class System;

struct model_parameters
{

};

class NetworkModel; 

class ModelCreator
{
public:
    ModelCreator();
    bool Create(System *system, NetworkModel *network );

    string Workingfolder;
    double Simulation_start_time;
    double Simulation_end_time; 
        
};

#endif // MODELCREATOR_H
