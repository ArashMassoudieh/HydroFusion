#include "modelcreator.h"
#include "System.h"
#include "QString"
#include <iostream>
#include <fstream>
#include <vector>
#include "networkmodel.h"
#include <QRandomGenerator>

using namespace std;

ModelCreator::ModelCreator()
{
}


bool ModelCreator::Create(System *system, NetworkModel *network)
{
#ifdef Behzad 
    system->GetQuanTemplate("/home/behzad/Projects/OpenHydroQual/resources/main_components.json");
    //system->AppendQuanTemplate("/home/behzad/Projects/OpenHydroQual/resources/unsaturated_soil.json");
    //system->AppendQuanTemplate("/home/behzad/Projects/OpenHydroQual/resources/Well.json");
    system->AppendQuanTemplate("/home/behzad/Projects/OpenHydroQual/resources/wastewater.json");
    system->AppendQuanTemplate("/home/behzad/Projects/OpenHydroQual/resources/mass_transfer.json");
    system->ReadSystemSettingsTemplate("/home/behzad/Projects/OpenHydroQual/resources/settings.json");
#endif // Behzad 
#ifdef Arash_Windows
    system->GetQuanTemplate("../../OpenHydroQual/resources/main_components.json");
    system->AppendQuanTemplate("../../OpenHydroQual/resources/physicsbaseddatadrivenhydrology.json");
    system->ReadSystemSettingsTemplate("../../OpenHydroQual/resources/settings.json");
#endif // Window_Arash

    
    for (int i = 0; i < network->getNumberOfNodes(); i++)
    {
        Block node;
        node.SetQuantities(system, "Storage Unit");
        node.SetName("Storage Unit " + QString::number(i).toStdString() );
        node.SetType("Storage Unit");
        node.SetVal("x", QRandomGenerator::global()->bounded(2001));
        node.SetVal("y", QRandomGenerator::global()->bounded(2001));
        node.SetProperty("Precipitation", "C:/Users/arash/Dropbox/Watershed_Modeling/Rain_Watts.csv");
        system->AddBlock(node, false);
    }

    Block dsboundary; 
    dsboundary.SetQuantities(system, "Fixed Latent Variable Unit");
    dsboundary.SetName("Downstream boundary");
    dsboundary.SetType("Fixed Latent Variable Unit");
    dsboundary.SetVal("x", QRandomGenerator::global()->bounded(2001));
    dsboundary.SetVal("y", QRandomGenerator::global()->bounded(2001));
    system->AddBlock(dsboundary, false);
   

    for (int i=0; i<network->getEdges().size(); i++)
    {   Link edge;
        edge.SetQuantities(system, "storage_unit_link");
        edge.SetName("edge(" + aquiutils::numbertostring(network->getEdges()[i].first) + "_" + aquiutils::numbertostring(network->getEdges()[i].second) + ")");
        edge.SetType("storage_unit_link");
        system->AddLink(edge, "Storage Unit " + QString::number(network->getEdges()[i].first).toStdString(), "Storage Unit " + QString::number(network->getEdges()[i].second).toStdString(), false);
    }

    Link edge;
    edge.SetQuantities(system, "storage_unit_link");
    edge.SetName("edge(" + aquiutils::numbertostring(0) + "- Downstream boundary)");
    edge.SetType("storage_unit_link");
    system->AddLink(edge, "Storage Unit 0", "Downstream boundary", false);
    

    //Parameters
    for (int i = 0; i < network->getNumberOfNodes(); i++)
    {
        Parameter P_Capacity; 
        P_Capacity.SetQuantities(system, "Parameter");
        P_Capacity.SetName("P_Capacity_" + QString::number(i).toStdString());
        P_Capacity.SetType("Parameter");
        P_Capacity.SetVal("high", "1e+6");
        P_Capacity.SetVal("low", "1e+4");
        P_Capacity.SetVal("value", 1e+5);
        P_Capacity.SetProperty("prior_distribution", "log-normal");
        system->Parameters().Append(P_Capacity.GetName(), P_Capacity);

        system->SetAsParameter("Storage Unit " + QString::number(i).toStdString(), "Capacity", "P_Capacity_" + QString::number(i).toStdString(),true);
        

        Parameter P_Initial_Content;
        P_Initial_Content.SetQuantities(system, "Parameter");
        P_Initial_Content.SetName("Initial_Content_" + QString::number(i).toStdString());
        P_Initial_Content.SetType("Parameter");
        P_Initial_Content.SetVal("high", "0.95");
        P_Initial_Content.SetVal("low", "0.1");
        P_Initial_Content.SetVal("value", 0.5);
        P_Initial_Content.SetProperty("prior_distribution", "log-normal");
        system->Parameters().Append(P_Initial_Content.GetName(), P_Initial_Content);

        Parameter P_Precip_Factor;
        P_Precip_Factor.SetQuantities(system, "Parameter");
        P_Precip_Factor.SetName("Precip_Factor" + QString::number(i).toStdString());
        P_Precip_Factor.SetType("Parameter");
        P_Precip_Factor.SetVal("high", "1e+7");
        P_Precip_Factor.SetVal("low", "1e+5");
        P_Precip_Factor.SetVal("value", 1e+6);
        P_Precip_Factor.SetProperty("prior_distribution", "log-normal");
        system->Parameters().Append(P_Precip_Factor.GetName(), P_Precip_Factor);

        Parameter P_Scale_Factor;
        P_Scale_Factor.SetQuantities(system, "Parameter");
        P_Scale_Factor.SetName("Scale_Factor" + QString::number(i).toStdString());
        P_Scale_Factor.SetType("Parameter");
        P_Scale_Factor.SetVal("high", "5");
        P_Scale_Factor.SetVal("low", "0.1");
        P_Scale_Factor.SetVal("value", 1);
        P_Scale_Factor.SetProperty("prior_distribution", "log-normal");
        system->Parameters().Append(P_Scale_Factor.GetName(), P_Scale_Factor);





    }

    

    /*
    // Observations
    Observation total_inflow;

    total_inflow.SetQuantities(system, "Observation");
    total_inflow.SetProperty("expression","inflow");
    total_inflow.SetProperty("object","Reactor (1)");
    total_inflow.SetName("Inflow");
    total_inflow.SetType("Observation");
    system->AddObservation(total_inflow,false);


    Observation s_inflow_concentration;

    s_inflow_concentration.SetQuantities(system, "Observation");
    s_inflow_concentration.SetProperty("expression","Solids:inflow_concentration");
    s_inflow_concentration.SetProperty("object","Reactor (1)");
    s_inflow_concentration.SetName("SolidsInflowConcentration");
    s_inflow_concentration.SetType("Observation");
    system->AddObservation(s_inflow_concentration,false);

    Observation coagulant_concentration;

    coagulant_concentration.SetQuantities(system, "Observation");
    coagulant_concentration.SetProperty("expression","Coagulant:external_mass_flow_timeseries");
    coagulant_concentration.SetProperty("object","Settling element (1)");
    coagulant_concentration.SetName("Coagulant_Concentration");
    coagulant_concentration.SetType("Observation");
    system->AddObservation(coagulant_concentration,false);

    Observation solids_concentration;

    solids_concentration.SetQuantities(system, "Observation");
    solids_concentration.SetProperty("expression","Solids:concentration");
    solids_concentration.SetProperty("object","Settling element (1)");
    solids_concentration.SetName("Solids_Concentration");
    solids_concentration.SetType("Observation");
    system->AddObservation(solids_concentration,false);
    */

    system->SetSettingsParameter("simulation_start_time",Simulation_start_time);
    system->SetSettingsParameter("simulation_end_time",Simulation_end_time);
    

    system->SetSystemSettings();
    cout<<"Populate functions"<<endl;
    system->PopulateOperatorsFunctions();
    cout<<"Variable parents"<<endl;
    system->SetVariableParents();
    return true;
}









