# =========================================================================
#     This makefile was generated by
#     Bakefile 0.2.9 (http://www.bakefile.org)
#     Do not modify, all changes will be overwritten!
# =========================================================================




prefix = /usr/local
exec_prefix = ${prefix}
datarootdir = ${prefix}/share
INSTALL = /usr/bin/install -c
EXEEXT = 
WINDRES = 
SETFILE = 
NM = nm
BK_DEPS = /home/liam/Desktop/wxWidgets-3.0.2customfontpatch/gtk-build-staticC/bk-deps
srcdir = ../../../samples/svg
top_srcdir = ../../..
LIBS = -ldl -lm
LDFLAGS_GUI = 
CXX = g++
CXXFLAGS = -DWX_PRECOMP -pthread -g -O0 -pthread -I/usr/include/gtk-unix-print-2.0 -I/usr/include/gtk-2.0 -I/usr/include/atk-1.0 -I/usr/include/cairo -I/usr/include/gdk-pixbuf-2.0 -I/usr/include/pango-1.0 -I/usr/lib/x86_64-linux-gnu/gtk-2.0/include -I/usr/include/gio-unix-2.0/ -I/usr/include/freetype2 -I/usr/include/glib-2.0 -I/usr/lib/x86_64-linux-gnu/glib-2.0/include -I/usr/include/pixman-1 -I/usr/include/libpng12 -I/usr/include/harfbuzz 
CPPFLAGS = -D_FILE_OFFSET_BITS=64 -I${wx_top_builddir}/lib/wx/include/gtk2-unicode-static-3.0 -I${top_srcdir}/include -pthread -I/usr/include/gtk-2.0 -I/usr/lib/x86_64-linux-gnu/gtk-2.0/include -I/usr/include/atk-1.0 -I/usr/include/cairo -I/usr/include/gdk-pixbuf-2.0 -I/usr/include/pango-1.0 -I/usr/include/gio-unix-2.0/ -I/usr/include/freetype2 -I/usr/include/glib-2.0 -I/usr/lib/x86_64-linux-gnu/glib-2.0/include -I/usr/include/pixman-1 -I/usr/include/libpng12 -I/usr/include/harfbuzz
LDFLAGS = -pthread   
WX_LIB_FLAVOUR = 
TOOLKIT = GTK
TOOLKIT_LOWERCASE = gtk
TOOLKIT_VERSION = 2
TOOLCHAIN_FULLNAME = gtk2-unicode-static-3.0
EXTRALIBS = -pthread    -ldl -lm 
EXTRALIBS_XML = 
EXTRALIBS_GUI = -pthread -lgtk-x11-2.0 -lgdk-x11-2.0 -latk-1.0 -lgio-2.0 -lpangoft2-1.0 -lpangocairo-1.0 -lgdk_pixbuf-2.0 -lcairo -lpango-1.0 -lfontconfig -lgobject-2.0 -lfreetype -lgthread-2.0 -lglib-2.0 -lX11 -lSM -lgtk-x11-2.0 -lgdk-x11-2.0 -latk-1.0 -lgio-2.0 -lpangoft2-1.0 -lpangocairo-1.0 -lgdk_pixbuf-2.0 -lcairo -lpango-1.0 -lfontconfig -lgobject-2.0 -lglib-2.0 -lfreetype
CXXWARNINGS = -Wall -Wundef -Wunused-parameter -Wno-ctor-dtor-privacy -Woverloaded-virtual
HOST_SUFFIX = 
SAMPLES_RPATH_FLAG = 
wx_top_builddir = /home/liam/Desktop/wxWidgets-3.0.2customfontpatch/gtk-build-staticC

### Variables: ###

DESTDIR = 
WX_RELEASE = 3.0
WX_VERSION = $(WX_RELEASE).2
LIBDIRNAME = $(wx_top_builddir)/lib
SVGTEST_CXXFLAGS = -D__WX$(TOOLKIT)__ $(__WXUNIV_DEFINE_p) $(__DEBUG_DEFINE_p) \
	$(__EXCEPTIONS_DEFINE_p) $(__RTTI_DEFINE_p) $(__THREAD_DEFINE_p) \
	-I$(srcdir) $(__DLLFLAG_p) -I$(srcdir)/../../samples $(CXXWARNINGS) \
	$(CPPFLAGS) $(CXXFLAGS)
SVGTEST_OBJECTS =  \
	$(__svgtest___win32rc) \
	$(__svgtest_os2_lib_res) \
	svgtest_svgtest.o

### Conditionally set variables: ###

