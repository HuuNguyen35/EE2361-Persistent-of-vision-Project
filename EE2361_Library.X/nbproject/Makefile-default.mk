#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Include project Makefile
ifeq "${IGNORE_LOCAL}" "TRUE"
# do not include local makefile. User is passing all local related variables already
else
include Makefile
# Include makefile containing local settings
ifeq "$(wildcard nbproject/Makefile-local-default.mk)" "nbproject/Makefile-local-default.mk"
include nbproject/Makefile-local-default.mk
endif
endif

# Environment
MKDIR=gnumkdir -p
RM=rm -f 
MV=mv 
CP=cp 

# Macros
CND_CONF=default
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
IMAGE_TYPE=debug
OUTPUT_SUFFIX=elf
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=${DISTDIR}/EE2361_Library.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=${DISTDIR}/EE2361_Library.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
endif

ifeq ($(COMPARE_BUILD), true)
COMPARISON_BUILD=-mafrlcsj
else
COMPARISON_BUILD=
endif

# Object Directory
OBJECTDIR=build/${CND_CONF}/${IMAGE_TYPE}

# Distribution Directory
DISTDIR=dist/${CND_CONF}/${IMAGE_TYPE}

# Source Files Quoted if spaced
SOURCEFILES_QUOTED_IF_SPACED=LEDAPI.c MainFinal_V1.c MainLEDTest_V1.c MainMotorDriverTest_V1.c MainSensorTest_V1.c MotorDriverAPI.c SensorAPI.c lcd_api_v2.c blommel_lab2b_asmLib_v1.s pattern.c

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/LEDAPI.o ${OBJECTDIR}/MainFinal_V1.o ${OBJECTDIR}/MainLEDTest_V1.o ${OBJECTDIR}/MainMotorDriverTest_V1.o ${OBJECTDIR}/MainSensorTest_V1.o ${OBJECTDIR}/MotorDriverAPI.o ${OBJECTDIR}/SensorAPI.o ${OBJECTDIR}/lcd_api_v2.o ${OBJECTDIR}/blommel_lab2b_asmLib_v1.o ${OBJECTDIR}/pattern.o
POSSIBLE_DEPFILES=${OBJECTDIR}/LEDAPI.o.d ${OBJECTDIR}/MainFinal_V1.o.d ${OBJECTDIR}/MainLEDTest_V1.o.d ${OBJECTDIR}/MainMotorDriverTest_V1.o.d ${OBJECTDIR}/MainSensorTest_V1.o.d ${OBJECTDIR}/MotorDriverAPI.o.d ${OBJECTDIR}/SensorAPI.o.d ${OBJECTDIR}/lcd_api_v2.o.d ${OBJECTDIR}/blommel_lab2b_asmLib_v1.o.d ${OBJECTDIR}/pattern.o.d

# Object Files
OBJECTFILES=${OBJECTDIR}/LEDAPI.o ${OBJECTDIR}/MainFinal_V1.o ${OBJECTDIR}/MainLEDTest_V1.o ${OBJECTDIR}/MainMotorDriverTest_V1.o ${OBJECTDIR}/MainSensorTest_V1.o ${OBJECTDIR}/MotorDriverAPI.o ${OBJECTDIR}/SensorAPI.o ${OBJECTDIR}/lcd_api_v2.o ${OBJECTDIR}/blommel_lab2b_asmLib_v1.o ${OBJECTDIR}/pattern.o

# Source Files
SOURCEFILES=LEDAPI.c MainFinal_V1.c MainLEDTest_V1.c MainMotorDriverTest_V1.c MainSensorTest_V1.c MotorDriverAPI.c SensorAPI.c lcd_api_v2.c blommel_lab2b_asmLib_v1.s pattern.c



CFLAGS=
ASFLAGS=
LDLIBSOPTIONS=

############# Tool locations ##########################################
# If you copy a project from one host to another, the path where the  #
# compiler is installed may be different.                             #
# If you open this project with MPLAB X in the new host, this         #
# makefile will be regenerated and the paths will be corrected.       #
#######################################################################
# fixDeps replaces a bunch of sed/cat/printf statements that slow down the build
FIXDEPS=fixDeps

