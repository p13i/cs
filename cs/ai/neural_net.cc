#include "cs/ai/neural_net.hh"

#include <functional>
#include <vector>

#include "cs/result/result.hh"
#include "cs/string/format.hh"

using cs::ai::nn::Layer;
using cs::ai::nn::NeuralNet;
using cs::result::Ok;
using cs::result::Result;
using cs::result::ResultOr;

ResultOr<float> cs::ai::nn::Perceptron::output() const {
  if (inputs.size() != weights.size()) {
    return cs::result::Error(cs::string::format(
        "Size of inputs doesn't match size of weights: "
        "`inputs.size()=%d` != `weights.size()=%d`.",
        inputs.size(), weights.size()));
  }
  const float n = inputs.size();
  float weighted_sum = 0.f;
  for (int i = 0; i < n; i++) {
    weighted_sum += inputs[i] * weights[i];
  }
  const float output = activation_fn(weighted_sum);
  return output;
};

Result NeuralNet::validate() { return Ok(); }

Result NeuralNet::train() { return Ok(); }

ResultOr<std::vector<float>> NeuralNet::apply(
    std::vector<float> inputs) const {
  std::vector<float> outputs = inputs;
  return outputs;
};