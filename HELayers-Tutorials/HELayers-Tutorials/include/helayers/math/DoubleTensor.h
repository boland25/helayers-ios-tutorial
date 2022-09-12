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

#ifndef MATH_DOUBLE_TENSOR_H_
#define MATH_DOUBLE_TENSOR_H_

#include <vector>
#include "TensorIterator.h"
#include "MathUtils.h"
#include "Padding2d.h"

#define BOOST_UBLAS_INLINE
#include <boost/numeric/ublas/tensor.hpp>

namespace helayers {

/// A thin wrapper over a boost::numeric::ublas::tensor<double> object.
///
/// Uses the first-order convention.
class DoubleTensor
{
public:
  /// Underlying type used internally to store the tensor
  typedef boost::numeric::ublas::tensor<double> TensorImpl;

private:
  TensorImpl tensor;

  /// Reshapes tensor given extents
  void reshape(
      const boost::numeric::ublas::tensor<double>::extents_type& extents);

  /// Calculate convolution into res for a given output pixel, sample num and
  /// filter.
  double calcConvolutionForPixel(const DoubleTensor& filters,
                                 DimInt x,
                                 DimInt y,
                                 DimInt b,
                                 DimInt f,
                                 int leftPadding = -1,
                                 int upPadding = -1) const;

  /// Calculate sum-pooling for a given output pixel
  double calcPoolingForPixel(bool average,
                             DimInt filterX,
                             DimInt filterY,
                             DimInt x,
                             DimInt y,
                             DimInt c,
                             DimInt b) const;

  /// Calculate sum-pooling or average-pooling for this tensor object
  void calcPooling(DoubleTensor& res,
                   bool average,
                   DimInt filterX,
                   DimInt filterY,
                   DimInt strideX = 1,
                   DimInt strideY = 1,
                   const Padding2d& padding = Padding2d()) const;

  ///@brief Reshapes tensor by iterating over dst, assuming src is a target of
  /// dst.
  ///
  ///@param dst Iterator for new shape
  ///@param src Iterator for existing shape
  void reshapeByIterators(TensorIterator& dst, TensorIterator& src);

  /// Computes matrix multiply for 3-d matrices. Called by getMatrixMultiply
  void getMatrixMultiplyBatch(const DoubleTensor& other,
                              DoubleTensor& res) const;

  ///@brief Reduces a given dimension using a given function. After this
  /// operation the dimension size will be 1, and it will contain
  /// reduceFunc(vals) computed over the values in this dimension.
  ///
  ///@param dim the dimension to reduce
  void reduceDim(DimInt dim,
                 double (*reduceFunc)(const std::vector<double>& vals));

public:
  ///@brief Construct an empty tensor
  ///
  DoubleTensor();

  ///@brief Construct copy of given tensor
  ///
  ///@param src tensor to copy
  DoubleTensor(const TensorImpl& src);

  ///@brief Construct a zeroed tensor with given shape
  ///
  ///@param shape shape
  DoubleTensor(std::initializer_list<DimInt> shape);

  ///@brief Construct a zeroed tensor with given shape
  ///
  ///@param shape shape
  DoubleTensor(const std::vector<DimInt>& shape);

  ///@brief Initialized tensor with given shape and values.
  ///
  ///@param shape New shape to set
  ///@param vals Vals to fill tensor with.
  ///@param firstOrder Indicates whether supplied values are in first-order.
  /// Note that internally they'll be stored according to internal convention.
  void init(const std::vector<DimInt>& shape,
            const std::vector<double>& vals,
            bool firstOrder = true);

  ///@brief Initialized tensor with given shape and value.
  ///
  ///@param shape New shape to set
  ///@param vals A single value to fill tensor with in all slots.
  void init(const std::vector<DimInt>& shape, double val);

  ///@brief Reshapes tensor
  ///
  ///@param shape shape
  void reshape(const std::vector<DimInt>& shape);

  ///@brief Reshapes tensor
  ///
  ///@param shape shape
  void reshape(std::initializer_list<DimInt> shape);

  ///@brief Reorders the dimensions of this tensor.
  /// For example, given {1,2,0}, the current second dimension will become
  /// first,
  /// the current third dimension will become second, and the current first
  /// dimension will become last.
  ///@param dimOrder New order of dimensions
  void reorderDims(const std::vector<DimInt>& dimOrder);

