################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/OpenFOAM_parser.cpp \
../src/openfoam_buf.cpp \
../src/openfoam_parser.cpp \
../src/openfoam_parser_labellist.cpp 

OBJS += \
./src/OpenFOAM_parser.o \
./src/openfoam_buf.o \
./src/openfoam_parser.o \
./src/openfoam_parser_labellist.o 

CPP_DEPS += \
./src/OpenFOAM_parser.d \
./src/openfoam_buf.d \
./src/openfoam_parser.d \
./src/openfoam_parser_labellist.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -std=c++0x -I"/home/asran/workspace/OpenFOAM_parser/OpenFOAM_parser/include" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


