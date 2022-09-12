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

#ifndef BOOTSTRAP_SEALBOOTSTRAPUTILS_H_
#define BOOTSTRAP_SEALBOOTSTRAPUTILS_H_

#include "helayers/hebase/OmpWrapper.h"

#include <complex>
#include <queue>
#include <utility>
#include <vector>
#include "helayers/hebase/hebase.h"
#include "helayers/hebase/HeContext.h"

namespace helayers {

class BootstrapConfig;
class Chebyshev;
class CTile;
class Encoder;
class FFTUtils;
class FunctionalMatrix;
class HeContext;
class SQLUtils;
class PatersonStockmeyer;
class PTile;

///@brief A class with various utility methods for performing CKKS bootstrapping
class SealBootstrapUtils
{
  HeContext& he;
  std::shared_ptr<PatersonStockmeyer> sp;
  std::shared_ptr<Chebyshev> ch;

  int M;
  int N;

  std::vector<std::complex<double>> rootsOfUnity;
  // coef2slot matrices
  std::shared_ptr<std::vector<PTile>> encU11;
  // slot2coef matrices
  std::shared_ptr<std::vector<PTile>> encU1;
  std::shared_ptr<std::vector<PTile>> encU2;
  std::shared_ptr<std::vector<PTile>> decReals;
  //
public:
  /// The configuration of the bootstrap process
  BootstrapConfig bc;
  /// expected depth of computation needed to perform the bootstrap
  int cost;
  /// Number of slots
  int SC;
  ///@brief Coeffs of cos approximation polynomial
  std::vector<long double> cos_coefs;
  ///@brief Coeffs of sin approximation polynomial
  std::vector<long double> sin_coefs;
  ///@brief The rotation upon which this rotation depends
  std::vector<int> dependentRotation;
  ///@brief How much to rotate the source rotation to get this target rotation
  std::vector<int> dependentRotationDelta;
  ///@brief The depth in the BFS order of the rotation
  std::vector<int> dependentRotationDepth;
  ///@brief Rotations needed at the corresponding depth of dependency in the BFS
  /// rotation computation order
  std::vector<std::vector<int>> rotsOfDepth;

private:
  void polyEvalDFS(CTile& res,
                   const std::vector<double>& coefs,
                   CTile& xVal,
                   int currExp,
                   CTile& currPower,
                   bool& zeroRes) const;

  void recursiveExp(CTile& res,
                    CTile& xVal,
                    int exp,
                    std::vector<std::shared_ptr<CTile>>& powersStore) const;

public:
  ///@brief Construct a new Bootstrap Utils object
  ///@param h the HE context
  ///@param bc the bootstrap configuration
  ///@param firstMod the first prime in the prime-chain.
  /// 0 => automatically deduce the first prime form the HE context
  SealBootstrapUtils(HeContext& h,
                     const BootstrapConfig& _bc,
                     uint64_t firstMod = 0);
  ///@brief Construct a new SealBootstrapUtils object using a given HE context
  ///@param h the HE context
  SealBootstrapUtils(HeContext& h);

  ///@brief The FFTUtils object for performing the FFT steps of the bootstrap
  std::shared_ptr<FFTUtils> fft;

  ///@brief Returns the number of products expected in the bootstrap process
  ///@param bc the bootstrap configuration
  ///@param SC the number of slots
  static int getExpectedBSCost(const BootstrapConfig& bc, int SC);

  ///@brief Returns the number of products expected in the bootstrap process
  int getExpectedBSCost() { return cost; }

  ///@brief Get the default expected number of products expected in the
  /// bootstrap process assuming a default BootstrapConfig, and ignoring the
  /// number of slots
  static int getDefaultExpectedBSCost();

  ///@brief Returns the R parameter of the bootstrap process,
  /// that is, the part of the factor that shrinks down the value in the slots
  /// as a preparation to the Sin step of the bootstrap
  ///@param SC the number of slots in a ciphertext
  static int computeR(int SC);
  ///@brief R is part of the factor that shrinks down the value in the slots
  /// as a preparation to the Sin step of the bootstrap
  int R;

  ///@brief r1 will be init to 2/qR and used as a factor for the coef2slot
  /// matrix, except for some tests when r1 should have no effect.
  double r1 = -1;
  ///@brief The factor to multiply the values by before the encode/coef2slot
  /// step
  double encodingFactor = -1;
  ///@brief The scale used for the encoding carried out during coef2slot.
  ///       This also equals the ciphertext scale at the entry to coef2slot.
  double coef2slotEncodingScale = -1;
  void setCoef2slotEncodingScale(double _coef2slotEncodingScale)
  {
    coef2slotEncodingScale = _coef2slotEncodingScale;
  }
  double getCoef2slotEncodingScale() { return coef2slotEncodingScale; }

  ///@brief r2 will be init to q/2Pi and used as a factor for the slot2coed
  /// matrix, except for some tests when r2 should have no effect.
  double r2 = -1;
  ///@brief The factor to multiply the values by before the decode/slot2coef
  /// step
  double decodingFactor = -1;

