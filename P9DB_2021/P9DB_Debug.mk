##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Debug
ProjectName            :=P9DB_Debug
ConfigurationName      :=Debug
WorkspacePath          :=/home/dexter/Desktop/P9/P9DB_2021
ProjectPath            :=/home/dexter/Desktop/P9/P9DB_2021
IntermediateDirectory  :=./Debug
OutDir                 := $(IntermediateDirectory)
CurrentFileName        :=
CurrentFilePath        :=
CurrentFileFullPath    :=
User                   :=dexter
Date                   :=31/10/21
CodeLitePath           :=/home/dexter/.codelite
LinkerName             :=/usr/bin/g++
SharedObjectLinkerName :=/usr/bin/g++ -shared -fPIC
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
OutputFile             :=$(IntermediateDirectory)/$(ProjectName)
Preprocessors          :=
ObjectSwitch           :=-o 
ArchiveOutputSwitch    := 
PreprocessOnlySwitch   :=-E
ObjectsFileList        :="P9DB_Debug.txt"
PCHCompileFlags        :=
MakeDirCommand         :=mkdir -p
LinkOptions            :=  -lpthread 
IncludePath            :=  $(IncludeSwitch). $(IncludeSwitch). 
IncludePCH             := 
RcIncludePath          := 
Libs                   := 
ArLibs                 :=  
LibPath                := $(LibraryPathSwitch). 

##
## Common variables
## AR, CXX, CC, AS, CXXFLAGS and CFLAGS can be overriden using an environment variables
##
AR       := /usr/bin/ar rcu
CXX      := /usr/bin/g++
CC       := /usr/bin/gcc
CXXFLAGS :=  -g -O0 -Wall $(Preprocessors)
CFLAGS   :=  -g -O0 -Wall $(Preprocessors)
ASFLAGS  := 
AS       := /usr/bin/as


##
## User defined environment variables
##
CodeLiteDir:=/usr/share/codelite
Objects0=$(IntermediateDirectory)/Storage_Impl_IndexTable.cpp$(ObjectSuffix) $(IntermediateDirectory)/Platform_Impl_Platform.cpp$(ObjectSuffix) $(IntermediateDirectory)/Interpreter_Impl_QueryOperation.cpp$(ObjectSuffix) $(IntermediateDirectory)/Interpreter_Impl_QuerySyntax.cpp$(ObjectSuffix) $(IntermediateDirectory)/Interpreter_Impl_InterpreterStandard.cpp$(ObjectSuffix) $(IntermediateDirectory)/_ExecutorDebug_Main.cpp$(ObjectSuffix) $(IntermediateDirectory)/Storage_Impl_DocumentIdentifier.cpp$(ObjectSuffix) $(IntermediateDirectory)/Storage_Impl_Storage.cpp$(ObjectSuffix) $(IntermediateDirectory)/Storage_Impl_Partition.cpp$(ObjectSuffix) 



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

MakeIntermediateDirs:
	@test -d ./Debug || $(MakeDirCommand) ./Debug


$(IntermediateDirectory)/.d:
	@test -d ./Debug || $(MakeDirCommand) ./Debug

PreBuild:


##
## Objects
##
$(IntermediateDirectory)/Storage_Impl_IndexTable.cpp$(ObjectSuffix): Storage/Impl/IndexTable.cpp $(IntermediateDirectory)/Storage_Impl_IndexTable.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dexter/Desktop/P9/P9DB_2021/Storage/Impl/IndexTable.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Storage_Impl_IndexTable.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Storage_Impl_IndexTable.cpp$(DependSuffix): Storage/Impl/IndexTable.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Storage_Impl_IndexTable.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Storage_Impl_IndexTable.cpp$(DependSuffix) -MM Storage/Impl/IndexTable.cpp

$(IntermediateDirectory)/Storage_Impl_IndexTable.cpp$(PreprocessSuffix): Storage/Impl/IndexTable.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Storage_Impl_IndexTable.cpp$(PreprocessSuffix) Storage/Impl/IndexTable.cpp

$(IntermediateDirectory)/Platform_Impl_Platform.cpp$(ObjectSuffix): Platform/Impl/Platform.cpp $(IntermediateDirectory)/Platform_Impl_Platform.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dexter/Desktop/P9/P9DB_2021/Platform/Impl/Platform.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Platform_Impl_Platform.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Platform_Impl_Platform.cpp$(DependSuffix): Platform/Impl/Platform.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Platform_Impl_Platform.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Platform_Impl_Platform.cpp$(DependSuffix) -MM Platform/Impl/Platform.cpp

$(IntermediateDirectory)/Platform_Impl_Platform.cpp$(PreprocessSuffix): Platform/Impl/Platform.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Platform_Impl_Platform.cpp$(PreprocessSuffix) Platform/Impl/Platform.cpp

$(IntermediateDirectory)/Interpreter_Impl_QueryOperation.cpp$(ObjectSuffix): Interpreter/Impl/QueryOperation.cpp $(IntermediateDirectory)/Interpreter_Impl_QueryOperation.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dexter/Desktop/P9/P9DB_2021/Interpreter/Impl/QueryOperation.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Interpreter_Impl_QueryOperation.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Interpreter_Impl_QueryOperation.cpp$(DependSuffix): Interpreter/Impl/QueryOperation.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Interpreter_Impl_QueryOperation.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Interpreter_Impl_QueryOperation.cpp$(DependSuffix) -MM Interpreter/Impl/QueryOperation.cpp

$(IntermediateDirectory)/Interpreter_Impl_QueryOperation.cpp$(PreprocessSuffix): Interpreter/Impl/QueryOperation.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Interpreter_Impl_QueryOperation.cpp$(PreprocessSuffix) Interpreter/Impl/QueryOperation.cpp

