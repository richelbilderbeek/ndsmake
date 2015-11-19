//---------------------------------------------------------------------------
/*
ndsmake, tool to generate NDS makefile from Qt Creator project file
Copyright (C) 2010-2015 Richel Bilderbeek

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
GNU General Public License for more details.
You should have received a copy of the GNU General Public License
along with this program.If not, see <http://www.gnu.org/licenses/>.
*/
//---------------------------------------------------------------------------
// From http://www.richelbilderbeek.nl/ToolNdsmake.htm
//---------------------------------------------------------------------------
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
  #ifndef NDEBUG
  Ndsmake(argv[0],"../ndsmake/ndsmake.pro");
  #endif

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