.build-conf:  ${BUILD_SUBPROJECTS}
ifneq ($(INFORMATION_MESSAGE), )
	@echo $(INFORMATION_MESSAGE)
endif
	${MAKE}  -f nbproject/Makefile-default.mk ${DISTDIR}/EE2361_Library.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

MP_PROCESSOR_OPTION=24FJ64GA002
MP_LINKER_FILE_OPTION=,--script=p24FJ64GA002.gld
# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/LEDAPI.o: LEDAPI.c  .generated_files/flags/default/72645137c86cae3d54483a94fdfc5729003b991e .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/LEDAPI.o.d 
	@${RM} ${OBJECTDIR}/LEDAPI.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  LEDAPI.c  -o ${OBJECTDIR}/LEDAPI.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/LEDAPI.o.d"      -g -D__DEBUG     -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/MainFinal_V1.o: MainFinal_V1.c  .generated_files/flags/default/b9f806a046a8427be36f4b6865e08b6e812f1127 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/MainFinal_V1.o.d 
	@${RM} ${OBJECTDIR}/MainFinal_V1.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  MainFinal_V1.c  -o ${OBJECTDIR}/MainFinal_V1.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/MainFinal_V1.o.d"      -g -D__DEBUG     -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/MainLEDTest_V1.o: MainLEDTest_V1.c  .generated_files/flags/default/e6df543bd26fb842690052d02dacd241d0c11cfc .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/MainLEDTest_V1.o.d 
	@${RM} ${OBJECTDIR}/MainLEDTest_V1.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  MainLEDTest_V1.c  -o ${OBJECTDIR}/MainLEDTest_V1.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/MainLEDTest_V1.o.d"      -g -D__DEBUG     -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/MainMotorDriverTest_V1.o: MainMotorDriverTest_V1.c  .generated_files/flags/default/cd4f04387306425005e7fe91837476da77e012ec .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/MainMotorDriverTest_V1.o.d 
	@${RM} ${OBJECTDIR}/MainMotorDriverTest_V1.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  MainMotorDriverTest_V1.c  -o ${OBJECTDIR}/MainMotorDriverTest_V1.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/MainMotorDriverTest_V1.o.d"      -g -D__DEBUG     -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/MainSensorTest_V1.o: MainSensorTest_V1.c  .generated_files/flags/default/eddd6dc82b76fadcf266f15dba8b91287fa441a6 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/MainSensorTest_V1.o.d 
	@${RM} ${OBJECTDIR}/MainSensorTest_V1.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  MainSensorTest_V1.c  -o ${OBJECTDIR}/MainSensorTest_V1.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/MainSensorTest_V1.o.d"      -g -D__DEBUG     -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/MotorDriverAPI.o: MotorDriverAPI.c  .generated_files/flags/default/6503491501376fb8fd70504b8fdd8a3b2dc22c36 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/MotorDriverAPI.o.d 
	@${RM} ${OBJECTDIR}/MotorDriverAPI.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  MotorDriverAPI.c  -o ${OBJECTDIR}/MotorDriverAPI.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/MotorDriverAPI.o.d"      -g -D__DEBUG     -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/SensorAPI.o: SensorAPI.c  .generated_files/flags/default/794ec4fbfe09d550a707fa25909c46bf01119908 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/SensorAPI.o.d 
	@${RM} ${OBJECTDIR}/SensorAPI.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  SensorAPI.c  -o ${OBJECTDIR}/SensorAPI.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/SensorAPI.o.d"      -g -D__DEBUG     -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/lcd_api_v2.o: lcd_api_v2.c  .generated_files/flags/default/709d2a4d8ae8001d6b3648957e0b07f8014c74d5 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/lcd_api_v2.o.d 
	@${RM} ${OBJECTDIR}/lcd_api_v2.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  lcd_api_v2.c  -o ${OBJECTDIR}/lcd_api_v2.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/lcd_api_v2.o.d"      -g -D__DEBUG     -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/pattern.o: pattern.c  .generated_files/flags/default/2d9eded2bb8ee98f5077a11651d05e2b5cffe619 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/pattern.o.d 
	@${RM} ${OBJECTDIR}/pattern.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  pattern.c  -o ${OBJECTDIR}/pattern.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/pattern.o.d"      -g -D__DEBUG     -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
