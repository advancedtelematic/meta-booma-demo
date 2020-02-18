SUMMARY = "Simple and lightweight module for working with RPLidar laser scanners"
HOMEPAGE = "https://github.com/Roboticia/RPLidar"

LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"

SRC_URI[sha256sum] = "709e9143f7701d69e8439231b065e676f7d5a6086cd2922113b055bedf99f0e3"

PYPI_PACKAGE = "rplidar-roboticia"

RDEPENDS_${PN} = "python3-modules python3-misc python3-pyserial"

inherit pypi
inherit setuptools3
