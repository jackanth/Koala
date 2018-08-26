/// @cond
/**
 * This file is subject to the terms and conditions defined in file 'LICENSE', which is part of this
 * source code package.
 */
/// @endcond

/**
 * @file koala/include/koala/Koala/Koala.h
 *
 * @brief Header file for the koala (Koala) class.
 */

#ifndef KL_KOALA_H
#define KL_KOALA_H 1

#include "koala/Definitions.h"
#include "koala/FunctionRecorder.h"
#include "koala/OutputStream.h"
#include "koala/ProgressBarManager.h"
#include "koala/Registry/ObjectAssociation.h"
#include "koala/Registry/ObjectRegistry.h"
#include "koala/Utilities/OutputUtility.h"

#include "koala/Utilities/FilesystemUtility.h"

#ifdef KOALA_ENABLE_CEREAL
#include "cereal/access.hpp"
#include "cereal/types/polymorphic.hpp"
#endif  // #ifdef KOALA_ENABLE_CEREAL

namespace kl
{
/**
 * @brief Forward declaration of the Algorithm class.
 */
class Algorithm;

/**
 * @brief Koala class.
 */
class Koala : public std::enable_shared_from_this<Koala>
{
private:
    using ObjectAssocTypeMap =
        std::unordered_multimap<std::string,
                                ObjectRegistryBase::sPtr>;  ///< Alias for object type-assoc map.

    mutable Mutex
        m_globalMutex;  ///< A gettable 'global' mutex for e.g. printing things to interface.

    mutable Mutex m_mutexSettings;    ///< A mutex for locking settings-related variables.
    mutable Mutex m_mutexRegistries;  ///< A mutex for locking registry-related variables.
    mutable Mutex m_mutexFnRecorder;  ///< A mutex for locking function-recorder-related variables.

    ObjectRegistryBase::sPtrSet m_serializableRegistries;    ///< The set of shared pointers to the
                                                             ///< serializable object registries.
    ObjectRegistryBase::sPtrSet m_unserializableRegistries;  ///< The set of shared pointers to the
                                                             ///< unserializable object registries.
    ObjectAssocTypeMap
        m_serializableAssocTypeMap;  ///< Map from object types to serializable object registries.
    ObjectAssocTypeMap m_unserializableAssocTypeMap;  ///< Map from object types to unserializable
                                                      ///< object registries.

    FunctionRecorder m_functionRecorder;           ///< The function recorder object.
    bool m_debugMode;                              ///< Whether to run in debug mode.
    mutable std::atomic<std::size_t> m_callLevel;  ///< Counter for calling sub-algorithms.
    ProgressBarManager<Koala>::sPtr m_spProgressBarManager;  ///< The progress bar manager.
    OutputStream::sPtr m_spStdoutStream;                      ///< The stdout stream.
    OutputStream::sPtr m_spStderrStream;                      ///< The stderr stream.
    std::shared_ptr<std::ofstream> m_spLogStream;             ///< The log stream.
    bool m_isInitialized;         ///< Whether this object is initialized.
    std::size_t m_terminalWidth;  ///< The terminal width.

    //----------------------------------------------------------------------------------------------

    /**
     * @brief Get this Koala object.
     *
     * @return This Koala object.
     */
    Koala &GetKoala() noexcept;

    /**
     * @brief Get this Koala object.
     *
     * @return This Koala object.
     */
    const Koala &GetKoala() const noexcept;

    /**
     * @brief Get an object registry (implementation).
     *
     * @return The object registry.
     */
    template <typename TOBJECT>
    auto &FetchRegistryImpl() const;

    /**
     * @brief Get whether there exists an object registry (implementation).
     *
     * @return Whether there exists an object registry.
     */
    template <typename TOBJECT>
    auto HasRegistryImpl() const noexcept;

    /**
     * @brief Work out the terminal width.
     *
     * @return The terminal width.
     */
    std::size_t GetTerminalWidth() const;

#ifdef KOALA_ENABLE_CEREAL
    /**
     * @brief Default constructor (for serialization purposes).
     */
    Koala() noexcept;

