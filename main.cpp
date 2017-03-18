#include <algorithm>
#include <cassert>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <iterator>
#include <string>
#include <sstream>
#include <vector>

#include "ndsmakecreateholymakefile.h"
#include "ndsmakecreatemakefile.h"
#include "ndsmake.h"
#include "qtcreatorprofile.h"

int main(int argc, char* argv[])
{
  using ribi::ndsm::Ndsmake;

  if ( argc!= 2
    || std::string(argv[1]) == "-help"
    || std::string(argv[1]) == "--help")
  {
    const std::vector<std::string>& v = Ndsmake::GetHelp();
    std::copy(v.begin(),v.end(),std::ostream_iterator<std::string>(std::cout,"\n"));
    return 0;
  }
  if (std::string(argv[1]) == "--history")
  {
    const std::vector<std::string>& v = Ndsmake::GetVersionHistory();
    std::copy(v.begin(),v.end(),std::ostream_iterator<std::string>(std::cout,"\n"));
    return 0;
  }
  if (std::string(argv[1]) == "--licence")
  {
    const std::vector<std::string>& v = Ndsmake::GetLicence();
    std::copy(v.begin(),v.end(),std::ostream_iterator<std::string>(std::cout,"\n"));
    return 0;
  }
  if (std::string(argv[1]) == "--version")
  {
    std::cout << Ndsmake::GetVersion() << '\n';
    return 0;
  }
  //Start ndsmake
  const Ndsmake n(argv[0],argv[1]);
  //Copy source files
  {
    std::clog << "Ndsmake cmd: " << n.GetCommand() << '\n';
    const int error = std::system(n.GetCommand().c_str());
    if (error) {
      std::stringstream msg;
      msg << __func__ << ": command '" << n.GetCommand() << "' failed";
      throw std::logic_error(msg.str());
    }
  }
  //Create makefile and make it
  {
    n.CreateMakefile();
    const std::string s =
      "export DEVKITARM=/opt/devkitpro/devkitARM; "
      "export DEVKITPRO=/opt/devkitpro; "
      "make;";
    const int error = std::system(s.c_str());
    if (error) {
      std::stringstream msg;
      msg << __func__ << ": command '" << n.GetCommand() << "' failed";
      throw std::logic_error(msg.str());
    }
  }
  //Run the NDS file
  {
    const int error = std::system(
      (std::string("desmume ")
      + n.GetTarget()
      + std::string(".nds")).c_str()
    );
    if (error) {
      std::stringstream msg;
      msg << __func__ << ": command '" << n.GetCommand() << "' failed";
      throw std::logic_error(msg.str());
    }
  }
}
