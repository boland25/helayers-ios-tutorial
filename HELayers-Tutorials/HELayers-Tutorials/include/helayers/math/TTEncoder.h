/*******************************************************************************
 *
 *   OCO Source Materials
 *   5737-A56
 *   © Copyright IBM Corp. 2017
 *
 *   The source code for this program is not published or other-wise divested
 *   of its trade secrets, irrespective of what has been deposited with the
 *   U.S. Copyright Office.
 ******************************************************************************/

#ifndef SRC_HELAYERS_TTENCODER_H
#define SRC_HELAYERS_TTENCODER_H

#include "helayers/hebase/hebase.h"
#include "CTileTensor.h"
#include "PTileTensor.h"

namespace helayers {

///@brief Encoder for tile tensors.
/// Can encode and/or encrypt a tensor into a tile tensor and back again.
class TTEncoder
{

  const HeContext& he;

  Encoder enc;

  LazyMode lazyMode;

public:
  /// Constructs a ready to use object.
  /// @param[in] he the underlying context.
  TTEncoder(const HeContext& he, LazyMode lazyMode = NONE);

  ~TTEncoder();

  /// Copy constructor deleted. Construct new objects when needed.
  TTEncoder(const TTEncoder& src) = delete;

  /// Assignment deleted. Construct new objects when needed.
  TTEncoder& operator=(const TTEncoder& src) = delete;

  /// Returns a reference to the basic encoder used by the TTEncoder
  inline Encoder& getEncoder() { return enc; }

  ///@brief Encodes an input double tensor to a PTileTensor
  ///
  ///@param res Output PTileTensor
  ///@param shape Tile tensor shape
  ///@param vals Input tensor
  ///@param chainIndex Chain index used for encoding (when applicable)
  void encode(PTileTensor& res,
              const TTShape& shape,
              const DoubleTensor& vals,
              int chainIndex = -1) const;

  ///@brief Encodes a PTileTensor filled with a single value
  ///
  ///@param res Output PTileTensor
  ///@param shape Tile tensor shape
  ///@param val The value to encode
  ///@param chainIndex Chain index used for encoding (when applicable)
  void encode(PTileTensor& res,
              const TTShape& shape,
              double val,
              int chainIndex = -1) const;

  ///@brief Returns the decoding result of input PTileTensor into a tensor of
  /// doubles.
  ///
  ///@param src Input PTileTensor
  DoubleTensor decodeDouble(const PTileTensor& src) const;

  ///@brief Encrypts a PTileTensor to a CTileTensor
  ///
  ///@param res Output CTileTensor
  ///@param src Input PTileTensor
  void encrypt(CTileTensor& res, const PTileTensor& src) const;

  ///@brief Decrypts a CTileTensor to a PTileTensor
  ///
  ///@param res Output PTileTensor
  ///@param src Input CTileTensor
  /// caller rsponsebility to call src.wakeup()
  void decrypt(PTileTensor& res, const CTileTensor& src) const;

  ///@brief Encrypts an input double tensor to a CTileTensor
  ///
  ///@param res Output CTileTensor
  ///@param shape Tile tensor shape
  ///@param vals Input tensor
  ///@param chainIndex Chain index used for encoding (when applicable)
  void encodeEncrypt(CTileTensor& res,
                     const TTShape& shape,
                     const DoubleTensor& vals,
                     int chainIndex = -1) const;

  ///@brief Encrypts a CTileTensor filled with a single value
  ///
  ///@param res Output CTileTensor
  ///@param shape Tile tensor shape
  ///@param val The value to encode
  ///@param chainIndex Chain index used for encoding (when applicable)
  void encodeEncrypt(CTileTensor& res,
                     const TTShape& shape,
                     double val,
                     int chainIndex = -1) const;

  ///@brief Returns the result of decryption and decoding of input
  /// CTileTensor
  ///
  ///@param src Input CTileTensor
  DoubleTensor decryptDecodeDouble(const CTileTensor& src) const;

  ///@brief Asserts that the content of given CTileTensor is equal to given
  /// double tensor.
  ///
  ///@param c CTileTensor input to compare
  ///@param title Text to include in error message
  ///@param expectedVals tensor input to compare
  ///@param eps Tolerance
  ///@param percent Relative diff indictoar
  double assertEquals(const CTileTensor& c,
                      const std::string& title,
                      const DoubleTensor& expectedVals,
                      double eps,
                      bool percent = false) const;

  ///@brief Computes the MSE (Mean Square Error) with expected values.
  /// If title!="", prints results to cout.
  /// If eps>=0, asserts MSE<=eps
  ///@param c CTileTensor input to compare
  ///@param expectedVals tensor input to compare
  ///@param title If not empty, will be output to cout along with MSE
  ///@param eps If non-negative, an exception will be thrown if MSE>eps
  double testMse(const CTileTensor& c,
                 const DoubleTensor& expectedVals,
                 const std::string& title = "",
                 double eps = -1) const;

  ///@brief Returns the context
  inline const HeContext& getHeContext() { return he; }
};
} // namespace helayers

#endif /* SRC_HELAYERS_TTENCODER_H */
