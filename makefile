# Compiler settings
CC := gcc
CFLAGS := -Wall -Wextra -Werror -lm

# Target executable name
TARGET := inclinometer_app

# Source files directory
SRC_DIR := main_app
MW_DIR := mware
DRIVER_DIR := driver/LSM6DSM
PLATFORM_DIR := platform

# Source files
SRCS := $(wildcard $(SRC_DIR)/*.c)
SRCS += $(wildcard $(MW_DIR)/*.c)
SRCS += $(wildcard $(PLATFORM_DIR)/*.c)  

# Header files
HEADERS := $(wildcard $(MW_DIR)/*.h)
HEADERS += $(wildcard $(DRIVER_DIR)/*.h)
HEADERS += $(wildcard $(PLATFORM_DIR)/*.h) 
HEADERS += $(wildcard driver/STM32CubeF4Dr/Drivers/STM32F4xx_HAL_Driver/Inc/*.h)
HEADERS += $(wildcard driver/STM32CubeF4Dr/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy/*.h)
HEADERS += $(wildcard driver/STM32CubeF4Dr/Drivers/CMSIS/Device/ST/STM32F4xx/Include/*.h)

# Object files
OBJS := $(SRCS:.c=.o)

# Compiler flags for driver integration
DRIVER_LDFLAGS := -L. -llsm6dsm_driver -lm

# Main build target
$(TARGET): $(OBJS) liblsm6dsm_driver.a
	$(CC) $(CFLAGS) $(OBJS) $(DRIVER_LDFLAGS) -o $@

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -I$(MW_DIR) -I$(DRIVER_DIR) -I$(PLATFORM_DIR) -Idriver/STM32CubeF4Dr/Drivers/STM32F4xx_HAL_Driver/Inc -Idriver/STM32CubeF4Dr/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -Idriver/STM32CubeF4Dr/Drivers/CMSIS/Device/ST/STM32F4xx/Include -c $< -o $@

# Compile driver files and create library
liblsm6dsm_driver.a: $(DRIVER_DIR)/lsm6dsm_reg.c $(DRIVER_DIR)/lsm6dsm_reg.h
	$(CC) $(CFLAGS) -I$(DRIVER_DIR) -c $< -o driver_objs.o
	ar rcs $@ driver_objs.o

.PHONY: clean
clean:
	rm -f $(TARGET) $(OBJS) liblsm6dsm_driver.a driver_objs.o
