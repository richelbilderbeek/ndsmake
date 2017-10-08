#include "ndsmake.h"

#include <boost/test/unit_test.hpp>
#include <QDir>

BOOST_AUTO_TEST_CASE(ndsmake_needs_valid_input_file)
{
  const std::string argv0 = "./";
  const std::string pro_file_name = "abs.ent";
  BOOST_CHECK_THROW(
    ribi::ndsm::Ndsmake(argv0, pro_file_name),
    std::invalid_argument);
}

#ifdef FIX_ISSUE
BOOST_AUTO_TEST_CASE(parse_ndsmake_pro)
{

  const std::string argv0 = QDir::currentPath().toStdString();
  const std::string pro_file_name = "/home/richel/GitHubs/ndsmake/ndsmake.pro";
  BOOST_CHECK_NO_THROW(
    ribi::ndsm::Ndsmake(argv0, pro_file_name)
  );
}
#endif // FIX_ISSUE

