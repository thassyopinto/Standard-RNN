/*
 * RNN_Connection.hpp
 *
 * Revision: November 2018
 * Author: Thassyo Pinto - thassyo@ieee.org
 */

#ifndef RNN_CONNECTION_HPP_
#define RNN_CONNECTION_HPP_

// A connection class for neural networks.
class Connection{
  public:
    // Constructor. Creates a new connection in the nerual network.
    // Sets the source, target, weight of this connection.
    Connection(size_t source, size_t target, double weight):
      _source(source),
      _target(target),
      _weight(weight){
    }

    // Returns the weight of this connection.
    const double& getWeight(){
      return _weight;
    }

    // Sets the weight of this connection.
    void setWeight(double weight){
      _weight = weight;
    }

    // Returns the source of this connection.
    const size_t& getSource(){
      return _source;
    }

    // Returns the target of this connection.
    const size_t& getTarget(){
      return _target;
    }

  protected:
    size_t _source;
    size_t _target;
    double _weight;
};

#endif /* RNN_CONNECTION_HPP_ */
