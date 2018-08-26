/// @cond
/**
 * This file is subject to the terms and conditions defined in file 'LICENSE', which is part of this
 * source code package.
 */
/// @endcond

/**
 * @file koala/include/koala/Koala/KoalaApi.h
 *
 * @brief Header file for the koala API (KoalaApi) class.
 */

#ifndef KL_API_H
#define KL_API_H 1

#include "koala/Algorithm.h"
#include "koala/Definitions.h"
#include "koala/Koala/Koala.h"
#include "koala/OutputStream.h"
#include "koala/ProgressBarManager.h"

#ifdef KOALA_ENABLE_CEREAL
#include "cereal/access.hpp"
#endif  // #ifdef KOALA_ENABLE_CEREAL

namespace kl
{
/**
 * @brief KoalaApi class.
 */
class KoalaApi
{
public:
    using sPtr = std::shared_ptr<KoalaApi>;  ///< Alias for shared pointer to this object.

    /**
     * @brief Constructor.
     *
     * @param debugMode The debug mode boolean.
     */
    explicit KoalaApi(bool debugMode);

    /**
     * @brief Default copy constructor.
     */
    KoalaApi(const KoalaApi &) = default;

    /**
     * @brief Default move constructor.
     */
    KoalaApi(KoalaApi &&) = default;

    /**
     * @brief Default copy assignment operator.
     */
    KoalaApi &operator=(const KoalaApi &) = default;

    /**
     * @brief Default move assignment operator.
     */
    KoalaApi &operator=(KoalaApi &&) = default;

    /**
     * @brief Destructor.
     */
    ~KoalaApi();

    //----------------------------------------------------------------------------------------------

    /**
     * @brief Get the Koala instance.
     *
     * @return The Koala instance.
     */
    KL_SIMPLE_GETTER_LVAL_RVAL(, GetKoala, *m_spKoala, const);

    /**
     * @brief Get the debug mode boolean.
     *
     * @return The debug mode boolean.
     */
    KL_SIMPLE_GETTER(, DebugMode, m_spKoala->DebugMode(), const);

    /**
     * @brief Set the debug mode boolean.
     *
     * @param debugMode The debug mode boolean.
     */
    void DebugMode(bool debugMode) const;

    /**
     * @brief Get the progress bar manager.
     *
     * @return The progress bar manager.
     */
    ProgressBarManager<Koala> &GetProgressBarManager() const noexcept;

    /**
     * @brief Get the stdout stream.
     *
     * @return The stdout stream.
     */
    OutputStream &GetStdout() const;

    /**
     * @brief Get the stderr stream.
     *
     * @return The stder stream.
     */
    OutputStream &GetStderr() const;

    /**
     * @brief Enable logging.
     *
     * @param logFilePath The log file path.
     */
    void EnableLogging(const kl::Path &logFilePath) const;

    /**
     * @brief Disable logging.
     */
    void DisableLogging() const;

    /**
     * @brief Register a registry for a given type.
     *
     * @param printableObjectName A print-worthy name for the object type.
     *
     * @return Success.
     */
    template <typename TOBJECT>
    auto RegisterRegistry(std::string printableObjectName) const & ->
        typename std::decay_t<TOBJECT>::Registry &;

    /**
     * @brief Get an object registry.
     *
     * @return The object registry.
     */
    template <typename TOBJECT>
    auto &FetchRegistry() const;

    /**
     * @brief Create an object of a given type.
     *
     * @param arguments The arguments to be passed to the object's constructor.
     *
     * @return The created object.
     */
    template <typename TOBJECT, typename... TARGS>
    auto &Create(TARGS &&... arguments) const;

    /**
     * @brief Create an object of a given type by given alias.
     *
     * @param alias The alias by which to create the new object.
     * @param arguments The arguments to be passed to the object's constructor.
     *
     * @return The created object.
     */
    template <typename TOBJECT, typename TALIAS, typename... TARGS>
    auto &CreateByAlias(TALIAS &&alias, TARGS &&... arguments) const;

