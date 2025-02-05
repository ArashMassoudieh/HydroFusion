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
    

    //Parameters for blocks
    for (int i = 0; i < network->getNumberOfNodes(); i++)
    {
        Parameter P_Capacity; 
        P_Capacity.SetQuantities(system, "Parameter");
        P_Capacity.SetName("P_Capacity_" + QString::number(i).toStdString());
        P_Capacity.SetType("Parameter");
        P_Capacity.SetVal("high", "1e+6");
        P_Capacity.SetVal("low", "5e+3");
        P_Capacity.SetVal("value", 1e+5);
        P_Capacity.SetProperty("prior_distribution", "log-normal");
        system->Parameters().Append(P_Capacity.GetName(), P_Capacity);

        system->SetAsParameter("Storage Unit " + QString::number(i).toStdString(), "Capacity", "P_Capacity_" + QString::number(i).toStdString(),true);
        

        Parameter P_Initial_Content;
        P_Initial_Content.SetQuantities(system, "Parameter");
        P_Initial_Content.SetName("Initial_Content_" + QString::number(i).toStdString());
        P_Initial_Content.SetType("Parameter");
        P_Initial_Content.SetVal("high", "0.95");
        P_Initial_Content.SetVal("low", "0.05");
        P_Initial_Content.SetVal("value", 0.5);
        P_Initial_Content.SetProperty("prior_distribution", "log-normal");
        system->Parameters().Append(P_Initial_Content.GetName(), P_Initial_Content);

        system->SetAsParameter("Storage Unit " + QString::number(i).toStdString(), "Initial_Storage_coeff", "Initial_Content_" + QString::number(i).toStdString(), true);

        Parameter P_Precip_Factor;
        P_Precip_Factor.SetQuantities(system, "Parameter");
        P_Precip_Factor.SetName("Precip_Factor_" + QString::number(i).toStdString());
        P_Precip_Factor.SetType("Parameter");
        P_Precip_Factor.SetVal("high", "1e+7");
        P_Precip_Factor.SetVal("low", "2e+4");
        P_Precip_Factor.SetVal("value", 1e+6);
        P_Precip_Factor.SetProperty("prior_distribution", "log-normal");
        system->Parameters().Append(P_Precip_Factor.GetName(), P_Precip_Factor);


        system->SetAsParameter("Storage Unit " + QString::number(i).toStdString(), "Precipitation_coefficient", "Precip_Factor_" + QString::number(i).toStdString(), true);

        Parameter P_Scale_Factor;
        P_Scale_Factor.SetQuantities(system, "Parameter");
        P_Scale_Factor.SetName("Scale_Factor_" + QString::number(i).toStdString());
        P_Scale_Factor.SetType("Parameter");
        P_Scale_Factor.SetVal("high", "5");
        P_Scale_Factor.SetVal("low", "0.05");
        P_Scale_Factor.SetVal("value", 1);
        P_Scale_Factor.SetProperty("prior_distribution", "log-normal");
        system->Parameters().Append(P_Scale_Factor.GetName(), P_Scale_Factor);

        system->SetAsParameter("Storage Unit " + QString::number(i).toStdString(), "Scale_factor", "Scale_Factor_" + QString::number(i).toStdString(), true);

    }

    //Parameters for edges
    for (int i = 0; i < network->getEdges().size(); i++)
    {
        Parameter P_Mean_Transmissivity;
        P_Mean_Transmissivity.SetQuantities(system, "Parameter");
        P_Mean_Transmissivity.SetName("P_Transmissivity_(" + QString::number(network->getEdges()[i].first).toStdString() + "_" + QString::number(network->getEdges()[i].second).toStdString() + ")");
        P_Mean_Transmissivity.SetType("Parameter");
        P_Mean_Transmissivity.SetVal("high", "2e5");
        P_Mean_Transmissivity.SetVal("low", "100");
        P_Mean_Transmissivity.SetVal("value", 10000);
        P_Mean_Transmissivity.SetProperty("prior_distribution", "log-normal");
        system->Parameters().Append(P_Mean_Transmissivity.GetName(), P_Mean_Transmissivity);

        system->SetAsParameter("edge(" + aquiutils::numbertostring(network->getEdges()[i].first) + "_" + aquiutils::numbertostring(network->getEdges()[i].second) + ")", "Mean_Transmissivity", "P_Transmissivity_(" + QString::number(network->getEdges()[i].first).toStdString() + "_" + QString::number(network->getEdges()[i].second).toStdString() + ")", true);


    }

    Parameter P_Mean_Transmissivity;
    P_Mean_Transmissivity.SetQuantities(system, "Parameter");
    P_Mean_Transmissivity.SetName("P_Transmissivity_(" + QString::number(0).toStdString() + "_ Downstream boundary)");
    P_Mean_Transmissivity.SetType("Parameter");
    P_Mean_Transmissivity.SetVal("high", "1e5");
    P_Mean_Transmissivity.SetVal("low", "100");
    P_Mean_Transmissivity.SetVal("value", 10000);
    P_Mean_Transmissivity.SetProperty("prior_distribution", "log-normal");
    system->Parameters().Append(P_Mean_Transmissivity.GetName(), P_Mean_Transmissivity);

    system->SetAsParameter("edge(" + aquiutils::numbertostring(0) + "- Downstream boundary)", "Mean_Transmissivity", "P_Transmissivity_(" + QString::number(0).toStdString() + "_ Downstream boundary)", true);

    Parameter P_Downstream_Latent_Variable;
    P_Downstream_Latent_Variable.SetQuantities(system, "Parameter");
    P_Downstream_Latent_Variable.SetName("P_DS_Latent_Variable");
    P_Downstream_Latent_Variable.SetType("Parameter");
    P_Downstream_Latent_Variable.SetVal("high", "10");
    P_Downstream_Latent_Variable.SetVal("low", "-10");
    P_Downstream_Latent_Variable.SetVal("value", 0);
    P_Downstream_Latent_Variable.SetProperty("prior_distribution", "normal");
    system->Parameters().Append(P_Downstream_Latent_Variable.GetName(), P_Downstream_Latent_Variable);

    system->SetAsParameter("Downstream boundary", "Latent_variable", "P_DS_Latent_Variable", true);


    
    // Observations
    Observation StreamFlow;
    StreamFlow.SetQuantities(system, "Observation");
    StreamFlow.SetProperty("expression","flow");
    StreamFlow.SetProperty("object", "edge(" + aquiutils::numbertostring(0) + "- Downstream boundary)");
    StreamFlow.SetName("StreamFlow");
    StreamFlow.SetType("Observation");
    StreamFlow.SetProperty("observed_data", "C:/Users/arash/Dropbox/Watershed_Modeling/Flow_Watts.csv");
    StreamFlow.SetProperty("error_structure", "log-normal");
    system->AddObservation(StreamFlow,false);


    
    system->SetSettingsParameter("simulation_start_time", 43737.8);
    system->SetSettingsParameter("simulation_end_time", 43826.8);
    system->SetSettingsParameter("maximum_time_allowed", 200);
        

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