  ///@brief Populates existing tensor with values. The tensor can have any
  /// number of dimensions, and its shape is maintained. Caller can specify
  /// whether input vector is in the first-order convention or not.
  /// Internally it is stored according to the internal convention.
  ///
  ///@param vals a vector with the values to populate. size of vector must equal
  /// the size of tensor
  ///@param firstOrder whether vals is given in the first order convention or
  /// last order.
  void init(const std::vector<double>& vals, bool firstOrder = true);

  ///@brief Populates existing 2D tensor with values from a matrix
  ///
  ///@param matrix a matrix with the values to populate. size of matrix must
  /// equal the size of the 2D tensor
  void init(const std::vector<std::vector<double>>& matrix);

  ///@brief Initialize object with a tensor object of the underlying type
  ///
  ///@param tensor Tensor object to copy from
  inline void init(const TensorImpl tensor) { this->tensor = tensor; }

  ///@brief Populates existing tensor with random values in [-0.5, 0.5]
  ///
  void initRandom();

  ///@brief Populates existing tensor with random values in the range [min, max)
  ///
  ///@param min A lower limit for the random values.
  ///@param max An upper limit for the random values.
  ///@param sparseRate The rate of zeroed values. Must be in the range of [0,1].
  /// For example, 0 means no values will be set to zero, while 0.3 means ~30%
  /// of the values will be set to zero and ~70% will get values in the range of
  /// [min,max).
  void initRandom(double min, double max, double sparseRate = 0);

  ///@brief Adds a new dimension to the tensor. The result is a tensor with
  /// number of dimensions increased by one. The added dimension is being padded
  /// with zeros.
  ///
  ///@param dim the dimension to add. Must be at the start or between or end of
  /// the existing dimensions. Any existing dimensions with index equal or
  /// greater will have their index be advanced in 1.
  ///@param size the size of the new dimension.
  void addDim(DimInt dim, DimInt size = 1);

  ///@brief Resizes an existing dimension. If the size is being increased, the
  /// resized dimension is being padded with zeros.
  ///
  ///@param dim the dimension to resize.
  ///@param newSize the new desired size
  void resizeDim(DimInt dim, DimInt newSize);

  ///@brief Duplicates the values of a specified existing dimension. The values
  /// will be taken from the slot at index 0.
  ///
  ///@param dim the dimension to duplicate over
  void duplicateOverDim(DimInt dim);

  ///@brief Computes arg-min over a given dimension. After this operation the
  /// dimension size will be 1, and it will contain the index of the previous
  /// minimal value.
  ///
  ///@param dim the dimension to run arg-min over
  void argMinOverDim(DimInt dim);

  ///@brief Save this object to a binary stream.
  ///
  ///@param stream stream to write to
  std::streamoff save(std::ostream& stream) const;

  ///@brief Load this object from a binary stream.
  ///
  ///@param stream stream to load from
  std::streamoff load(std::istream& stream);

  ///@brief Prints state for debugging.
  ///
  ///@param title Title to print alongside state
  ///@param verbose Verbosity of printout
  ///@param out output stream to print to
  void debugPrint(const std::string& title,
                  int verbose = 0,
                  std::ostream& out = std::cout) const;

  ///@brief Perform convolution of this object with filters.
  /// Expects this object to be a 4-dimensional tensor: [X,Y,CHANNELS,BATCH]
  /// The filters should be [FILTERX,FILTERY,CHANNELS,FILTERS]
  /// The biases can be any tensor with FILTERS number of elements.
  /// The result will be [OUTX,OUTY,FILTERS,BATCH]
  ///
  /// @param res Resulting tensor
  /// @param filters Filters
  /// @param biases Biases
  /// @param strideX Stride on the X axis (first dimension)
  /// @param strideY Stride on the Y axis (second dimension)
  /// @param padding The padding values (default, all zeros)
  void calcConvolution(DoubleTensor& res,
                       const DoubleTensor& filters,
                       const DoubleTensor& biases,
                       DimInt strideX = 1,
                       DimInt strideY = 1,
                       const Padding2d& padding = Padding2d()) const;

  ///@brief Perform sum-pooling of this object.
  /// Expects this object to be a 4-dimensional tensor: [X,Y,CHANNELS,BATCH]
  /// The sum-pooling is done over a (filterRows X filterCols)  filter.
  /// The result will be [OUTX,OUTY,FILTERS,BATCH]
  ///
  /// @param res Resulting tensor
  /// @param filterX Width of filter on the X axis (first dimension) to sum over
  /// @param filterY Width of filter on the Y axis (second dimension) to sum
  /// over
  /// @param strideX Stride on the X axis (first dimension)
  /// @param strideY Stride on the Y axis (second dimension)
  /// @param padding The padding values (default, all zeros)
  void calcSumPooling(DoubleTensor& res,
                      DimInt filterX,
                      DimInt filterY,
                      DimInt strideX = 1,
                      DimInt strideY = 1,
                      const Padding2d& padding = Padding2d()) const;

