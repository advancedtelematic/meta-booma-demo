SUMMARY = "HERE Open Location Platform Edge SDK"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"

SRC_URI = "gitsm://github.com/heremaps/here-olp-sdk-cpp;rev=v1.2.0"

DEPENDS = "boost curl openssl"

inherit cmake

S = "${WORKDIR}/git"

EXTRA_OECMAKE = ""

do_install_append() {
        install -d ${D}${libdir}/cmake/Snappy
        install -d ${D}${libdir}/cmake/leveldb
        install -m 755 ${WORKDIR}/build/install/lib/*.a ${D}${libdir}
        cp -r ${WORKDIR}/build/install/lib/cmake/Snappy ${D}${libdir}/cmake/
        cp -r ${WORKDIR}/build/install/lib/cmake/leveldb ${D}${libdir}/cmake/
}

FILES_${PN} += "${libdir}/libolp-cpp-sdk* ${libdir}/cmake"
