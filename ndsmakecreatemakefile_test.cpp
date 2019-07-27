#include "ndsmakecreatemakefile.h"
#include "qtcreatorprofile.h"

#include <boost/test/unit_test.hpp>

using namespace ribi::ndsm;

BOOST_AUTO_TEST_CASE(CreateMakefile_creates_a_makefile)
{
  ribi::QtCreatorProFile pro_file(
    "libnds_example_1.pro"
  );
  CreateMakefile(pro_file);

}

