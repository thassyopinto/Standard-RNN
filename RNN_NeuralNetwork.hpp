/*
 * RNN_NeuralNetwork.hpp
 *
 * Revision: November 2018
 * Author: Thassyo Pinto - thassyo@ieee.org
 */

#ifndef RNN_NEURALNETWORK_HPP_
#define RNN_NEURALNETWORK_HPP_

// Standard libraries
#include <algorithm>
#include <fstream>
#include <vector>

// Local libraries
#include "Misc_Random.hpp"
#include "RNN_Neuron.hpp"
#include "RNN_Connection.hpp"

// An artificial neural network class.
template<typename Neuron_t = Neuron, typename Connection_t = Connection>
class NeuralNetwork{
  public:
    // Builds a recurrent neural network with the supplied number of input and output neurons.
    NeuralNetwork(size_t nbOfInputs = 4, size_t nbOfOutputs = 8){
      this->setInputs(nbOfInputs);
      this->setOutputs(nbOfOutputs);
      size_t numberOfNeurons = _nbOfInputs + _nbOfOutputs;
      // Creates neurons.
      for(int i = 0; i < numberOfNeurons; i++){
        this->addNeuron();
      }
      // Creates connections.
      for(int i = 0; i < _nbOfInputs; i++){
        for(int j = 0; j < _nbOfOutputs; j++){
          this->addConnection(i, j + _nbOfInputs, 0.0);
        }
      }
    }

    // Adds a neuron to this network.
    void addNeuron(){
      _neurons.push_back(Neuron());
    }

    // Adds a connection between the two indicated neurons.
    void addConnection(size_t sourceIndex, size_t targetIndex, double weight = 0.0){
      _connections.push_back(Connection(sourceIndex, targetIndex, weight));
      this->addIncoming(targetIndex, _connections.size() - 1);
      this->addOutgoing(sourceIndex, _connections.size() - 1);
    }

    // Removes a connection between the two indicated neurons.
    void removeConnection(size_t connectionIndex, size_t sourceIndex, size_t targetIndex){
      _connections.erase(_connections.begin() + connectionIndex);
      this->updateIndices(_neurons[sourceIndex].getOutgoingIndices(), connectionIndex);
      this->updateIndices(_neurons[targetIndex].getIncomingIndices(), connectionIndex);
    }

    // Updates the vector of connection indices after removing an existing connection.
    void updateIndices(std::vector<size_t> vect, size_t value){
      vect.erase(std::remove(vect.begin(), vect.end(), value), vect.end());
      for(int i = 0; i < vect.size(); i++){
        if(vect[i] > value){
          vect[i] = vect[i]-1;
        }
      }
    }

    // Sets the number of inputs of this network.
    void setInputs(size_t nbOfInputs){
      _nbOfInputs = nbOfInputs;
    }

    // Sets the number of outputs of this network.
    void setOutputs(size_t nbOfOutputs){
      _nbOfOutputs = nbOfOutputs;
    }

    // Returns the number of inputs of this network.
    size_t getInputs(){
      return _nbOfInputs;
    }

    // Returns the number of outputs of this network.
    size_t getOutputs(){
      return _nbOfOutputs;
    }

    // Sets the minimum weight that a connection may get due to randomization or mutation.
    void setMinWeight(double minWeight){
      _minWeight = minWeight;
    }

    // Sets the maximum weight that a connection may get due to randomization or mutation.
    void setMaxWeight(double maxWeight){
      _maxWeight = maxWeight;
    }

    // Sets the weight mutation rate.
    void setWeightMutRate(double weightMutRate){
      _weightMutRate = weightMutRate;
    }

    // Sets the neuron mutation rate.
    void setNeuronMutRate(double neuronMutRate){
      _neuronMutRate = neuronMutRate;
    }

    // Sets the add neuron mutation rate.
    void setAddNeuronMutRate(double addNeuronMutRate){
      _addNeuronMutRate = addNeuronMutRate;
    }

    // Sets the add connection mutation rate.
    void setAddConnectionMutRate(double addConnectionMutRate){
      _addConnectionMutRate = addConnectionMutRate;
    }

    // Returns a reference to the vector of neurons of this network.
    std::vector<Neuron_t>& getNeurons(){
      return _neurons;
    }

    // Returns a reference to the vector of connections of this network.
    std::vector<Connection_t>& getConnections(){
      return _connections;
    }

    // Sets the activation value of the indicated neuron.
    void setValue(size_t neuronIndex, double value){
      _neurons[neuronIndex].setValue(value);
    }

