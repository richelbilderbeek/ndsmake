#ifndef CREATEMAKEFILE_H
#define CREATEMAKEFILE_H

namespace ribi {

struct QtCreatorProFile;

namespace ndsm {

void CreateMakefile(const QtCreatorProFile& pro_file) noexcept;

} //~namespace ndsm
} //~namespace ribi

#endif // CREATEMAKEFILE_H
