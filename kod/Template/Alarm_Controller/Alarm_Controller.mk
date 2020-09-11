##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Debug
ProjectName            :=Alarm_Controller
ConfigurationName      :=Debug
WorkspacePath          :=C:/Users/nazif/Documents/Ovning_2
ProjectPath            :=C:/Users/nazif/Documents/Ovning_2/Alarm_Controller
IntermediateDirectory  :=./Debug
OutDir                 := $(IntermediateDirectory)
CurrentFileName        :=
CurrentFilePath        :=
CurrentFileFullPath    :=
User                   :=nazif
Date                   :=11/09/2020
CodeLitePath           :=C:/cseapp/CodeLite
LinkerName             :=$(CodeLiteDir)/tools/gcc-arm/bin/arm-none-eabi-g++.exe
SharedObjectLinkerName :=$(CodeLiteDir)/tools/gcc-arm/arm-none-eabi-g++.exe -shared -fPIC
ObjectSuffix           :=.o
DependSuffix           :=.o.d
PreprocessSuffix       :=.i
DebugSwitch            :=-g 
IncludeSwitch          :=-I
LibrarySwitch          :=-l
OutputSwitch           :=-o 
LibraryPathSwitch      :=-L
PreprocessorSwitch     :=-D
SourceSwitch           :=-c 
OutputFile             :=$(IntermediateDirectory)/$(ProjectName).elf
Preprocessors          :=
ObjectSwitch           :=-o 
ArchiveOutputSwitch    := 
PreprocessOnlySwitch   :=-E
ObjectsFileList        :="Alarm_Controller.txt"
PCHCompileFlags        :=
MakeDirCommand         :=makedir
RcCmpOptions           := 
RcCompilerName         :=
LinkOptions            :=  -T$(ProjectPath)/md407-ram.x -L$(ARM_V6LIB) -L$(ARM_GCC_V6LIB) -nostartfiles
IncludePath            :=  $(IncludeSwitch). $(IncludeSwitch). $(IncludeSwitch)../can/inc 
IncludePCH             := 
RcIncludePath          := 
Libs                   := $(LibrarySwitch)gcc $(LibrarySwitch)c_nano 
ArLibs                 :=  "gcc" "c_nano" 
LibPath                := $(LibraryPathSwitch). 

##
## Common variables
## AR, CXX, CC, AS, CXXFLAGS and CFLAGS can be overriden using an environment variables
##
AR       := $(CodeLiteDir)/tools/gcc-arm/bin/arm-none-eabi-ar.exe rcu
CXX      := $(CodeLiteDir)/tools/gcc-arm/bin/arm-none-eabi-g++.exe
CC       := $(CodeLiteDir)/tools/gcc-arm/bin/arm-none-eabi-gcc.exe
CXXFLAGS :=  -g -O0 -W $(Preprocessors)
CFLAGS   :=  -Wa,-adhln=test.s -g -O0 -w -mthumb -march=armv6-m  -mfloat-abi=soft -std=c99 -mthumb -march=armv6-m $(Preprocessors)
ASFLAGS  := 
AS       := $(CodeLiteDir)/tools/gcc-arm/bin/arm-none-eabi-as.exe


##
## User defined environment variables
##
CodeLiteDir:=C:\cseapp\CodeLite
ARM_V6LIB:=$(CodeLiteDir)/tools/gcc-arm/arm-none-eabi/lib/thumb/v6-m
ARM_GCC_V6LIB:=$(CodeLiteDir)/tools/gcc-arm/lib/gcc/arm-none-eabi/7.2.1/thumb/v6-m
ARM_M4FPLIB:=$(CodeLiteDir)/tools/gcc-arm/arm-none-eabi/lib/thumb/v7e-m/fpv4-sp/hard
ARM_GCC_M4FPLIB:=$(CodeLiteDir)/tools/gcc-arm/lib/gcc/arm-none-eabi/7.2.1/thumb/v7e-m
Objects0=$(IntermediateDirectory)/up_can_src_stm32f4xx_rcc.c$(ObjectSuffix) $(IntermediateDirectory)/up_can_src_stm32f4xx_gpio.c$(ObjectSuffix) $(IntermediateDirectory)/up_can_src_stm32f4xx_can.c$(ObjectSuffix) $(IntermediateDirectory)/up_can_src_can.c$(ObjectSuffix) $(IntermediateDirectory)/startup.c$(ObjectSuffix) 



Objects=$(Objects0) 

##
## Main Build Targets 
##
.PHONY: all clean PreBuild PrePreBuild PostBuild MakeIntermediateDirs
all: $(OutputFile)

$(OutputFile): $(IntermediateDirectory)/.d $(Objects) 
	@$(MakeDirCommand) $(@D)
	@echo "" > $(IntermediateDirectory)/.d
	@echo $(Objects0)  > $(ObjectsFileList)
	$(LinkerName) $(OutputSwitch)$(OutputFile) @$(ObjectsFileList) $(LibPath) $(Libs) $(LinkOptions)

