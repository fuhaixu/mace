//
// Copyright (c) 2017 XiaoMi All rights reserved.
//

#ifndef MACE_KERNELS_ADDN_H_
#define MACE_KERNELS_ADDN_H_

#include "mace/core/tensor.h"

namespace mace {
namespace kernels {

template<DeviceType D, typename T>
struct AddNFunctor {
  void operator()(std::vector<const Tensor *> &input_tensors, Tensor *output_tensor) {
    Tensor::MappingGuard output_map(output_tensor);
    index_t size = input_tensors[0]->size();
    T *output_ptr = output_tensor->mutable_data<T>();
    memset(output_ptr, 0, size * sizeof(T));
    int n = input_tensors.size();
    for (int i = 0; i < n; ++i) {
      Tensor::MappingGuard input_map(input_tensors[i]);
      const T *input_ptr = input_tensors[i]->data<T>();
      for (index_t j = 0; j < size; ++j) {
        output_ptr[j] += input_ptr[j];
      }
    }
  }
};

template<>
void AddNFunctor<DeviceType::NEON, float>::operator()(
    std::vector<const Tensor *> &input_tensors, Tensor *output_tensor);

template<>
void AddNFunctor<DeviceType::OPENCL, float>::operator()(
    std::vector<const Tensor *> &inputs, Tensor *output);

}  //  namespace kernels
}  //  namespace mace

#endif  // MACE_KERNELS_ADDN_H_