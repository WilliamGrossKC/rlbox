// Check that the tainted type is correctly reflected in rlbox_sandbox when:
// - the sandbox plugin leaves the tainted type as default
// - the sandbox plugin overrides the tainted type

#include "test_include.hpp"

#include "rlbox_tainted_fixed_aligned.hpp"
#include "rlbox_tainted_relocatable.hpp"

#include <type_traits>

namespace rlbox {

class rlbox_default_tainted_testsandbox
{};

class rlbox_custom_tainted_testsandbox
{
public:
  template<typename T>
  using tainted = tainted_fixed_aligned<T, rlbox_custom_tainted_testsandbox>;
};

class rlbox_custom_tainted_volatile_testsandbox
{
public:
  template<typename T>
  using tainted_volatile =
    rlbox::tainted_fixed_aligned<T, rlbox_custom_tainted_volatile_testsandbox>;
};
}

using namespace rlbox;

RLBOX_DEFINE_BASE_TYPES_FOR(libtest_default_tainted,
                            rlbox_default_tainted_testsandbox);
RLBOX_DEFINE_BASE_TYPES_FOR(libtest_custom_tainted,
                            rlbox_custom_tainted_testsandbox);
RLBOX_DEFINE_BASE_TYPES_FOR(libtest_custom_tainted_volatile,
                            rlbox_custom_tainted_volatile_testsandbox);

TEST_CASE("Test tainted reflection", "[rlbox plugin reflection]")
{
  // Check that by default, sandboxes assume that the plugin uses
  // tainted_relocatable and tainted_volatile_standard.
  REQUIRE(std::is_same_v<
          rlbox_sandbox_libtest_default_tainted::tainted<int>,
          tainted_relocatable<int, rlbox_default_tainted_testsandbox>>);
  REQUIRE(std::is_same_v<
          rlbox_sandbox_libtest_default_tainted::tainted_volatile<int>,
          tainted_volatile_standard<int, rlbox_default_tainted_testsandbox>>);

  // Check that plugin override of tainted uses the specified value
  REQUIRE(std::is_same_v<
          rlbox_sandbox_libtest_custom_tainted::tainted<int>,
          tainted_fixed_aligned<int, rlbox_custom_tainted_testsandbox>>);
  REQUIRE(std::is_same_v<
          rlbox_sandbox_libtest_custom_tainted::tainted_volatile<int>,
          tainted_volatile_standard<int, rlbox_custom_tainted_testsandbox>>);

  // Check that plugin override of tainted_volatile uses the specified value
  REQUIRE(std::is_same_v<
          rlbox_sandbox_libtest_custom_tainted_volatile::tainted<int>,
          tainted_relocatable<int, rlbox_custom_tainted_volatile_testsandbox>>);

  rlbox_test_helper_print_type<
    rlbox_sandbox_libtest_custom_tainted_volatile::tainted_volatile<int>,
    tainted_fixed_aligned<int, rlbox_custom_tainted_volatile_testsandbox>>();

  REQUIRE(
    std::is_same_v<
      rlbox_sandbox_libtest_custom_tainted_volatile::tainted_volatile<int>,
      tainted_fixed_aligned<int, rlbox_custom_tainted_volatile_testsandbox>>);
}