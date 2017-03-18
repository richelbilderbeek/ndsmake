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
};

} //~namespace ndsm
} //~namespace ribi

#endif // NDSMAKE_H
