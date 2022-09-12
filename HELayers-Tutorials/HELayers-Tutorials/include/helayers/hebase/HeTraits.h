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

#ifndef SRC_HELAYERS_HETRAITS_H_
#define SRC_HELAYERS_HETRAITS_H_

namespace helayers {

/// A set of flags characterizing the underyling HE scheme.
class HeTraits
{
  /// Whether the underlying He scheme supports CKKS bootstrapping.
  bool supportsCKKSBootstrap = false;

  /// Whether the underlying He scheme supports bootstrapping.
  bool supportsBootstrapping = false;

  /// Whether the underlying He scheme supports explicit rescaling.
  bool supportsExplicitRescale = false;

  /// Whether the underlying He scheme provides a scale setter function.
  bool supportsSetScale = false;

  /// Whether the underlying He scheme automatically manages chain indices.
  bool automaticallyManagesChainIndices = false;

  /// Whether the underlying He scheme requires to manually make chain indices
  /// of two operands of a binary operation equal before performing it.
  bool internalHebaseChainIndicesAdjustmentsRequired = false;

  /// Whether the underlying He scheme automatically manages rescaling.
  bool automaticallyManagesRescale = false;

  /// Whether the underlying He scheme supports explicit chain indices.
  bool supportsExplicitChainIndices = false;

  /// Whether the underlying He scheme supports complex numbers.
  bool supportsComplexNumbers = false;

  /// Whether the underlying He scheme supports bitwise operations.
  bool supportsBitwiseOperations = false;

  /// Whether the underlying He context supports retrieving maximal allowed
  /// values for each chain index.
  bool supportsValuesLimitation = false;

  /// Whether the underlying He context supports retrieving maximal allowed
  /// values going through a bootstrap operation.
  bool supportsBsValuesLimitation = false;

  /// Whether the underlying He context, while bootstrappable, requires extra
  /// rotation keys other than all the positive and negative powers of two.
  bool bsRequiresExtraRotationKeys = false;

  /// Whether the arithmetic of the underlying He scheme is modular
  bool isModularArithmetic = false;

  /// Whether the underlying He scheme allows the user to provide a scale upon
  /// encoding.
  bool supportsScaledEncoding = false;

  /// In case the arithmetic of the underlying He scheme is modular, this will
  /// contain the arithmetic modulus.
  unsigned long arithmeticModulus = 0;

  /// Whether the underlying He scheme is an empty context (i.e. it can only
  /// create emty ciphertexts and plaintexts). Such contexts can be used to
  /// keepy track of the metadata of a ciphertext without having to do heavy
  /// computations. See also EmptyContext.
  bool isDebugEmpty = false;

  /// Whether the underlying he scheme supports the "decrypt added noise"
  /// technique. The "decrypt added noise" technique is a technique used to
  /// tackle a chosen cipher text attack on the CKKS scheme by adding some
  /// noise to decrypted ciphertexts.
  bool supportsDecryptAddedNoise = false;

  /// True when the underlying He scheme is a mockup scheme,
  /// i.e., a scheme that performs all the operations on plain values.
  bool isMockup = false;

  /// Whether the underlying He scheme can be initialized from an
  /// HeConfigRequirement object.
  bool supportsInitFromConfigRequirement = true;

  /// Whether the underlying He scheme supports secret key generation from seed.
  bool supportsSecKeySeed = false;

  /// Whether the underlying He scheme supports using GPU
  bool supportsGpu = false;

public:
  /// @brief Sets the value of supportsCKKSBootstrapping trait. See also
  /// HeTraits::supportsCKKSBootstrap.
  /// @param val The value to be set.
  inline void setSupportsCKKSBootstrapping(bool val)
  {
    supportsCKKSBootstrap = val;
  }

  /// @brief Sets the value of supportsBootstrapping trait. See also
  /// HeTraits::supportsBootstrapping.
  /// @param val The value to be set.
  inline void setSupportsBootstrapping(bool val)
  {
    supportsBootstrapping = val;
  }

  /// @brief Sets the value of supportsExplicitRescale trait. See also
  /// HeTraits::supportsExplicitRescale.
  /// @param val The value to be set.
  inline void setSupportsExplicitRescale(bool val)
  {
    supportsExplicitRescale = val;
  }

  /// @brief Sets the value of supportsSetScale trait. See also
  /// HeTraits::supportsSetScale.
  /// @param val The value to be set.
  inline void setSupportsSetScale(bool val) { supportsSetScale = val; }

  /// @brief Sets the value of automaticallyManagesChainIndices trait. See also
  /// HeTraits::automaticallyManagesChainIndices.
  /// @param val The value to be set.
  inline void setAutomaticallyManagesChainIndices(bool val)
  {
    automaticallyManagesChainIndices = val;
  }