    /**
     * @brief Method template for serializing object.
     *
     * @param archive The cereal archive object.
     */
    template <typename TARCHIVE>
    void serialize(TARCHIVE &archive);

    /**
     * @brief Method template for loading and constructing serialized object.
     *
     * @param archive The cereal archive object.
     * @param construct The cereal construction object for this object.
     */
    template <typename TARCHIVE>
    static void load_and_construct(TARCHIVE &archive, cereal::construct<Koala> &construct);
#endif  // #ifdef KOALA_ENABLE_CEREAL

public:
    using sPtr = std::shared_ptr<Koala>;  ///< Alias for a shared pointer to a Koala object.
    using wPtr = std::weak_ptr<Koala>;    ///< Alias for a weak pointer to a Koala object.

    //----------------------------------------------------------------------------------------------

    /**
     * @brief Copy constructor.
     *
     * @param other The object from which to copy-construct.
     */
    Koala(const Koala &other) noexcept;

    /**
     * @brief Move constructor.
     *
     * @param other The object from which to move-construct.
     */
    Koala(Koala &&other) noexcept;

    /**
     * @brief Copy assignment operator.
     *
     * @param other The object from which to copy-assign.
     */
    Koala &operator=(const Koala &other) noexcept;

    /**
     * @brief Move assignment operator.
     *
     * @param other The object from which to move-assign.
     */
    Koala &operator=(Koala &&other) noexcept;

    /**
     * @brief Default destructor.
     */
    ~Koala() = default;

    //----------------------------------------------------------------------------------------------

    /**
     * @brief Get the global mutex.
     *
     * @return The global mutex.
     */
    KL_SIMPLE_GETTER_LVAL(, GetGlobalMutex, m_globalMutex, const);

    /**
     * @brief Get the progress bar manager.
     *
     * @return The progress bar manager.
     */
    ProgressBarManager<Koala> &GetProgressBarManager() const;

    /**
     * @brief Get a weak pointer to this object.
     *
     * @return A weak pointer to this object.
     */
    Koala::wPtr GetWeakPointer();

    /**
     * @brief Get an object registry.
     *
     * @return The object registry.
     */
    template <typename TOBJECT>
    auto &FetchRegistry() const;

    /**
     * @brief Get whether there exists an object registry.
     *
     * @return Whether there exists an object registry.
     */
    template <typename TOBJECT>
    auto HasRegistry() const noexcept;

    /**
     * @brief Register a registry for a given type.
     *
     * @param printableObjectName A print-worthy name for the object type.
     *
     * @return Success.
     */
    template <typename TOBJECT>
    auto RegisterRegistry(std::string printableObjectName) noexcept(
        std::is_nothrow_constructible<typename std::decay_t<TOBJECT>::Registry, Koala::wPtr &&,
                                      std::string &&>::value) ->
        typename std::decay_t<TOBJECT>::Registry &;

    /**
     * @brief Run an algorithm. The template parameter TALGORITHM can just be the Algorithm type.
     *
     * @param algorithmName The name of the algorithm.
     * @param arguments The arguments to be passed to the algorithm.
     */
    template <typename TALGORITHM, typename... TARGS>
    void RunAlgorithm(const std::string &algorithmName, TARGS &&... arguments);

    /**
     * @brief Get the debug mode boolean.
     *
     * @return The debug mode boolean.
     */
    KL_SIMPLE_GETTER_LOCK(, DebugMode, m_debugMode, const, ReadLock, m_mutexSettings);

    /**
     * @brief Set the debug mode boolean.
     *
     * @param debugMode The debug mode boolean.
     */
    KL_SIMPLE_SETTER_LOCK(DebugMode, const bool, debugMode, m_debugMode, WriteLock,
                          m_mutexSettings);

    /**
     * @brief Get the function recorder object.
     *
     * @return The function recorder object.
     */
    KL_SIMPLE_GETTER_LVAL_CONSTS_RVAL_LOCK(KoalaFunctionRecorder, m_functionRecorder, ReadLock,
                                           m_mutexFnRecorder);

    /**
     * @brief Get the stdout stream.
     *
     * @return The stdout stream.
     */
    OutputStream &GetStdout() const;

