#include "System.h"
#include "Script.h"
#include "qfileinfo.h"
#include "modelcreator.h"
#include "networkmodel.h"
//#include "vtk.h"


int main(int argc, char *argv[])
{

NetworkModel networkmodel(6);
cout<<networkmodel.MaxDecimalRepresentation()<<endl;
std::vector<std::vector<int>> matrix = networkmodel.createConnectivityMatrix(6, networkmodel.MaxDecimalRepresentation());

networkmodel.displayMatrix(matrix);


#ifdef Behzad
    string Workingfolder="/home/behzad/Projects/ASM_Models/";
#else
    string Workingfolder="/home/arash/Projects/Watershed_Modeling/";
#endif



}
