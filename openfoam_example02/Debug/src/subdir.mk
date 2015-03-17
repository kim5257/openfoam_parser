################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/openfoam_example02.cpp 

OBJS += \
./src/openfoam_example02.o 

CPP_DEPS += \
./src/openfoam_example02.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -std=c++0x -I"/home/asran/workspace/OpenFOAM_parser/OpenFOAM_parser" -I"/home/asran/workspace/OpenFOAM_parser/OpenFOAM_parser/include" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


