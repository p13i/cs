#include "cs/ai/neural_net.hh"

#include <vector>

#include "cs/ai/neural_net.hh"
#include "cs/result/result.hh"
#include "gmock/gmock.h"
#include "gtest/gtest.h"

using cs::ai::nn::Layer;
using cs::ai::nn::NeuralNet;
using cs::ai::nn::Perceptron;
using cs::result::ResultOr;

TEST(NeuralNet, BaseCase) {
  NeuralNet nn;
  Layer input_layer;
  input_layer.nodes = {Perceptron(), Perceptron()};
  Layer output_layer;
  output_layer.nodes = {Perceptron(), Perceptron()};
  nn.layers = {input_layer, output_layer};
  std::vector<float> data;
  cs::result::ResultOr<std::vector<float>> outputs_or =
      nn.apply(data);
  EXPECT_TRUE(outputs_or.ok());
}
