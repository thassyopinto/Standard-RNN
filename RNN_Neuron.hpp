/*
 * RNN_Neuron.hpp
 *
 * Revision: November 2018
 * Author: Thassyo Pinto - thassyo@ieee.org
 */

#ifndef RNN_NEURON_HPP_
#define RNN_NEURON_HPP_

// Standard libraries
#include <cmath>

// A neuron class for neural networks.
class Neuron{
  public:
    // Enumerator for the different possible activation functions.
    // - linear: identity, except that it truncates values to lie in [-1, 1]
    // - sin: calculates the sine-wave (f(x)=sin(x))
    // - gaussian: calculates a gaussian (f(x)=e^(-x*x)), scaled to lie in [-1, 1]
    // - sigmoid: calculates the sigmoid (f(x)=tanh(x*lambda))
    enum af_t{
      linear,
      sine,
      gaussian,
      sigmoid,
      nbActivationFunctions
    };
    // Constructor. Creates a new neuron with the indicated number of incoming connection.
    // The current and new activations are initialized to zero.
    Neuron():
      _value(0),
      _incoming(0),
      _bias(0),
      _lambda(5.0),
      _activationFunction(sigmoid){
    }

    // Return the current activation value of this neuron.
    double getValue(){
      return _value;
    }

    // Sets the current value.
    void setValue(double value){
      _value = value;
    }

    // Returns the current amount of incoming potential.
    const double& getIncoming(){
      return _incoming;
    }

    // Sets the amount of incoming potential.
    void setIncoming(double incoming){
      _incoming = incoming;
    }

    // Adds the supplied value to the current incoming potential.
    void updateIncoming(double update){
      _incoming += update;
    }

    // Returns the bias of this neuron.
    double getBias(){
      return _bias;
    }

    // Sets the bias of this neuron.
    void setBias(double bias){
      _bias = bias;
    }

    // Adds the index of an incoming connection.
    void addIncoming(size_t incoming){
      _incomingIndices.push_back(incoming);
    }

    // Adds the index of an outgoing connection.
    void addOutgoing(size_t outgoing){
      _outgoingIndices.push_back(outgoing);
    }

    // Returns the vector of incoming connection indices.
    const std::vector<size_t>& getIncomingIndices(){
      return _incomingIndices;
    }

    // Returns the vector of outgoing connection indices.
    const std::vector<size_t>& getOutgoingIndices(){
      return _outgoingIndices;
    }

    // Resets the state of this neuron.
    void reset(){
      _value = 0;
      _incoming = 0;
    }

    // Updates the activation value based on the incoming potential and the bias.
    void propagate(){
      double x = this->getIncoming() + this->getBias();
      switch(_activationFunction){
        case linear:
          if(x > 1) _value = 1;
          else if(x < -1) _value = -1;
          else _value = x;
          break;
        case sine:
          _value = std::sin(x);
          break;
        case gaussian:
          _value = std::exp(float(-x*x)) * 2.0 - 1.0;
          break;
        case sigmoid:
          _value = std::tanh(x * _lambda);
          break;
        default:
          std::cout << "Error! No activation function found!" << std::endl;
          break;
      }
    }

    // Sets the current activation function.
    // Possible values are:
    // - linear
    // - sin
    // - guassian
    // - sigmoid
    void setActivationFunction(af_t activation){
      _activationFunction = activation;
    }

    // Returns the current activation function.
    af_t getActivationFunction(){
      return _activationFunction;
    }

  protected:
    // ANN attributes
    af_t _activationFunction;
    std::vector<size_t> _incomingIndices;
    std::vector<size_t> _outgoingIndices;
    double _value;
    double _incoming;
    double _bias;
    double _lambda;
};

#endif /* RNN_NEURON_HPP_ */