PostBuild:
	@echo Executing Post Build commands ...
	arm-none-eabi-objcopy -S -O srec  ./Debug/Alarm_Controller.elf ./Debug/Alarm_Controller.s19
	arm-none-eabi-objdump -D -S ./Debug/Alarm_Controller.elf > ./Debug/Alarm_Controller.dass
	@echo Done

MakeIntermediateDirs:
	@$(MakeDirCommand) "./Debug"


$(IntermediateDirectory)/.d:
	@$(MakeDirCommand) "./Debug"

PreBuild:


##
## Objects
##
$(IntermediateDirectory)/up_can_src_stm32f4xx_rcc.c$(ObjectSuffix): ../can/src/stm32f4xx_rcc.c $(IntermediateDirectory)/up_can_src_stm32f4xx_rcc.c$(DependSuffix)
	$(CC) $(SourceSwitch) "C:/Users/nazif/Documents/Ovning_2/can/src/stm32f4xx_rcc.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_can_src_stm32f4xx_rcc.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_can_src_stm32f4xx_rcc.c$(DependSuffix): ../can/src/stm32f4xx_rcc.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_can_src_stm32f4xx_rcc.c$(ObjectSuffix) -MF$(IntermediateDirectory)/up_can_src_stm32f4xx_rcc.c$(DependSuffix) -MM ../can/src/stm32f4xx_rcc.c

$(IntermediateDirectory)/up_can_src_stm32f4xx_rcc.c$(PreprocessSuffix): ../can/src/stm32f4xx_rcc.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_can_src_stm32f4xx_rcc.c$(PreprocessSuffix) ../can/src/stm32f4xx_rcc.c

$(IntermediateDirectory)/up_can_src_stm32f4xx_gpio.c$(ObjectSuffix): ../can/src/stm32f4xx_gpio.c $(IntermediateDirectory)/up_can_src_stm32f4xx_gpio.c$(DependSuffix)
	$(CC) $(SourceSwitch) "C:/Users/nazif/Documents/Ovning_2/can/src/stm32f4xx_gpio.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_can_src_stm32f4xx_gpio.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_can_src_stm32f4xx_gpio.c$(DependSuffix): ../can/src/stm32f4xx_gpio.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_can_src_stm32f4xx_gpio.c$(ObjectSuffix) -MF$(IntermediateDirectory)/up_can_src_stm32f4xx_gpio.c$(DependSuffix) -MM ../can/src/stm32f4xx_gpio.c

$(IntermediateDirectory)/up_can_src_stm32f4xx_gpio.c$(PreprocessSuffix): ../can/src/stm32f4xx_gpio.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_can_src_stm32f4xx_gpio.c$(PreprocessSuffix) ../can/src/stm32f4xx_gpio.c

$(IntermediateDirectory)/up_can_src_stm32f4xx_can.c$(ObjectSuffix): ../can/src/stm32f4xx_can.c $(IntermediateDirectory)/up_can_src_stm32f4xx_can.c$(DependSuffix)
	$(CC) $(SourceSwitch) "C:/Users/nazif/Documents/Ovning_2/can/src/stm32f4xx_can.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_can_src_stm32f4xx_can.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_can_src_stm32f4xx_can.c$(DependSuffix): ../can/src/stm32f4xx_can.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_can_src_stm32f4xx_can.c$(ObjectSuffix) -MF$(IntermediateDirectory)/up_can_src_stm32f4xx_can.c$(DependSuffix) -MM ../can/src/stm32f4xx_can.c

$(IntermediateDirectory)/up_can_src_stm32f4xx_can.c$(PreprocessSuffix): ../can/src/stm32f4xx_can.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_can_src_stm32f4xx_can.c$(PreprocessSuffix) ../can/src/stm32f4xx_can.c

$(IntermediateDirectory)/up_can_src_can.c$(ObjectSuffix): ../can/src/can.c $(IntermediateDirectory)/up_can_src_can.c$(DependSuffix)
	$(CC) $(SourceSwitch) "C:/Users/nazif/Documents/Ovning_2/can/src/can.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_can_src_can.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_can_src_can.c$(DependSuffix): ../can/src/can.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_can_src_can.c$(ObjectSuffix) -MF$(IntermediateDirectory)/up_can_src_can.c$(DependSuffix) -MM ../can/src/can.c

$(IntermediateDirectory)/up_can_src_can.c$(PreprocessSuffix): ../can/src/can.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_can_src_can.c$(PreprocessSuffix) ../can/src/can.c

$(IntermediateDirectory)/startup.c$(ObjectSuffix): startup.c $(IntermediateDirectory)/startup.c$(DependSuffix)
	$(CC) $(SourceSwitch) "C:/Users/nazif/Documents/Ovning_2/Alarm_Controller/startup.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/startup.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/startup.c$(DependSuffix): startup.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/startup.c$(ObjectSuffix) -MF$(IntermediateDirectory)/startup.c$(DependSuffix) -MM startup.c

$(IntermediateDirectory)/startup.c$(PreprocessSuffix): startup.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/startup.c$(PreprocessSuffix) startup.c


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) -r ./Debug/


