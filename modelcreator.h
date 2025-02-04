
#ifndef MODELCREATOR_H
#define MODELCREATOR_H

#include <BTCSet.h>
#include <Vector.h>

class System;

struct model_parameters
{

};

class NetworkModel; 

enum class modeltype { hyperbolic, powerlaw };

class ModelCreator
{
public:
    ModelCreator();
    bool Create(System *system, NetworkModel *network );

    string Workingfolder;
    double Simulation_start_time;
    double Simulation_end_time; 
    modeltype ModelType = modeltype::powerlaw;
        
};

#endif // MODELCREATOR_H
