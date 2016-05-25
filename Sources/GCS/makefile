#Constants

#Generic
CXX = g++
BIN_X86 = bin/Linux/x86
BIN_X64 = bin/Linux/x64

PUBLISH_DIR = bin/Linux/Publish
VERSION = 1.0

#wxGCS
wxGCS_CXXFLAGS = -c -DWX_PRECOMP -DwxSIZE_T_IS_UINT -pthread -g -O0 -fno-strict-aliasing -fvisibility=hidden -fvisibility-inlines-hidden -D__WXGTK__ -DWXUSINGDLL -D_FILE_OFFSET_BITS=64
WX_TOP_BUILD_DIR_X86 = ../../External/wxWidgets/gtk-build-static/x86
WX_TOP_BUILD_DIR_X64 = ../../External/wxWidgets/gtk-build-static/x64
BK_DEPS_X86 = $(WX_TOP_BUILD_DIR_X86)/bk-deps
BK_DEPS_X64 = $(WX_TOP_BUILD_DIR_X64)/bk-deps
WXGCS_SRC_DIR = wxGCS/Source
wxGCS_INCLUDE_X86 = -I$(WX_TOP_BUILD_DIR_X86)/lib/wx/include/gtk2-unicode-static-3.0  -I../../External/wxWidgets/include -I/usr/include/gtk-unix-print-2.0 -I/usr/include/gtk-2.0 -I/usr/include/atk-1.0 -I/usr/include/cairo -I/usr/include/gdk-pixbuf-2.0 -I/usr/include/pango-1.0 -I/usr/lib/i386-linux-gnu/gtk-2.0/include -I/usr/include/gio-unix-2.0/ -I/usr/include/freetype2 -I/usr/include/glib-2.0 -I/usr/lib/i386-linux-gnu/glib-2.0/include -I/usr/include/pixman-1 -I/usr/include/libpng12 -I/usr/include/harfbuzz -I../../External/qrencode/qrencode-3.4.4/include -I$(SERVER_SRC_DIR)/pugixml -I/usr/include/libappindicator-0.1/libappindicator
wxGCS_INCLUDE_X64 = -I$(WX_TOP_BUILD_DIR_X64)/lib/wx/include/gtk2-unicode-static-3.0  -I../../External/wxWidgets/include -I/usr/include/gtk-unix-print-2.0 -I/usr/include/gtk-2.0 -I/usr/include/atk-1.0 -I/usr/include/cairo -I/usr/include/gdk-pixbuf-2.0 -I/usr/include/pango-1.0 -I/usr/lib/x86_64-linux-gnu/gtk-2.0/include -I/usr/include/gio-unix-2.0/ -I/usr/include/freetype2 -I/usr/include/glib-2.0 -I/usr/lib/x86_64-linux-gnu/glib-2.0/include -I/usr/include/pixman-1 -I/usr/include/libpng12 -I/usr/include/harfbuzz -I../../External/qrencode/qrencode-3.4.4/include -I$(SERVER_SRC_DIR)/pugixml -I/usr/include/libappindicator-0.1/libappindicator
WX_LIB_DIR_X86 = -L$(WX_TOP_BUILD_DIR_X86)/lib
WX_LIB_DIR_X64 = -L$(WX_TOP_BUILD_DIR_X64)/lib
QRENCODE_LIB_DIR_X86 = -L../../External/qrencode/qrencode-3.4.4/x86/lib
QRENCODE_LIB_DIR_X64 = -L../../External/qrencode/qrencode-3.4.4/x64/lib
wxGCS_LIBS_X86 = -L./$(BIN_X86) -L/usr/local/lib -L$(WX_TOP_BUILD_DIR_X86)/lib -pthread -lwx_gtk2u_qa-3.0 -lwx_baseu_net-3.0 -lwx_gtk2u_html-3.0 -lwx_gtk2u_adv-3.0 -lwx_gtk2u_core-3.0 -lwx_baseu_xml-3.0 -lwx_baseu-3.0 -pthread -lgthread-2.0 -lX11 -lSM -lgtk-x11-2.0 -lgdk-x11-2.0 -latk-1.0 -lpangoft2-1.0 -lpangocairo-1.0 -lgdk_pixbuf-2.0 -lcairo -lpango-1.0 -lfontconfig -lglib-2.0 -lgio-2.0 -lgobject-2.0 -lfreetype -lwxregexu-3.0 -lwxtiff-3.0 -lwxjpeg-3.0 -lpng -lz -ldl -lm -lqrencode -lappindicator -lServer
wxGCS_LIBS_X64 = -L./$(BIN_X64) -L/usr/local/lib -L$(WX_TOP_BUILD_DIR_X64)/lib -pthread -lwx_gtk2u_qa-3.0 -lwx_baseu_net-3.0 -lwx_gtk2u_html-3.0 -lwx_gtk2u_adv-3.0 -lwx_gtk2u_core-3.0 -lwx_baseu_xml-3.0 -lwx_baseu-3.0 -pthread -lgthread-2.0 -lX11 -lSM -lgtk-x11-2.0 -lgdk-x11-2.0 -latk-1.0 -lpangoft2-1.0 -lpangocairo-1.0 -lgdk_pixbuf-2.0 -lcairo -lpango-1.0 -lfontconfig -lglib-2.0 -lgio-2.0 -lgobject-2.0 -lfreetype -lwxregexu-3.0 -lwxtiff-3.0 -lwxjpeg-3.0 -lwxpng-3.0 -lwxzlib-3.0 -ldl -lm -lqrencode -lappindicator -lServer
WL_X86 = -Wl,-rpath,$(WX_TOP_BUILD_DIR_X86)/lib
WL_X64 = -Wl,-rpath,$(WX_TOP_BUILD_DIR_X64)/lib
wxGCS_OBJ_DIR_X86 = obj/RemoteKeyboard/x86
wxGCS_OBJ_DIR_X64 = obj/RemoteKeyboard/x64
WXGCS_CPP_FILES := $(wildcard $(WXGCS_SRC_DIR)/*.cpp)

$(wxGCS_TEST_OBJ_DIR_X86)/%.o: $(WXGCS_SRC_DIR)/%.cpp
	mkdir -p $(wxGCS_TEST_OBJ_DIR_X86)
	$(BK_DEPS_X86) $(CXX) -m32 $(wxGCS_TEST_CXXFLAGS) $(wxGCS_INCLUDE_X86) -c -o $@ $<

$(wxGCS_TEST_OBJ_DIR_X64)/%.o: $(WXGCS_SRC_DIR)/%.cpp
	mkdir -p $(wxGCS_TEST_OBJ_DIR_X64)
	$(BK_DEPS_X64) $(CXX) -m64 $(wxGCS_TEST_CXXFLAGS) $(wxGCS_INCLUDE_X64) -c -o $@ $<
WXGCS_CPP_FILES := $(filter-out $(WXGCS_SRC_DIR)/borland_ide.cpp, $(WXGCS_CPP_FILES))
WXGCS_OBJ_FILES_X86 := $(addprefix $(wxGCS_OBJ_DIR_X86)/,$(notdir $(WXGCS_CPP_FILES:.cpp=.o)))
WXGCS_OBJ_FILES_X64 := $(addprefix $(wxGCS_OBJ_DIR_X64)/,$(notdir $(WXGCS_CPP_FILES:.cpp=.o)))

#Server
SERVER_CXXFLAGS = -c -pthread -g -O0
SERVER_INCLUDE = -I$(SERVER_SRC_DIR) -I$(SERVER_SRC_DIR)/pugixml
SERVER_LIBS = -pthread -lX11
SERVER_OBJ_DIR_X86 = obj/Server/x86
SERVER_OBJ_DIR_X64 = obj/Server/x64
SERVER_SRC_DIR = GCSNativeInterface
SERVER_CPP_FILES := $(wildcard $(SERVER_SRC_DIR)/*.cpp)
SERVER_OBJ_FILES_X86 := $(addprefix $(SERVER_OBJ_DIR_X86)/,$(notdir $(SERVER_CPP_FILES:.cpp=.o)))
SERVER_OBJ_FILES_X64 := $(addprefix $(SERVER_OBJ_DIR_X64)/,$(notdir $(SERVER_CPP_FILES:.cpp=.o)))

#InputHandler
INPUT_CXXFLAGS = -c -g -O0
INPUT_INCLUDE = -I$(SERVER_SRC_DIR) -I$(INPUT_SRC_DIR)
INPUT_OBJ_DIR_X86 = obj/InputHandler/x86
INPUT_OBJ_DIR_X64 = obj/InputHandler/x64
INPUT_SRC_DIR = $(SERVER_SRC_DIR)/LinuxInputHandler
INPUT_CPP_FILES := $(wildcard $(INPUT_SRC_DIR)/*.cpp)
INPUT_OBJ_FILES_X86 := $(addprefix $(INPUT_OBJ_DIR_X86)/,$(notdir $(INPUT_CPP_FILES:.cpp=.o)))
INPUT_OBJ_FILES_X64 := $(addprefix $(INPUT_OBJ_DIR_X64)/,$(notdir $(INPUT_CPP_FILES:.cpp=.o)))

$(wxGCS_TEST_OBJ_DIR_X86)/%.o: $(WXGCS_SRC_DIR)/%.cpp
	mkdir -p $(wxGCS_TEST_OBJ_DIR_X86)
	$(BK_DEPS_X86) $(CXX) -m32 $(wxGCS_TEST_CXXFLAGS) $(wxGCS_INCLUDE_X86) -c -o $@ $<

$(wxGCS_TEST_OBJ_DIR_X64)/%.o: $(WXGCS_SRC_DIR)/%.cpp
	mkdir -p $(wxGCS_TEST_OBJ_DIR_X64)
	$(BK_DEPS_X64) $(CXX) -m64 $(wxGCS_TEST_CXXFLAGS) $(wxGCS_INCLUDE_X64) -c -o $@ $<PUT_CPP_FILES:.cpp=.o)))

#Targets
all:
	make InitialSetup
	make SetupPublishDependencies
	make SetupDependencies_x86
	make all_x86
	make post_build_x86
	make Publish_x86
	make SetupDependencies_x64
	make all_x64
	make post_build_x64
	make Publish_x64
all_x86:
	mkdir -p $(BIN_X86)
	make InputHandler_x86
	make RemoteKeyboardAndServer_x86
all_x64:
	mkdir -p $(BIN_X64)
	make InputHandler_x64
	make RemoteKeyboardAndServer_x64

InitialSetup:
	sudo groupadd -f RemoteKeyboard
	sudo chmod 777 ../../External/wxWidgets/gtk-build-static/x86/bk-deps
	sudo chmod 777 ../../External/wxWidgets/gtk-build-static/x64/bk-deps

SetupDependencies_x86:
	sudo apt-get install -y gcc-multilib
	sudo apt-get install -y g++-multilib
	sudo apt-get install -y libgirepository-1.0:i386
	sudo apt-get install -y gir1.2-glib-2.0:i386
	sudo apt-get install -y gir1.2-appindicator-0.1:i386
	sudo apt-get install -y libdbus-glib-1-dev:i386
	sudo apt-get install -y libdbusmenu-glib-dev:i386
	sudo apt-get install -y libappindicator-dev:i386

SetupDependencies_x64:
	sudo apt-get install -y libglib2.0-dev
	sudo apt-get install -y libappindicator-dev

SetupPublishDependencies:
	sudo apt-get update
	sudo apt-get install -y ruby-dev
	sudo gem install fpm

Publish:
	make Publish_x86
	make Publish_x64

Publish_x86:
	rm -rf $(PUBLISH_DIR)/x86
	mkdir -p $(PUBLISH_DIR)/x86/Package/usr/bin/RemoteKeyboard/tmp/Driver
	mkdir -p $(PUBLISH_DIR)/x86/Package/usr/share/applications
	cp $(BIN_X86)/InputHandler $(BIN_X86)/RemoteKeyboard $(PUBLISH_DIR)/x86/Package/usr/bin/RemoteKeyboard
	cp -r ./Resources $(PUBLISH_DIR)/x86/Package/usr/bin/RemoteKeyboard
	cp -r Publish/README.txt $(PUBLISH_DIR)/x86/Package/usr/bin/RemoteKeyboard
	cp 10-local-RemoteKeyboard.rules Publish/RemoteKeyboard.desktop $(PUBLISH_DIR)/x86/Package/usr/bin/RemoteKeyboard/tmp
	cp -r  ../../Tools/Drivers/vUSB_HC_Driver_Linux/* $(PUBLISH_DIR)/x86/Package/usr/bin/RemoteKeyboard/tmp/Driver
	rm -f $(PUBLISH_DIR)/x86/Package/usr/bin/RemoteKeyboard/tmp/Driver/vhost_controller.ko
	cp Publish/RemoteKeyboard.desktop $(PUBLISH_DIR)/x86/Package/usr/share/applications
	fpm -s dir -t deb -C $(PUBLISH_DIR)/x86/Package --force \
	    -p $(PUBLISH_DIR)/x86/remote-keyboard-$(VERSION)-i386.deb \
	    -a i386 \
	    --name remote-keyboard \
	    --version $(VERSION) \
	    --iteration 1 \
	    --depends libappindicator1 \
	    --depends libgtk2.0-0 \
	    --directories /usr/bin/RemoteKeyboard \
		--deb-custom-control Publish/control_x86 \
	    --after-install Publish/post_install_ubuntu.sh \
	    --before-remove Publish/pre_remove_ubuntu.sh
	cp $(PUBLISH_DIR)/x86/remote-keyboard-$(VERSION)-i386.deb $(PUBLISH_DIR)

Publish_x64:
	rm -rf $(PUBLISH_DIR)/x64
	mkdir -p $(PUBLISH_DIR)/x64/Package/usr/bin/RemoteKeyboard/tmp/Driver
	mkdir -p $(PUBLISH_DIR)/x64/Package/usr/share/applications
	cp $(BIN_X64)/InputHandler $(BIN_X64)/RemoteKeyboard $(PUBLISH_DIR)/x64/Package/usr/bin/RemoteKeyboard
	cp -r ./Resources $(PUBLISH_DIR)/x64/Package/usr/bin/RemoteKeyboard
	cp -r Publish/README.txt $(PUBLISH_DIR)/x64/Package/usr/bin/RemoteKeyboard
	cp 10-local-RemoteKeyboard.rules Publish/RemoteKeyboard.desktop $(PUBLISH_DIR)/x64/Package/usr/bin/RemoteKeyboard/tmp
	cp -r  ../../Tools/Drivers/vUSB_HC_Driver_Linux/* $(PUBLISH_DIR)/x64/Package/usr/bin/RemoteKeyboard/tmp/Driver
	rm -f $(PUBLISH_DIR)/x64/Package/usr/bin/RemoteKeyboard/tmp/Driver/vhost_controller.ko
	cp Publish/RemoteKeyboard.desktop $(PUBLISH_DIR)/x64/Package/usr/share/applications
	fpm -s dir -t deb -C $(PUBLISH_DIR)/x64/Package --force \
	    -p $(PUBLISH_DIR)/x64/remote-keyboard-$(VERSION)-amd64.deb \
	    -a amd64 \
	    --name remote-keyboard \
	    --version $(VERSION) \
	    --iteration 1 \
	    --depends libappindicator1 \
	    --depends libgtk2.0-0 \
	    --depends build-essential \
	    --directories /usr/bin/RemoteKeyboard \
		--deb-custom-control Publish/control_x64 \
	    --after-install Publish/post_install_ubuntu.sh \
	    --before-remove Publish/pre_remove_ubuntu.sh
	cp $(PUBLISH_DIR)/x64/remote-keyboard-$(VERSION)-amd64.deb $(PUBLISH_DIR)

post_build_x86:
	sudo chown :RemoteKeyboard $(BIN_X86)/InputHandler
	sudo chmod g+s $(BIN_X86)/InputHandler

post_build_x64:
	sudo chown :RemoteKeyboard $(BIN_X64)/InputHandler
	sudo chmod g+s $(BIN_X64)/InputHandler

post_build_x86_BM:
	chown :RemoteKeyboard $(BIN_X86)/InputHandler
	chmod g+s $(BIN_X86)/InputHandler

post_build_x64_BM:
	chown :RemoteKeyboard $(BIN_X64)/InputHandler
	chmod g+s $(BIN_X64)/InputHandler

RemoteKeyboardAndServer_x86:
	make Server_x86
	make RemoteKeyboard_x86

RemoteKeyboardAndServer_x64:
	make Server_x64
	make RemoteKeyboard_x64

cleanServer_x86:
	rm -f $(SERVER_OBJ_DIR_X86)/*.o
	rm -f $(BIN_X86)/libServer.a
	-rmdir $(SERVER_OBJ_DIR_X86)

cleanServer_x64:
	rm -f $(SERVER_OBJ_DIR_X64)/*.o
	rm -f $(BIN_X64)/libServer.a
	-rmdir $(SERVER_OBJ_DIR_X64)

cleanInputHandler:
	make cleanInputHandler_x86
	make cleanInputHandler_x64

cleanInputHandler_x86:
	rm -f $(INPUT_OBJ_DIR_X86)/*.o
	rm -f $(BIN_X86)/InputHandler
	-rmdir $(INPUT_OBJ_DIR_X86)

cleanInputHandler_x64:
	rm -f $(INPUT_OBJ_DIR_X64)/*.o
	rm -f $(BIN_X64)/InputHandler
	-rmdir $(INPUT_OBJ_DIR_X64)

cleanRemoteKeyboard:
	make cleanRemoteKeyboard_x86
	make cleanRemoteKeyboard_x64

cleanRemoteKeyboard_x86:
	rm -f $(wxGCS_OBJ_DIR_X86)/*.*
	rm -f $(wxGCS_OBJ_DIR_X86)/.deps/*.*
	rm -f $(BIN_X86)/RemoteKeyboard
	-rmdir $(wxGCS_OBJ_DIR_X86)/.deps
	-rmdir $(wxGCS_OBJ_DIR_X86)

cleanRemoteKeyboard_x64:
	rm -f $(wxGCS_OBJ_DIR_X64)/*.*
	rm -f $(wxGCS_OBJ_DIR_X64)/.deps/*.*
	rm -f $(BIN_X64)/RemoteKeyboard
	-rmdir $(wxGCS_OBJ_DIR_X64)/.deps
	-rmdir $(wxGCS_OBJ_DIR_X64)

clean:
	make clean_x86
	make clean_x64

clean_x86:
	make cleanServer_x86
	make cleanRemoteKeyboard_x86

clean_x64:
	make cleanServer_x64
	make cleanRemoteKeyboard_x64

cleanall:
	make cleanInputHandler
	make clean

cleanall_x86:
	make cleanInputHandler
	make clean_x86

cleanall_x64:
	make cleanInputHandler
	make clean_x64

ServerChanged:
	make ServerChanged_x86
	make ServerChanged_x64

ServerChanged_x86:
	rm -f $(BIN_X86)/libServer.a
	rm -f $(BIN_X86)/RemoteKeyboard
	make RemoteKeyboardAndServer_x86

ServerChanged_x64:
	rm -f $(BIN_X64)/libServer.a
	rm -f $(BIN_X64)/RemoteKeyboard
	make RemoteKeyboardAndServer_x64

InputHandlerChanged:
	rm -f $(BIN_X86)/InputHandler
	rm -f $(BIN_X64)/InputHandler
	make InputHandler
	make post_build_x86
	make post_build_x64

InputHandler:
	make InputHandler_x86
	make InputHandler_x64

InputHandler_x86: $(INPUT_OBJ_FILES_X86)
	$(CXX) -m32 -o $(BIN_X86)/InputHandler $^

InputHandler_x64: $(INPUT_OBJ_FILES_X64)
	$(CXX) -m64 -o $(BIN_X64)/InputHandler $^

$(INPUT_OBJ_DIR_X86)/%.o: $(INPUT_SRC_DIR)/%.cpp
	mkdir -p $(INPUT_OBJ_DIR_X86)
	$(CXX) -m32 $(INPUT_CXXFLAGS) $(INPUT_INCLUDE) -c -o $@ $<

$(INPUT_OBJ_DIR_X64)/%.o: $(INPUT_SRC_DIR)/%.cpp
	mkdir -p $(INPUT_OBJ_DIR_X64)
	$(CXX) -m64 $(INPUT_CXXFLAGS) $(INPUT_INCLUDE) -c -o $@ $<

Server_x86: $(SERVER_OBJ_FILES_X86)
	$(CXX) -m32 $(INPUT_CXXFLAGS) $(INPUT_INCLUDE) -c -o $(SERVER_OBJ_DIR_X86)/pugixml.o $(SERVER_SRC_DIR)/pugixml/pugixml.cpp
	ar rcs $(BIN_X86)/libServer.a $(SERVER_OBJ_DIR_X86)/*.o

Server_x64: $(SERVER_OBJ_FILES_X64)
	$(CXX) -m64 $(INPUT_CXXFLAGS) $(INPUT_INCLUDE) -c -o $(SERVER_OBJ_DIR_X64)/pugixml.o $(SERVER_SRC_DIR)/pugixml/pugixml.cpp
	ar rcs $(BIN_X64)/libServer.a $(SERVER_OBJ_DIR_X64)/*.o

$(SERVER_OBJ_DIR_X86)/%.o: $(SERVER_SRC_DIR)/%.cpp
	mkdir -p $(SERVER_OBJ_DIR_X86)
	$(CXX) -m32 $(SERVER_CXXFLAGS) $(SERVER_INCLUDE) -c -o $@ $<

$(SERVER_OBJ_DIR_X64)/%.o: $(SERVER_SRC_DIR)/%.cpp
	mkdir -p $(SERVER_OBJ_DIR_X64)
	$(CXX) -m64 $(SERVER_CXXFLAGS) $(SERVER_INCLUDE) -c -o $@ $<

RemoteKeyboard_x86: $(WXGCS_OBJ_FILES_X86)
	$(CXX) -m32 -o $(BIN_X86)/RemoteKeyboard $^ $(wxGCS_LIBS_X86) $(LDADD) $(WX_LIB_DIR_X86) $(QRENCODE_LIB_DIR_X86) $(PUGIXML_LIB_DIR) $(WL_X86)

RemoteKeyboard_x64: $(WXGCS_OBJ_FILES_X64)
	$(CXX) -m64 -o $(BIN_X64)/RemoteKeyboard $^ $(wxGCS_LIBS_X64) $(LDADD) $(WX_LIB_DIR_X64) $(QRENCODE_LIB_DIR_X64) $(PUGIXML_LIB_DIR) $(WL_X64)

$(wxGCS_OBJ_DIR_X86)/%.o: $(WXGCS_SRC_DIR)/%.cpp
	mkdir -p $(wxGCS_OBJ_DIR_X86)
	$(BK_DEPS_X86) $(CXX) -m32 $(wxGCS_CXXFLAGS) $(wxGCS_INCLUDE_X86) -c -o $@ $<

$(wxGCS_OBJ_DIR_X64)/%.o: $(WXGCS_SRC_DIR)/%.cpp
	mkdir -p $(wxGCS_OBJ_DIR_X64)
	$(BK_DEPS_X64) $(CXX) -m64 $(wxGCS_CXXFLAGS) $(wxGCS_INCLUDE_X64) -c -o $@ $<
