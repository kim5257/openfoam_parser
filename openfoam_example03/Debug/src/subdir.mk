################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/boundaries.cpp \
../src/cells.cpp \
../src/faces.cpp \
../src/neighbors.cpp \
../src/openfoam_example03.cpp \
../src/owners.cpp \
../src/points.cpp \
../src/volumes.cpp 

OBJS += \
./src/boundaries.o \
./src/cells.o \
./src/faces.o \
./src/neighbors.o \
./src/openfoam_example03.o \
./src/owners.o \
./src/points.o \
./src/volumes.o 

CPP_DEPS += \
./src/boundaries.d \
./src/cells.d \
./src/faces.d \
./src/neighbors.d \
./src/openfoam_example03.d \
./src/owners.d \
./src/points.d \
./src/volumes.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -std=c++0x -I"/home/asran/workspace/OpenFOAM_parser/OpenFOAM_parser" -I"/home/asran/workspace/OpenFOAM_parser/OpenFOAM_parser/include" -I"/home/asran/workspace/OpenFOAM_parser/openfoam_example03/include" -I../../../SparseMatrix/matrix/matrix/include -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