    /**
     * @brief Get the stderr stream.
     *
     * @return The stderr stream.
     */
    OutputStream &GetStderr() const;

    /**
     * @brief Get the terminal width.
     *
     * @return The terminal width.
     */
    std::size_t TerminalWidth() const;

    /**
     * @brief Set the terminal width.
     *
     * @param terminalWidth The terminal width.
     */
    void TerminalWidth(const std::size_t terminalWidth);

    /**
     * @brief Enable logging.
     *
     * @param logFilePath The log file path.
     */
    void EnableLogging(const kl::Path &logFilePath);

    /**
     * @brief Find out whether logging is enabled.
     *
     * @return Whether logging is enabled.
     */
    bool IsLoggingEnabled() const;

    /**
     * @brief Disable logging.
     */
    void DisableLogging();

    /**
     * @brief Adopt a registry from another Koala.
     */
    template <typename TOBJECT>
    void AdoptRegistry(Koala &koala);

    /**
     * @brief Delete a registry.
     */
    template <typename TOBJECT>
    void DeleteRegistry();

protected:
    /**
     * @brief Constructor.
     *
     * @param debugMode The debug mode boolean.
     */
    explicit Koala(const bool debugMode) noexcept;

    /**
     * @brief Initialize this object.
     */
    void Initialize();

    /**
     * @brief Get a shared pointer to this object.
     *
     * @return A shared pointer to this object.
     */
    Koala::sPtr GetSharedPointer();

    /**
     * @brief Get the call level.
     *
     * @return The call level.
     */
    KL_SIMPLE_GETTER(, CallLevel, m_callLevel.load(), const);

    /**
     * @brief Get the serializable association type map.
     *
     * @return The serializable association type map.
     */
    KL_SIMPLE_GETTER_LVAL_RVAL(const, SerializableAssocTypeMap, m_serializableAssocTypeMap, const);

    /**
     * @brief Get the serializable registries.
     *
     * @return The serializable registries.
     */
    KL_SIMPLE_GETTER_LVAL_RVAL(const, SerializableRegistries, m_serializableRegistries, const);

    /**
     * @brief Get the unserializable association type map.
     *
     * @return The unserializable association type map.
     */
    KL_SIMPLE_GETTER_LVAL_RVAL(const, UnserializableAssocTypeMap, m_unserializableAssocTypeMap,
                               const);

    /**
     * @brief Get the unserializable registries.
     *
     * @return The unserializable registries.
     */
    KL_SIMPLE_GETTER_LVAL_RVAL(const, UnserializableRegistries, m_unserializableRegistries, const);

    /**
     * @brief Get the is-initialized boolean.
     *
     * @return The is-initialized boolean.
     */
    KL_SIMPLE_GETTER(, IsInitialized, m_isInitialized, const);

    friend class KoalaApi;
    friend class OutputUtility;

#ifdef KOALA_ENABLE_CEREAL
    friend class cereal::access;
#endif  // #ifdef KOALA_ENABLE_CEREAL
};

//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------

inline Koala &Koala::GetKoala() noexcept { return *this; }

//--------------------------------------------------------------------------------------------------

inline const Koala &Koala::GetKoala() const noexcept { return *this; }

//--------------------------------------------------------------------------------------------------

inline std::size_t Koala::GetTerminalWidth() const
{
    struct winsize wins;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &wins);
    return SIZE_T(wins.ws_col);
}

//--------------------------------------------------------------------------------------------------

#ifdef KOALA_ENABLE_CEREAL
inline Koala::Koala() noexcept
    : std::enable_shared_from_this<Koala>{},
      m_globalMutex{},
      m_mutexSettings{},
      m_mutexRegistries{},
      m_mutexFnRecorder{},
      m_serializableRegistries{},
      m_unserializableRegistries{},
      m_serializableAssocTypeMap{},
      m_unserializableAssocTypeMap{},
      m_functionRecorder{},
      m_debugMode{false},
      m_callLevel{SIZE_T(0UL)},
      m_spProgressBarManager{nullptr},
      m_spStdoutStream{nullptr},
      m_spStderrStream{nullptr},
      m_spLogStream{nullptr},
      m_isInitialized{false},
      m_terminalWidth{SIZE_T(0UL)}
{
}
#endif  // #ifdef KOALA_ENABLE_CEREAL