  ///@brief The final chain index to target the bootstrap process at
  int targetChainIndex;

  ///@brief The default target chain index
  static int defaultTargetChainIndex;

  ///@brief The chain index at the encoding/coef2slot step of the bootstrap
  int encodingChainIndex;
  ///@brief The chain index at the decoding/slot2coef step of the bootstrap
  int decodingChainIndex;

  // pre-computed PTiles and values

  ///@brief 1/N (N being 2*numSlots)
  std::complex<double> oneDivN;
  ///@brief Vector with all entries = -i/2
  std::shared_ptr<std::vector<std::complex<double>>> zMinusIDiv2;

  ///@brief Returns the HeContext object
  HeContext& getHe() { return he; };
  ///@brief Print signature of Bootstrap process configuration to output stream
  ///@param out the output stream
  void printSignature(std::ostream& out) const;
  ///@brief Evaluate a polynomial over a ciphertext (DFS method)
  ///@param coeffs coeffs of polynomial
  ///@param c encrypted value to set the polynomial variable to
  void polyEvalInPlace(const std::vector<double>& coefs, CTile& c) const;
  ///@brief Evaluate a polynomial over a ciphertext (with powers cache)
  ///@param coeffs coeffs of polynomial
  ///@param c encrypted value to set the polynomial variable to
  void polyEvalInPlace2(const std::vector<double>& coefs, CTile& c) const;
  ///@brief Returns evaluation of a polynomial over a ciphertext
  ///@param coeffs coeffs of polynomial
  ///@param c encrypted value to set the polynomial variable to
  CTile polyEval(const std::vector<double>& coefs, const CTile& c) const;
  ///@brief Returns evaluation of a polynomial over a ciphertext term by term
  ///@param coeffs coeffs of polynomial
  ///@param c encrypted value to set the polynomial variable to
  CTile polyEvalCoefFirst(const std::vector<double>& coefs,
                          const CTile& c) const;
  ///@brief Returns evaluation of a polynomial over a ciphertext using Horner's
  /// method
  ///@param coeffs coeffs of polynomial
  ///@param c encrypted value to set the polynomial variable to
  CTile polyEvalHorner(std::vector<double>& coeffs, const CTile& c) const;
  ///@brief Returns evaluation of a polynomial over plain value using Horner's
  /// method
  ///@param coeffs coeffs of polynomial
  ///@param v value to set the polynomial variable to
  double polyEvalPlainHorner(std::vector<double> coeffs, double v) const;
  ///@brief Raise ciphertext to given power
  ///@param c ciphertext to raise
  ///@param d the power to raise by
  void powInPlace(CTile& c, int d) const;

  ///@brief Search various mod configurations. Used by corresponding test.
  ///@param q the mod
  ///@param Z the max value to mod
  ///@param M expected number of q-s that will drop
  ///@param R factor
  ///@param numZsteps number of Zs to test
  ///@param outfilename file to save the results in
  double searchMods(double q,
                    double Z,
                    int M,
                    int R,
                    int numZsteps,
                    const std::string& outfilename);

  ///@brief Decrypt and decode a mockup ciphertext into a vector of complex vals
  ///@param c the ciphertext to decryp and decode
  std::vector<std::complex<double>> decryptDecodeComplexMockup(CTile& c) const;

  ///@brief Compute mod (c%q) over slots using Sin approximation
  ///@param c the ciphertext with the slots to compute the mod on
  ///@param q the mod
  ///@param verbose
  ///@param _R factor. -1 => use R field of this SealBootstrapUtils object.
  void modInPlace(CTile& c, double q, bool verbose, int _R = -1) const;
  ///@brief Compute mod (c%q) over slots as part of the bootstrap process
  ///@param c the ciphertext with the slots to compute the mod on
  ///@param q the mod
  ///@param verbose
  void modInPlaceForBootstrap(CTile& c, double q, bool verbose) const;
  ///@brief Compute mod (c%q) over slots using a polynomial estimation
  ///@param c the ciphertext with the slots to compute the mod on
  ///@param q the mod (1st prime in the chain)
  void modInPlaceViaPolyEstimate(CTile& c, double q) const;
  ///@brief Given noisy cos/bin of input angle,
  ///       returns cos/sin of the andle multiplies by n
  ///       (not used. consider removing)
  ///@param cosA cos of input angle (noisy)
  ///@param sinA sin of input angle (noisy)
  ///@param n required multiple of input angle
  void multiplyAngleInPlace(CTile& cosA, CTile& sinA, int n) const;

  ///@brief Returns the raising of a ciphertext by a given power
  ///@param c the ciphertext to raise
  ///@param d the power
  CTile pow(const CTile& c, int d) const;
  ///@brief Create the dependency graph between rotations where
  ///       the dependency is a rotation by a power of two
  void createRotationDependencyMap();
  ///@brief Get all the required rotations the given ciphertext
  ///@param c the ciphertext to rotate
  ///@param rots the resulting rotations
  void getAllRotationsOf(const CTile& c, std::vector<CTile>& rots) const;

