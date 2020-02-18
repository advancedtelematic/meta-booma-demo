DESCRIPTION = "Python based Booma client to interact with RPLIDAR sensor"
SECTION = "booma"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"

SRC_URI = "file://booma-client.py"

S = "${WORKDIR}"

do_install_append () {
    install -d ${D}${bindir}
    install -m 0755 booma-client.py ${D}${bindir}
}

DEPENDS_${PN} = "rplidar-roboticia geopy python3-numpy python3-pandas "
