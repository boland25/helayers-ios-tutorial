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

#ifndef BOOTSTRAP_ENCODINGDECODINGMATRIX_H_
#define BOOTSTRAP_ENCODINGDECODINGMATRIX_H_

#include "helayers/math/FunctionalMatrix.h"
#include "helayers/math/bootstrap/seal/SealBootstrapUtils.h"

namespace helayers {

///@brief A Functional matrix for performing CKKS encoding
class EncodingMatrix : public FunctionalMatrix
{
public:
  ///@brief Construct a new EncodingMatrix object
  ///@param rows Number of rows in matrix
  ///@param cols Number of columns in matrix
  ///@param _bs SealBootstrapUtils object to use
  EncodingMatrix(int rows, int cols, SealBootstrapUtils& _bs)
      : FunctionalMatrix(rows, cols), bs(_bs)
  {
    SC = bs.SC;
  };

private:
  SealBootstrapUtils& bs;
  std::complex<double> getter(int i, int j) override;
};

///@brief A Functional matrix for performing CKKS decoding
/// when all the slots are real ( non complex)
class DecodingRealsMatrix : public FunctionalMatrix
{
public:
  ///@brief Construct a new DecodingRealsMatrix object
  ///@param rows Number of rows in matrix
  ///@param cols Number of columns in matrix
  ///@param _bs SealBootstrapUtils object to use
  DecodingRealsMatrix(int rows, int cols, SealBootstrapUtils& _bs)
      : FunctionalMatrix(rows, cols), bs(_bs)
  {
    SC = bs.SC;
  };

private:
  SealBootstrapUtils& bs;
  std::complex<double> getter(int i, int j) override;
};

///@brief A Functional matrix for performing CKKS decoding
class DecodingMatrix : public FunctionalMatrix
{
public:
  ///@brief Construct a new DecodingMatrix object
  ///@param rows Number of rows in matrix
  ///@param cols Number of columns in matrix
  ///@param _bs SealBootstrapUtils object to use
  DecodingMatrix(int rows, int cols, SealBootstrapUtils& _bs)
      : FunctionalMatrix(rows, cols), bs(_bs)
  {
    SC = bs.SC;
  };

private:
  SealBootstrapUtils& bs;
  std::complex<double> getter(int i, int j) override;
};
} // namespace helayers
#endif /* BOOTSTRAP_ENCODINGDECODINGMATRIX_H_ */
