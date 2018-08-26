/// @cond
/**
 * This file is subject to the terms and conditions defined in file 'LICENSE', which is part of this
 * source code package.
 */
/// @endcond

/**
 * @file koala/include/koala/Algorithm.txx
 *
 * @brief Template implementation header file for the algorithm (Algorithm) abstract class.
 */

#ifndef KL_ALGORITHM_IMPL_H
#define KL_ALGORITHM_IMPL_H 1

#include "koala/Definitions.h"
#include "koala/Koala/Koala.h"

namespace kl
{
template <typename TALG, typename... TARGS>
inline auto &Algorithm::CreateAlgorithm(std::string algorithmName, TARGS &&... arguments)
{
    return this->GetRegistry().template CreateByAlias<TALG>(std::move(algorithmName),
                                                            std::forward<TARGS>(arguments)...);
}

//--------------------------------------------------------------------------------------------------

template <typename... TARGS>
void Algorithm::RunAlgorithm(const std::string &algorithmName, TARGS &&... arguments) const
{
    // First we need to try to get the algorithm (this will throw an error if the algorithm alias
    // doesn't exist).
    this->GetRegistry().template Get<Algorithm>(algorithmName);
    this->GetKoala().template RunAlgorithm<Algorithm>(algorithmName,
                                                      std::forward<TARGS>(arguments)...);
}

//--------------------------------------------------------------------------------------------------

template <typename TALG, typename... TARGS>
inline auto &Algorithm::CreateAndRunAlgorithm(const std::string &algorithmName,
                                              TARGS &&... arguments)
{
    auto &newAlgorithm =
        this->CreateAlgorithm<TALG>(algorithmName, std::forward<TARGS>(arguments)...);
    this->RunAlgorithm(algorithmName);
    return newAlgorithm;
}

//--------------------------------------------------------------------------------------------------

template <typename TALG, typename... TARGS>
inline void Algorithm::CreateRunAndDeleteAlgorithm(const std::string &algorithmName,
                                                   TARGS &&... arguments)
{
    this->CreateAlgorithm<TALG>(algorithmName, std::forward<TARGS>(arguments)...);
    this->RunAlgorithm(algorithmName);
    this->GetRegistry().Delete(algorithmName);
}

//--------------------------------------------------------------------------------------------------

template <typename TALG, typename TRETURN, typename... TARGS>
auto Algorithm::CreateRunExtractAndDeleteAlgorithm(
    std::string algorithmName, const std::function<TRETURN(const TALG &)> &extractor,
    TARGS &&... arguments)
{
    auto &algorithm = this->CreateAlgorithm<TALG>(algorithmName, std::forward<TARGS>(arguments)...);
    this->RunAlgorithm(algorithmName);
    auto extractedVar = extractor(algorithm);

    this->GetRegistry().Delete(algorithmName);
    return extractedVar;
}

//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------

#ifdef KOALA_ENABLE_CEREAL
template <typename TARCHIVE>
inline void Algorithm::serialize(TARCHIVE &archive)
{
    archive(cereal::base_class<HierarchicalObject>(this));
}
#endif  // #ifdef KOALA_ENABLE_CEREAL
}  // namespace kl

#endif  // #ifndef KL_ALGORITHM_IMPL_H