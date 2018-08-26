/// @cond
/**
 * This file is subject to the terms and conditions defined in file 'LICENSE', which is part of this
 * source code package.
 */
/// @endcond

/**
 * @file koala/include/koala/KoalaApi.txx
 *
 * @brief Template implementation header file for the koala API (KoalaApi) class.
 */

#ifndef KL_API_IMPL_H
#define KL_API_IMPL_H 1

#include "koala/Koala/Koala.h"

namespace kl
{
template <typename TOBJECT>
inline auto KoalaApi::RegisterRegistry(std::string printableObjectName) const & ->
    typename std::decay_t<TOBJECT>::Registry &
{
    return m_spKoala->RegisterRegistry<std::decay_t<TOBJECT>>(std::move(printableObjectName));
}

//--------------------------------------------------------------------------------------------------

template <typename TOBJECT>
inline auto &KoalaApi::FetchRegistry() const
{
    return m_spKoala->FetchRegistry<std::decay_t<TOBJECT>>();
}

//--------------------------------------------------------------------------------------------------

template <typename TOBJECT, typename... TARGS>
inline auto &KoalaApi::Create(TARGS &&... arguments) const
{
    using TOBJECT_D = std::decay_t<TOBJECT>;
    return m_spKoala->FetchRegistry<TOBJECT_D>().template Create<TOBJECT_D>(
        std::forward<TARGS>(arguments)...);
}

//--------------------------------------------------------------------------------------------------

template <typename TOBJECT, typename TALIAS, typename... TARGS>
inline auto &KoalaApi::CreateByAlias(TALIAS &&alias, TARGS &&... arguments) const
{
    using TOBJECT_D = std::decay_t<TOBJECT>;
    return m_spKoala->FetchRegistry<TOBJECT_D>().template CreateByAlias<TOBJECT_D>(
        std::forward<TALIAS>(alias), std::forward<TARGS>(arguments)...);
}

//--------------------------------------------------------------------------------------------------

template <typename TOBJECT, typename T>
inline auto &KoalaApi::Get(T &&arg) const
{
    using TOBJECT_D = std::decay_t<TOBJECT>;
    return m_spKoala->FetchRegistry<TOBJECT_D>().template Get<TOBJECT_D>(std::forward<T>(arg));
}

//--------------------------------------------------------------------------------------------------

template <typename TOBJECT>
inline auto KoalaApi::GetAllList() const noexcept
{
    using TOBJECT_D = std::decay_t<TOBJECT>;
    return m_spKoala->FetchRegistry<TOBJECT_D>().template GetAllList<TOBJECT_D>();
}

//--------------------------------------------------------------------------------------------------

template <typename TOBJECT>
inline auto KoalaApi::GetAll() const noexcept
{
    using TOBJECT_D = std::decay_t<TOBJECT>;
    return m_spKoala->FetchRegistry<TOBJECT_D>().template GetAll<TOBJECT_D>();
}

//--------------------------------------------------------------------------------------------------

template <typename TOBJECT>
inline auto KoalaApi::Count() const noexcept
{
    using TOBJECT_D = std::decay_t<TOBJECT>;
    return m_spKoala->FetchRegistry<TOBJECT_D>().template Count<TOBJECT_D>();
}

//--------------------------------------------------------------------------------------------------

template <typename TOBJECT, typename T>
inline auto KoalaApi::Delete(T &&arg) const noexcept
{
    return m_spKoala->FetchRegistry<std::decay_t<TOBJECT>>().Delete(std::forward<T>(arg));
}

//--------------------------------------------------------------------------------------------------

template <typename TOBJECT>
inline void KoalaApi::DeleteAll() const noexcept
{
    return m_spKoala->FetchRegistry<std::decay_t<TOBJECT>>().DeleteAll();
}

//--------------------------------------------------------------------------------------------------

template <typename TOBJECT, typename T>
inline auto KoalaApi::GetAlias(T &&arg) const
{
    using TOBJECT_D = std::decay_t<TOBJECT>;
    return m_spKoala->FetchRegistry<TOBJECT_D>().template GetAlias<TOBJECT_D>(std::forward<T>(arg));
}

//--------------------------------------------------------------------------------------------------

template <typename TOBJECT, typename T>
inline auto KoalaApi::DoesObjectExist(T &&arg) const noexcept
{
    using TOBJECT_D = std::decay_t<TOBJECT>;
    return m_spKoala->FetchRegistry<TOBJECT_D>().template DoesObjectExist<TOBJECT_D>(
        std::forward<T>(arg));
}

//--------------------------------------------------------------------------------------------------

template <typename TALG, typename... TARGS>
inline auto &KoalaApi::CreateAlgorithm(std::string algorithmName, TARGS &&... arguments) const
{
    return m_spKoala->FetchRegistry<Algorithm>().template CreateByAlias<std::decay_t<TALG>>(
        std::move(algorithmName), std::forward<TARGS>(arguments)...);
}

//--------------------------------------------------------------------------------------------------

template <typename TALG, typename... TARGS>
inline auto &KoalaApi::CreateAndRunAlgorithm(const std::string &algorithmName,
                                             TARGS &&... arguments) const
{
    auto &algorithm =
        this->CreateAlgorithm<std::decay_t<TALG>>(algorithmName, std::forward<TARGS>(arguments)...);
    this->RunAlgorithm(algorithmName);
    return algorithm;
}

//--------------------------------------------------------------------------------------------------

template <typename TALG, typename... TARGS>
inline void KoalaApi::CreateRunAndDeleteAlgorithm(const std::string &algorithmName,
                                                  TARGS &&... arguments) const
{
    this->CreateAndRunAlgorithm<std::decay_t<TALG>>(algorithmName,
                                                    std::forward<TARGS>(arguments)...);
    this->DeleteAlgorithm(algorithmName);
}

//--------------------------------------------------------------------------------------------------

template <typename TALG, typename TRETURN, typename... TARGS>
auto KoalaApi::CreateRunExtractAndDeleteAlgorithm(
    std::string algorithmName, const std::function<TRETURN(const TALG &)> &extractor,
    TARGS &&... arguments)
{
    auto &algorithm = this->CreateAndRunAlgorithm<std::decay_t<TALG>>(
        algorithmName, std::forward<TARGS>(arguments)...);
    const auto extractedVar = extractor(algorithm);

    this->DeleteAlgorithm(algorithmName);
    return extractedVar;
}

//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------

#ifdef KOALA_ENABLE_CEREAL
template <typename TARCHIVE>
inline void KoalaApi::load(TARCHIVE &archive)
{
    archive(m_spKoala);
    m_spKoala->Initialize();
}
#endif  // #ifdef KOALA_ENABLE_CEREAL

//--------------------------------------------------------------------------------------------------

#ifdef KOALA_ENABLE_CEREAL
template <typename TARCHIVE>
inline void KoalaApi::save(TARCHIVE &archive) const
{
    archive(m_spKoala);
}
#endif  // #ifdef KOALA_ENABLE_CEREAL
}  // namespace kl

#endif  // #ifndef KL_API_IMPL_H