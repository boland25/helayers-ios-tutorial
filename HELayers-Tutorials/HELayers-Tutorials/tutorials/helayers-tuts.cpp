//
//  helayers-tuts.cpp
//  HELayers-Tutorials
//
//  Created by boland on 9/10/22.
//

#include "HELayers-Tutorials-Bridging-Header.h"

#include <string>
#include <iostream>
#include <random>
#include <algorithm>

#include "helayers/hebase/hebase.h"
#include "helayers/hebase/seal/SealCkksContext.h"

using namespace helayers;
using namespace std;

void euclideanDistance(void)
{
  try {
    cout << "Initializing context . . ." << endl;

    HeConfigRequirement req(8192, 2, 40, 20);

    SealCkksContext he;
    he.init(req);

    Encoder encoder(he);

    uniform_real_distribution<double> unif(-1, 1);
    default_random_engine re;

    int vecSize = 128;

    vector<double> v1(vecSize), v2(vecSize);
    for (size_t i = 0; i < vecSize; i++) {
      v1[i] = unif(re);
      v2[i] = unif(re);
      cout << "Your v1 is " << v1[i] << endl;
      cout << "Your v2 is " << v2[i] << endl;
    }

    vector<double> v(v1);
    for (size_t i = 0; i < vecSize; i++)
      v[i] -= v2[i];
    transform(v.begin(), v.end(), v.begin(), [](double n) { return n * n; });
    double plainDistance = sqrt(accumulate(v.begin(), v.end(), 0.0));
    cout << "Euclidean distance is " << plainDistance << endl;

    cout << "And now homomorphically . . . " << endl;
    CTile c1(he), c2(he);
    encoder.encodeEncrypt(c1, v1);
    encoder.encodeEncrypt(c2, v2);

    c1.sub(c2);
    c1.square();
    c1.innerSum(1, vecSize);

    double heDistance = sqrt(encoder.decryptDecodeDouble(c1)[0]);

    cout << "HE Euclidean distance " << heDistance << endl;

    if (fabs(heDistance - plainDistance) > 0.01) {
      throw runtime_error("Result is far from expected");
    }
  } catch (const std::exception& e) {
    std::cout << e.what() << endl;
    throw e;
  }
}
