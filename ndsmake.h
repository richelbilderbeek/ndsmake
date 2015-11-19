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
#ifndef NDSMAKE_H
#define NDSMAKE_H

#include <string>
#include <vector>

#include "qtcreatorprofile.h"

namespace ribi {

struct QtCreatorProFile;

namespace ndsm {

struct Ndsmake
{
  ///Call ndsmake on a .pro file
  Ndsmake(
    const std::string& argv0,
    const std::string& pro_file_name
  );

  void CreateMakefile() const noexcept;

  const std::string& GetCommand() const noexcept { return m_command; }
  static std::vector<std::string> GetHelp() noexcept;
  static std::vector<std::string> GetLicence() noexcept;

  ///Obtain the TARGET of the possibly multiple QtCreatorProFile::m_targets
  std::string GetTarget() const noexcept;

  static std::string GetVersion() noexcept;
  static std::vector<std::string> GetVersionHistory() noexcept;

  private:

  std::string m_argv0; //argv[0]
  std::string m_command; //A terminal command
  QtCreatorProFile m_pro_file;

  std::string CreateCommand() const;

  //From http://www.richelbilderbeek.nl/CppRemoveExtension.htm
  static std::string RemoveExtension(const std::string& filename);

  #ifndef NDEBUG
  static void Test() noexcept;
  #endif
};

} //~namespace ndsm
} //~namespace ribi

#endif // NDSMAKE_H