//--------------------------------------------------------------------------------------------------

inline Koala::Koala(const Koala &other) noexcept
    : m_globalMutex{},
      m_mutexSettings{},
      m_mutexRegistries{},
      m_mutexFnRecorder{},
      m_serializableRegistries{},
      m_unserializableRegistries{},
      m_serializableAssocTypeMap{},
      m_unserializableAssocTypeMap{},
      m_functionRecorder{},
      m_debugMode{false},
      m_callLevel{SIZE_T(0UL)},
      m_spProgressBarManager{nullptr},
      m_spStdoutStream{nullptr},
      m_spStderrStream{nullptr},
      m_spLogStream{nullptr},
      m_isInitialized{false},
      m_terminalWidth{SIZE_T(0UL)}
{
    const auto thisLockSettings = WriteLock{m_mutexSettings};
    const auto thisLockRegistries = WriteLock{m_mutexRegistries};
    const auto thisLockFnRecorder = WriteLock{m_mutexFnRecorder};

    const auto otherLockSettings = ReadLock{other.m_mutexSettings};
    const auto otherLockRegistries = ReadLock{other.m_mutexRegistries};
    const auto otherLockFnRecorder = ReadLock{other.m_mutexFnRecorder};

    m_serializableRegistries = other.m_serializableRegistries;
    m_unserializableRegistries = other.m_unserializableRegistries;
    m_functionRecorder = other.m_functionRecorder;
    m_debugMode = other.m_debugMode;
    m_callLevel.store(other.m_callLevel.load());
    m_spProgressBarManager = other.m_spProgressBarManager;
    m_spStdoutStream = other.m_spStdoutStream;
    m_spStderrStream = other.m_spStderrStream;
    m_spLogStream = other.m_spLogStream;
    m_isInitialized = other.m_isInitialized;
    m_terminalWidth = other.m_terminalWidth;
}

//--------------------------------------------------------------------------------------------------

inline Koala::Koala(Koala &&other) noexcept
    : m_globalMutex{},
      m_mutexSettings{},
      m_mutexRegistries{},
      m_mutexFnRecorder{},
      m_serializableRegistries{},
      m_unserializableRegistries{},
      m_serializableAssocTypeMap{},
      m_unserializableAssocTypeMap{},
      m_functionRecorder{},
      m_debugMode{false},
      m_callLevel{SIZE_T(0UL)},
      m_spProgressBarManager{nullptr},
      m_spStdoutStream{nullptr},
      m_spStderrStream{nullptr},
      m_spLogStream{nullptr},
      m_isInitialized{false},
      m_terminalWidth{SIZE_T(0UL)}
{
    const auto thisLockSettings = WriteLock{m_mutexSettings};
    const auto thisLockRegistries = WriteLock{m_mutexRegistries};
    const auto thisLockFnRecorder = WriteLock{m_mutexFnRecorder};

    const auto otherLockSettings = WriteLock{other.m_mutexSettings};
    const auto otherLockRegistries = WriteLock{other.m_mutexRegistries};
    const auto otherLockFnRecorder = WriteLock{other.m_mutexFnRecorder};

    m_serializableRegistries = std::move_if_noexcept(other.m_serializableRegistries);
    m_unserializableRegistries = std::move_if_noexcept(other.m_unserializableRegistries);
    m_functionRecorder = std::move_if_noexcept(other.m_functionRecorder);
    m_debugMode = other.m_debugMode;
    m_callLevel.store(other.m_callLevel.load());
    m_spProgressBarManager = std::move_if_noexcept(other.m_spProgressBarManager);
    m_spStdoutStream = std::move_if_noexcept(other.m_spStdoutStream);
    m_spStderrStream = std::move_if_noexcept(other.m_spStderrStream);
    m_spLogStream = std::move_if_noexcept(other.m_spLogStream);
    m_isInitialized = other.m_isInitialized;
    m_terminalWidth = other.m_terminalWidth;
}

