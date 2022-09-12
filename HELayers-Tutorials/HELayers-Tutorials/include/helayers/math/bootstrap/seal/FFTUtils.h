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

#ifndef BOOTSTRAP_FFTUTILS_H_
#define BOOTSTRAP_FFTUTILS_H_
#include <complex>
#include <vector>
#include "FFTLayer.h"

namespace helayers {

class SealBootstrapUtils;

///@brief A class with utilities for computing the the coef2slot
/// matrix multiplication using FFT.
class FFTUtils
{
  SealBootstrapUtils& bs;

  int SC; // number of slots ("l" in the algorithm from the paper)
  int M;  // The 1st root is the M'th root of unity. M=4*SC.
  const int generator = 5;

  int verbose;

public:
  ///@brief layers for the coef2slot FFT
  std::vector<FFTLayer> layersCoef2Slot;
  ///@brief layers for the slot2coef FFT
  std::vector<FFTLayer> layersSlot2Coef;
  ///@brief layers for the slot2coef mirrored version FFT
  std::vector<FFTLayer> layersSlot2CoefMirror;

public:
  ///@brief Construct a new FFTUtils object
  ///@param bs the SealBootstrapUtils class that will be used for the
  /// computations.
  FFTUtils(SealBootstrapUtils& bs);

  ///@brief Calculate the coefs in PTile form for the coef2slot FFT layers.
  /// For an SC slot configuration there are log(SC) layers
  /// each with 3 PTile coefs.
  ///@param layers the layers to calculate (should refer to coef2slot)
  ///@param factor factor to multiply all the coeffs of the layers
  void calculateLayersCoef2Slot(std::vector<FFTLayer>& layers, double factor);

  ///@brief Calculate the coefs in PTile form for the slot2coef FFT layers.
  /// For an SC slot configuration there are log(SC) layers
  /// each with 3 PTile coefs.
  ///@param layers the layers to calculate (should refer to slot2coef)
  ///@param factor factor to multiply all the coeffs of the layers
  void calculateLayersSlot2Coef(std::vector<FFTLayer>& layers, double factor);

  ///@brief Calculate the coefs in PTile form for the slot2coefMirror FFT
  /// layers.
  /// For an SC slot configuration there are log(SC) layers
  /// each with 3 PTile coefs.
  ///@param layers the layers to calculate (should refer to slot2coefMirror)
  ///@param factor factor to multiply all the coeffs of the layers
  void calculateLayersSlot2CoefMirror(std::vector<FFTLayer>& layers,
                                      double factor);

  ///@brief Join the layers into less layers of identical functionality
  ///@param layers the layers to join on (coef2slot or slot2coef)
  void joinLayers(std::vector<FFTLayer>& layers);

  ///@brief Encodes all the layers
  ///
  ///@param layers the layers to encode (coef2slot or slot2coef)
  ///@param chainIndex encode at this chain index
  ///@param isCoef2Slot true iff encoding coef2slot (rather than slot2coef)
  void encodeLayers(std::vector<FFTLayer>& layers,
                    int chainIndex,
                    bool isCoef2Slot = true);

  ///@brief Create a the rotation dependency maps for the layers' rotations
  ///@param layers layers to create the rotation dependency maps for
  void createRotationDependencyMaps(std::vector<FFTLayer>& layers);

  ///@brief Load the layers from an input file
  ///@param layers the layers to load (coef2slot or slot2coef)
  ///@param fileName input file name
  bool loadEncodedLayersFromFile(std::vector<FFTLayer>& layers,
                                 const std::string& fileName);

  ///@brief Store the layers to an output file
  ///@param layers the layers to store (coef2slot or slot2coef)
  ///@param fileName output file name
  void storeEncodedLayersToFile(std::vector<FFTLayer>& layers,
                                const std::string& fileName);

  ///@brief Process the evaluation of the FFT layers for the given
  /// ciphertext.
  ///@param layers the layers to process (coef2slot or slot2coef)
  ///@param c The ciphertext to pass through the layers
  void processCiphertextByLayers(std::vector<FFTLayer>& layers, CTile& c);

  ///@brief Print the layers values.
  ///@param layers the layers to print (coef2slot or slot2coef)
  void printLayers(std::vector<FFTLayer>& layers) const;
};
} // namespace helayers

#endif /* BOOTSTRAP_FFTUTILS_H_ */