  /// @brief Sets the value of internalHebaseChainIndicesAdjustmentsRequired
  /// trait. See also HeTraits::internalHebaseChainIndicesAdjustmentsRequired.
  /// @param val The value to be set.
  inline void setInternalHebaseChainIndicesAdjustmentsRequired(bool val)
  {
    internalHebaseChainIndicesAdjustmentsRequired = val;
  }

  /// @brief Sets the value of automaticallyManagesRescale trait. See also
  /// HeTraits::automaticallyManagesRescale.
  /// @param val The value to be set.
  inline void setAutomaticallyManagesRescale(bool val)
  {
    automaticallyManagesRescale = val;
  }

  /// @brief Sets the value of supportsExplicitChainIndices trait. See also
  /// HeTraits::supportsExplicitChainIndices.
  /// @param val The value to be set.
  inline void setSupportsExplicitChainIndices(bool val)
  {
    supportsExplicitChainIndices = val;
  }

  /// @brief Sets the value of supportsComplexNumbers trait. See also
  /// HeTraits::supportsComplexNumbers.
  /// @param val The value to be set.
  inline void setSupportsComplexNumbers(bool val)
  {
    supportsComplexNumbers = val;
  }

  /// @brief Sets the value of supportsBitwiseOperations trait. See also
  /// HeTraits::supportsBitwiseOperations.
  /// @param val The value to be set.
  inline void setSupportsBitwiseOperations(bool val)
  {
    supportsBitwiseOperations = val;
  }

  /// @brief Sets the value of supportsValuesLimitation trait. See also
  /// HeTraits::supportsValuesLimitation.
  /// @param val The value to be set.
  inline void setSupportsValuesLimitation(bool val)
  {
    supportsValuesLimitation = val;
  }

  /// @brief Sets the value of supportsBsValuesLimitation trait. See also
  /// HeTraits::supportsBsValuesLimitation.
  /// @param val The value to be set.
  inline void setSupportsBsValuesLimitation(bool val)
  {
    supportsBsValuesLimitation = val;
  }

  /// @brief Sets the value of bsRequiresExtraRotationKeys trait. See also
  /// HeTraits::bsRequiresExtraRotationKeys.
  /// @param val The value to be set.
  inline void setBsRequiresExtraRotationKeys(bool val)
  {
    bsRequiresExtraRotationKeys = val;
  }

  /// @brief Sets the value of isModularArithmetic trait. See also
  /// HeTraits::isModularArithmetic.
  /// @param val The value to be set.
  inline void setIsModularArithmetic(bool val) { isModularArithmetic = val; }

  /// @brief Sets the value of supportsScaledEncoding trait. See also
  /// HeTraits::supportsScaledEncoding.
  /// @param val The value to be set.
  inline void setSupportsScaledEncoding(bool val)
  {
    supportsScaledEncoding = val;
  }

  /// @brief Sets the value of arithmeticModulus trait. See also
  /// HeTraits::arithmeticModulus.
  /// @param val The value to be set.
  inline void setArithmeticModulus(unsigned long val)
  {
    arithmeticModulus = val;
  }

  /// @brief Sets the value of isDebugEmpty trait. See also
  /// HeTraits::isDebugEmpty.
  /// @param val The value to be set.
  inline void setIsDebugEmpty(bool val) { isDebugEmpty = val; }

  /// @brief Sets the value of supportsDecryptAddedNoise trait. See also
  /// HeTraits::supportsDecryptAddedNoise.
  /// @param val The value to be set.
  inline void setSupportsDecryptAddedNoise(bool val)
  {
    supportsDecryptAddedNoise = val;
  }

  /// @brief Sets the value of isMockup trait. See also
  /// HeTraits::isMockup.
  /// @param val The value to be set.
  inline void setIsMockup(bool val) { isMockup = val; }

  /// @brief Sets the value of supportsInitFromConfigRequirement trait. See also
  /// HeTraits::supportsInitFromConfigRequirement.
  /// @param val The value to be set.
  inline void setSupportsInitFromConfigRequirement(bool val)
  {
    supportsInitFromConfigRequirement = val;
  }

  inline void setSupportsSecKeySeed(bool val) { supportsSecKeySeed = val; }

  /// @brief Sets the value of supportsGpu trait. See also
  /// HeTraits::supportsGpu
  /// @param val The value to be set.
  inline void setSupportsGpu(bool val) { supportsGpu = val; }

  /// @brief Returns the value of supportsBootstrapping trait. See also
  /// HeTraits::supportsBootstrapping.
  inline bool getSupportsBootstrapping() const { return supportsBootstrapping; }