//--------------------------------------------------------------------------------------------------

inline Koala &Koala::operator=(const Koala &other) noexcept
{
    if (this != &other)
    {
        auto thisLockSettings = WriteLock{m_mutexSettings, std::defer_lock};
        auto thisLockRegistries = WriteLock{m_mutexRegistries, std::defer_lock};
        auto thisLockFnRecorder = WriteLock{m_mutexFnRecorder, std::defer_lock};

        auto otherLockSettings = ReadLock{other.m_mutexSettings, std::defer_lock};
        auto otherLockRegistries = ReadLock{other.m_mutexRegistries, std::defer_lock};
        auto otherLockFnRecorder = ReadLock{other.m_mutexFnRecorder, std::defer_lock};

        Lock<>::DoLock(thisLockSettings, thisLockRegistries, thisLockFnRecorder, otherLockSettings,
                       otherLockRegistries, otherLockFnRecorder);

        auto tmp = Koala{other};
        *this = std::move_if_noexcept(tmp);
    }

    return *this;
}

//--------------------------------------------------------------------------------------------------

inline Koala &Koala::operator=(Koala &&other) noexcept
{
    if (this != &other)
    {
        auto thisLockSettings = WriteLock{m_mutexSettings, std::defer_lock};
        auto thisLockRegistries = WriteLock{m_mutexRegistries, std::defer_lock};
        auto thisLockFnRecorder = WriteLock{m_mutexFnRecorder, std::defer_lock};

        auto otherLockSettings = WriteLock{other.m_mutexSettings, std::defer_lock};
        auto otherLockRegistries = WriteLock{other.m_mutexRegistries, std::defer_lock};
        auto otherLockFnRecorder = WriteLock{other.m_mutexFnRecorder, std::defer_lock};

        Lock<>::DoLock(thisLockSettings, thisLockRegistries, thisLockFnRecorder, otherLockSettings,
                       otherLockRegistries, otherLockFnRecorder);

        m_serializableRegistries = std::move_if_noexcept(other.m_serializableRegistries);
        m_unserializableRegistries = std::move_if_noexcept(other.m_unserializableRegistries);
        m_functionRecorder = std::move_if_noexcept(other.m_functionRecorder);
        m_debugMode = other.m_debugMode;
        m_callLevel.store(other.m_callLevel.load());
        m_spProgressBarManager = std::move_if_noexcept(other.m_spProgressBarManager);
        m_spProgressBarManager = std::move_if_noexcept(other.m_spProgressBarManager);
        m_spStdoutStream = std::move_if_noexcept(other.m_spStdoutStream);
        m_spStderrStream = std::move_if_noexcept(other.m_spStderrStream);
        m_spLogStream = std::move_if_noexcept(other.m_spLogStream);
        m_terminalWidth = other.m_terminalWidth;
    }

    return *this;
}

//--------------------------------------------------------------------------------------------------

inline ProgressBarManager<Koala> &Koala::GetProgressBarManager() const
{
    if (!m_isInitialized) KL_THROW("Cannot get progress bar manager before initialization");

    if (m_spProgressBarManager) return *m_spProgressBarManager;

    KL_THROW("Failed to get progress bar manager");
}

//--------------------------------------------------------------------------------------------------

inline Koala::wPtr Koala::GetWeakPointer() { return Koala::wPtr{this->GetSharedPointer()}; }

//--------------------------------------------------------------------------------------------------

inline OutputStream &Koala::GetStdout() const
{
    if (m_isInitialized)
    {
        if (m_spStdoutStream) return *m_spStdoutStream;

        KL_THROW("Failed to get stdout stream");
    }

    KL_THROW("Cannot get stdout stream before object is initialized");
}

//--------------------------------------------------------------------------------------------------

inline OutputStream &Koala::GetStderr() const
{
    if (m_isInitialized)
    {
        if (m_spStderrStream) return *m_spStderrStream;

        KL_THROW("Failed to get stderr stream");
    }

    KL_THROW("Cannot get stderr stream before object is initialized");
}

//--------------------------------------------------------------------------------------------------

