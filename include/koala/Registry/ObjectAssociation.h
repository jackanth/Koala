/// @cond
/**
 * This file is subject to the terms and conditions defined in file 'LICENSE', which is part of this
 * source code package.
 */
/// @endcond

/**
 * @file koala/include/koala/Registry/ObjectAssociation.h
 *
 * @brief Header file for the object association (ObjectAssociation) class template, the object
 * association base (ObjectAssociationBase) abstract class and the association information
 * (AssociationInformation) class.
 */

#ifndef KL_OBJECT_ASSOCIATION_H
#define KL_OBJECT_ASSOCIATION_H 1

#ifdef KOALA_ENABLE_CEREAL
#include "cereal/access.hpp"
#include "cereal/types/polymorphic.hpp"
#endif  // #ifdef KOALA_ENABLE_CEREAL

namespace kl
{
/**
 * @brief Forward declaration of RegisteredObjectTemplate abstract class template.
 */
template <typename TBASE, typename TALIAS>
class RegisteredObjectTemplate;

/**
 * @brief Forward declaration of RangeBasedAssociationContainer class template.
 */
template <typename TOBJECT, typename TINDICATOR>
class RangeBasedAssociationContainer;

/**
 * @brief Forward declaration of the AssociationInformation class.
 */
class AssociationInformation;

/**
 * @brief ObjectAssociationBase abstract class.
 */
class ObjectAssociationBase
{
public:
    /**
     * @brief Default copy constructor.
     */
    ObjectAssociationBase(const ObjectAssociationBase &) = default;

    /**
     * @brief Default move constructor.
     */
    ObjectAssociationBase(ObjectAssociationBase &&) = default;

    /**
     * @brief Default copy assignment operator.
     */
    ObjectAssociationBase &operator=(const ObjectAssociationBase &) = default;

    /**
     * @brief Default move assignment operator.
     */
    ObjectAssociationBase &operator=(ObjectAssociationBase &&) = default;

    /**
     * @brief Default virtual destructor.
     */
    virtual ~ObjectAssociationBase() = default;

    //----------------------------------------------------------------------------------------------

    /**
     * @brief Get the associated object's ID.
     *
     * @return The associated object's ID.
     */
    KL_SIMPLE_GETTER(, ID, m_id, const);

    /**
     * @brief Get the is-cereal-serializable boolean.
     *
     * @return The is-cereal-serializable boolean.
     */
    KL_SIMPLE_GETTER(, IsCerealSerializable, m_isCerealSerializable, const);

    /**
     * @brief Get the object's printable type name.
     *
     * @return The printable type name.
     */
    virtual auto GetPrintableTypeName() const -> std::string = 0;

    /**
     * @brief Get the object's registry's name.
     *
     * @return The object's registry's name.
     */
    virtual auto GetRegistryName() const -> std::string = 0;

    /**
     * @brief Get the object's identifier string.
     *
     * @return The object's identifier string.
     */
    virtual auto GetIdentifierString() const -> std::string = 0;

    /**
     * @brief Check if the associated object is still alive.
     *
     * @return Whether the associated object is still alive.
     */
    virtual auto IsAlive() const noexcept -> bool = 0;

    /**
     * @brief Get whether this association has an indicator.
     *
     * @return Whether this association has an indicator.
     */
    virtual auto HasIndicator() const noexcept -> bool = 0;

    /**
     * @brief Get the object's indicator string.
     *
     * @return The object's indicator string.
     */
    virtual auto GetIndicatorString() const -> std::string = 0;

protected:
    using sPtr = std::shared_ptr<ObjectAssociationBase>;  ///< Alias for a shared pointer.
    using sPtrSet = std::unordered_set<sPtr>;  ///< Alias for an unordered set of shared pointers.

    /**
     * @brief Constructor.
     *
     * @param id The unique ID of the associated object.
     * @param isCerealSerializable Whether this association is cereal serializable.
     */
    ObjectAssociationBase(const kl::ID_t id, const bool isCerealSerializable);