// ----- Producing OUProcessed Random Inflow -----

/*

    CTimeSeries<double> SolidConcentrationNS;
    SolidConcentrationNS.CreateOUProcess(0,Simulation_time,0.05,1);
    SolidConcentrationNS.writefile(Workingfolder + "inflow_concentration_NS.csv");
    vector<double> params; params.push_back(3); params.push_back(1);
    CTimeSeries<double> SolidConcentration = SolidConcentrationNS.MapfromNormalScoreToDistribution("lognormal", params);
//Reactor.Variable("Solids:inflow_concentration")->SetTimeSeries(SolidConcentration);
    SolidConcentration.writefile(Workingfolder + "inflow_concentration.csv");
    Reactor.SetProperty("Solids:inflow_concentration",Workingfolder + "inflow_concentration.csv");

/*
    CTimeSeries<double> InflowNS;
    InflowNS.CreateOUProcess(0,100,0.05,1);
    vector<double> i_params; i_params.push_back(1.5); i_params.push_back(0.7);
    CTimeSeries<double> Inflow = InflowNS.MapfromNormalScoreToDistribution("lognormal", i_params);
    //Reactor.Variable("inflow")->SetTimeSeries(Inflow);
    Inflow.writefile(Workingfolder + "inflow.csv");
    Reactor.SetProperty("inflow",Workingfolder + "inflow.csv");
    */