    // Returns the activation value of the indicated neuron.
    double getValue(size_t neuronIndex){
      if(neuronIndex >= _neurons.size()){
        std::cerr << "Index out of bounds! Index: " << neuronIndex << " size: " << _neurons.size() << std::endl;
      }
      return _neurons[neuronIndex].getValue();
    }

    // Sets the bias of the indicated neuron.
    void setBias(size_t neuronIndex, double bias){
      _neurons[neuronIndex].setBias(bias);
    }

    // Returns the bias of the indicated neuron.
    double getBias(size_t neuronIndex){
      if(neuronIndex >= _neurons.size()){
        std::cerr << "Index out of bounds! Index: " << neuronIndex << " size: " << _neurons.size() << std::endl;
      }
      return _neurons[neuronIndex].getBias();
    }

    // Returns the activation value of the indicated output neuron.
    double getOutputValue(size_t neuronIndex){
      return this->getValue(neuronIndex + _nbOfInputs);
    }

    // Sets the amount of incoming potential of the indicated neuron.
    void setIncoming(size_t neuronIndex, double incoming){
      _neurons[neuronIndex].setIncoming(incoming);
    }

    // Updates the current incoming potential of the indicated neuron.
    void updateIncoming(size_t neuronIndex, double update){
      _neurons[neuronIndex].updateIncoming(update);
    }

    // Updates the activation value of the indicated neuron.
    void propagateNeuron(size_t neuronIndex){
      _neurons[neuronIndex].propagate();
    }

    // Resets the state of the indicated neuron.
    void resetNeuron(size_t neuronIndex){
      _neurons[neuronIndex].reset();
    }

    // Adds the index of an incoming connection to the indicated neuron.
    void addIncoming(size_t neuronIndex, size_t incomingIndex){
      _neurons[neuronIndex].addIncoming(incomingIndex);
    }

    // Adds the index of an outgoing connection to the indicated neuron.
    void addOutgoing(size_t neuronIndex, size_t outgoingIndex){
      _neurons[neuronIndex].addOutgoing(outgoingIndex);
    }

    // Sets the weight of the indicated connection.
    void setWeight(size_t connectionIndex, double weight){
        _connections[connectionIndex].setWeight(weight);
    }

    // Returns the weight of the indicated connection.
    double getWeight(size_t connectionIndex){
      if(connectionIndex >= _connections.size()){
        std::cerr << "Index out of bounds! Index: " << connectionIndex << " size: " << _connections.size() << std::endl;
      }
      return _connections[connectionIndex].getWeight();
    }

    // Returns the source of the indicated connection.
    size_t getSource(size_t connectionIndex){
      if(connectionIndex >= _connections.size()){
        std::cerr << "Index out of bounds! Index: " << connectionIndex << " size: " << _connections.size() << std::endl;
      }
      return _connections[connectionIndex].getSource();
    }

    // Returns the target of the indicated connection.
    size_t getTarget(size_t connectionIndex){
      if(connectionIndex >= _connections.size()){
        std::cerr << "Index out of bounds! Index: " << connectionIndex << " size: " << _connections.size() << std::endl;
      }
      return _connections[connectionIndex].getTarget();
    }

    // Performs one update of network activation.
    void update(){
      // Resets the incoming values of neurons to 0.
      for(int i = 0; i < _neurons.size(); i++){
        this->setIncoming(i, 0.0);
      }
      // Updates the incoming values of all neurons by iterating over all their incoming connections.
      for(int i = 0; i < _neurons.size(); i++){
        std::vector<size_t> incomingConnections = _neurons[i].getIncomingIndices();
        for(int j = 0; j < incomingConnections.size(); j++){
          double updateValue = this->getValue(this->getSource(incomingConnections[j])) * this->getWeight(incomingConnections[j]);
          this->updateIncoming(i, updateValue);
        }
      }
      // Propagates the incoming value to become the current activation of that neuron.
      for(int i = 0; i < _neurons.size(); i++){
        this->propagateNeuron(i);
      }
    }

    // Randomizes the network.
    void randomize(){
      // Assigns each neuron, uniform randomly, a bias in [_minWeight, _maxWeight]
      for(int i = 0; i < _neurons.size(); i++){
        this->setBias(i, randDouble(_minWeight, _maxWeight));
      }
      // Assigns each connection, uniform randomly, a weight in [_minWeight, _maxWeight]
      for(int i = 0; i < _connections.size(); i++){
        this->setWeight(i, randDouble(_minWeight, _maxWeight));
      }
    }