    /**
     * @brief Default constructor (for serialization purposes).
     */
    ObjectAssociationBase();

    friend class AssociationInformation;

    template <typename TA, typename TB>
    friend class RangeBasedAssociationContainer;

    template <typename TA, typename TB>
    friend class RegisteredObjectTemplate;

#ifdef KOALA_ENABLE_CEREAL
    friend class cereal::access;
#endif  // #ifdef KOALA_ENABLE_CEREAL

private:
    kl::ID_t m_id;                ///< The unique ID of the associated object.
    bool m_isCerealSerializable;  ///< Whether this association is cereal-serializable.

#ifdef KOALA_ENABLE_CEREAL
    /**
     * @brief Method template for serializing object.
     *
     * @param archive The cereal archive object.
     */
    template <typename TARCHIVE>
    void serialize(TARCHIVE &archive);
#endif  // #ifdef KOALA_ENABLE_CEREAL
};

//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------

/**
 * @brief ObjectAssociation class template.
 */
template <typename TOBJECT, typename TINDICATOR = std::string>
class ObjectAssociation : public ObjectAssociationBase
{
public:
    /**
     * @brief Default copy constructor.
     */
    ObjectAssociation(const ObjectAssociation &) = default;

    /**
     * @brief Default move constructor.
     */
    ObjectAssociation(ObjectAssociation &&) = default;

    /**
     * @brief Default copy assignment operator.
     */
    ObjectAssociation &operator=(const ObjectAssociation &) = default;

    /**
     * @brief Default move assignment operator.
     */
    ObjectAssociation &operator=(ObjectAssociation &&) = default;

    /**
     * @brief Default destructor.
     */
    ~ObjectAssociation() = default;

    //----------------------------------------------------------------------------------------------

    /**
     * @brief Get the associated object.
     *
     * @return The associated object.
     */
    auto &Get() const;

    /**
     * @brief Get the object's printable type name.
     *
     * @return The printable type name.
     */
    auto GetPrintableTypeName() const -> std::string override;

    /**
     * @brief Get the object's registry's name.
     *
     * @return The object's registry's name.
     */
    auto GetRegistryName() const -> std::string override;

    /**
     * @brief Try to get a string representation of the object's alias.
     *
     * @return The object's identifier string.
     */
    auto GetIdentifierString() const -> std::string override;

    /**
     * @brief Check if the associated object is still alive.
     *
     * @return Whether the associated object is still alive.
     */
    auto IsAlive() const noexcept -> bool override;

    /**
     * @brief Get whether this association has an indicator.
     *
     * @return Whether this association has an indicator.
     */
    auto HasIndicator() const noexcept -> bool override;

    /**
     * @brief Try to get a string representation of the object's indicator.
     *
     * @return The object's indicator string.
     */
    auto GetIndicatorString() const -> std::string override;

    /**
     * @brief Get the indicator flag.
     *
     * @return The indicator flag.
     */
    KL_SIMPLE_GETTER_LVAL_RVAL(const, Indicator, m_indicator, const);

protected:
    using sPtr =
        std::shared_ptr<ObjectAssociation>;  ///< Alias for a shared pointer to this association.

    /**
     * @brief Constructor.
     *
     * @param thisObject The originating object.
     * @param assocObject The object of association.
     * @param reciprocate Whether to reciprocate the association.
     */
    template <typename TTHIS>
    ObjectAssociation(TTHIS &&thisObject, TOBJECT &&assocObject, const bool reciprocate);

    /**
     * @brief Constructor.
     *
     * @param thisObject The originating object.
     * @param assocObject The object of association.
     * @param reciprocate Whether to reciprocate the association.
     * @param indicator An indicator flag for the association to differentiate between types of
     * association.
     */
    template <typename TTHIS>
    ObjectAssociation(TTHIS &&thisObject, TOBJECT &&assocObject, const bool reciprocate,
                      TINDICATOR &&indicator);