#CXXC = $(CXX)
CXXC = $(BK_DEPS) $(CXX)
#PORTNAME = base
PORTNAME = $(TOOLKIT_LOWERCASE)$(TOOLKIT_VERSION)
#WXBASEPORT = _carbon
#WXDEBUGFLAG = d
WXUNICODEFLAG = u
#WXUNIVNAME = univ
EXTRALIBS_FOR_BASE = $(EXTRALIBS)
#EXTRALIBS_FOR_BASE = $(EXTRALIBS) \
#	$(EXTRALIBS_XML) $(EXTRALIBS_GUI)
EXTRALIBS_FOR_GUI = $(EXTRALIBS_GUI)
#EXTRALIBS_FOR_GUI = 
__svgtest___mac_setfilecmd = @true
#__svgtest___mac_setfilecmd = \
#	$(SETFILE) -t APPL svgtest$(EXEEXT)
#__WXUNIV_DEFINE_p = -D__WXUNIVERSAL__
#__WXUNIV_DEFINE_p_2 = --define __WXUNIVERSAL__
#__DEBUG_DEFINE_p = -DwxDEBUG_LEVEL=0
#__DEBUG_DEFINE_p_2 = --define wxDEBUG_LEVEL=0
#__EXCEPTIONS_DEFINE_p = -DwxNO_EXCEPTIONS
#__EXCEPTIONS_DEFINE_p_2 = --define wxNO_EXCEPTIONS
#__RTTI_DEFINE_p = -DwxNO_RTTI
#__RTTI_DEFINE_p_2 = --define wxNO_RTTI
#__THREAD_DEFINE_p = -DwxNO_THREADS
#__THREAD_DEFINE_p_2 = --define wxNO_THREADS
#__DLLFLAG_p = -DWXUSINGDLL
#__DLLFLAG_p_2 = --define WXUSINGDLL
COND_PLATFORM_OS2_1___svgtest___os2_emxbindcmd = $(NM) svgtest$(EXEEXT) | if \
	grep -q pmwin.763 ; then emxbind -ep svgtest$(EXEEXT) ; fi
#__svgtest___os2_emxbindcmd = $(COND_PLATFORM_OS2_1___svgtest___os2_emxbindcmd)
#__RCDEFDIR_p_1 = --include-dir \
#	$(LIBDIRNAME)/wx/include/$(TOOLCHAIN_FULLNAME)
#__svgtest___win32rc = svgtest_sample_rc.o
#__svgtest_os2_lib_res = \
#	$(top_srcdir)/include/wx/os2/wx.res
#__svgtest_app_Contents_PkgInfo___depname \
#	= svgtest.app/Contents/PkgInfo
#__svgtest_bundle___depname = svgtest_bundle
#____svgtest_BUNDLE_TGT_REF_DEP = \
#	$(__svgtest_app_Contents_PkgInfo___depname)
#____svgtest_BUNDLE_TGT_REF_DEP \
#	= $(__svgtest_app_Contents_PkgInfo___depname)
#____svgtest_BUNDLE_TGT_REF_DEP \
#	= $(__svgtest_app_Contents_PkgInfo___depname)
#____svgtest_BUNDLE_TGT_REF_DEP \
#	= $(__svgtest_app_Contents_PkgInfo___depname)
#____svgtest_BUNDLE_TGT_REF_DEP = \
#	$(__svgtest_app_Contents_PkgInfo___depname)
COND_MONOLITHIC_0___WXLIB_CORE_p = \
	-lwx_$(PORTNAME)$(WXUNIVNAME)$(WXUNICODEFLAG)$(WXDEBUGFLAG)$(WX_LIB_FLAVOUR)_core-$(WX_RELEASE)$(HOST_SUFFIX)
__WXLIB_CORE_p = $(COND_MONOLITHIC_0___WXLIB_CORE_p)
COND_MONOLITHIC_0___WXLIB_BASE_p = \
	-lwx_base$(WXBASEPORT)$(WXUNICODEFLAG)$(WXDEBUGFLAG)$(WX_LIB_FLAVOUR)-$(WX_RELEASE)$(HOST_SUFFIX)
__WXLIB_BASE_p = $(COND_MONOLITHIC_0___WXLIB_BASE_p)
COND_MONOLITHIC_1___WXLIB_MONO_p = \
	-lwx_$(PORTNAME)$(WXUNIVNAME)$(WXUNICODEFLAG)$(WXDEBUGFLAG)$(WX_LIB_FLAVOUR)-$(WX_RELEASE)$(HOST_SUFFIX)
#__WXLIB_MONO_p = $(COND_MONOLITHIC_1___WXLIB_MONO_p)
#__LIB_SCINTILLA_IF_MONO_p \
#	= \
#	-lwxscintilla$(WXDEBUGFLAG)$(WX_LIB_FLAVOUR)-$(WX_RELEASE)$(HOST_SUFFIX)
__LIB_TIFF_p \
	= \
	-lwxtiff$(WXDEBUGFLAG)$(WX_LIB_FLAVOUR)-$(WX_RELEASE)$(HOST_SUFFIX)
