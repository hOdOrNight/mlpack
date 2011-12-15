/**
 * @file aug_lagrangian.hpp
 * @author Ryan Curtin
 *
 * Definition of AugLagrangian class, which implements the Augmented Lagrangian
 * optimization method (also called the 'method of multipliers'.  This class
 * uses the L-BFGS optimizer.
 */

#ifndef __MLPACK_CORE_OPTIMIZERS_AUG_LAGRANGIAN_AUG_LAGRANGIAN_HPP
#define __MLPACK_CORE_OPTIMIZERS_AUG_LAGRANGIAN_AUG_LAGRANGIAN_HPP

#include <mlpack/core.hpp>

namespace mlpack {
namespace optimization {

/**
 * The AugLagrangian class implements the Augmented Lagrangian method of
 * optimization.  In this scheme, a penalty term is added to the Lagrangian.
 * This method is also called the "method of multipliers".
 *
 * The template class LagrangianFunction must implement the following three
 * methods:
 *   double Evaluate(const arma::mat& coordinates);
 *   void Gradient(const arma::mat& coordinates, arma::mat& gradient);
 *   int NumConstraints();
 *   double EvaluateConstraint(int index, const arma::mat& coordinates);
 *   double GradientConstraint(int index, const arma::mat& coordinates,
 *       arma::mat& gradient);
 *
 * The number of constraints must be greater than or equal to 0, and
 * EvaluateConstraint() should evaluate the constraint at the given index for
 * the given coordinates.  Evaluate() should provide the objective function
 * value for the given coordinates.
 */
template<typename LagrangianFunction>
class AugLagrangian
{
 public:
  AugLagrangian(LagrangianFunction& function, size_t numBasis);
      // not sure what to do here yet

  bool Optimize(size_t num_iterations,
                arma::mat& coordinates,
                double sigma = 0.5);

  //! Get the LagrangianFunction.
  const LagrangianFunction& Function() const { return function; }
  //! Modify the LagrangianFunction.
  LagrangianFunction& Function() { return function; }

  //! Get the number of memory points used by L-BFGS.
  size_t NumBasis() const { return numBasis; }
  //! Modify the number of memory points used by L-BFGS.
  size_t& NumBasis() { return numBasis; }

 private:
  LagrangianFunction& function;
  size_t numBasis;

  /**
   * This is a utility class, which we will pass to L-BFGS during the
   * optimization.  We use a utility class so that we do not have to expose
   * Evaluate() and Gradient() to the AugLagrangian public interface; instead,
   * with a private class, these methods are correctly protected (since they
   * should not be being used anywhere else).
   */
  class AugLagrangianFunction
  {
   public:
    AugLagrangianFunction(LagrangianFunction& function_in,
                          arma::vec& lambda_in,
                          double sigma);

    double Evaluate(const arma::mat& coordinates);
    void Gradient(const arma::mat& coordinates, arma::mat& gradient);

    const arma::mat& GetInitialPoint();

    //! Get the Lagrangian multipliers.
    const arma::vec& Lambda() const { return lambda; }
    //! Modify the Lagrangian multipliers.
    arma::vec& Lambda() { return lambda; }

    //! Get sigma.
    double Sigma() const { return sigma; }
    //! Modify sigma.
    double& Sigma() { return sigma; }

    //! Get the Lagrangian function.
    const LagrangianFunction& Function() const { return function; }
    //! Modify the Lagrangian function.
    LagrangianFunction& Function() { return function; }

   private:
    arma::vec lambda;
    double sigma;

    LagrangianFunction& function;
  };
};

}; // namespace optimization
}; // namespace mlpack

#include "aug_lagrangian_impl.hpp"

#endif // __MLPACK_CORE_OPTIMIZERS_AUG_LAGRANGIAN_AUG_LAGRANGIAN_HPP