    template <typename TB, typename TA>
    friend class RegisteredObjectTemplate;

#ifdef KOALA_ENABLE_CEREAL
    friend class cereal::access;
#endif  // #ifdef KOALA_ENABLE_CEREAL

private:
    using TOBJECT_D = std::decay_t<TOBJECT>;        ///< Alias for decayed TOBJECT.
    using TINDICATOR_D = std::decay_t<TINDICATOR>;  ///< Alias for decayed TOBJECT.
    using TOBJECT_D_wPtr =
        std::weak_ptr<TOBJECT_D>;  ///< Alias for a weak pointer to the associated object.

    TOBJECT_D_wPtr m_wpObject;  ///< Weak pointer to the associated object.
    TINDICATOR_D m_indicator;   ///< The optional association indicator.
    bool m_hasIndicator;        ///< Whether this association has an indicator.

    //----------------------------------------------------------------------------------------------

    /**
     * @brief Try to get a string representation of the object's indicator (implementation).
     *
     * @return The object's indicator string.
     */
    auto GetIndicatorStringImpl(std::true_type, std::true_type) const -> std::string;

    /**
     * @brief Try to get a string representation of the object's indicator (implementation).
     *
     * @return The object's indicator string.
     */
    auto GetIndicatorStringImpl(std::false_type, std::true_type) const -> std::string;

    /**
     * @brief Try to get a string representation of the object's indicator (implementation).
     *
     * @return The object's indicator string.
     */
    auto GetIndicatorStringImpl(std::true_type, std::false_type) const noexcept -> std::string;

    /**
     * @brief Try to get a string representation of the object's indicator (implementation).
     *
     * @return The object's indicator string.
     */
    auto GetIndicatorStringImpl(std::false_type, std::false_type) const noexcept -> std::string;

#ifdef KOALA_ENABLE_CEREAL
    /**
     * @brief Method template for serializing object.
     *
     * @param archive The cereal archive object.
     */
    template <typename TARCHIVE>
    void serialize(TARCHIVE &archive);

    /**
     * @brief Default constructor (for serialization purposes).
     */
    ObjectAssociation() noexcept;
#endif  // #ifdef KOALA_ENABLE_CEREAL
};

//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------

/**
 * @brief AssociationInformation class.
 */
class AssociationInformation
{
public:
    using Vector = std::vector<AssociationInformation>;  ///< Alias for a vector of
                                                         ///< AssociationInformation objects.

    //----------------------------------------------------------------------------------------------

    /**
     * @brief Default copy constructor.
     */
    AssociationInformation(const AssociationInformation &) = default;

    /**
     * @brief Default move constructor.
     */
    AssociationInformation(AssociationInformation &&) = default;

    /**
     * @brief Default copy assignment operator.
     */
    AssociationInformation &operator=(const AssociationInformation &) = default;

    /**
     * @brief Default move assignment operator.
     */
    AssociationInformation &operator=(AssociationInformation &&) = default;

    /**
     * @brief Default destructor.
     */
    ~AssociationInformation() = default;

    //----------------------------------------------------------------------------------------------

    /**
     * @brief Get the associated object's ID.
     *
     * @return The associated object's ID.
     */
    KL_SIMPLE_GETTER(, ID, m_id, const);

    /**
     * @brief Get whether the associated object is alive.
     *
     * @return Whether the associated object is alive.
     */
    KL_SIMPLE_GETTER(, IsAlive, m_isAlive, const);

    /**
     * @brief Get whether the association is cereal-serializable.
     *
     * @return Whether the association is cereal-serializable.
     */
    KL_SIMPLE_GETTER(, IsCerealSerializable, m_isCerealSerializable, const);

