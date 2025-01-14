/* Copyright 2019 The TensorFlow Authors. All Rights Reserved.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
==============================================================================*/

#include <memory>
#include <utility>

#include "absl/memory/memory.h"
#include "tensorflow/compiler/xla/literal.h"
#include "tensorflow/compiler/xla/service/hlo_computation.h"
#include "tensorflow/compiler/xla/service/hlo_instruction.h"
#include "tensorflow/compiler/xla/service/hlo_module.h"
#include "tensorflow/compiler/xla/service/hlo_opcode.h"
#include "tensorflow/compiler/xla/service/mlir_gpu/mlir_irgen_test_base.h"
#include "tensorflow/compiler/xla/shape_util.h"
#include "tensorflow/core/platform/test.h"

namespace xla {
namespace mlir_gpu {

class LhloGenTest : public MlirIrGenTestBase {};

TEST_F(LhloGenTest, Add) {
  CompileAndVerifyIr(R"(
HloModule Add

ENTRY %Add (x: f32[2,2], y: f32[2,2]) -> f32[2,2] {
  %x = f32[2,2]{1,0} parameter(0)
  %y = f32[2,2]{1,0} parameter(1)
  ROOT %add = f32[2,2]{1,0} add(f32[2,2]{1,0} %x, f32[2,2]{1,0} %y)
})",
                     R"(
;CHECK: module {
;CHECK:  func @add(%{{.*}}: memref<2x2xf32>, %{{.*}}: memref<2x2xf32>, %{{.*}}: memref<2x2xf32>) {
;CHECK:    "xla_lhlo.add"(%{{.*}}, %{{.*}}, %{{.*}}) {name = "add"} : (memref<2x2xf32>, memref<2x2xf32>, memref<2x2xf32>) -> ()
;CHECK:  }
;CHECK: }
      )");
}

}  // namespace mlir_gpu
}  // namespace xla
