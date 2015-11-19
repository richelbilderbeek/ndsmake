#include "ndsmakecreatemakefile.h"

#include <fstream>
#include "qtcreatorprofile.h"

void ribi::ndsm::CreateMakefile(const QtCreatorProFile& pro_file) noexcept
{
  std::ofstream f("Makefile");
  f
  //Add header
  << "# ndsmake version 0.2\n"
  << "# by Richel Bilderbeek\n"
  << "# www.richelbilderbeek.nl\n"
  << "# Arguments: " << pro_file.GetQtCreatorProFilename() << "\n"
  << "#---------------------------------------------------------------------------------\n"
  << ".SUFFIXES:\n"
  << "#---------------------------------------------------------------------------------\n"
  //<< "DEVKITARM:=/opt/devkitpro/devkitARM\n" //Bilderbikkel addition
  //<< "DEVKITPRO:=/opt/devkitpro\n" //Bilderbikkel addition
  << "\n"
  << "ifeq ($(strip $(DEVKITARM)),)\n"
  << "$(error \"Please set DEVKITARM in your environment. export DEVKITARM=<path to>devkitARM\")\n"
  << "endif\n"
  << "\n"
  << "include $(DEVKITARM)/ds_rules\n"
  << "\n"
  << "#---------------------------------------------------------------------------------\n"
  << "# TARGET is the name of the output\n"
  << "# BUILD is the directory where object files & intermediate files will be placed\n"
  << "# SOURCES is a list of directories containing source code\n"
  << "# INCLUDES is a list of directories containing extra header files\n"
  << "#---------------------------------------------------------------------------------\n"
  << "TARGET := " << (*pro_file.GetTarget().begin()) << "\n" //Bilderbikkel addition
  << "BUILD		:=	build\n"
  << "SOURCES		:=	gfx source data  \n"
  << "INCLUDES	:=	include build /usr/include /opt/devkitpro/libnds-1.4.7/include\n"
  << "\n"
  << "#---------------------------------------------------------------------------------\n"
  << "# options for code generation\n"
  << "#---------------------------------------------------------------------------------\n"
  << "ARCH	:=	-mthumb -mthumb-interwork\n"
  << "\n"
  << "CFLAGS	:=	-g -Wall -O2\\\n"
  << " 			-march=armv5te -mtune=arm946e-s -fomit-frame-pointer\\\n"
  << "			-ffast-math \\\n"
  << "			$(ARCH)\n"
  << "\n"
  << "CFLAGS	+=	$(INCLUDE) -DARM9\n"
  << "CXXFLAGS	:= $(CFLAGS) -fno-rtti -fno-exceptions\n"
  << "\n"
  << "ASFLAGS	:=	-g $(ARCH)\n"
  << "LDFLAGS	=	-specs=ds_arm9.specs -g $(ARCH) -mno-fpu -Wl,-Map,$(notdir $*.map)\n"
  << "\n"
  << "#---------------------------------------------------------------------------------\n"
  << "# any extra libraries we wish to link with the project\n"
  << "#---------------------------------------------------------------------------------\n"
  << "LIBS	:= -lnds9\n"
  << " \n"
  << " \n"
  << "#---------------------------------------------------------------------------------\n"
  << "# list of directories containing libraries, this must be the top level containing\n"
  << "# include and lib\n"
  << "#---------------------------------------------------------------------------------\n"
  << "LIBDIRS	:=	$(LIBNDS)\n"
  << " \n"
  << "#---------------------------------------------------------------------------------\n"
  << "# no real need to edit anything past this point unless you need to add additional\n"
  << "# rules for different file extensions\n"
  << "#---------------------------------------------------------------------------------\n"
  << "ifneq ($(BUILD),$(notdir $(CURDIR)))\n"
  << "#---------------------------------------------------------------------------------\n"
  << " \n"
  << "export OUTPUT	:=	$(CURDIR)/$(TARGET)\n"
  << " \n"
  << "export VPATH	:=	$(foreach dir,$(SOURCES),$(CURDIR)/$(dir))\n"
  << "export DEPSDIR	:=	$(CURDIR)/$(BUILD)\n"
  << "\n"
  << "CFILES		:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.c)))\n"
  << "CPPFILES	:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.cpp)))\n"
  << "SFILES		:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.s)))\n"
  << "BINFILES	:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.bin)))\n"
  << " \n"
  << "#---------------------------------------------------------------------------------\n"
  << "# use CXX for linking C++ projects, CC for standard C\n"
  << "#---------------------------------------------------------------------------------\n"
  << "ifeq ($(strip $(CPPFILES)),)\n"
  << "#---------------------------------------------------------------------------------\n"
  << "	export LD	:=	$(CC)\n"
  << "#---------------------------------------------------------------------------------\n"
  << "else\n"
  << "#---------------------------------------------------------------------------------\n"
  << "	export LD	:=	$(CXX)\n"
  << "#---------------------------------------------------------------------------------\n"
  << "endif\n"
  << "#---------------------------------------------------------------------------------\n"
  << "\n"
  << "export OFILES	:=	$(BINFILES:.bin=.o) \\\n"
  << "					$(CPPFILES:.cpp=.o) $(CFILES:.c=.o) $(SFILES:.s=.o)\n"
  << " \n"
  << "export INCLUDE	:=	$(foreach dir,$(INCLUDES),-I$(CURDIR)/$(dir)) \\\n"
  << "					$(foreach dir,$(LIBDIRS),-I$(dir)/include) \\\n"
  << "					$(foreach dir,$(LIBDIRS),-I$(dir)/include) \\\n"
  << "					-I$(CURDIR)/$(BUILD)\n"
  << " \n"
  << "export LIBPATHS	:=	$(foreach dir,$(LIBDIRS),-L$(dir)/lib)\n"
  << " \n"
  << ".PHONY: $(BUILD) clean\n"
  << " \n"
  << "#---------------------------------------------------------------------------------\n"
  << "$(BUILD):\n"
  << "	@[ -d $@ ] || mkdir -p $@\n"
  << "	@make --no-print-directory -C $(BUILD) -f $(CURDIR)/Makefile\n"
  << " \n"
  << "#---------------------------------------------------------------------------------\n"
  << "clean:\n"
  << "	@echo clean ...\n"
  << "	@rm -fr $(BUILD) $(TARGET).elf $(TARGET).nds $(TARGET).ds.gba \n"
  << " \n"
  << " \n"
  << "#---------------------------------------------------------------------------------\n"
  << "else\n"
  << " \n"
  << "DEPENDS	:=	$(OFILES:.o=.d)\n"
  << " \n"
  << "#---------------------------------------------------------------------------------\n"
  << "# main targets\n"
  << "#---------------------------------------------------------------------------------\n"
  << "$(OUTPUT).nds	: 	$(OUTPUT).elf\n"
  << "$(OUTPUT).elf	:	$(OFILES)\n"
  << " \n"
  << "#---------------------------------------------------------------------------------\n"
  << "%.o	:	%.bin\n"
  << "#---------------------------------------------------------------------------------\n"
  << "	@echo $(notdir $<)\n"
  << "	$(bin2o)\n"
  << " \n"
  << " \n"
  << "-include $(DEPENDS)\n"
  << " \n"
  << "#---------------------------------------------------------------------------------------\n"
  << "endif\n"
  << "#---------------------------------------------------------------------------------------\n";
}
