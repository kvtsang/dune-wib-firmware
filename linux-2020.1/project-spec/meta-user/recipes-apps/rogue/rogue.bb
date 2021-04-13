#
# This is the GPIO-DEMO apllication recipe
#
#

ROGUE_VERSION = "5.6.0"
ROGUE_MD5SUM  = "710712ef00707473a7707782a05d1de8"

SUMMARY = "Rogue Application"
SECTION = "PETALINUX/apps"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"

SRC_URI = "https://github.com/slaclab/rogue/archive/v${ROGUE_VERSION}.tar.gz"
SRC_URI[md5sum] = "${ROGUE_MD5SUM}"
S = "${WORKDIR}/rogue-${ROGUE_VERSION}/"

DEPENDS += "python3 python3-numpy python3-native python3-numpy-native cmake boost zeromq bzip2"

PROVIDES = "rogue"
EXTRA_OECMAKE += "-DROGUE_INSTALL=system -DROGUE_VERSION=v${ROGUE_VERSION}"

inherit cmake python3native distutils3

FILES_${PN}-dev += "/usr/include/rogue/*"
FILES_${PN} += "/usr/lib/*"

do_configure() {
   cmake_do_configure
}

do_install() {
   cmake_do_install
   distutils3_do_install
}