    /**
     * @brief Get the associated object's identifier string.
     *
     * @return The associated object's identifier string.
     */
    KL_SIMPLE_GETTER_LVAL_RVAL(const, IdentifierString, m_identifierString, const);

    /**
     * @brief Get the associated object's type name.
     *
     * @return The associated object's type name.
     */
    KL_SIMPLE_GETTER_LVAL_RVAL(const, TypeName, m_typeName, const);

    /**
     * @brief Get the associated object's registry name.
     *
     * @return The associated object's registry name.
     */
    KL_SIMPLE_GETTER_LVAL_RVAL(const, RegistryName, m_registryName, const);

    /**
     * @brief Get whether the association has an indicator.
     *
     * @return Whether the association has an indicator.
     */
    KL_SIMPLE_GETTER(, HasIndicator, m_hasIndicator, const);

    /**
     * @brief Get the associated object's indicator string.
     *
     * @return The associated object's indicator string.
     */
    KL_SIMPLE_GETTER_LVAL_RVAL(const, IndicatorString, m_indicatorString, const);

protected:
    /**
     * @brief Constructor.
     *
     * @param spAssociationBase Shared pointer to the association base.
     */
    explicit AssociationInformation(const ObjectAssociationBase::sPtr &spAssociationBase);

    template <typename TA, typename TB>
    friend class RegisteredObjectTemplate;

private:
    kl::ID_t m_id;                ///< The ID of the associated object.
    bool m_isAlive;               ///< Whether the association is alive.
    bool m_isCerealSerializable;  ///< Whether the association is cereal-serializable.
    bool m_hasIndicator;          ///< Whether the association has an indicator.

    std::string m_identifierString;  ///< The associated object's identifier string.
    std::string m_typeName;          ///< The name of the associated object's type.
    std::string m_registryName;      ///< The name of the associated object's registry.
    std::string m_indicatorString;   ///< The indicator string if it can be created.

    //----------------------------------------------------------------------------------------------

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
    AssociationInformation();
#endif  // #ifdef KOALA_ENABLE_CEREAL
};

//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------

inline ObjectAssociationBase::ObjectAssociationBase(const kl::ID_t id,
                                                    const bool isCerealSerializable)
    : m_id{id}, m_isCerealSerializable{isCerealSerializable}
{
}

//--------------------------------------------------------------------------------------------------

inline ObjectAssociationBase::ObjectAssociationBase()
    : m_id{SIZE_T(0UL)}, m_isCerealSerializable{false}
{
}

//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------

inline AssociationInformation::AssociationInformation(
    const ObjectAssociationBase::sPtr &spAssociationBase)
    : m_id{spAssociationBase->ID()},
      m_isAlive{spAssociationBase->IsAlive()},
      m_isCerealSerializable{m_isAlive ? spAssociationBase->IsCerealSerializable() : false},
      m_hasIndicator{m_isAlive ? spAssociationBase->HasIndicator() : false},
      m_identifierString{m_isAlive ? spAssociationBase->GetIdentifierString() : std::string{}},
      m_typeName{m_isAlive ? spAssociationBase->GetPrintableTypeName() : std::string{}},
      m_registryName{m_isAlive ? spAssociationBase->GetRegistryName() : std::string{}},
      m_indicatorString{m_isAlive ? spAssociationBase->GetIndicatorString() : std::string{}}
{
}

//--------------------------------------------------------------------------------------------------

#ifdef KOALA_ENABLE_CEREAL
inline AssociationInformation::AssociationInformation()
    : m_id{SIZE_T(0UL)},
      m_isAlive{false},
      m_isCerealSerializable{false},
      m_hasIndicator{false},
      m_identifierString{},
      m_typeName{},
      m_registryName{},
      m_indicatorString{}
{
}
#endif  // #ifdef KOALA_ENABLE_CEREAL
}  // namespace kl

//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------

#include "koala/Registry/ObjectAssociation.txx"

#endif  // #ifndef KL_OBJECT_ASSOCIATION_H