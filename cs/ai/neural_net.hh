
#ifndef CS_AI_NN_hh
#define CS_AI_NN_hh

#include <functional>
#include <vector>

#include "cs/result/result.hh"
#include "cs/string/format.h"

namespace cs::ai::nn {
struct Perceptron {
 public:
  Perceptron() {}
  std::vector<float> inputs;
  std::vector<float> weights;
  std::function<float(float)> activation_fn;
  cs::result::ResultOr<float> output() const;
};
struct Layer {
 public:
  std::vector<cs::ai::nn::Perceptron> nodes;
};
struct NeuralNet {
 public:
  std::vector<cs::ai::nn::Layer> layers;
  cs::result::Result validate();
  cs::result::Result train();
  cs::result::ResultOr<std::vector<float>> apply(
      std::vector<float> inputs) const;
};
}  // namespace cs::ai::nn

#endif  // CS_AI_NN_hh
