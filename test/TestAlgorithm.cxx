/// @cond
/**
 * This file is subject to the terms and conditions defined in file 'LICENSE', which is part of this
 * source code package.
 */
/// @endcond

/**
 * @file koala/test/TestAlgorithm.cxx
 *
 * @brief Implementation of the test algorithm (TestAlgorithm) class.
 */

#include "TestAlgorithm.h"
#include "TestObject.h"
#include "TestEdge.h"
#include "koala/Utilities/HierarchicalVisualizationUtility.h"

namespace kl
{
TestAlgorithm::TestAlgorithm(Registry_wPtr wpRegistry, const kl::ID_t id,
                             Koala_wPtr wpKoala) noexcept
    : Algorithm{std::move_if_noexcept(wpRegistry), id, std::move_if_noexcept(wpKoala)}
{
}

//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------

bool TestAlgorithm::Run()
{
    // Define members.
    auto &brother = KL_CREATE_BY_ALIAS(TestObject, "Brother");
    auto &sister = KL_CREATE_BY_ALIAS(TestObject, "Sister");
    auto &mother = KL_CREATE_BY_ALIAS(TestObject, "Mother");
    auto &father = KL_CREATE_BY_ALIAS(TestObject, "Father");

    auto &maternalGrandfather = KL_CREATE_BY_ALIAS(TestObject, "Maternal\nGrandfather");
    auto &maternalGrandmother = KL_CREATE_BY_ALIAS(TestObject, "Maternal\nGrandmother");
    auto &paternalGrandfather = KL_CREATE_BY_ALIAS(TestObject, "Paternal\nGrandfather");
    auto &paternalGrandmother = KL_CREATE_BY_ALIAS(TestObject, "Paternal\nGrandmother");

    auto &uncle = KL_CREATE_BY_ALIAS(TestObject, "Uncle");
    auto &aunt = KL_CREATE_BY_ALIAS(TestObject, "Aunt");
    auto &cousin = KL_CREATE_BY_ALIAS(TestObject, "Cousin");

    // Add daughter links.
    mother.AddDaughterEdge(brother);
    father.AddDaughterEdge(brother);
    mother.AddDaughterEdge(sister);
    father.AddDaughterEdge(sister);

    paternalGrandfather.AddDaughterEdge(father);
    paternalGrandmother.AddDaughterEdge(father);
    paternalGrandfather.AddDaughterEdge<TestEdge>(uncle);
    paternalGrandmother.AddDaughterEdge<TestEdge>(uncle);
    maternalGrandfather.AddDaughterEdge(mother);
    maternalGrandmother.AddDaughterEdge(mother);

    uncle.AddDaughterEdge<TestEdge>(cousin);
    aunt.AddDaughterEdge<TestEdge>(cousin);

    // Add containers.
    auto &family = KL_CREATE_BY_ALIAS(TestObject, "Family");
    family.SubsumeSet(TestObject::UnorderedRefSet{
        mother, father, brother, sister, maternalGrandfather, maternalGrandmother,
        paternalGrandfather, paternalGrandmother, uncle, aunt, cousin});

    auto &grandparents = KL_CREATE_BY_ALIAS(TestObject, "Grandparents");
    grandparents.SubsumeSet(TestObject::UnorderedRefSet{maternalGrandfather, maternalGrandmother,
                                                        paternalGrandfather, paternalGrandmother});

    auto &parents = KL_CREATE_BY_ALIAS(TestObject, "Parents");
    parents.SubsumeSet(TestObject::UnorderedRefSet{mother, father});

    auto &siblings = KL_CREATE_BY_ALIAS(TestObject, "Siblings");
    siblings.SubsumeSet(TestObject::UnorderedRefSet{brother, sister});

    // Visualize.
    HierarchicalVisualizationOptions options;
    options.m_displayPseudoEdges = false;
    options.m_graphTitle = "Koala graph";
    options.m_saveSvgToFile = true;
    options.m_svgFilePath = "test.svg";
    HierarchicalVisualizationUtility<TestObject>::Visualize(this->GetKoala(), family, options);
    
    return true;
}
}  // namespace kl