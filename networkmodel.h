#ifndef NETWORKMODEL_H
#define NETWORKMODEL_H


#include <iostream>
#include <vector>

class NetworkModel {
private:
    int numberOfNodes;                       // Number of nodes in the network
    std::vector<std::pair<int, int>> edges; // Edges connecting nodes (node1, node2)

public:
    // Default constructor
    NetworkModel();

    // Parameterized constructor
    NetworkModel(int nodes);

    // Copy constructor
    NetworkModel(const NetworkModel& other);

    // Assignment operator
    NetworkModel& operator=(const NetworkModel& other);

    // Destructor
    ~NetworkModel();

    // Getters
    int getNumberOfNodes() const;
    const std::vector<std::pair<int, int>>& getEdges() const;

    // Setters
    void setNumberOfNodes(int nodes);
    void setEdges(const std::vector<std::pair<int, int>>& edges);

    // Utility function to display the network
    void display() const;

    std::vector<std::vector<int>> createConnectivityMatrix(int numNodes, int decimalNumber);
    void displayMatrix(const std::vector<std::vector<int>>& matrix);
    int MaxDecimalRepresentation() {
        int requiredBits = (numberOfNodes * (numberOfNodes - 1)) / 2;
        int maxDecimalValue = (1 << requiredBits) - 1;
        return maxDecimalValue;
    }
};

#endif // NETWORKMODEL_H
