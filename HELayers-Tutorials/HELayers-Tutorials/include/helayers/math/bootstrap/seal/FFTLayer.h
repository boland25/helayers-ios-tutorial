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

#ifndef BOOTSTRAP_FFTLAYER_H_
#define BOOTSTRAP_FFTLAYER_H_
#include <complex>
#include <map>
#include <vector>
namespace helayers {

class CTile;
class PTile;

class SealBootstrapUtils;

///@brief A class with utilities for holding the coefs of FFT transorm layers
class FFTLayer
{
  SealBootstrapUtils& bs;

  int SC; // number of slots ("l" in the algorithm from the paper)
  std::map<int, std::vector<std::complex<double>>> rotCoefMap;
  std::map<int, PTile> rotPCoefMap;

  std::map<int, int> dependentRotation;
  std::map<int, int> dependentRotationDelta;
  std::map<int, int> dependentRotationDepth;

  int verbose;

public:
  ///@brief Construct a new FFTLayer object
  ///@param bs the SealBootstrapUtils class that will be used for the
  ///computations.
  FFTLayer(SealBootstrapUtils& bs);

  ///@brief Returns the map of "raw" coefs in this layer
  std::map<int, std::vector<std::complex<double>>>& getCoefMap()
  {
    return rotCoefMap;
  };
  ///@brief Returns the map of plaintext coefs in this layer
  std::map<int, PTile>& getPCoefMap() { return rotPCoefMap; };

  ///@brief Get the coefs for the given rotation
  ///@param rot the rotation for the coefs
  std::vector<std::complex<double>>& getCoefsForRot(int rot);

  ///@brief Encode the layer into a set of PTiles for the rotations
  ///@param chainIndex encode at this chain index
  ///@param scale encode at this scale
  void encodeLayer(int chainIndex, double scale);

  ///@brief Merge this layer with another layer to produce the target layer
  ///@param other Other layer to merge this layer with
  ///@param target The resulting merged layer
  void mergeWith(const FFTLayer& other, FFTLayer& target);

  ///@brief Process the evaluation of the FFT layer for the given
  /// ciphertext.
  ///@param c The ciphertext to pass through the layer
  void processCiphertextByLayer(CTile& c);

  ///@brief Load the layer PTile map from an open input file
  ///@param in Open input file
  void loadEncodedLayerFromFile(std::ifstream& in);

  ///@brief Store the layer PTile map to an open output file
  ///@param out open output file
  void storeEncodedLayerToFile(std::ofstream& out);

  ///@brief Create a the rotation dependency map for the layer's rotations
  void createRotationDependencyMap();

  ///@brief Compute and return all the needed rotations of the given ciphertext
  ///@param c The ciphertext to rotate
  ///@param rots The resulting rotations
  void getAllRotationsOf(const CTile& c, std::vector<std::shared_ptr<CTile>>& rots);

  ///@brief Code for a thread job for computing rotations until all are done
  ///
  ///@param c The ciphertext to rotate
  ///@param rots The resulting rotations
  ///@param rotJobs Queue with outstanding rotation tasks
  ///@param jobsLeft Number of outstanding rotation tasks
  ///@param queueLock Semaphore for locking the shared queue during access
  ///@param rotsLock Semaphore for accessing shared objects
  void rotJob(const CTile& c,
              std::vector<std::shared_ptr<CTile>>& rots,
              std::queue<int>& rotJobs,
              int& jobsLeft,
              omp_lock_t& queueLock,
              omp_lock_t& rotsLock);

  ///@brief Print the dependency DAG of the rotations
  void printDependencyDAG();

  ///@brief Print layer coefs
  ///@param title print title
  ///

  ///@brief Print layer coefs
  ///@param title Print title
  ///@param verbose Verbosity level (default = 0 implies minimal reporting)
  ///@param out output stream
  void debugPrint(const std::string& title,
                  int verbose = 0,
                  std::ostream& out = std::cout) const;
};
} // namespace helayers

#endif /* BOOTSTRAP_FFTLAYER_H_ */