inline std::size_t Koala::TerminalWidth() const
{
    const auto lock = kl::ReadLock{m_mutexSettings};

    if (m_isInitialized) return m_terminalWidth;

    KL_THROW("Cannot get terminal width before object is initialized");
}

//--------------------------------------------------------------------------------------------------

inline void Koala::TerminalWidth(const std::size_t terminalWidth)
{
    const auto lock = kl::WriteLock{m_mutexSettings};

    if (!m_isInitialized) KL_THROW("Cannot set terminal width until object is initialized");

    m_terminalWidth = terminalWidth;
}

//--------------------------------------------------------------------------------------------------

inline void Koala::EnableLogging(const kl::Path &logFilePath)
{
    if (!m_isInitialized) KL_THROW("Must initialize Koala object before enabling logging");

    if (m_spLogStream) KL_THROW("Logging was already enabled");

    m_spLogStream = std::shared_ptr<std::ofstream>{
        new std::ofstream{logFilePath, std::ofstream::out | std::ofstream::app}};

    // Initialize the streams using the progress bar manager callback.
    m_spStdoutStream = OutputStream::sPtr{
        new OutputStream{std::cout, *m_spLogStream, [&](std::streambuf *const pStreamBuf) {
                             m_spProgressBarManager->StreamNewlineCallback(pStreamBuf);
                         }}};

    m_spStderrStream = OutputStream::sPtr{
        new OutputStream{std::cerr, *m_spLogStream, [&](std::streambuf *const pStreamBuf) {
                             m_spProgressBarManager->StreamNewlineCallback(pStreamBuf);
                         }}};
}

//--------------------------------------------------------------------------------------------------

inline bool Koala::IsLoggingEnabled() const { return static_cast<bool>(m_spLogStream); }

//--------------------------------------------------------------------------------------------------

inline void Koala::DisableLogging()
{
    if (!m_isInitialized) KL_THROW("Must initialize Koala object before disabling logging");

    if (!m_spLogStream) KL_THROW("Logging was not enabled");

    m_spLogStream = nullptr;

    // Initialize the streams using the progress bar manager callback.
    m_spStdoutStream = OutputStream::sPtr{
        new OutputStream{std::cout, [&](std::streambuf *const pStreamBuf) {
                             m_spProgressBarManager->StreamNewlineCallback(pStreamBuf);
                         }}};

    m_spStderrStream = OutputStream::sPtr{
        new OutputStream{std::cerr, [&](std::streambuf *const pStreamBuf) {
                             m_spProgressBarManager->StreamNewlineCallback(pStreamBuf);
                         }}};
}

//--------------------------------------------------------------------------------------------------

template <typename TOBJECT>
void Koala::AdoptRegistry(Koala &koala)
{
    using TOBJECT_D = std::decay_t<TOBJECT>;

    if (this->HasRegistry<TOBJECT_D>())
        KL_THROW("Could not adopt registry because this koala already had one of the same type");

    if (!koala.HasRegistry<TOBJECT_D>())
        KL_THROW("Could not adopt registry because other koala did not have it");

    const auto spRegistry = koala.FetchRegistry<TOBJECT_D>().GetSharedPointer();

    for (auto &object : spRegistry->GetAll()) object.KoalaWeakPointer(this->GetWeakPointer());

    spRegistry->KoalaWeakPointer(this->GetWeakPointer());

    if (TOBJECT_D::IsCerealSerializable())
    {
        m_serializableRegistries.insert(spRegistry);
        m_serializableAssocTypeMap.emplace(typeid(typename TOBJECT_D::Registry).name(), spRegistry);
    }

    else
    {
        m_unserializableRegistries.insert(spRegistry);
        m_unserializableAssocTypeMap.emplace(typeid(typename TOBJECT_D::Registry).name(),
                                             spRegistry);
    }

    koala.DeleteRegistry<TOBJECT_D>();
    KL_IF_DEBUG_MESSAGE("Koala at " << KL_YELLOW << this << KL_NORMAL << " adopted registry "
                                     << KL_WHITE_BOLD << spRegistry->PrintableBaseName()
                                     << KL_NORMAL << " from Koala at " << KL_YELLOW << &koala);
}

