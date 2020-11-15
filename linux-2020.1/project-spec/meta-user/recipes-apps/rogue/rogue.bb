#
# This is the GPIO-DEMO apllication recipe
#
#

SUMMARY = "Rogue Application"
SECTION = "PETALINUX/apps"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"

#SRC_URI = "https://github.com/slaclab/rogue/archive/v5.5.2.tar.gz"
#SRC_URI[md5sum] = "29b014b80718b4a686cabf7fd3b58d2e"
#S = "${WORKDIR}/rogue-5.5.2/"

SRC_URI = "file://rogue-petalinux.zip"
S = "${WORKDIR}/rogue-petalinux/"

#DEPENDS += "python3 cmake boost zeromq bzip2"
DEPENDS += "cmake zeromq bzip2"

PROVIDES = "rogue"
EXTRA_OECMAKE += "-DNO_PYTHON=1 -DROGUE_INSTALL=system -DROGUE_VERSION=v5.5.2"

inherit cmake

#FILES_${PN} += "/usr/lib/RogueCOnfig.cmake"
FILES_${PN} += "/usr/lib/*"
#FILES_${PN} += "/usr/lib/* /usr/include/*"

#do_install() {
#    cd ${WORKDIR}/build; make install
#}
