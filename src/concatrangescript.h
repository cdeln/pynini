// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
// Copyright 2017 and onwards Google, Inc.
//
// For general information on the Pynini grammar compilation library, see
// pynini.opengrm.org.

#ifndef PYNINI_CONCATRANGESCRIPT_H_
#define PYNINI_CONCATRANGESCRIPT_H_

#include <utility>

#include <fst/script/fst-class.h>
#include "concatrange.h"

namespace fst {
namespace script {

using ConcatRangeArgs = std::tuple<MutableFstClass *, int32, int32>;

template <class Arc>
void ConcatRange(ConcatRangeArgs *args) {
  MutableFst<Arc> *fst = std::get<0>(*args)->GetMutableFst<Arc>();
  ConcatRange(fst, std::get<1>(*args), std::get<2>(*args));
}

void ConcatRange(MutableFstClass *fst, int32 lower = 0, int32 upper = 0);

}  // namespace script
}  // namespace fst

#endif  // PYNINI_CONCATRANGESCRIPT_H_