    /**
     * @brief Get an object by alias, ID or a copy of the object.
     *
     * @param arg The argument.
     *
     * @return The object.
     */
    template <typename TOBJECT, typename T>
    auto &Get(T &&arg) const;

    /**
     * @brief Get all the objects of a given type as a list of reference wrappers.
     *
     * @return The objects.
     */
    template <typename TOBJECT>
    auto GetAllList() const noexcept;

    /**
     * @brief Get all the objects of a given type.
     *
     * @return The objects.
     */
    template <typename TOBJECT>
    auto GetAll() const noexcept;

    /**
     * @brief Get the number of objects of a given type.
     *
     * @return The number of objects of a given type.
     */
    template <typename TOBJECT>
    auto Count() const noexcept;

    /**
     * @brief Delete an object by alias, ID or a copy of the object.
     *
     * @param arg The argument.
     *
     * @return Success.
     */
    template <typename TOBJECT, typename T>
    auto Delete(T &&arg) const noexcept;

    /**
     * @brief Delete all the objects of a given type.
     */
    template <typename TOBJECT>
    void DeleteAll() const noexcept;

    /**
     * @brief Get an object's alias by alias, ID or a copy of the object.
     *
     * @param arg The argument.
     *
     * @return The alias.
     */
    template <typename TOBJECT, typename T>
    auto GetAlias(T &&arg) const;

    /**
     * @brief Find out whether an object exists by alias, ID or a copy of the object.
     *
     * @param arg The argument.
     *
     * @return Whether the object exists.
     */
    template <typename TOBJECT, typename T>
    auto DoesObjectExist(T &&arg) const noexcept;

    /**
     * @brief Create an algorithm.
     *
     * @param algorithmName The name of the algorithm.
     * @param arguments The constructor arguments of the algorithm.
     *
     * @return A reference to the new algorithm object.
     */
    template <typename TALG, typename... TARGS>
    auto &CreateAlgorithm(std::string algorithmName, TARGS &&... arguments) const;

    /**
     * @brief Run an algorithm.
     *
     * @param algorithmName The name of the algorithm.
     */
    void RunAlgorithm(const std::string &algorithmName) const;

    /**
     * @brief Delete an algorithm.
     *
     * @param algorithmName The name of the algorithm.
     */
    void DeleteAlgorithm(const std::string &algorithmName) const;

    /**
     * @brief Run and delete an algorithm.
     *
     * @param algorithmName The name of the algorithm.
     */
    void RunAndDeleteAlgorithm(const std::string &algorithmName) const;

    /**
     * @brief Delete all the registered algorithms.
     */
    void DeleteAllRegisteredAlgorithms() const noexcept;

    /**
     * @brief Create and run an algorithm.
     *
     * @param algorithmName The name of the algorithm.
     * @param arguments The constructor arguments of the algorithm.
     *
     * @return A reference to the new algorithm object.
     */
    template <typename TALG, typename... TARGS>
    auto &CreateAndRunAlgorithm(const std::string &algorithmName, TARGS &&... arguments) const;

    /**
     * @brief Create, run and delete an algorithm.
     *
     * @param algorithmName The name of the algorithm.
     * @param arguments The constructor arguments of the algorithm.
     */
    template <typename TALG, typename... TARGS>
    void CreateRunAndDeleteAlgorithm(const std::string &algorithmName, TARGS &&... arguments) const;

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

    /**
     * @brief Reset the function recorder clock.
     */
    void ResetFunctionRecorderClock() const noexcept;

    /**
     * @brief Adopt a registry from another koala API.
     *
     * @param koalaApi The other koala API.
     */
    template <typename TOBJECT>
    void AdoptRegistry(const KoalaApi &koalaApi) const;

    /**
     * @brief Delete a registry.
     */
    template <typename TOBJECT>
    void DeleteRegistry() const;

#ifdef KOALA_ENABLE_CEREAL
    friend class cereal::access;
#endif  // #ifdef KOALA_ENABLE_CEREAL

private:
    Koala::sPtr m_spKoala;  ///< The instance of Koala to which this API corresponds.

#ifdef KOALA_ENABLE_CEREAL
    /**
     * @brief Method template for loading object.
     *
     * @param archive The cereal archive object.
     */
    template <typename TARCHIVE>
    void load(TARCHIVE &archive);