    // Mutates the neural network.
    void mutate(){
      // Probability of neuron-i bias to be mutated.
      for(int i = 0; i < _neurons.size(); i++){
        if(randDouble() <= _neuronMutRate){
          double randBias = randGaussian();
          if(randBias < _minWeight) this->setBias(i, _minWeight);
          else if(randBias > _maxWeight) this->setBias(i, _maxWeight);
          else this->setBias(i, randBias);
        }
      }
      // Probability of connection-i weight to be mutated.
      for(int i = 0; i < _connections.size(); i++){
        if(randDouble() <= _weightMutRate){
          double randWeight = randGaussian();
          if(randWeight < _minWeight) this->setWeight(i, _minWeight);
          else if(randWeight > _maxWeight) this->setWeight(i, _maxWeight);
          else this->setWeight(i, randWeight);
        }
      }
      // Probability of adding a new neuron.
      if(randDouble() <= _addNeuronMutRate){
        // Adds a new neuron.
        this->addNeuron();
        // Randomly selects an existing connection.
        size_t randConnection = randDouble(_connections.size() - 1);
        size_t randSource = this->getSource(randConnection);
        size_t randTarget = this->getTarget(randConnection);
        double randWeight = this->getWeight(randConnection);
        // Removes random connection and update indices.
        this->removeConnection(randConnection, randSource, randTarget);
        // Adds connections of the new neuron.
        addConnection(randSource, _neurons.size()-1, 1.0);
        addConnection(_neurons.size()-1, randTarget, randWeight);
      }
      // Probability of adding a new connection.
      if(randDouble() <= _addConnectionMutRate){
        size_t nodeIndex = 0;
        size_t newSource = randIndex(_neurons.size());
        std::vector<size_t> nodeSet;
        nodeSet.clear();
        // Remove input neurons from node set.
        for(int i = _nbOfInputs; i < _neurons.size(); i++){
          nodeSet.push_back(i);
        }
        // Adds new connection to available node.
        if(!nodeSet.empty()){
          size_t newTarget = nodeSet[randIndex(nodeSet.size())];
          this->addConnection(newSource, newTarget, randDouble(_minWeight, _maxWeight));
        }
      }
    }

    // Initializes the network with specific values and weights.
    void initialize(double initValue, double initWeight){
      for(int i = 0; i < _neurons.size(); i++){
        this->setValue(i, initValue);
      }
      for(int i = 0; i < _connections.size(); i++){
        this->setWeight(i, initWeight);
      }
    }

    // Resets all neurons in the network.
    void reset(){
      for(int i = 0; i < _neurons.size(); i++){
        this->resetNeuron(i);
      }
    }

    // Writes the current activation of the network to the output stream.
    void logActivation(std::ofstream& activationFile){
      if(activationFile.is_open()){
        activationFile << this->getValue(0);
        for(int i = 1; i < _neurons.size(); i++){
          activationFile << " " << this->getValue(i);
        }
        activationFile << "\n";
      }
    }

    // Run the neural network for a given number of updates.
    void run(size_t numberOfUpdates, std::string activationFileName = ""){
      std::ofstream actFile;
      if(activationFileName != "") actFile.open(activationFileName);
      for(int i = 0; i < numberOfUpdates; i++){
        this->update();
        this->logActivation(actFile);
        this->mutate();
      }
      actFile.close();
    }

  protected:
    //Vectors containing neurons and connections.
    std::vector<Neuron_t> _neurons;
    std::vector<Connection_t> _connections;

    // Number of inputs and outputs.
    size_t _nbOfInputs;
    size_t _nbOfOutputs;

    // Weight constraints.
    double _minWeight;
    double _maxWeight;

    //Mutation rates
    double _weightMutRate;
    double _neuronMutRate;
    double _addNeuronMutRate;
    double _addConnectionMutRate;
};

// Convenience function for writing network connections to a file-stream.
std::ostream& operator<<(std::ostream& is, NeuralNetwork<Neuron, Connection>& obj){
  std::vector<Neuron> neurons = obj.getNeurons();
  std::vector<Connection> connections = obj.getConnections();
  size_t nbOfInputs = obj.getInputs();
  size_t nbOfOutputs = obj.getOutputs();

  is << nbOfInputs << " ";
  is << nbOfOutputs << " ";
  is << neurons.size() << " ";
  is << connections.size() << " ";

  // Write neurons to file.
  for(size_t i=0; i<neurons.size(); ++i){
    is << neurons[i].getBias() << " ";
  }

  // Write connections to file.
  for(size_t i=0; i<connections.size(); ++i){
    is << connections[i].getSource() << " " << connections[i].getTarget() << " "<< connections[i].getWeight()<< " ";
  }
  return is;
}

#endif /* RNN_NEURALNETWORK_HPP_ */
