#include "networkmodel.h"
#include "BinaryNumber.h"
#include <iomanip>

NetworkModel::NetworkModel()
{

}

// Parameterized constructor
NetworkModel::NetworkModel(int nodes)
{
    numberOfNodes = nodes;

}

// Copy constructor
NetworkModel::NetworkModel(const NetworkModel& other)
{
    numberOfNodes = other.numberOfNodes;
    edges = other.edges;
}

// Assignment operator
NetworkModel& NetworkModel::operator=(const NetworkModel& other)
{
    numberOfNodes = other.numberOfNodes;
    edges = other.edges;
    return *this;
}

// Destructor
NetworkModel::~NetworkModel()
{

}

// Getters
int NetworkModel::getNumberOfNodes() const
{
    return numberOfNodes;
}
const std::vector<std::pair<int, int>>& NetworkModel::getEdges() const
{
    return edges;
}

// Setters
void NetworkModel::setNumberOfNodes(int nodes)
{
    numberOfNodes = nodes;
}
void NetworkModel::setEdges(const std::vector<std::pair<int, int>>& _edges)
{
    edges = _edges;
}

// Utility function to display the network
void NetworkModel::display() const
{
    for (const auto& edge : edges) {
        std::cout << std::setw(2) << edge.first <<","<< edge.second << std::endl;
    }
}

std::vector<std::vector<int>> NetworkModel::createConnectivityMatrix(int numNodes, int decimalNumber) {
        if (numNodes <= 0) {
            throw std::invalid_argument("Number of nodes must be greater than 0.");
        }

        // Calculate the required number of bits for the upper triangular part of the matrix
        int requiredBits = (numNodes * (numNodes - 1)) / 2;
        int maxDecimalValue = (1 << requiredBits) - 1;

        if (decimalNumber < 0 || decimalNumber > maxDecimalValue) {
            throw std::invalid_argument("Decimal number out of range for the given number of nodes.");
        }

        // Convert the decimal number to binary
        BinaryNumber binaryRepresentation = BinaryNumber::decimalToBinary(decimalNumber);
        binaryRepresentation.fixSize(requiredBits); // Ensure the binary has enough bits

        // Create the connectivity matrix
        std::vector<std::vector<int>> matrix(numNodes, std::vector<int>(numNodes, 0));

        // Map the binary string to the upper triangular part of the matrix
        std::string binaryString = binaryRepresentation.getBinary();
        size_t currentIndex = 0;
        for (int i = 0; i < numNodes; ++i) {
            for (int j = i + 1; j < numNodes; ++j) { // Only iterate over upper triangular part
                if (currentIndex < binaryString.size()) {
                    matrix[i][j] = binaryString[currentIndex] - '0'; // Convert char to int
                    ++currentIndex;
                }
            }
        }
        for (size_t i = 0; i < matrix.size(); ++i) {
            for (size_t j = i + 1; j < matrix[i].size(); ++j) { // Only consider upper triangular part
                if (matrix[i][j] == 1) {
                    edges.emplace_back(i, j); // Add edge (i, j)
                }
            }
        }
        return matrix;
    }

// Function to display the connectivity matrix
void NetworkModel::displayMatrix(const std::vector<std::vector<int>>& matrix) {
    for (const auto& row : matrix) {
        for (int value : row) {
            std::cout << std::setw(2) << value << " ";
        }
        std::cout << std::endl;
    }
}