  ///@brief Perform average-pooling of this object.
  /// Expects this object to be a 4-dimensional tensor: [X,Y,CHANNELS,BATCH]
  /// The average-pooling is done over a (filterRows X filterCols)  filter.
  /// The result will be [OUTX,OUTY,FILTERS,BATCH]
  ///
  /// @param res Resulting tensor
  /// @param filterX Width of filter on the X axis (first dimension) to average
  /// over
  /// @param filterY Width of filter on the Y axis (second dimension) to average
  /// over
  /// @param strideX Stride on the X axis (first dimension)
  /// @param strideY Stride on the Y axis (second dimension)
  /// @param padding The padding values (default, all zeros)
  void calcAveragePooling(DoubleTensor& res,
                          DimInt filterX,
                          DimInt filterY,
                          DimInt strideX = 1,
                          DimInt strideY = 1,
                          const Padding2d& padding = Padding2d()) const;

  ///@brief Perform batch normalization on this object. The object can be at any
  /// shape. This operation does not change the shape of the object.
  ///
  ///@param dim The features dimension. Must exist in this object. Its size must
  /// equal the size of the vectors of means, vars, scales and biases.
  ///@param means A vector with the mean of each feature along the features
  /// dimension.
  ///@param vars A vector with the variance of each feature along the features
  /// dimension.
  ///@param scales A vector with the scale of each feature along the features
  /// dimension.
  ///@param biases A vector with the bias of each feature along the features
  /// dimension.
  ///@param eps The epsilon value to use for the normalization, to avoid
  /// deviding by zero.
  void batchNorm(DimInt dim,
                 const std::vector<double>& means,
                 const std::vector<double>& vars,
                 const std::vector<double>& scales,
                 const std::vector<double>& biases,
                 double eps);

  ///@brief Flattens a 4D tensor that is an output of a convolution operation
  /// into 3D tensor with sizes [dim0 x dim1 x dim2, 1, dim3], where dimX is the
  /// size of dimension X of the original tensor.
  ///
  ///@param channelsLast Indication to whether the flatten should be performed
  /// in the channels-last or channels-first convention.
  void flattenConvolutionOutput(bool channelsLast);

  ///@brief Returns number of dimensions
  ///
  inline DimInt order() const { return tensor.order(); }

  ///@brief Returns number of elements in tensor
  ///
  inline DimInt size() const { return tensor.size(); }

  ///@brief Returns size for specific dimension.
  ///
  ///@param dim Dimension to query
  inline DimInt getDimSize(DimInt dim) const { return tensor.extents()[dim]; }

  ///@brief Returns shape. Note: allocates a new vector. Don't use in
  /// efficiency-critical code.
  ///
  std::vector<DimInt> getShape() const;

  ///@brief Returns underlying tensor object
  ///
  inline TensorImpl& getTensor() { return tensor; }

  ///@brief Returns underlying tensor object
  ///
  inline const TensorImpl& getTensor() const { return tensor; }

  ///@brief Returns the string representation of shape
  ///
  ///@param shape Shape
  static std::string shapeToString(std::initializer_list<DimInt> shape);

  ///@brief Returns the string representation of shape
  ///
  ///@param shape Shape
  static std::string shapeToString(const std::vector<DimInt>& shape);

  ///@brief Returns the string representation of this object's shape
  std::string getShapeAsString() const;

  ///@brief Asserts given shape equals tensor's shape
  ///
  ///@param shape Shape. If a zero value is passed for a certain dimension, it
  /// means this dimension should be skipped when verifying its size
  /// @throw runtime_error If assertion fails
  void assertShapeEquals(std::initializer_list<DimInt> shape) const;

  ///@brief Returns a slice of the tensor in specific dimension. Result has the
  /// same number of dimensions as the original tensor.
  ///
  ///@param dim the dimension to slice along. Specifying negative value will
  /// count from the last dim backwards (-1 fot the last dim, -2 for the
  /// previoud one etc.).
  ///@param startIndex the index to start slicing at along the dimension
  ///@param sliceDepth the depth/size of the slice along the dimension
  DoubleTensor getSlice(DimInt dim,
                        DimInt startIndex,
                        DimInt sliceDepth = 1) const;

