#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Environment
MKDIR=mkdir
CP=cp
GREP=grep
NM=nm
CCADMIN=CCadmin
RANLIB=ranlib
CC=gcc
CCC=g++
CXX=g++
FC=gfortran
AS=as

# Macros
CND_PLATFORM=GNU-Linux
CND_DLIB_EXT=so
CND_CONF=Release
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/EDCrypt.o \
	${OBJECTDIR}/UserInfo.o \
	${OBJECTDIR}/W_NFC.o \
	${OBJECTDIR}/main.o \
	${OBJECTDIR}/utils/mifare.o \
	${OBJECTDIR}/utils/nfc-utils.o


# C Compiler Flags
CFLAGS=-std=gnu99

# CC Compiler Flags
CCFLAGS=
CXXFLAGS=

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=`pkg-config --libs libnfc` `pkg-config --libs libcrypto`  

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/nfctagwrite

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/nfctagwrite: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.c} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/nfctagwrite ${OBJECTFILES} ${LDLIBSOPTIONS}

${OBJECTDIR}/EDCrypt.o: EDCrypt.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -O2 `pkg-config --cflags libnfc` `pkg-config --cflags libcrypto`   -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/EDCrypt.o EDCrypt.c

${OBJECTDIR}/UserInfo.o: UserInfo.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -O2 `pkg-config --cflags libnfc` `pkg-config --cflags libcrypto`   -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/UserInfo.o UserInfo.c

${OBJECTDIR}/W_NFC.o: W_NFC.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -O2 `pkg-config --cflags libnfc` `pkg-config --cflags libcrypto`   -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/W_NFC.o W_NFC.c

${OBJECTDIR}/main.o: main.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -O2 `pkg-config --cflags libnfc` `pkg-config --cflags libcrypto`   -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/main.o main.c

${OBJECTDIR}/utils/mifare.o: utils/mifare.c 
	${MKDIR} -p ${OBJECTDIR}/utils
	${RM} "$@.d"
	$(COMPILE.c) -O2 `pkg-config --cflags libnfc` `pkg-config --cflags libcrypto`   -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/utils/mifare.o utils/mifare.c

${OBJECTDIR}/utils/nfc-utils.o: utils/nfc-utils.c 
	${MKDIR} -p ${OBJECTDIR}/utils
	${RM} "$@.d"
	$(COMPILE.c) -O2 `pkg-config --cflags libnfc` `pkg-config --cflags libcrypto`   -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/utils/nfc-utils.o utils/nfc-utils.c

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/nfctagwrite

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
