MAKE                = make
ROOT_DIR            = bin
PLIST               = package/backup_server.plist
RESOURCE_DIR        = package/resources
VERSION             = 1.0.0
SCRIPTS             = 
INSTALL_LOCATION    = /Applications/App
OUTPUT_DIR          = ../output
OBJ_DIR             = package
PACKAGE             = App.pkg
PKG_XML             = package/Distribution.xml
SUBDIRS             = BackupServer BackupManager

all:
	@for i in $(SUBDIRS) ; \
	do \
	    cd $$i ; \
	    echo "current directory: $$i" ;\
	    $(MAKE) $@ ; \
	    cd .. ; \
	done
	mkdir -p $(OBJ_DIR)
	mkdir -p $(OUTPUT_DIR)
	pkgbuild --root $(CURDIR)/$(ROOT_DIR) --identifier $(PLIST) --install-location $(INSTALL_LOCATION) --version $(VERSION) --ownership preserve $(OBJ_DIR)/$(PACKAGE)
	productbuild --distribution $(PKG_XML) --resources $(RESOURCE_DIR) $(OBJ_DIR)/$(PACKAGE) 

