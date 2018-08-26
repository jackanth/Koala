/// @cond
/**
 * This file is subject to the terms and conditions defined in file 'LICENSE', which is part of this
 * source code package.
 */
/// @endcond

/**
 * @file koala/include/koala/Koala.txx
 *
 * @brief Template implementation header file for the koala (Koala) class.
 */

#ifndef KL_KOALA_IMPL_H
#define KL_KOALA_IMPL_H

#include "koala/Definitions.h"
#include "koala/Registry/ObjectRegistry.h"

namespace kl
{
template <typename TOBJECT>
auto &Koala::FetchRegistryImpl() const
{
    using TOBJECT_D = std::decay_t<TOBJECT>;
    const auto ret =
        (TOBJECT_D::IsCerealSerializable()
             ? m_serializableAssocTypeMap.equal_range(typeid(typename TOBJECT_D::Registry).name())
             : m_unserializableAssocTypeMap.equal_range(
                   typeid(typename TOBJECT_D::Registry).name()));

    for (auto iter = ret.first; iter != ret.second; ++iter)
    {
        if (const auto spCastRegistry =
                std::dynamic_pointer_cast<typename TOBJECT_D::Registry>(iter->second))
            return *spCastRegistry;
    }

    KL_THROW("No such object registry exists: " << KL_WHITE_BOLD
                                                << TOBJECT_D::StaticPrintableName());
}

//--------------------------------------------------------------------------------------------------

template <typename TOBJECT>
auto Koala::HasRegistryImpl() const noexcept
{
    using TOBJECT_D = std::decay_t<TOBJECT>;
    const auto ret =
        (TOBJECT_D::IsCerealSerializable()
             ? m_serializableAssocTypeMap.equal_range(typeid(typename TOBJECT_D::Registry).name())
             : m_unserializableAssocTypeMap.equal_range(
                   typeid(typename TOBJECT_D::Registry).name()));

    for (auto iter = ret.first; iter != ret.second; ++iter)
    {
        if (std::dynamic_pointer_cast<typename TOBJECT_D::Registry>(iter->second)) return true;
    }

    return false;
}

//--------------------------------------------------------------------------------------------------

#ifdef KOALA_ENABLE_CEREAL
template <typename TARCHIVE>
inline void Koala::serialize(TARCHIVE &archive)
{
    archive(m_serializableRegistries, m_serializableAssocTypeMap, m_functionRecorder, m_debugMode,
            m_callLevel.load());
}
#endif  // #ifdef KOALA_ENABLE_CEREAL

//--------------------------------------------------------------------------------------------------

#ifdef KOALA_ENABLE_CEREAL
template <typename TARCHIVE>
void Koala::load_and_construct(TARCHIVE &archive, cereal::construct<Koala> &construct)
{
    auto serializableRegistries = ObjectRegistryBase::sPtrSet{};
    auto serializableAssocTypeMap = ObjectAssocTypeMap{};
    auto functionRecorder = FunctionRecorder{};
    auto debugMode = false;
    auto callLevel = SIZE_T(0UL);

    // Load archived variables and construct the object.
    construct();
    archive(serializableRegistries, serializableAssocTypeMap, functionRecorder, debugMode,
            callLevel);

    construct->m_serializableRegistries = std::move(serializableRegistries);
    construct->m_serializableAssocTypeMap = std::move(serializableAssocTypeMap);
    construct->m_functionRecorder = std::move(functionRecorder);
    construct->m_debugMode = debugMode;
    construct->m_callLevel.store(callLevel);  // atomicity not guaranteed
}
#endif  // #ifdef KOALA_ENABLE_CEREAL

//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------

template <typename TOBJECT>
inline auto &Koala::FetchRegistry() const
{
    const auto lock = ReadLock{m_mutexRegistries};
    return this->FetchRegistryImpl<std::decay_t<TOBJECT>>();
}

//--------------------------------------------------------------------------------------------------

template <typename TOBJECT>
inline auto Koala::HasRegistry() const noexcept
{
    const auto lock = ReadLock{m_mutexRegistries};
    return this->HasRegistryImpl<std::decay_t<TOBJECT>>();
}

//--------------------------------------------------------------------------------------------------

template <typename TOBJECT>
auto Koala::RegisterRegistry(std::string printableObjectName) noexcept(
    std::is_nothrow_constructible<typename std::decay_t<TOBJECT>::Registry, Koala::wPtr &&,
                                  std::string &&>::value) ->
    typename std::decay_t<TOBJECT>::Registry &
{
    using TOBJECT_D = std::decay_t<TOBJECT>;
    using TREGISTRY = typename TOBJECT_D::Registry;

    // If we already have the registry, return it.
    if (this->HasRegistryImpl<TOBJECT_D>()) return this->FetchRegistryImpl<TOBJECT_D>();

    auto wpThis = static_cast<Koala::wPtr>(this->shared_from_this());

    const auto spRegistry =
        typename TREGISTRY::sPtr{new TREGISTRY{std::move(wpThis), std::move(printableObjectName)}};
    auto spCastRegistry =
        std::dynamic_pointer_cast<ObjectRegistryBase>(spRegistry);  // cast guaranteed to succeed

    KL_ASSERT(spCastRegistry, "Failed to cast registry");

    if (TOBJECT_D::IsCerealSerializable())
    {
        const auto lock = WriteLock{m_mutexRegistries};
        KL_ASSERT(m_serializableRegistries.insert(spCastRegistry).second, "Failed to add registry");
        m_serializableAssocTypeMap.emplace(typeid(TREGISTRY).name(), std::move(spCastRegistry));
    }

    else
    {
        KL_IF_DEBUG_WARN("Not serializing registry for (or associations involving) "
                         << spCastRegistry->PrintableBaseName());

        const auto lock = WriteLock{m_mutexRegistries};
        KL_ASSERT(m_unserializableRegistries.insert(spCastRegistry).second,
                  "Failed to add registry");
        m_unserializableAssocTypeMap.emplace(typeid(TREGISTRY).name(), std::move(spCastRegistry));
    }

    KL_IF_DEBUG_MESSAGE("Koala at " << KL_YELLOW << this << KL_NORMAL << " registered registry "
                                    << KL_WHITE_BOLD << spRegistry->PrintableBaseName());

    return *spRegistry;
}

//--------------------------------------------------------------------------------------------------

template <typename TALGORITHM, typename... TARGS>
void Koala::RunAlgorithm(const std::string &algorithmName, TARGS &&... arguments)
{
    ++m_callLevel;
    m_functionRecorder.PushDepthStack();

    auto &algorithm = this->FetchRegistry<TALGORITHM>().Get(
        algorithmName);  // will throw an error if alias does not exist

    try
    {
        if (this->DebugMode())
        {
            OutputUtility::PrintAlgorithm(*this, algorithmName);

            // Give the algorithm the chance to either return false or throw.
            auto functionId = m_functionRecorder.RecordFunctionStart(*this, algorithmName, true);

            if (!algorithm.Run(std::forward<TARGS>(arguments)...))
                KL_THROW("Algorithm " << KL_WHITE_BOLD << algorithmName << KL_NORMAL
                                      << " returned false");

            m_functionRecorder.RecordFunctionEnd(*this, functionId);
        }

        else
        {
            if (!algorithm.Run(std::forward<TARGS>(arguments)...))
                KL_THROW("Algorithm " << KL_WHITE_BOLD << algorithmName << KL_NORMAL
                                      << " returned false");
        }
    }

    catch (KoalaException &koalaException)
    {
        KL_CONCATENATE_EXCEPTION_THROW(
            koalaException, "Could not run algorithm " << KL_WHITE_BOLD << algorithmName);
    }

    catch (const std::runtime_error &runtimeError)
    {
        KL_THROW("Could not run algorithm "
                 << KL_WHITE_BOLD << algorithmName << KL_NORMAL
                 << " - threw non-koala exception: " << runtimeError.what());
    }

    catch (...)
    {
        KL_THROW("Could not run algorithm " << KL_WHITE_BOLD << algorithmName << KL_NORMAL
                                            << " (threw non-koala exception)");
    }

    KL_ASSERT(m_callLevel > SIZE_T(0UL), "Call level could not be decremented");

    --m_callLevel;
    m_functionRecorder.PopDepthStack();
}
}  // namespace kl

#endif  // #ifndef KL_KOALA_IMPL_H
