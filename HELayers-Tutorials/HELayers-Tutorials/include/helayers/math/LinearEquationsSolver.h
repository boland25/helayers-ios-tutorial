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

#ifndef SRC_MATH_LINEAR_EQUATIONSSOLVER_H_
#define SRC_MATH_LINEAR_EQUATIONS_SOLVER_H_

#include <cstring>
#include <map>
#include <vector>
#include "helayers/math/CTileTensor.h"

namespace helayers {

class HeContext;

///@brief A LinearEquationSolver solvers a set of equations using Cramer's rule
class LinearEquationsSolver : public Printable
{
public:
  ///@brief Construct a new LinearEquationSolver object
  ///@param[in] he The context
  ///@param LHS The left-hand-side of the equations (coefficients)
  ///@param RHS The right-hand-side of the equations (resulting values)
  LinearEquationsSolver(const HeContext& he,
                        const CTileTensor& lhs,
                        const CTileTensor& rhs);

  ///@brief Solve the system of equations to find a valid variable assignment
  ///@param res The resulting variable values
  void solve(CTileTensor& res);

  ///@brief Get private member: numEquations,
  /// i.e. the number of equations and variables
  int getNumEquations() { return numEquations; }
  ///@brief Set private member: reciprocalMinVal
  /// i.e. the minimal value of inverted value (determinant of the
  /// left-hand-side of the equations)
  void setReciprocalMinVal(double val) { reciprocalMinVal = val; }
  ///@brief Set private member: reciprocalMaxVal
  /// i.e. the maximal value of inverted value (determinant of the
  /// left-hand-side of the equations)
  void setReciprocalMaxVal(double val) { reciprocalMaxVal = val; }
  ///@brief Set private member: reciprocalBitResolution
  /// Controls the accuracy of the inversal result.
  /// A higher bitResolution value will increase the accuracy of the result
  /// at the account of consumig more multiplication depth.
  void setReciprocalBitResolution(double val) { reciprocalBitResolution = val; }

  void debugPrint(const std::string& title = "",
                  Verbosity verbosity = VERBOSITY_REGULAR,
                  std::ostream& out = std::cout) const override;

private:
  ///@brief The number of equations and variables (must equal)
  int numEquations;

  // Configuration of the inverse operation
  ///@brief minimal value of inverted value (determinant of the left-hand-side
  /// of the equations)
  double reciprocalMinVal = 0.001;
  ///@brief maximal value of inverted value (determinant of the left-hand-side
  /// of the equations)
  double reciprocalMaxVal = 10.0;
  ///@brief Controls the accuracy of the inversal result.
  /// A higher bitResolution value will increase the accuracy of the result
  /// at the account of consumig more multiplication depth.
  int reciprocalBitResolution = 13;

  const HeContext& he;
  ///@brief The left-hand-side of the equations (the coefficients)
  /// - a matrix of shape nxn
  const CTileTensor& lhs;
  ///@brief The right-hand-side of the equations
  /// - a vector of size n
  const CTileTensor& rhs;

  /// Verbosity level
  Verbosity verbose = VERBOSITY_NONE;

  ///@brief Set the verbosity Level
  ///@param verbose the required verbosity level
  inline void setVerbosityLevel(Verbosity verbose) { this->verbose = verbose; }

  ///@brief Replaces a column in a square matrix with another input column
  ///@param replacedLhs The resulting matrix after the column has been replaced
  ///@param replacedCol The index of the column in lhs to replace with RHS
  void replaceColumn(CTileTensor& replacedLhs, int replacedCol);
};
} // namespace helayers

#endif /* SRC_MATH_LINEAR_EQUATIONS_SOLVER_H_ */
