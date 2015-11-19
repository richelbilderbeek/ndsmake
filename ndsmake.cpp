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
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#include "ndsmake.h"

#include <fstream>
#include <sstream>

#include "fileio.h"
#include "qtcreatorprofile.h"
#include "ndsmakecreateholymakefile.h"
#include "ndsmakecreatemakefile.h"
#pragma GCC diagnostic pop

ribi::ndsm::Ndsmake::Ndsmake(
  const std::string& argv0,
  const std::string& pro_file_name
)
  : m_argv0{argv0},
    m_command{},
    m_pro_file(pro_file_name)
{
  #ifndef NDEBUG
  Test();
  #endif
  if (!fileio::FileIo().IsRegularFile(pro_file_name))
  {
    std::stringstream msg;
    msg << __func__ << ": file '" << pro_file_name << "' not found";
    throw std::logic_error(msg.str());
  }
  m_command = CreateCommand();
}

std::string ribi::ndsm::Ndsmake::CreateCommand() const
{
  //Copy ProFile files to the needed folders
  const std::string build_dir_full{
    //m_pro_file.GetBuildDirFull()
    fileio::FileIo().GetPath(m_pro_file.GetQtCreatorProFilename())
  };
  const std::string build_dir{
    build_dir_full
  };
  const std::string cur_dir_full{
    fileio::FileIo().GetPath(m_argv0)
    //m_pro_file.GetCurDirFull()
  };
  std::string s
    = "cd .. ;"
    + std::string("mkdir ") + build_dir + "; "
    + "cd " + build_dir + "; "
    + "mkdir source; ";
  //Start copying all files
  {
    for(const std::string& f: m_pro_file.GetHeaders())
    {
      //const std::string f = m_pro_file.GetHeaders()[i];
      const std::string from = cur_dir_full + std::string("/") + f;
      const std::string to = build_dir_full + std::string("/source/") + f;
      if (!fileio::FileIo().IsRegularFile(from)) {
        std::stringstream msg;
        msg << __func__ << ": cannot find file '" << from << "' to copy from";
        throw std::logic_error(msg.str());
      }
      s += std::string("cp ")
        + from
        + std::string(" ")
        + to
        + std::string("; ");
    }
  }
  {
    const std::size_t sz = m_pro_file.GetSources().size();
    for (std::size_t i = 0; i!=sz; ++i)
    for(const std::string& f:m_pro_file.GetSources())
    {
      //const std::string f = m_pro_file.GetSources()[i];
      const std::string from = cur_dir_full + std::string("/") + f;
      const std::string to = build_dir_full + std::string("/source/") + f;
      assert(fileio::FileIo().IsRegularFile(from));

      s += std::string("cp ")
        + from
        + std::string(" ")
        + to
        + std::string("; ");
    }
  }
  return s;
}

void ribi::ndsm::Ndsmake::CreateMakefile() const noexcept
{
  ::ribi::ndsm::CreateMakefile(m_pro_file);
}

std::vector<std::string> ribi::ndsm::Ndsmake::GetHelp() noexcept
{
  std::vector<std::string> v;
  v.push_back("Usage: ndsmake [options] [target]");
  v.push_back("");
  v.push_back("[target] must be the full path and filename of your Qt Creator project");
  v.push_back("for example: 'ndsmake /myqtfolder/myqtproject.pro'");
  v.push_back("");
  v.push_back("Options:");
  v.push_back("  --help: prints this message and exits");
  v.push_back("  --history: prints this ndsmake version history and exits");
  v.push_back("  --licence: prints the ndsmake licence and exits");
  v.push_back("  --version: print the current version of ndsmake and exits");
  return v;
}

std::string ribi::ndsm::Ndsmake::GetVersion() noexcept
{
  return "2.0";
}

std::vector<std::string> ribi::ndsm::Ndsmake::GetVersionHistory() noexcept
{
  std::vector<std::string> v = {
    "2010-10-10: version 0.1: incomplete, could not be run from Qt Creator",
    "2010-10-13: version 1.0: initial release, works from Qt Creator, tested by TestNdsmake",
    "2010-12-19: version 1.1: added extra commands, use of ProFile class",
    "2010-12-19: version 1.2: use of Ndsmake class",
    "2015-11-19: version 2.0: moved to own GitHub"
  };
  return v;
}

std::vector<std::string> ribi::ndsm::Ndsmake::GetLicence() noexcept
{
  std::vector<std::string> v;
  v.push_back("ndsmake, tool to generate NDS makefile from Qt Creator project file");
  v.push_back("Copyright (C) 2010-2015 Richel Bilderbeek");
  v.push_back("");
  v.push_back("This program is free software: you can redistribute it and/or modify");
  v.push_back("it under the terms of the GNU General Public License as published by");
  v.push_back("the Free Software Foundation, either version 3 of the License, or");
  v.push_back("(at your option) any later version.");
  v.push_back("");
  v.push_back("This program is distributed in the hope that it will be useful,");
  v.push_back("but WITHOUT ANY WARRANTY; without even the implied warranty of");
  v.push_back("MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the");
  v.push_back("GNU General Public License for more details.");
  v.push_back("You should have received a copy of the GNU General Public License");
  v.push_back("along with this program.If not, see <http://www.gnu.org/licenses/>.");
  return v;
}

std::string ribi::ndsm::Ndsmake::GetTarget() const noexcept
{
  return m_pro_file.GetTarget().empty()
    ? RemoveExtension( this->m_pro_file.GetQtCreatorProFilename() )
    : (*m_pro_file.GetTarget().begin());
}

std::string ribi::ndsm::Ndsmake::RemoveExtension(const std::string& filename)
{
  const int dot_index = filename.rfind(".",filename.size());
  assert(dot_index != -1 && "No dot found in filename");
  return filename.substr(0,dot_index);
}

#ifndef NDEBUG
void ribi::ndsm::Ndsmake::Test() noexcept
{
  ///Test exactly once
  {
    static bool is_tested{false};
    if (is_tested) return;
    is_tested = true;
  }
  {
    Ndsmake a("/home/p230198/GitHubs/ndsmake/ndsmake.pro","../ndsmake/ndsmake.pro");
    Ndsmake b(a);
  }
}
#endif
