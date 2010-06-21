DESCRIPTION = "Qtopia4 hello world program"
PR = "r0"

SRC_URI = " \
  file://posix_qextserialport.cpp \
  file://qextserialport.h \
  file://qextserialport_global.h \
  file://qextserialport.cpp \
  file://serial.h \
  file://serial.cpp \
  file://tcp.h \
  file://tcp.cpp \
  file://rayaairbot.cpp \
  file://rayaairbot.h \
  file://main.cpp \
  file://Rayaairbot.pro \
 "

QMAKE_PROFILES="Rayaairbot.pro"

do_configure_prepend() {
  cp ${WORKDIR}/*.pro ${S}/
  cp ${WORKDIR}/*.cpp ${S}/
  cp ${WORKDIR}/*.h ${S}/
}

do_compile() {
	oe_runmake
}

do_install () {
  install -d ${D}${bindir}/
	install -m 0755 ${S}/Rayaairbot ${D}${bindir}/
}

PACKAGES = "${PN}"
FILES_${PN} = "${bindir}/*"

inherit qtopia4core
