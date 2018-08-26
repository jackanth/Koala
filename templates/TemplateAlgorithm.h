/// @cond
/**
 * This file is subject to the terms and conditions defined in file 'LICENSE',
 * which is part of source code package.
 */
/// @endcond

/**
 * @file include/TemplateAlgorithm.h
 *
 * @brief Header file for the template algorithm (TemplateAlgorithm) class.
 */

#ifndef TEMPLATE_ALGORITHM_H
#define TEMPLATE_ALGORITHM_H 1

#include "koala/Algorithm.h"

/**
 * @brief TemplateAlgorithm class.
 */
class TemplateAlgorithm : public kl::Algorithm
{
public:
    /**
     * @brief Deleted copy constructor.
     */
    TemplateAlgorithm(const TemplateAlgorithm &) = delete;

    /**
     * @brief Deleted move constructor.
     */
    TemplateAlgorithm(TemplateAlgorithm &&) = delete;

    /**
     * @brief Deleted copy assignment operator.
     */
    TemplateAlgorithm &operator=(const TemplateAlgorithm &) = delete;

    /**
     * @brief Deleted move assignment operator.
     */
    TemplateAlgorithm &operator=(TemplateAlgorithm &&) = delete;

    /**
     * @brief Default destructor.
     */
    ~TemplateAlgorithm() = default;

    //----------------------------------------------------------------------------------------------

    /**
     * @brief Get a printable name for the object.
     *
     * @return A printable name for the object.
     */
    KL_PRINTABLE_NAME("TemplateAlgorithm");

    /**
     * @brief Get a string that identifies a given instantiation of the object.
     *
     * @return A string that identifies a given instantiation of the object.
     */
    KL_IDENTIFIER_STRING(this->HasAlias() ? this->Alias() : std::string{});

protected:
    using kl::Algorithm::Algorithm;  ///< Use the base class constructor.

    /**
     * @brief Get the is-cereal-serializable boolean.
     *
     * @return The is-cereal-serializable boolean.
     */
    KL_IS_SERIALIZABLE(false);

    /**
     * @brief Run the algorithm.
     *
     * @return Success.
     */
    bool Run() override;

    friend Registry;  ///< Alias for the object registry from the base class.
    friend class kl::Koala;
};

#endif  // #ifndef TEMPLATE_ALGORITHM_H