//--------------------------------------------------------------------------------------------------

template <typename TOBJECT>
void Koala::DeleteRegistry()
{
    using TOBJECT_D = std::decay_t<TOBJECT>;

    if (!this->HasRegistry<TOBJECT_D>())
        KL_THROW("Could not delete registry because it did not exist");

    const auto spRegistry = this->FetchRegistry<TOBJECT_D>().GetSharedPointer();

    if (TOBJECT_D::IsCerealSerializable())
    {
        const auto findIter = m_serializableRegistries.find(spRegistry);
        KL_ASSERT(findIter != m_serializableRegistries.end(),
                  "Failed to find object registry that did exist");
        m_serializableRegistries.erase(findIter);

        auto foundRegistry = false;

        for (auto iter = m_serializableAssocTypeMap.begin();
             iter != m_serializableAssocTypeMap.end(); ++iter)
        {
            if (iter->second == spRegistry)
            {
                m_serializableAssocTypeMap.erase(iter);
                foundRegistry = true;
                break;
            }
        }

        KL_ASSERT(foundRegistry, "Failed to find existing registry in type map");
    }

    else
    {
        const auto findIter = m_unserializableRegistries.find(spRegistry);
        KL_ASSERT(findIter != m_unserializableRegistries.end(),
                  "Failed to find object registry that did exist");
        m_unserializableRegistries.erase(findIter);

        auto foundRegistry = false;

        for (auto iter = m_unserializableAssocTypeMap.begin();
             iter != m_unserializableAssocTypeMap.end(); ++iter)
        {
            if (iter->second == spRegistry)
            {
                m_unserializableAssocTypeMap.erase(iter);
                foundRegistry = true;
                break;
            }
        }

        KL_ASSERT(foundRegistry, "Failed to find existing registry in type map");
    }

    KL_IF_DEBUG_MESSAGE("Deleted registry " << KL_WHITE_BOLD << spRegistry->PrintableBaseName()
                                            << KL_NORMAL << " from Koala at " << KL_YELLOW
                                            << this);
}

//--------------------------------------------------------------------------------------------------

inline Koala::Koala(const bool debugMode) noexcept
    : m_globalMutex{},
      m_mutexSettings{},
      m_mutexRegistries{},
      m_mutexFnRecorder{},
      m_serializableRegistries{},
      m_unserializableRegistries{},
      m_serializableAssocTypeMap{},
      m_unserializableAssocTypeMap{},
      m_functionRecorder{},
      m_debugMode{debugMode},
      m_callLevel{SIZE_T(0UL)},
      m_spProgressBarManager{nullptr},
      m_spStdoutStream{nullptr},
      m_spStderrStream{nullptr},
      m_isInitialized{false},
      m_terminalWidth{SIZE_T(0UL)}
{
    m_callLevel.store(SIZE_T(0UL));
}

//--------------------------------------------------------------------------------------------------

inline void Koala::Initialize()
{
    if (m_isInitialized) KL_THROW("Koala object is already initialized");

    m_spProgressBarManager =
        ProgressBarManager<Koala>::sPtr{new ProgressBarManager<Koala>{this->GetWeakPointer()}};

    // Initialize the streams using the progress bar manager callback.
    m_spStdoutStream = OutputStream::sPtr{
        new OutputStream{std::cout, [&](std::streambuf *const pStreamBuf) {
                             m_spProgressBarManager->StreamNewlineCallback(pStreamBuf);
                         }}};

    m_spStderrStream = OutputStream::sPtr{
        new OutputStream{std::cerr, [&](std::streambuf *const pStreamBuf) {
                             m_spProgressBarManager->StreamNewlineCallback(pStreamBuf);
                         }}};

    m_terminalWidth = this->GetTerminalWidth();
    m_isInitialized = true;
}

//--------------------------------------------------------------------------------------------------

inline Koala::sPtr Koala::GetSharedPointer() { return this->shared_from_this(); }
}  // namespace kl

//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------

#include "koala/Koala/Koala.txx"

#endif  // #ifndef KL_KOALA_H