else
${OBJECTDIR}/LEDAPI.o: LEDAPI.c  .generated_files/flags/default/7ef89b9dcf1516ae058ab6c961a9f84734d9c81 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/LEDAPI.o.d 
	@${RM} ${OBJECTDIR}/LEDAPI.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  LEDAPI.c  -o ${OBJECTDIR}/LEDAPI.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/LEDAPI.o.d"        -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/MainFinal_V1.o: MainFinal_V1.c  .generated_files/flags/default/abaeae832c0516bd21ce4db56fa4e5657d9a51b6 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/MainFinal_V1.o.d 
	@${RM} ${OBJECTDIR}/MainFinal_V1.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  MainFinal_V1.c  -o ${OBJECTDIR}/MainFinal_V1.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/MainFinal_V1.o.d"        -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/MainLEDTest_V1.o: MainLEDTest_V1.c  .generated_files/flags/default/39fc35550e3d082dce505ff8497b5c49098d9c3c .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/MainLEDTest_V1.o.d 
	@${RM} ${OBJECTDIR}/MainLEDTest_V1.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  MainLEDTest_V1.c  -o ${OBJECTDIR}/MainLEDTest_V1.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/MainLEDTest_V1.o.d"        -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/MainMotorDriverTest_V1.o: MainMotorDriverTest_V1.c  .generated_files/flags/default/b38e7fb6940b88c4f596a0e30858ebe96d461b0e .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/MainMotorDriverTest_V1.o.d 
	@${RM} ${OBJECTDIR}/MainMotorDriverTest_V1.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  MainMotorDriverTest_V1.c  -o ${OBJECTDIR}/MainMotorDriverTest_V1.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/MainMotorDriverTest_V1.o.d"        -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/MainSensorTest_V1.o: MainSensorTest_V1.c  .generated_files/flags/default/6a433291050464c7242461f0c6bb143ef2ac07e7 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/MainSensorTest_V1.o.d 
	@${RM} ${OBJECTDIR}/MainSensorTest_V1.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  MainSensorTest_V1.c  -o ${OBJECTDIR}/MainSensorTest_V1.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/MainSensorTest_V1.o.d"        -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/MotorDriverAPI.o: MotorDriverAPI.c  .generated_files/flags/default/4da0aeebdeb8413de5d61d532cf5cedaec698f49 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/MotorDriverAPI.o.d 
	@${RM} ${OBJECTDIR}/MotorDriverAPI.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  MotorDriverAPI.c  -o ${OBJECTDIR}/MotorDriverAPI.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/MotorDriverAPI.o.d"        -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/SensorAPI.o: SensorAPI.c  .generated_files/flags/default/17d94252c80241bf124f6d3d36055ba1a3eebfab .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/SensorAPI.o.d 
	@${RM} ${OBJECTDIR}/SensorAPI.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  SensorAPI.c  -o ${OBJECTDIR}/SensorAPI.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/SensorAPI.o.d"        -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/lcd_api_v2.o: lcd_api_v2.c  .generated_files/flags/default/88639c24914c732170d3c9509d5e081b4ef7ebae .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/lcd_api_v2.o.d 
	@${RM} ${OBJECTDIR}/lcd_api_v2.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  lcd_api_v2.c  -o ${OBJECTDIR}/lcd_api_v2.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/lcd_api_v2.o.d"        -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/pattern.o: pattern.c  .generated_files/flags/default/a8a8e42ba3aad4b049f5c939436f783f7f0158ec .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/pattern.o.d 
	@${RM} ${OBJECTDIR}/pattern.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  pattern.c  -o ${OBJECTDIR}/pattern.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/pattern.o.d"        -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assemble
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/blommel_lab2b_asmLib_v1.o: blommel_lab2b_asmLib_v1.s  .generated_files/flags/default/5bef301543c6394152d722289f16156a8ee17c8e .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/blommel_lab2b_asmLib_v1.o.d 
	@${RM} ${OBJECTDIR}/blommel_lab2b_asmLib_v1.o 
	${MP_CC} $(MP_EXTRA_AS_PRE)  blommel_lab2b_asmLib_v1.s  -o ${OBJECTDIR}/blommel_lab2b_asmLib_v1.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -D__DEBUG   -omf=elf -DXPRJ_default=$(CND_CONF)    -Wa,-MD,"${OBJECTDIR}/blommel_lab2b_asmLib_v1.o.d",--defsym=__MPLAB_BUILD=1,--defsym=__ICD2RAM=1,--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,,-g,--no-relax$(MP_EXTRA_AS_POST)  -mdfp="${DFP_DIR}/xc16"
	