$(IntermediateDirectory)/Interpreter_Impl_QuerySyntax.cpp$(ObjectSuffix): Interpreter/Impl/QuerySyntax.cpp $(IntermediateDirectory)/Interpreter_Impl_QuerySyntax.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dexter/Desktop/P9/P9DB_2021/Interpreter/Impl/QuerySyntax.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Interpreter_Impl_QuerySyntax.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Interpreter_Impl_QuerySyntax.cpp$(DependSuffix): Interpreter/Impl/QuerySyntax.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Interpreter_Impl_QuerySyntax.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Interpreter_Impl_QuerySyntax.cpp$(DependSuffix) -MM Interpreter/Impl/QuerySyntax.cpp

$(IntermediateDirectory)/Interpreter_Impl_QuerySyntax.cpp$(PreprocessSuffix): Interpreter/Impl/QuerySyntax.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Interpreter_Impl_QuerySyntax.cpp$(PreprocessSuffix) Interpreter/Impl/QuerySyntax.cpp

$(IntermediateDirectory)/Interpreter_Impl_InterpreterStandard.cpp$(ObjectSuffix): Interpreter/Impl/InterpreterStandard.cpp $(IntermediateDirectory)/Interpreter_Impl_InterpreterStandard.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dexter/Desktop/P9/P9DB_2021/Interpreter/Impl/InterpreterStandard.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Interpreter_Impl_InterpreterStandard.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Interpreter_Impl_InterpreterStandard.cpp$(DependSuffix): Interpreter/Impl/InterpreterStandard.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Interpreter_Impl_InterpreterStandard.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Interpreter_Impl_InterpreterStandard.cpp$(DependSuffix) -MM Interpreter/Impl/InterpreterStandard.cpp

$(IntermediateDirectory)/Interpreter_Impl_InterpreterStandard.cpp$(PreprocessSuffix): Interpreter/Impl/InterpreterStandard.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Interpreter_Impl_InterpreterStandard.cpp$(PreprocessSuffix) Interpreter/Impl/InterpreterStandard.cpp

$(IntermediateDirectory)/_ExecutorDebug_Main.cpp$(ObjectSuffix): _ExecutorDebug/Main.cpp $(IntermediateDirectory)/_ExecutorDebug_Main.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dexter/Desktop/P9/P9DB_2021/_ExecutorDebug/Main.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/_ExecutorDebug_Main.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/_ExecutorDebug_Main.cpp$(DependSuffix): _ExecutorDebug/Main.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/_ExecutorDebug_Main.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/_ExecutorDebug_Main.cpp$(DependSuffix) -MM _ExecutorDebug/Main.cpp

$(IntermediateDirectory)/_ExecutorDebug_Main.cpp$(PreprocessSuffix): _ExecutorDebug/Main.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/_ExecutorDebug_Main.cpp$(PreprocessSuffix) _ExecutorDebug/Main.cpp

$(IntermediateDirectory)/Storage_Impl_DocumentIdentifier.cpp$(ObjectSuffix): Storage/Impl/DocumentIdentifier.cpp $(IntermediateDirectory)/Storage_Impl_DocumentIdentifier.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dexter/Desktop/P9/P9DB_2021/Storage/Impl/DocumentIdentifier.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Storage_Impl_DocumentIdentifier.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Storage_Impl_DocumentIdentifier.cpp$(DependSuffix): Storage/Impl/DocumentIdentifier.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Storage_Impl_DocumentIdentifier.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Storage_Impl_DocumentIdentifier.cpp$(DependSuffix) -MM Storage/Impl/DocumentIdentifier.cpp

$(IntermediateDirectory)/Storage_Impl_DocumentIdentifier.cpp$(PreprocessSuffix): Storage/Impl/DocumentIdentifier.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Storage_Impl_DocumentIdentifier.cpp$(PreprocessSuffix) Storage/Impl/DocumentIdentifier.cpp

$(IntermediateDirectory)/Storage_Impl_Storage.cpp$(ObjectSuffix): Storage/Impl/Storage.cpp $(IntermediateDirectory)/Storage_Impl_Storage.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dexter/Desktop/P9/P9DB_2021/Storage/Impl/Storage.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Storage_Impl_Storage.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Storage_Impl_Storage.cpp$(DependSuffix): Storage/Impl/Storage.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Storage_Impl_Storage.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Storage_Impl_Storage.cpp$(DependSuffix) -MM Storage/Impl/Storage.cpp

$(IntermediateDirectory)/Storage_Impl_Storage.cpp$(PreprocessSuffix): Storage/Impl/Storage.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Storage_Impl_Storage.cpp$(PreprocessSuffix) Storage/Impl/Storage.cpp

$(IntermediateDirectory)/Storage_Impl_Partition.cpp$(ObjectSuffix): Storage/Impl/Partition.cpp $(IntermediateDirectory)/Storage_Impl_Partition.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dexter/Desktop/P9/P9DB_2021/Storage/Impl/Partition.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Storage_Impl_Partition.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Storage_Impl_Partition.cpp$(DependSuffix): Storage/Impl/Partition.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Storage_Impl_Partition.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Storage_Impl_Partition.cpp$(DependSuffix) -MM Storage/Impl/Partition.cpp

$(IntermediateDirectory)/Storage_Impl_Partition.cpp$(PreprocessSuffix): Storage/Impl/Partition.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Storage_Impl_Partition.cpp$(PreprocessSuffix) Storage/Impl/Partition.cpp


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) -r ./Debug/


