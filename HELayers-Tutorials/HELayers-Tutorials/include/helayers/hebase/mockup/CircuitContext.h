/*
 * MIT License
 *
 * Copyright (c) 2020 International Business Machines
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef SRC_HELAYERS_CIRCUITCONTEXT_H
#define SRC_HELAYERS_CIRCUITCONTEXT_H

#include "helayers/hebase/HeContext.h"
#include "helayers/hebase/CTile.h"
#include "helayers/math/CTileTensor.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <mutex>

#include "helayers/circuit/OpType.h"
#include "helayers/circuit/Types.h"

namespace helayers {

namespace circuit {
class AbstractIo;
}

class CircuitPlaintext;
class CircuitCiphertext;

class CircuitContext : public HeContext
{
private:
  // bool hasSecKey = false;
  int slots;
  // bool supportsChainIndices;
  int topChainIndex;

  int nextCtxtId;
  int nextPtxtId;

  circuit::AbstractIo* output;

  std::map<std::string, circuit::CtxtId> labelToId;
  std::map<circuit::CtxtId, circuit::LeveledRawValues> unlabeledInputs;

  std::recursive_mutex dataMutex;

  void saveImpl(std::ostream& out,
                const PublicFunctions& publicFunctions) const override;
  void loadImpl(std::istream& out) override;

public:
  CircuitContext();
  ~CircuitContext();

  void init(const HeConfigRequirement& req) override;

  circuit::CtxtId getCtxtId();
  circuit::PtxtId getPtxtId();

  void setCircuitOutput(circuit::AbstractIo* o);

  void labelCtxt(const CTile& c, const std::string& label);
  void labelCtxt(const CTileTensor& c, const std::string& label);

  virtual std::shared_ptr<AbstractCiphertext> createAbstractCipher()
      const override;

  virtual std::shared_ptr<AbstractPlaintext> createAbstractPlain()
      const override;

  std::shared_ptr<AbstractEncoder> getEncoder() const override;

  virtual inline int slotCount() const override { return slots; }

  virtual inline int getTopChainIndex() const override { return topChainIndex; }

  bool hasSecretKey() const override { return true; }

  inline int getSecurityLevel() const override { return 0; }

  virtual void printSignature(std::ostream& out) const override;

  void debugPrint(const std::string& title = "",
                  int verbose = 0,
                  std::ostream& out = std::cout) const override;

  void saveSecretKey(std::ostream& out, bool seedOnly = false) override {}
  void loadSecretKey(std::istream& out, bool seedOnly = false) override {}

  std::string getLibraryName() const override { return "Circuit"; }

  std::string getSchemeName() const override { return "Circuit"; }

  std::string getSignature() const override;

  bool isConfigRequirementFeasible(
      const HeConfigRequirement& req) const override;

  ///@brief Log operations with no input ctxts (e.g. READ)
  ///@param t Type of operation
  ///@param out Id of ciphertext
  void log(circuit::OpType t, const CircuitCiphertext& out);

  ///@brief Log operations with a single input ctxt (e.g. NEG, CONJUGATE)
  ///@param t Type of operation
  ///@param in Id of in ciphertext
  ///@param out Id of out ciphertext
  void log(circuit::OpType t,
           const circuit::CtxtId& out,
           const circuit::CtxtId& in);

  ///@brief Log operations with a two input ctxts (e.g. ADD, MUL, SUB)
  ///@param t Type of operation
  ///@param out Id of out ciphertext
  ///@param in1 Id of first input ciphertext
  ///@param in2 Id of second input ciphertext
  void log(circuit::OpType t,
           const circuit::CtxtId& out,
           const circuit::CtxtId& in1,
           const circuit::CtxtId& in2);

  ///@brief Log operations with a input ptxts (e.g. ADD, MUL, SUB)
  ///@param t Type of operation
  ///@param out Id of out ciphertext
  ///@param in1 Id of first input ciphertext
  ///@param in2 Id of second input ciphertext
  void log(circuit::OpType t,
           const circuit::CtxtId& out,
           const circuit::CtxtId& in1,
           const CircuitPlaintext& in2);

  ///@brief Log operations with a integer ptxts (e.g. ROTATIONS)
  ///@param t Type of operation
  ///@param out Id of out ciphertext
  ///@param in1 Id of first input ciphertext
  ///@param n Integer
  void log(circuit::OpType t,
           const circuit::CtxtId& out,
           const circuit::CtxtId& in1,
           int n);

  ///@brief Record a ctxt we encounter. The ctxt is added to a database of ctxts
  /// that were not labeled yet. The database include (1) the id of the ctxt;
  /// (2) the raw values of the ctxt and (3) the chain index of the ctxt.
  ///@param id The ctxt id.
  ///@param v The raw values of the ctxt.
  ///@param chainIndex The chainIndex in which the ctxt was encrypted in.
  void setUnlabeledInput(const circuit::CtxtId& id,
                         const circuit::RawValues& v,
                         int chainIndex);
  ///@brief Remove a ctxt from the unlabeled database. This typically happens
  /// when a ctxt is given a label.
  ///@param id The id of the ctxt.
  void unsetUnlabeledInput(const circuit::CtxtId& id);

  ///@brief Record all unlabeled ctxt the circuit encountered. Since ctxt that
  /// were not labeled are not input it must mean they are encrypted parameters
  /// of the circuit. With this function these ctxts can be saved to an Io to be
  /// later encrypted and be used when running the circuit.
  ///@param out An Io to write the unlabeled ctxts to.
  void saveUnlabeledInputs(circuit::AbstractIo* out);

  ///@brief Finalize writing to the output
  void flush();

  bool publicFunctionsSupported(
      const PublicFunctions& publicFunctions) const override;
};
} // namespace helayers

#endif