else
${OBJECTDIR}/blommel_lab2b_asmLib_v1.o: blommel_lab2b_asmLib_v1.s  .generated_files/flags/default/1cb3fc940828be24e5ba1e0fd60599955f16f72 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/blommel_lab2b_asmLib_v1.o.d 
	@${RM} ${OBJECTDIR}/blommel_lab2b_asmLib_v1.o 
	${MP_CC} $(MP_EXTRA_AS_PRE)  blommel_lab2b_asmLib_v1.s  -o ${OBJECTDIR}/blommel_lab2b_asmLib_v1.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -omf=elf -DXPRJ_default=$(CND_CONF)    -Wa,-MD,"${OBJECTDIR}/blommel_lab2b_asmLib_v1.o.d",--defsym=__MPLAB_BUILD=1,-g,--no-relax$(MP_EXTRA_AS_POST)  -mdfp="${DFP_DIR}/xc16"
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assemblePreproc
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${DISTDIR}/EE2361_Library.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk    
	@${MKDIR} ${DISTDIR} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -o ${DISTDIR}/EE2361_Library.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}      -mcpu=$(MP_PROCESSOR_OPTION)        -D__DEBUG=__DEBUG   -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)   -mreserve=data@0x800:0x81F -mreserve=data@0x820:0x821 -mreserve=data@0x822:0x823 -mreserve=data@0x824:0x825 -mreserve=data@0x826:0x84F   -Wl,,,--defsym=__MPLAB_BUILD=1,--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,-D__DEBUG=__DEBUG,,$(MP_LINKER_FILE_OPTION),--stack=16,--check-sections,--data-init,--pack-data,--handles,--isr,--no-gc-sections,--fill-upper=0,--stackguard=16,--no-force-link,--smart-io,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map",--report-mem,--memorysummary,${DISTDIR}/memoryfile.xml$(MP_EXTRA_LD_POST)  -mdfp="${DFP_DIR}/xc16" 
	
else
${DISTDIR}/EE2361_Library.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk   
	@${MKDIR} ${DISTDIR} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -o ${DISTDIR}/EE2361_Library.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}      -mcpu=$(MP_PROCESSOR_OPTION)        -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -Wl,,,--defsym=__MPLAB_BUILD=1,$(MP_LINKER_FILE_OPTION),--stack=16,--check-sections,--data-init,--pack-data,--handles,--isr,--no-gc-sections,--fill-upper=0,--stackguard=16,--no-force-link,--smart-io,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map",--report-mem,--memorysummary,${DISTDIR}/memoryfile.xml$(MP_EXTRA_LD_POST)  -mdfp="${DFP_DIR}/xc16" 
	${MP_CC_DIR}\\xc16-bin2hex ${DISTDIR}/EE2361_Library.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} -a  -omf=elf   -mdfp="${DFP_DIR}/xc16" 
	
endif


# Subprojects
.build-subprojects:


# Subprojects
.clean-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${OBJECTDIR}
	${RM} -r ${DISTDIR}

# Enable dependency checking
.dep.inc: .depcheck-impl

DEPFILES=$(wildcard ${POSSIBLE_DEPFILES})
ifneq (${DEPFILES},)
include ${DEPFILES}
endif
