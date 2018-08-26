/// @cond
/**
 * This file is subject to the terms and conditions defined in file 'LICENSE', which is part of this
 * source code package.
 */
/// @endcond

/**
 * @file koala/test/test.cxx
 *
 * @brief The koala test script.
 */

#include "koala/Koala/KoalaApi.h"
#include "koala/Utilities/ExecUtility.h"

#include "TestAlgorithm.h"
#include "TestObject.h"

int main()
{
    const auto koalaApi = kl::KoalaApi{true};
    koalaApi.GetKoala().EnableLogging(kl::Path{"koala.log"});
    auto &progressBarManager = koalaApi.GetProgressBarManager();
    progressBarManager.CreateProgressBar("Test bar", "Test bar");

    koalaApi.RegisterRegistry<TestObject>("TestObject");
    koalaApi.Create<TestObject>();
    koalaApi.CreateRunAndDeleteAlgorithm<kl::TestAlgorithm>("TestAlgorithm");

    koalaApi.GetKoala().GetStdout() << "Stdout test" << std::endl;
    koalaApi.GetKoala().GetStderr() << "Stderr test" << std::endl;

    return 0;
}