  ///@brief Rotation job
  ///@param c the ciphertext to rotate
  ///@param rots the resulting rotations
  ///@param rotJobs the queue of remaining rotation jobs
  ///@param jobsLeft number of jobs left
  ///@param queueLock semaphore for accesses to shared storage
  void rotJob(const CTile& c,
              std::vector<CTile>& rots,
              std::queue<int>& rotJobs,
              int& jobsLeft,
              omp_lock_t& queueLock) const;

  /// Pre-calculate all the roots of unity expected to be used
  void setRootsOfUnity();
  ///@brief Returns the k'th M'th root of unity
  ///@param k
  std::complex<double> getRootOfUnity(int k) { return rootsOfUnity[k % M]; }
  ///@brief Returns encoded plaintexts From a functional matrix object
  ///@param fm the functional-matrix to extract the plaintexts from
  ///@param chainIndex the chain-index ot encode in
  ///@param factor a factor to multiply all the values by
  ///@param scale the encoding scale
  std::shared_ptr<std::vector<PTile>> getEncodedVectorsFromFunctionalMatrix(
      FunctionalMatrix& fm,
      int chainIndex = -1,
      double factor = 1.0,
      double scale = -1);
  ///@brief Initialize the encoding matrices
  void setEncodingMatrices();
  ///@brief Initialize the encoding matrices when all slots are non-complex
  void setEncodingRealsMatrices();

  ///@brief Returns power under a given modulu (a^b)%m in O(log b)
  ///@param a base
  ///@param b power
  ///@param m modulus
  int powMod(int a, int b, int m);
  ///@brief Returns the power of the corresponding root of unity
  ///       (j*(5^i)%M)%M
  ///@param i row
  ///@param j column
  std::complex<double> zeta(int i, int j);
  ///@brief Returns an encoding without scaling (scale=2^0=1)
  ///@param val the values to encode
  ///@param chainIndex the chain index to encode in
  PTile getScale1Encoding(std::complex<double> val, int chainIndex = -1) const;
  /// Returns PTile for an integer with specific chain-index and scale.
  PTile encode(int val, int chainIndex = -1, double defaultScale = 1.0) const;
  /// Returns PTile for a double with specific chain-index and scale.
  PTile encode(double val,
               int chainIndex = -1,
               double defaultScale = 1.0) const;

  ///@brief Get the required ~double scale of a multiplied PTile object such
  /// that the desired scale (on the normal ladder) would result after the
  /// product followed by 2 rescales
  ///@param inScale input scale of multiplied ciphertext
  ///@param inLevel input level of multiplied ciphertext
  ///(may deviate from the normal scales ladder)
  ///@param outLevel the targeted output level after the product and 2 rescales
  double getRequiredScaleOfMultipliedPTile(double inScale,
                                           int inLevel,
                                           double outLevel) const;

  ///@brief Returns product of the ciphertext with a given diagonalized matrix
  ///@param c the cipher to multiply
  ///@param m the matrix to multiply by
  CTile getMulCipherWithDiagonalizedMatrix(const CTile& c,
                                           const std::vector<PTile>& m) const;
  ///@brief Returns product of the ciphertext with a given diagonalized encoding
  /// matrix
  ///@param c the cipher to multiply
  ///@param m the matrix to multiply by
  std::pair<CTile, CTile> getMulCipherWithDiagonalizedEncodingMatrix(
      const CTile& c,
      const std::vector<PTile>& m11) const;
  ///@brief Returns product of the ciphertext with a given diagonalized decoding
  /// matrix
  ///@param c the cipher to multiply
  ///@param m the matrix to multiply by
  CTile getMulCipherWithDiagonalizedDecodingMatrix(
      const CTile& c,
      const std::vector<PTile>& m) const;

  ///@brief Compute the coef2slot step over the given ciphertext using FFT.
  /// Returns the result in two ciphertext for the two halves of the vector
  ///@param c the transformed ciphertext
  std::pair<CTile, CTile> coef2slotFFT(const CTile& c) const;
  ///@brief Perform the coef2slot step over the given ciphertext
  /// Returns the result in two ciphertext for the two halves of the vector
  ///@param c the transformed ciphertext
  std::pair<CTile, CTile> coef2slot(const CTile& c) const;
  ///@brief Returns the result of slot2coef step over the given ciphertext
  ///@param c the 1st half of the vector
  ///       the 2nd half is deduced because we assume non complex slots here
  CTile slot2coefFFT(const CTile& c) const;
  ///@brief Returns the result of the slot2coef step over the given ciphertexts
  /// using FFT
  ///@param c1 the 1st half of the vector
  ///@param c2 the 2nd part of the vector - only used when supporting complex
  /// slots
  CTile slot2coef(CTile& c1, CTile& c2) const;
};
} // namespace helayers

#endif /* BOOTSTRAP_SEALBOOTSTRAPUTILS_H_ */
