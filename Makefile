#
# Makefile - 
#

.PHONY: construct_makefile build clean

BUILD_DIR = build

default: construct_makefile build

construct_makefile:
	mkdir ${BUILD_DIR}
	cd ${BUILD_DIR} && cmake .. 
    
build:
	cd ${BUILD_DIR} && ${MAKE}
    
clean:
	rm -rf ${BUILD_DIR}


run:
	cd ${BUILD_DIR}/src && ./p1


