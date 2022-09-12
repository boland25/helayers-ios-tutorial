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

#ifndef SRC_HELAYERS_SAVEABLE_H
#define SRC_HELAYERS_SAVEABLE_H

#include <fstream>
#include <functional>
#include <iostream>
#include <map>
#include <memory>
#include <string>
#include "Printable.h"
#include "SaveableBasic.h"
#include "SaveableHeader.h"

namespace helayers {

class HeContext;
class HeModel;
class CTileTensor;
class CTile;
class ModelIoProcessor;

/// Add this at the top of your class cpp file, after the includes
#define REGISTER_SAVEABLE(saveableClass)                                       \
  static std::shared_ptr<Saveable> saveableClass##loadObj(                     \
      HeContext& context, const SaveableHeader& header, istream& in)           \
  {                                                                            \
    std::shared_ptr<saveableClass> res = make_shared<saveableClass>(context);  \
    res->loadAfterLoadingHeader(in, header);                                   \
    return res;                                                                \
  }                                                                            \
                                                                               \
  static bool saveableClass##_static_entry =                                   \
      Saveable::internalRegisterSaveable(#saveableClass,                       \
                                         &saveableClass##loadObj);             \
                                                                               \
  std::string saveableClass::getClassNameImpl() const { return #saveableClass; }

/// Add this in the very beginning of your class declaration.
/// It adds private methods.
#define REGISTER_SAVEABLE_HEADER                                               \
private:                                                                       \
  std::string getClassNameImpl() const override;

///@brief Parent class to all object that the user can save/load. When
/// saving classes inhereting from Saveable, an additional header will be
/// dumped, which contains meta-data about the saved object. This meta-data will
/// be used upon loading to verify type and version matches upon loading.
class Saveable : public SaveableBasic
{

  /// A type for a function receiving (HeContext&, SaveableHeader&, istream&)
  /// and returning shared_ptr<Saveable>.
  typedef std::function<std::shared_ptr<Saveable>(HeContext&,
                                                  const SaveableHeader& header,
                                                  std::istream&)>
      LoadFunction;

  /// A type for a map mapping a class name to its load function
  typedef std::map<std::string, LoadFunction> SaveableMap;

  /// @brief Returns the contextId of the HeContext this object was
  /// initialized with. This contextId can be used, for example, to
  /// verify heContext match when loading a saved Saveable object.
  virtual int32_t getContextId() const = 0;

  ///@brief Verifies that the class name and context id metadata stored in the
  /// given header matches the metadata of this object.
  ///
  /// @throw runtime_error If the tests fail.
  void verifyHeader(const SaveableHeader& header) const;

  /// @brief Saves the content of this object to the given stream.
  /// @param stream The stream to save to.
  virtual void saveImpl(std::ostream& stream) const = 0;

  /// @brief Loads the content of this object from the given stream.
  /// @param stream The stream to load from.
  virtual void loadImpl(std::istream& stream) = 0;

  ///@brief Returns registered Saveable map.
  /// Used instead of a static member to ensure it's already initialized when
  /// needed.
  static SaveableMap& getRegisteredSaveableMap();

  friend class HeContext;

  /// @brief Internal: Returns the name of this class.
  /// Overriden by the REGISTER_SAVEABLE macros
  virtual std::string getClassNameImpl() const = 0;

protected:
  /// @brief Saves a header containing metadata about this object. This header
  /// will be used to apply sanity checks when loading the saved object.
  /// @param stream The stream to save to.
  void saveHeader(std::ostream& stream) const;

public:
  virtual ~Saveable() {}

  ///@brief Open a file output stream.
  /// Turns on flags to throw exception in case of subsequent write failure.
  ///@param fileName File to open
  ///@throw runtime_error if file can't be opened.
  static std::ofstream openBinaryOfstream(const std::string& fileName);

  ///@brief Open a file input stream.
  /// Turns on flags to throw exception in case of subsequent read failure.
  ///@param fileName File to open
  ///@throw runtime_error if file can't be opened.
  static std::ifstream openBinaryIfstream(const std::string& fileName);

  ///  @brief Saves this Saveable object to a file in binary form.
  ///  Returns the number of bytes written to the saved file.
  ///
  ///  @param[in] fileName name of file to write to
  std::streamoff saveToFile(const std::string& fileName) const;

  ///  @brief Loads this Saveable object from a file saved by saveToFile().
  ///  Returns the number of bytes read from the loaded file.
  ///
  ///  @param[in] fileName name of file to read from
  std::streamoff loadFromFile(const std::string& fileName);

  /// @brief Returns the name of this class.
  inline std::string getClassName() const { return getClassNameImpl(); }

  ///@brief Registers a Saveable object for the purpose of dynamic loading.
  /// Don't call this directly. Use REGISTER_SAVEABLE (see above).
  static bool internalRegisterSaveable(const std::string& className,
                                       LoadFunction loadFunction);

  ///@brief Saves this Saveable object to the given stream.
  ///
  ///@param stream The stream to save to.
  std::streamoff save(std::ostream& stream) const;

  ///@brief Loads this Saveable object from the given stream
  ///
  ///@param stream The stream to load from.
  ///@throw runtime_error If loading fails due to an illegal header of the
  /// Saveable object in the given stream (for example, if the header specifies
  /// an unsupported helayers version).
  std::streamoff load(std::istream& stream);

  ///@brief Loads the Saveable object stored in the given stream. Returns a
  /// shared pointer to the loaded object.
  ///
  ///@param context The HeContext to initialize the Saveable object with.
  ///@param stream  The stream to load from.
  ///@throw runtime_error If loading fails due to an illegal header of the
  /// Saveable object in the given stream (for example, if the header specifies
  /// an unsupported helayers version).
  static std::shared_ptr<Saveable> load(HeContext& context,
                                        std::istream& stream);

  ///@brief Loads the Saveable object stored in the given file. Returns the
  /// pointer to the loaded object.
  ///
  ///@param context  The HeContext to initialize the Saveable object with.
  ///@param fileName The name of file to load from.
  ///@throw runtime_error If loading fails due to an illegal header of the
  /// Saveable object in the given file (for example, if the header specifies
  /// an unsupported helayers version).
  static std::shared_ptr<Saveable> loadFromFile(HeContext& context,
                                                const std::string& fileName);

  ///@brief Loads this Saveable object from the given stream, assuming its
  /// header has already been loaded.
  ///
  ///@param stream The stream to load from.
  ///@param header The header of the object to be loaded.
  ///@throw runtime_error If loading fails due to an illegal header of the
  /// Saveable object in the given stream (for example, if the header specifies
  /// a className field which is different than the class name of this object).
  void loadAfterLoadingHeader(std::istream& stream,
                              const SaveableHeader& header);

  ///@brief Loads an object stored in the given stream, verifying the result is
  /// a class T object
  ///
  ///@tparam T The type the resulting object should have
  ///@param context   The HeContext to intialize the Saveable object with.
  ///@param stream    The stream to load from
  ///@param className The class name of the object to be loaded, used in error
  /// messages.
  ///@throw runtime_error If the class name of the loaded object is
  /// not a T type object.
  ///@throw runtime_error If loading fails due to an illegal header of the
  /// Saveable object in the given stream (for example, if the header
  // specifies
  /// an unsupported helayers version).
  template <class T>
  static std::shared_ptr<T> loadAndVerifyClass(HeContext& context,
                                               std::istream& stream,
                                               const std::string& className)
  {
    std::shared_ptr<Saveable> rawRes = load(context, stream);
    std::shared_ptr<T> res = std::dynamic_pointer_cast<T>(rawRes);
    if (res == nullptr) {
      throw std::runtime_error("Expected input to contain an object of type " +
                               className + ". Instead found " +
                               rawRes->getClassName());
    }
    return res;
  }

  ///@brief See loadAndVerifyClass()
  template <class T>
  static std::shared_ptr<T> loadFromFileAndVerifyClass(
      HeContext& context,
      const std::string& fileName,
      const std::string& className)
  {
    std::shared_ptr<Saveable> rawRes = loadFromFile(context, fileName);
    std::shared_ptr<T> res = std::dynamic_pointer_cast<T>(rawRes);
    if (res == nullptr) {
      throw std::runtime_error("Expected input to contain an object of type " +
                               className + ". Instead found " +
                               rawRes->getClassName());
    }
    return res;
  }
};

} // namespace helayers

#endif
