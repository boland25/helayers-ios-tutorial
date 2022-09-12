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

#ifndef SRC_HELAYERS_MATH_GLOBALS_H
#define SRC_HELAYERS_MATH_GLOBALS_H

#include "helayers/math/CTileTensor.h"
#include "helayers/math/DoubleTensor.h"

namespace helayers {

///@brief Loads encrypted data (a CTileTensor object) from the given stream.
///
///@param context The HeContext to initialize the CTileTensor with.
///@param stream  The stream to load from.
///@throw runtime_error If the given stream does not contain a CTileTensor
/// object or if loading fails due to an illegal header of the CTileTensor
/// object in the given stream.
std::shared_ptr<CTileTensor> loadEncryptedData(HeContext& context,
                                               std::istream& stream);

///@brief Loads encrypted data (a CTileTensor object) from the given file.
///
///@param context The HeContext to initialize the CTileTensor with.
///@param fileName The name of the file to load from.
///@throw runtime_error If the given file does not contain a CTileTensor object
/// or if loading fails due to an illegal header of the CTileTensor object in
/// the given file.
std::shared_ptr<CTileTensor> loadEncryptedDataFromFile(
    HeContext& context,
    const std::string& fileName);

} // namespace helayers

#endif /* SRC_HELAYERS_MATH_GLOBALS_H */