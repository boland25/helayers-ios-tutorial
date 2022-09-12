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

#ifndef SRC_HELAYERS_HEBASE_GLOBALS_H
#define SRC_HELAYERS_HEBASE_GLOBALS_H

#include "HeContext.h"
#include "CTile.h"

namespace helayers {

///@brief Macro to be used to catch exceptions and throw or print a nice error
/// message to be concatenated with the message of the original catched
/// exception, if exists
///
#define CATCH_WITH_MESSAGE(message)                                            \
  catch (const std::exception& e)                                              \
  {                                                                            \
    std::throw_with_nested(std::runtime_error(message + std::string(": ") +    \
                                              std::string(e.what())));         \
  }                                                                            \
  catch (...)                                                                  \
  {                                                                            \
    throw std::runtime_error(message + std::string(": unknown exception"));    \
  }

///@brief Macro to be used to throw pretty errors whenever a function is called
/// in WebAssembly version when it shouldn't
///
#define HELAYERS_THROW_UNDEFINED_IN_WASM                                       \
  throw std::runtime_error(                                                    \
      "The method " + std::string(__PRETTY_FUNCTION__) +                       \
      " is currently not implemented in the WebAssembly version")

/// Loads a context object from the given stream.
/// Context type is dynamically determined by content of stream.
///
///@param stream  The stream to load from.
std::shared_ptr<HeContext> loadHeContext(std::istream& stream);

/// Loads a context object from the given file.
/// Context type is dynamically determined by content of file.
///
///@param fileName  The name of the file to load from.
std::shared_ptr<HeContext> loadHeContextFromFile(const std::string& fileName);

///@brief Loads a CTile object from the given stream.
///
///@param context The HeContext to initialize the CTile with.
///@param stream  The stream to load from.
///@throw runtime_error If the given stream does not contain a CTile object or
/// if loading fails due to an illegal header of the CTile object in the given
/// stream.
std::shared_ptr<CTile> loadCTile(HeContext& context, std::istream& stream);

///@brief Loads a CTile object from the given file.
///
///@param context The HeContext to initialize the CTile with.
///@param fileName  The name of the file to load from.
///@throw runtime_error If the given file does not contain a CTile object or
/// if loading fails due to an illegal header of the CTile object in the given
/// file.
std::shared_ptr<CTile> loadCTileFromFile(HeContext& context,
                                         const std::string& fileName);

} // namespace helayers

#endif /* SRC_HELAYERS_HEBASE_GLOBALS_H */