  ///@brief Put a tensor slice into an existing tensor. Putting the slice will
  /// override existing values in indexes positioned where the slice will be
  /// put, and the tensor size along the provided domension will be increased if
  /// needed.
  ///
  ///@param dim an existing dimension to put the slice in. Specifying negative
  /// value will count from the last dim backwards (-1 fot the last dim, -2 for
  /// the previoud one etc.).
  ///@param startIndex the index to start positioning the slice in along the
  /// dimension. specifying -1 will append slice after last current index.
  ///@param slice the slice to put. must be a tensor with the same number of
  /// dimensions as this, and with sizes of dimensions exactly maching the sizes
  /// of this except for the provided dimension to put the slice in, that might
  /// be at any size.
  void putSlice(DimInt dim, DimInt startIndex, const DoubleTensor& slice);

  ///@brief Removes the specified slice from this DoubleTensor, in place. The
  /// removed slice starts at startIndex of the given dimension and ends at
  /// startIndex + sliceDepth - 1.
  ///@param dim the dimension to slice along. Specifying negative value will
  /// count from the last dim backwards (-1 fot the last dim, -2 for the
  /// previoud one etc.).
  ///@param startIndex the index to start slicing at along the dimension
  ///@param sliceDepth the depth/size of the slice along the dimension
  void removeSlice(DimInt dim, DimInt startIndex, DimInt sliceDepth = 1);

  ///@brief Returns the element at specified indices
  template <class... DimInts>
  inline const double& at(DimInt i, DimInts... is) const
  {
    return tensor.at(i, is...);
  }

  ///@brief Returns the element at specified indices
  template <class... DimInts>
  inline double& at(DimInt i, DimInts... is)
  {
    return tensor.at(i, is...);
  }

  ///@brief Flips the order of the first two dimensions.
  /// For a 2d matrix, this means the transposed matrix.
  /// Requires at least 2 dimensions.
  void transpose();

  ///@brief Rotates the content of this DoubleTensor along the specified
  /// dimension
  ///
  ///@param dim The dimension to rotate along
  void rotateAlongDim(DimInt dim);

  ///@brief Returns tensor content as flat vector.
  ///
  ///@param firstOrder Whether to populate the vector using first order
  /// convention
  std::vector<double> getFlattened(bool firstOrder = true) const;

  ///@brief Returns the sum of all elements in the tensor
  ///
  double sumOfElements();

  ///@brief Performs element-wise add of other into this.
  ///
  ///@param other the tensor to add into this. must have the same shape as this.
  void elementAdd(const DoubleTensor& other);

  ///@brief Performs element-wise subtract of other from this.
  ///
  ///@param other the tensor to subtract from this. must have the same shape as
  /// this.
  void elementSub(const DoubleTensor& other);

  ///@brief Performs element-wise multiplication of other into this.
  ///
  ///@param other the tensor to multiply into this. must have the same shape as
  /// this.
  void elementMultiply(const DoubleTensor& other);

  ///@brief Calculates the result of matrix multiplication of this and other
  /// in-place.
  /// Both this and other tensor can be either 2D or 3D. If they are 3D, the
  /// third dimension is considered the batch dimension. The output will have a
  /// batch dimension of either inputs have it.
  /// If both inputs have the batch dimension, we require it to be the same.
  ///
  ///@param other the matrix to multiply with this.
  void matrixMultiply(const DoubleTensor& other);

  ///@brief Same as matrixMutiply, but computes other*this, and stores results
  /// in this. See matrixMultiply()
  ///
  ///@param other the matrix to multiply with this on the left.
  void matrixMultiplyLeft(const DoubleTensor& other);

  ///@brief Sets in res the result of matrix-multiplication of this and other.
  /// Both this and other tensor can be either 2D or 3D. If they are 3D, the
  /// third dimension is considered the batch dimension.
  /// If they are 3D they both need to be 3D with the same batch dimension,
  /// and so is the output.
  ///
  ///@param other Other matrix to multiply with.
  ///@param res Output tensor
  void getMatrixMultiply(const DoubleTensor& other, DoubleTensor& res) const;

  ///@brief Calculates the result of adding a vector to this vector. The
  /// other
  /// vector is a 2D tensor of sizes [a,1] and this vector is a 3D tensor of
  /// sizes [a,1,batch]. The result is a 3D tensor of sizes [a,1,batch].
  ///
  ///@param vector the 2D vector to add to this.
  void addVector(const DoubleTensor& vector);

  ///@brief Adds given scalar to all elements
  ///
  ///@param val Value to add
  void addScalar(double val);