__LIB_JPEG_p \
	= \
	-lwxjpeg$(WXDEBUGFLAG)$(WX_LIB_FLAVOUR)-$(WX_RELEASE)$(HOST_SUFFIX)
__LIB_PNG_p \
	= \
	-lwxpng$(WXDEBUGFLAG)$(WX_LIB_FLAVOUR)-$(WX_RELEASE)$(HOST_SUFFIX)
__LIB_ZLIB_p = \
	-lwxzlib$(WXDEBUGFLAG)$(WX_LIB_FLAVOUR)-$(WX_RELEASE)$(HOST_SUFFIX)
COND_wxUSE_REGEX_builtin___LIB_REGEX_p = \
	-lwxregex$(WXUNICODEFLAG)$(WXDEBUGFLAG)$(WX_LIB_FLAVOUR)-$(WX_RELEASE)$(HOST_SUFFIX)
__LIB_REGEX_p = $(COND_wxUSE_REGEX_builtin___LIB_REGEX_p)
__LIB_EXPAT_p = \
	-lwxexpat$(WXDEBUGFLAG)$(WX_LIB_FLAVOUR)-$(WX_RELEASE)$(HOST_SUFFIX)

### Targets: ###

all: svgtest$(EXEEXT) $(__svgtest_bundle___depname)

install: 

uninstall: 

install-strip: install

clean: 
	rm -rf ./.deps ./.pch
	rm -f ./*.o
	rm -f svgtest$(EXEEXT)
	rm -rf svgtest.app

distclean: clean
	rm -f config.cache config.log config.status bk-deps bk-make-pch shared-ld-sh Makefile

svgtest$(EXEEXT): $(SVGTEST_OBJECTS) $(__svgtest___win32rc)
	$(CXX) -o $@ $(SVGTEST_OBJECTS)    -L$(LIBDIRNAME) $(LDFLAGS_GUI) $(SAMPLES_RPATH_FLAG) $(LDFLAGS)  $(__WXLIB_CORE_p)  $(__WXLIB_BASE_p)  $(__WXLIB_MONO_p) $(__LIB_SCINTILLA_IF_MONO_p) $(__LIB_TIFF_p) $(__LIB_JPEG_p) $(__LIB_PNG_p)  $(EXTRALIBS_FOR_GUI) $(__LIB_ZLIB_p) $(__LIB_REGEX_p) $(__LIB_EXPAT_p) $(EXTRALIBS_FOR_BASE) $(LIBS)
	
	$(__svgtest___mac_setfilecmd)
	$(__svgtest___os2_emxbindcmd)

#svgtest.app/Contents/PkgInfo: svgtest$(EXEEXT) $(top_srcdir)/src/osx/carbon/Info.plist.in $(top_srcdir)/src/osx/carbon/wxmac.icns
#	mkdir -p svgtest.app/Contents
#	mkdir -p svgtest.app/Contents/MacOS
#	mkdir -p svgtest.app/Contents/Resources
#	
#	
#	sed -e "s/IDENTIFIER/`echo $(srcdir) | sed -e 's,\.\./,,g' | sed -e 's,/,.,g'`/" \
#	-e "s/EXECUTABLE/svgtest/" \
#	-e "s/VERSION/$(WX_VERSION)/" \
#	$(top_srcdir)/src/osx/carbon/Info.plist.in >svgtest.app/Contents/Info.plist
#	
#	
#	/bin/echo "APPL????" >svgtest.app/Contents/PkgInfo
#	
#	
#	ln -f svgtest$(EXEEXT) svgtest.app/Contents/MacOS/svgtest
#	
#	
#	cp -f $(top_srcdir)/src/osx/carbon/wxmac.icns svgtest.app/Contents/Resources/wxmac.icns

#svgtest_bundle: $(____svgtest_BUNDLE_TGT_REF_DEP)

svgtest_sample_rc.o: $(srcdir)/../../samples/sample.rc
	$(WINDRES) -i$< -o$@    --define __WX$(TOOLKIT)__ $(__WXUNIV_DEFINE_p_2) $(__DEBUG_DEFINE_p_2)  $(__EXCEPTIONS_DEFINE_p_2) $(__RTTI_DEFINE_p_2) $(__THREAD_DEFINE_p_2)   --include-dir $(srcdir) $(__DLLFLAG_p_2) --include-dir $(srcdir)/../../samples $(__RCDEFDIR_p_1) --include-dir $(top_srcdir)/include

svgtest_svgtest.o: $(srcdir)/svgtest.cpp
	$(CXXC) -c -o $@ $(SVGTEST_CXXFLAGS) $(srcdir)/svgtest.cpp


# Include dependency info, if present:
-include ./.deps/*.d

.PHONY: all install uninstall clean distclean svgtest_bundle
