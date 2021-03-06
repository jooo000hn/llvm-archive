# APPLE LOCAL file build machinery
# Apple GCC Compiler Makefile for use by buildit.  
#
# This makefile is intended only for use with B&I buildit. For "normal"
# builds use the conventional FSF top-level makefile.
#
# You can specify TARGETS=ppc (or i386) on the buildit command line to 
# limit the build to just one target.  The default is for ppc and i386. 
# The compiler targetted at this host gets built anyway, but not installed
# unless it's listed in TARGETS.

# The following default target is for the benefit of those who try to
# build llvm-gcc in the source directory.  GNU make will prefer to use
# this GNUmakefile instead of the Makefile generated by configure.  Since
# buildit always invokes this makefile with an explicit target, report an
# error for the default target and some other common gcc targets.
.PHONY: all all-gcc all-host all-target bootstrap
all all-gcc all-host all-target bootstrap:
	@echo
	@echo "Error: llvm-gcc cannot be built in the source directory."
	@echo "Please follow the instructions in the README.LLVM file."
	@exit 1

# Include the set of standard Apple makefile definitions.
ifndef CoreOSMakefiles
CoreOSMakefiles = $(MAKEFILEPATH)/CoreOS
endif
include $(CoreOSMakefiles)/Standard/Standard.make

# Enable Apple extensions to (gnu)make.
USE_APPLE_PB_SUPPORT = all

RC_ARCHS := ppc i386
HOSTS = $(RC_ARCHS)
targets = echo $(RC_ARCHS)
TARGETS := $(shell $(targets))
ifneq ($(strip $(RC_SUPPORTED_ARCHS)),)
TGT_ARCHS := $(shell echo $(RC_SUPPORTED_ARCHS))
else
TGT_ARCHS := $(shell echo $(TARGETS))
endif

SRCROOT = .

SRC = $(shell cd $(SRCROOT) && pwd | sed s,/private,,)
OBJROOT = $(SRC)/obj
SYMROOT = $(OBJROOT)/../sym
DSTROOT = $(OBJROOT)/../dst

PREFIX = /usr

#######################################################################

# LLVM LOCAL begin
# LLVM gets installed into /Developer/usr/local, not /usr.
ifndef DEVELOPER_DIR
PREFIX = /Developer/usr/llvm-gcc-4.2
else
PREFIX = ${DEVELOPER_DIR}/usr/llvm-gcc-4.2
endif

# Default to not reinstall libLTO.dylib.
INSTALL_LIBLTO := no

# Unless assertions are forced on in the GMAKE command line, disable them.
ifndef ENABLE_ASSERTIONS
ENABLE_ASSERTIONS := no
endif

ifndef LLVMCORE_PATH
ifndef DEVELOPER_DIR
LLVMCORE_PATH = /Developer/usr/local
else
LLVMCORE_PATH = ${DEVELOPER_DIR}/usr/local
endif
endif

# Default is optimized build.
ifeq ($(LLVM_DEBUG),1)
LLVM_OPTIMIZED := no
else
LLVM_OPTIMIZED := yes
endif

# Cross-builds for ARM hosts or iOS Simulator are not supported here.
ARM_HOSTED_BUILD := no
IOS_SIM_BUILD := no

ifndef RC_ProjectSourceVersion
RC_ProjectSourceVersion = 9999
endif

ifndef RC_ProjectSourceSubversion
RC_ProjectSourceSubversion = 00
endif

install:
	$(MAKE) OBJROOT=$(OBJROOT)/obj-llvmCore \
	        SYMROOT=$(OBJROOT)/sym-llvmCore \
	        DSTROOT=$(OBJROOT)/dst-llvmCore llvmCore
	$(MAKE) LLVMCORE_PATH=$(OBJROOT)/dst-llvmCore/Developer/usr/local \
	        llvmgcc42

llvmCore: $(OBJROOT) $(SYMROOT) $(DSTROOT)
	if [ ! -d $(SRC)/llvmCore ]; then \
	  echo "Error: llvmCore source directory is missing"; \
	  exit 1; \
	fi
	cd $(OBJROOT) && \
	  $(SRC)/llvmCore/utils/buildit/build_llvm "$(RC_ARCHS)" "$(TGT_ARCHS)"\
	    $(SRC)/llvmCore /Developer/usr/local $(DSTROOT) $(SYMROOT) \
	    $(ENABLE_ASSERTIONS) $(LLVM_OPTIMIZED) $(INSTALL_LIBLTO) \
	    $(ARM_HOSTED_BUILD) $(IOS_SIM_BUILD) \
	    $(RC_ProjectSourceVersion) $(RC_ProjectSourceSubversion) 

llvmgcc42: $(OBJROOT) $(SYMROOT) $(DSTROOT)
	cd $(OBJROOT) && \
	  $(SRC)/build_gcc "$(RC_ARCHS)" "$(TGT_ARCHS)" \
	    $(SRC) $(PREFIX) $(DSTROOT) $(SYMROOT) $(INSTALL_LIBLTO) \
	    $(ENABLE_ASSERTIONS) $(LLVMCORE_PATH) \
	    $(RC_ProjectSourceVersion) $(RC_ProjectSourceSubversion) 

Embedded:
	@if [ -z "$(ARM_SDK)" ]; then \
	  echo "ARM_SDK must be set to build the Embedded target"; \
	  exit 1; \
	fi
	ARM_PLATFORM=`xcodebuild -version -sdk $(ARM_SDK) PlatformPath` && \
	$(MAKE) DSTROOT=$(DSTROOT)$$ARM_PLATFORM DISABLE_USR_LINKS=1 install

# LLVM LOCAL end

# installhdrs does nothing, because the headers aren't useful until
# the compiler is installed.
installhdrs:

# We build and install in one shell script.
build: 

installsrc:
	@echo
	@echo ++++++++++++++++++++++
	@echo + Installing sources +
	@echo ++++++++++++++++++++++
	@echo
	if [ $(SRCROOT) != . ]; then \
	  $(PAX) -rw . $(SRCROOT); \
	fi
	# LLVM LOCAL begin: Avoid verification error due to binaries in libjava.
	rm -rf "$(SRCROOT)/libjava/"
	find -d "$(SRCROOT)" \( -type d -a -name .svn -o \
	                        -type f -a -name .DS_Store -o \
				-name \*~ -o -name .\#\* \) \
	  -exec rm -rf {} \;
	rm -rf "$(SRCROOT)/gcc/testsuite"
	rm -rf "$(SRCROOT)/llvmCore/test"

#######################################################################

clean:
	@echo
	@echo ++++++++++++
	@echo + Cleaning +
	@echo ++++++++++++
	@echo
	@if [ -d $(OBJROOT) -a "$(OBJROOT)" != / ]; then \
	  echo '*** DELETING ' $(OBJROOT); \
	  rm -rf $(OBJROOT); \
	fi
	@if [ -d $(SYMROOT) -a "$(SYMROOT)" != / ]; then \
	  echo '*** DELETING ' $(SYMROOT); \
	  rm -rf $(SYMROOT); \
	fi
	@if [ -d $(DSTROOT) -a "$(DSTROOT)" != / ]; then \
	  echo '*** DELETING ' $(DSTROOT); \
	  rm -rf $(DSTROOT); \
	fi

#######################################################################

$(OBJROOT) $(SYMROOT) $(DSTROOT):
	mkdir -p $@

.PHONY: install installsrc clean llvmCore llvmgcc42 Embedded