    /**
     * @brief Method template for saving object.
     *
     * @param archive The cereal archive object.
     */
    template <typename TARCHIVE>
    void save(TARCHIVE &archive) const;

    /**
     * @brief Default constructor (for serialization purposes).
     */
    KoalaApi() noexcept;
#endif  // #ifdef KOALA_ENABLE_CEREAL
};

//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------

inline KoalaApi::KoalaApi(bool debugMode) : m_spKoala{Koala::sPtr{new Koala{debugMode}}}
{
    try
    {
        m_spKoala->Initialize();
        this->RegisterRegistry<Algorithm>(std::string{"Algorithm"});
    }

    catch (...)
    {
        KL_ASSERT(false, "Failed to initialize koala instance");
    }
}

//--------------------------------------------------------------------------------------------------

inline KoalaApi::~KoalaApi()
{
    if (m_spKoala->IsInitialized()) m_spKoala->GetProgressBarManager().DisplayProgressBars(false);
}

//--------------------------------------------------------------------------------------------------

inline void KoalaApi::DebugMode(bool debugMode) const { m_spKoala->DebugMode(debugMode); }

//--------------------------------------------------------------------------------------------------

inline ProgressBarManager<Koala> &KoalaApi::GetProgressBarManager() const noexcept
{
    return m_spKoala->GetProgressBarManager();
}

//--------------------------------------------------------------------------------------------------

inline OutputStream &KoalaApi::GetStdout() const { return m_spKoala->GetStdout(); }

//--------------------------------------------------------------------------------------------------

inline OutputStream &KoalaApi::GetStderr() const { return m_spKoala->GetStderr(); }

//--------------------------------------------------------------------------------------------------

inline void KoalaApi::EnableLogging(const kl::Path &logFilePath) const
{
    m_spKoala->EnableLogging(logFilePath);
}

//--------------------------------------------------------------------------------------------------

inline void KoalaApi::DisableLogging() const { m_spKoala->DisableLogging(); }

//--------------------------------------------------------------------------------------------------

inline void KoalaApi::RunAlgorithm(const std::string &algorithmName) const
{
    m_spKoala->RunAlgorithm<Algorithm>(algorithmName);
}

//--------------------------------------------------------------------------------------------------

inline void KoalaApi::DeleteAlgorithm(const std::string &algorithmName) const
{
    m_spKoala->FetchRegistry<Algorithm>().Delete(algorithmName);
}

//--------------------------------------------------------------------------------------------------

inline void KoalaApi::RunAndDeleteAlgorithm(const std::string &algorithmName) const
{
    this->RunAlgorithm(algorithmName);
    this->DeleteAlgorithm(algorithmName);
}

//--------------------------------------------------------------------------------------------------

inline void KoalaApi::DeleteAllRegisteredAlgorithms() const noexcept
{
    m_spKoala->FetchRegistry<Algorithm>().DeleteAll();
}

//--------------------------------------------------------------------------------------------------

inline void KoalaApi::ResetFunctionRecorderClock() const noexcept
{
    m_spKoala->KoalaFunctionRecorder().ResetClockState();
}

//--------------------------------------------------------------------------------------------------

template <typename TOBJECT>
inline void KoalaApi::AdoptRegistry(const KoalaApi &koalaApi) const
{
    m_spKoala->AdoptRegistry<std::decay_t<TOBJECT>>(koalaApi.GetKoala());
}

//--------------------------------------------------------------------------------------------------

template <typename TOBJECT>
inline void KoalaApi::DeleteRegistry() const
{
    m_spKoala->DeleteRegistry<std::decay_t<TOBJECT>>();
}

//--------------------------------------------------------------------------------------------------

#ifdef KOALA_ENABLE_CEREAL
inline KoalaApi::KoalaApi() noexcept : m_spKoala{} {}
#endif  // #ifdef KOALA_ENABLE_CEREAL
}  // namespace kl

//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------

#include "koala/Koala/KoalaApi.txx"

#endif  // #ifndef KL_API_H