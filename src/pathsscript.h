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

#ifndef PYNINI_PATHSSCRIPT_H_
#define PYNINI_PATHSSCRIPT_H_

#include <algorithm>
#include <vector>

#include <fst/script/arg-packs.h>
#include <fst/script/fstscript.h>
#include "paths.h"

namespace fst {
namespace script {

// Virtual interface implemented by each concrete StatesImpl<F>.
class StringPathIteratorImplBase {
 public:
  virtual bool Done() const = 0;
  virtual bool Error() const = 0;
  virtual void ILabels(std::vector<int64> *labels) const = 0;
  virtual std::vector<int64> ILabels() const = 0;
  virtual void IString(std::string *result) const = 0;
  virtual std::string IString() const = 0;
  virtual void Next() = 0;
  virtual void OLabels(std::vector<int64> *labels) const = 0;
  virtual std::vector<int64> OLabels() const = 0;
  virtual void OString(std::string *result) const = 0;
  virtual std::string OString() const = 0;
  virtual void Reset() = 0;
  virtual WeightClass Weight() const = 0;
  virtual ~StringPathIteratorImplBase() {}
};

// Templated implementation.
template <class Arc>
class StringPathIteratorImpl : public StringPathIteratorImplBase {
 public:
  using Label = typename Arc::Label;

  explicit StringPathIteratorImpl(const Fst<Arc> &fst,
                                  StringTokenType itype = BYTE,
                                  StringTokenType otype = BYTE,
                                  const SymbolTable *isyms = nullptr,
                                  const SymbolTable *osyms = nullptr)
      : impl_(new StringPathIterator<Arc>(fst, itype, otype, isyms, osyms)) {}

  bool Done() const override { return impl_->Done(); }

  bool Error() const override { return impl_->Error(); }

  void ILabels(std::vector<int64> *labels) const override {
    const auto &typed_labels = impl_->ILabels();
    labels->clear();
    labels->resize(typed_labels.size());
    std::copy(typed_labels.begin(), typed_labels.end(), labels->begin());
  }

  std::vector<int64> ILabels() const override {
    std::vector<int64> labels;
    ILabels(&labels);
    return labels;
  }

  void IString(std::string *result) const override { impl_->IString(result); }

  std::string IString() const override { return impl_->IString(); }

  void Next() override { impl_->Next(); }

  void Reset() override { impl_->Reset(); }

  void OLabels(std::vector<int64> *labels) const override {
    const auto &typed_labels = impl_->OLabels();
    labels->clear();
    labels->resize(typed_labels.size());
    std::copy(typed_labels.begin(), typed_labels.end(), labels->begin());
  }

  std::vector<int64> OLabels() const override {
    std::vector<int64> labels;
    OLabels(&labels);
    return labels;
  }

  void OString(std::string *result) const override { impl_->OString(result); }

  std::string OString() const override { return impl_->OString(); }

  WeightClass Weight() const override { return WeightClass(impl_->Weight()); }

 private:
  std::unique_ptr<StringPathIterator<Arc>> impl_;
};

class StringPathIteratorClass;

using InitStringPathIteratorClassArgs =
    std::tuple<const FstClass &, StringTokenType, StringTokenType,
               const SymbolTable *, const SymbolTable *,
               StringPathIteratorClass *>;

// Untemplated user-facing class holding templated pimpl.
class StringPathIteratorClass {
 public:
  explicit StringPathIteratorClass(const FstClass &fst,
                                   StringTokenType itype = BYTE,
                                   StringTokenType otype = BYTE,
                                   const SymbolTable *isyms = nullptr,
                                   const SymbolTable *osyms = nullptr);

  // Same as above, but applies the same string token type and symbol table
  // to both tapes.
  StringPathIteratorClass(const FstClass &fst, StringTokenType type,
                          const SymbolTable *syms = nullptr)
      : StringPathIteratorClass(fst, type, type, syms, syms) {}

  bool Done() const { return impl_->Done(); }

  bool Error() const { return impl_->Error(); }

  void ILabels(std::vector<int64> *labels) const { impl_->ILabels(labels); }

  std::vector<int64> ILabels() const { return impl_->ILabels(); }

  template <class Arc>
  friend void InitStringPathIteratorClass(
      InitStringPathIteratorClassArgs *args);

  void IString(std::string *result) const { impl_->IString(result); }

  std::string IString() const { return impl_->IString(); }

  void Next() { impl_->Next(); }

  void Reset() { impl_->Reset(); }

  void OLabels(std::vector<int64> *labels) const { impl_->OLabels(labels); }

  std::vector<int64> OLabels() const { return impl_->OLabels(); }

  std::string OString() const { return impl_->OString(); }

  WeightClass Weight() const { return WeightClass(impl_->Weight()); }

 private:
  std::unique_ptr<StringPathIteratorImplBase> impl_;
};

template <class Arc>
void InitStringPathIteratorClass(InitStringPathIteratorClassArgs *args) {
  const Fst<Arc> &fst = *(std::get<0>(*args).GetFst<Arc>());
  std::get<5>(*args)->impl_.reset(new StringPathIteratorImpl<Arc>(
      fst, std::get<1>(*args), std::get<2>(*args), std::get<3>(*args),
      std::get<4>(*args)));
}

}  // namespace script
}  // namespace fst

#endif  // PYNINI_PATHSSCRIPT_H_