  ///@brief Subtracts given scalar to all elements
  ///
  ///@param val Value to subtract
  void subtractScalar(double val);

  ///@brief Multiplies all elements by a given scalar
  ///
  ///@param val Value to multiply by
  void multiplyScalar(double val);

  ///@brief Multiplies all elements by a scalar, where with respect to a given
  /// dimension, every "slice" along that dimension will be multiplied by a
  /// different scalar.
  /// For example, if the given dimension is dimension 0 and its size is 3, a
  /// vector of 3 scalars [s0,s1,s2] should be provided. All the slots in the
  /// result tensor at index 0/1/2 along dimension 0 will be multiplied by the
  /// scalar s0/s1/s2, respectively.
  ///
  ///@param dim The given dimension.
  ///@param vals The scalars to multiply by.
  void multiplyScalarsDimWise(DimInt dim, const std::vector<double>& vals);

  ///@brief Performs element-wise polynomial computation into this.
  ///
  ///@param coeffs the coefficients of the polynomial, in the order:
  /// c[0],c[1],...,c[n]
  void elementPolyEval(const std::vector<double>& coeffs);

  ///@brief Returns an element-wise indication of the sign of a number into
  /// this.
  void sign();

  ///@brief Returns the index of the element with maximal value in the tensor.
  ///
  DimInt argmax() const;

  ///@brief Returns the maximal absolute value in the tensor.
  ///
  double maxAbs() const;

  ///@brief Reads a matrix from a CSV file. Any current content or shape of this
  /// is overwritten.
  ///
  ///@param filePath Path to legal CSV file to read from.
  ///@param ignoreFirstRow if true, it assumes the first row is column names.
  void readMatrixFromCsvFile(const std::string& filePath,
                             bool ignoreFirstRow = false);

  ///@brief Reads a matrix from a stream containing the content of a CSV file.
  /// Any current content or shape of this is overwritten.
  ///
  ///@param stream Stream containing the content of a legal CSV file to read
  /// from.
  ///@param ignoreFirstRow if true, it assumes the first row is column names.
  void readMatrixFromCsvStream(std::istream& stream,
                               bool ignoreFirstRow = false);

  ///@brief Writes the content of this to a CSV file. The shape of this must be
  /// 2D.
  ///
  ///@param filePath Path to CSV file to write to.
  void writeMatrixToCsv(const std::string& filePath) const;

  ///@brief Writes the content of this as CSV to a stream. The shape of this
  /// must be 2D.
  ///
  ///@param out Stream to write to.
  void writeMatrixToCsv(std::ostream& out) const;

  ///@brief Returns a stream containing this tensor as CSV. The shape of this
  /// must be 2D
  ///
  std::shared_ptr<std::istream> getCsvMatrixStream() const;

  ///@brief Asserts \p other has the same shape as `this`, except an optional
  /// skipped dimension that is allowed to have a different size (-1 to compare
  /// all dimensions).
  ///
  ///@param other Other tensor to test with
  ///@param title Optional title for error printouts
  ///@param skippedDim Optional dimension to skip
  void assertSameShape(const DoubleTensor& other,
                       const std::string& title = "",
                       DimInt skippedDim = -1) const;

  // DoubleTensor argminAlongDimension(int dim) const;

  ///@brief Verifies this tensor equals other tensor. Returns the mean squared
  /// error (MSE).
  ///
  ///@param other The other tensor to verify equality to.
  ///@param title A title to print in case the assertion fails.
  ///@param eps The maximal diff allowed in each slot between this and other.
  /// A negative value indicates not to throw an exception, and use this
  /// function just for its return value.
  ///@param relative Whether to apply comparison with respect to relative
  /// difference between values. Defaults to false.
  double assertEquals(const DoubleTensor& other,
                      const std::string& title = "",
                      double eps = 1e-5,
                      bool relative = false) const;

  ///@brief Computes the MSE (Mean Square Error) with expected values.
  /// If title!="", prints results to cout.
  /// If eps>=0, asserts MSE<=eps
  ///@param expectedVals tensor input to compare
  ///@param title If not empty, will be output to cout along with MSE
  ///@param eps If non-negative, an exception will be thrown if MSE>eps
  double testMse(const DoubleTensor& expectedVals,
                 const std::string& title = "",
                 double eps = -1) const;
};

std::ostream& operator<<(std::ostream& out, const DoubleTensor& dt);

typedef std::shared_ptr<const DoubleTensor> DoubleTensorCPtr;

} // namespace helayers

#endif /* MATH_DOUBLE_TENSOR_H_ */