  /// @brief Returns the value of supportsCKKSBootstrap trait. See also
  /// HeTraits::supportsCKKSBootstrap.
  inline bool getSupportsCKKSBootstrapping() const
  {
    return supportsCKKSBootstrap;
  }

  /// @brief Returns the value of supportsExplicitRescale trait. See also
  /// HeTraits::supportsExplicitRescale.
  inline bool getSupportsExplicitRescale() const
  {
    return supportsExplicitRescale;
  }

  /// @brief Returns the value of supportsSetScale trait. See also
  /// HeTraits::supportsSetScale.
  inline bool getSupportsSetScale() const { return supportsSetScale; }

  /// @brief Returns the value of automaticallyManagesChainIndices trait. See
  /// also HeTraits::automaticallyManagesChainIndices.
  inline bool getAutomaticallyManagesChainIndices() const
  {
    return automaticallyManagesChainIndices;
  }

  /// @brief Returns the value of internalHebaseChainIndicesAdjustmentsRequired
  /// trait. See also HeTraits::internalHebaseChainIndicesAdjustmentsRequired.
  inline bool getInternalHebaseChainIndicesAdjustmentsRequired() const
  {
    return internalHebaseChainIndicesAdjustmentsRequired;
  }

  /// @brief Returns the value of automaticallyManagesRescale trait. See also
  /// HeTraits::automaticallyManagesRescale.
  inline bool getAutomaticallyManagesRescale() const
  {
    return automaticallyManagesRescale;
  }

  /// @brief Returns the value of supportsExplicitChainIndices trait. See also
  /// HeTraits::supportsExplicitChainIndices.
  inline bool getSupportsExplicitChainIndices() const
  {
    return supportsExplicitChainIndices;
  }

  /// @brief Returns the value of supportsComplexNumbers trait. See also
  /// HeTraits::supportsComplexNumbers.
  inline bool getSupportsComplexNumbers() const
  {
    return supportsComplexNumbers;
  }

  /// @brief Returns the value of supportsBitwiseOperations trait. See also
  /// HeTraits::supportsBitwiseOperations.
  inline bool getSupportsBitwiseOperations() const
  {
    return supportsBitwiseOperations;
  }

  /// @brief Returns the value of supportsValuesLimitation trait. See also
  /// HeTraits::supportsValuesLimitation.
  inline bool getSupportsValuesLimitation() const
  {
    return supportsValuesLimitation;
  }

  /// @brief Returns the value of supportsBsValuesLimitation trait. See also
  /// HeTraits::supportsBsValuesLimitation.
  inline bool getSupportsBsValuesLimitation() const
  {
    return supportsBsValuesLimitation;
  }

  /// @brief Returns the value of bsRequiresExtraRotationKeys trait. See also
  /// HeTraits::bsRequiresExtraRotationKeys.
  inline bool getBsRequiresExtraRotationKeys() const
  {
    return bsRequiresExtraRotationKeys;
  }

  /// @brief Returns the value of isModularArithmetic trait. See also
  /// HeTraits::isModularArithmetic.
  inline bool getIsModularArithmetic() const { return isModularArithmetic; }

  /// @brief Returns the value of supportsScaledEncoding trait. See also
  /// HeTraits::supportsScaledEncoding.
  inline bool getSupportsScaledEncoding() const
  {
    return supportsScaledEncoding;
  }

  /// @brief Returns the value of arithmeticModulus trait. See also
  /// HeTraits::arithmeticModulus.
  inline unsigned long getArithmeticModulus() const
  {
    return arithmeticModulus;
  }

  /// @brief Returns the value of isDebugEmpty trait. See also
  /// HeTraits::isDebugEmpty.
  inline bool getIsDebugEmpty() const { return isDebugEmpty; }

  /// @brief Returns the value of supportsDecryptAddedNoise trait. See also
  /// HeTraits::supportsDecryptAddedNoise.
  inline bool getSupportsDecryptAddedNoise() const
  {
    return supportsDecryptAddedNoise;
  }

  /// @brief Returns the value of isMockup trait. See also
  /// HeTraits::isMockup.
  inline bool getIsMockup() const { return isMockup; }

  /// @brief Returns the value of supportsInitFromConfigRequirement trait. See
  /// also HeTraits::supportsInitFromConfigRequirement.
  inline bool getSupportsInitFromConfigRequirement() const
  {
    return supportsInitFromConfigRequirement;
  }

  inline bool getSupportsSecKeySeed() const { return supportsSecKeySeed; }

  /// @brief Returns the value of supportsGpu trait. See
  /// also HeTraits::supportsGpu.
  inline bool getSupportsGpu() const { return supportsGpu; }
};
} // namespace helayers

#endif /* SRC_HELAYERS_HETRAITS_H_ */
