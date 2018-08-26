/// @cond
/**
 * This file is subject to the terms and conditions defined in file 'LICENSE', which is part of this
 * source code package.
 */
/// @endcond

/**
 * @file koala/include/koala/Algorithm.h
 *
 * @brief Header file for the algorithm (Algorithm) abstract class.
 */

#ifndef KL_ALGORITHM_H
#define KL_ALGORITHM_H 1

#include "koala/Registry/ObjectRegistry.h"
#include "koala/Templates/HierarchicalObjectTemplate.h"

#ifdef KOALA_ENABLE_CEREAL
#include "cereal/access.hpp"
#include "cereal/types/base_class.hpp"
#include "cereal/types/polymorphic.hpp"
#include "koala/Registry/ObjectAssociation.h"
#endif  // #ifdef KOALA_ENABLE_CEREAL

namespace kl
{
/**
 * @brief Algorithm abstract class.
 */
class Algorithm : public HierarchicalObjectTemplate<Algorithm, std::string>,
                  public std::enable_shared_from_this<Algorithm>
{
protected:
    /**
     * @brief Constructor.
     *
     * @param wpRegistry Weak pointer to the associated registry.
     * @param id Unique ID for the object.
     * @param wpKoala Weak pointer to the instance of Koala.
     */
    Algorithm(Registry_wPtr wpRegistry, const kl::ID_t id, Koala_wPtr wpKoala) noexcept;

    //----------------------------------------------------------------------------------------------

    /**
     * @brief Get the is-cereal-serializable boolean.
     *
     * @return The is-cereal-serializable boolean.
     */
    KL_IS_SERIALIZABLE(true);

    /**
     * @brief Run the algorithm.
     *
     * @return Success.
     */
    virtual bool Run() = 0;

    /**
     * @brief Create an instance of a sub-algorithm.
     *
     * @param algorithmName The new (unique) name of the algorithm to create.
     * @param arguments The arguments to be passed to the algorithm's constructor.
     *
     * @return The new algorithm.
     */
    template <typename TALG, typename... TARGS>
    auto &CreateAlgorithm(std::string algorithmName, TARGS &&... arguments);

    /**
     * @brief Run another algorithm, if registered as a sub-algorithm of this algorithm.
     *
     * @param algorithmName The name of the algorithm to run.
     * @param arguments The arguments to forward to the algorithm.
     */
    template <typename... TARGS>
    void RunAlgorithm(const std::string &algorithmName, TARGS &&... arguments) const;

    /**
     * @brief Create a sub-algorithm and run it.
     *
     * @param algorithmName The new (unique) name of the algorithm to create.
     * @param arguments The arguments to be passed to the algorithm's constructor.
     *
     * @return The new algorithm.
     */
    template <typename TALG, typename... TARGS>
    auto &CreateAndRunAlgorithm(const std::string &algorithmName, TARGS &&... arguments);

    /**
     * @brief Create a sub-algorithm, run it and delete it.
     *
     * @param algorithmName The new (arbitrary) name of the algorithm to create.
     * @param arguments The arguments to be passed to the algorithm's constructor.
     */
    template <typename TALG, typename... TARGS>
    void CreateRunAndDeleteAlgorithm(const std::string &algorithmName, TARGS &&... arguments);

    /**
     * @brief Create a sub-algorithm, run it, extract a variable and then delete the algorithm.
     *
     * @param algorithmName The new (unique) name of the algorithm to create.
     * @param extractor A function to extract the variable.
     * @param arguments The arguments to be passed to the algorithm's constructor.
     *
     * @return The extracted variable.
     */
    template <typename TALG, typename TRETURN, typename... TARGS>
    auto CreateRunExtractAndDeleteAlgorithm(std::string algorithmName,
                                            const std::function<TRETURN(const TALG &)> &extractor,
                                            TARGS &&... arguments);

    friend Registry;  ///< Alias for the object registry from the base class.
    friend class Koala;

    template <typename T>
    friend class HierarchicalVisualizationUtility;

#ifdef KOALA_ENABLE_CEREAL
    friend class cereal::access;
#endif  // #ifdef KOALA_ENABLE_CEREAL

public:
    KL_OBJECT_ALIASES(Algorithm);  ///< Aliases for reference wrappers, unordered sets and vectors.

    //----------------------------------------------------------------------------------------------

    /**
     * @brief Deleted copy constructor.
     */
    Algorithm(const Algorithm &) = delete;

    /**
     * @brief Deleted move constructor.
     */
    Algorithm(Algorithm &&) = delete;

    /**
     * @brief Deleted copy assignment operator.
     */
    Algorithm &operator=(const Algorithm &) = delete;

    /**
     * @brief Deleted move assignment operator.
     */
    Algorithm &operator=(Algorithm &&) = delete;

    /**
     * @brief Default virtual destructor.
     */
    virtual ~Algorithm() = default;

    //----------------------------------------------------------------------------------------------

    /**
     * @brief Get a printable name for the object.
     *
     * @return A printable name for the object.
     */
    KL_PRINTABLE_NAME("Algorithm");

    /**
     * @brief Get a shared pointer to the object.
     *
     * @return A shared pointer to the object.
     */
    KL_GET_SHARED_POINTER(Algorithm);

    /**
     * @brief Get a string that identifies a given instantiation of the object.
     *
     * @return A string that identifies a given instantiation of the object.
     */
    KL_IDENTIFIER_STRING(this->HasAlias() ? this->Alias() : std::string{});

private:
#ifdef KOALA_ENABLE_CEREAL
    /**
     * @brief Method template for serializing object.
     *
     * @param archive The cereal archive object.
     */
    template <typename TARCHIVE>
    void serialize(TARCHIVE &archive);

    /**
     * @brief Default constructor for serialization purposes.
     */
    Algorithm() = default;
#endif  // #ifdef KOALA_ENABLE_CEREAL
};

//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------

inline Algorithm::Algorithm(Registry_wPtr wpRegistry, const kl::ID_t id, Koala_wPtr wpKoala) noexcept
    : HierarchicalObject{std::move_if_noexcept(wpRegistry), id, std::move_if_noexcept(wpKoala)}
{
}
}  // namespace kl

//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------

#ifdef KOALA_ENABLE_CEREAL
CEREAL_REGISTER_TYPE(kl::Algorithm)
CEREAL_REGISTER_TYPE(kl::Algorithm::Registry)
CEREAL_REGISTER_TYPE(kl::Algorithm::HierarchicalObject)
CEREAL_REGISTER_TYPE(kl::ObjectAssociation<kl::Algorithm>)
#endif  // #ifdef KOALA_ENABLE_CEREAL

//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------

#include "koala/Algorithm.txx"

#endif  // #ifndef KL_ALGORITHM_H