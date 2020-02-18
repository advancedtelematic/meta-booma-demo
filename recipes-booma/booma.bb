SUMMARY = "Booma"
SECTION = "booma"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"

SRC_URI = "file://CMakeLists.txt file://example.cpp file://example.h file://main.cpp"

inherit cmake

S = "${WORKDIR}"

EXTRA_OECMAKE = ""

do_install() {
        install -d ${D}${bindir}
        chrpath -d dataserviceWriteApp
        install -m 0755 dataserviceWriteApp ${D}${bindir}
}

DEPENDS += "olp-cpp-sdk boost"
