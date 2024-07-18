#include "collentdatathread.h"
#include <stdint.h>
#include <string.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <QDebug>

#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#include <iomanip>
#include <cstdint>
#include <iostream>
#include <gpiod.h>
// BH1750FVI地址和命令
const char BH1750_address = 0x23; // 通常是0x23，但有些模块可能是0x5C
const char BH1750_POWER_ON = 0x01;
const char BH1750_RESET = 0x07;
const char BH1750_CONTINUOUS_HIGH_RES_MODE = 0x10;

// 打开I2C总线并设置设备
int initI2CBus(const char* bus, int address) {
  int file;
  if ((file = open(bus, O_RDWR)) < 0) {
    std::cout << "Failed to open the bus." << std::endl;
    return -1;
  }

  if (ioctl(file, I2C_SLAVE, address) < 0) {
    std::cout << "Failed to connect to the sensor." << std::endl;
    return -1;
  }

  return file;
}

// 发送命令到传感器
bool writeI2C(int file, char command) {
  if (write(file, &command, 1) != 1) {
    std::cout << "Failed to write command to the sensor." << std::endl;
    return false;
  }
  return true;
}

// 读取传感器数据
int readI2CLight(int file) {
  char buf[2] = {0};
  if (read(file, buf, 2) != 2) {
    std::cerr << "Failed to read light level from sensor." << std::endl;
    return -1;
  }
  int value = (buf[0] << 8) | buf[1]; // 将两个字节的数据合并为一个整数
  return value;
}

// SHTC3 I2C address
constexpr uint8_t SHTC3_ADDRESS = 0x70;

// SHTC3 Commands
constexpr uint16_t MEASURE_HIGH_PRECISION = 0x7CA2;  // Measurement command

// Function to initialize the I2C interface
int init_i2c(const char* filename) {
    int file_i2c = open(filename, O_RDWR);
    if (file_i2c < 0) {
        std::cerr << "Failed to open the I2C bus" << std::endl;
        return -1;
    }
    if (ioctl(file_i2c, I2C_SLAVE, SHTC3_ADDRESS) < 0) {
        std::cerr << "Failed to communicate with the I2C device" << std::endl;
        return -1;
    }
    return file_i2c;
}

// Function to write a command to the SHTC3
bool write_command(int file_i2c, uint16_t command) {
    uint8_t buffer[2] = {uint8_t(command >> 8), uint8_t(command & 0xFF)};
    if (write(file_i2c, buffer, 2) != 2) {
        std::cerr << "Failed to write to the I2C device" << std::endl;
        return false;
    }
    return true;
}

// Function to read temperature and humidity from SHTC3
bool read_temperature_humidity(int file_i2c, float &temperature, float &humidity) {
    uint8_t data[6];
    if (read(file_i2c, data, 6) != 6) {
        std::cerr << "Failed to read from the I2C device" << std::endl;
        return false;
    }

    uint16_t raw_temperature = (data[0] << 8) | data[1];
    uint16_t  raw_humidity = (data[3] << 8) | data[4];

    // Convert the raw values to actual temperature and humidity
    humidity = 100.0 * (float)raw_humidity / 65535.0;
    temperature = -45.0 + 175.0 * (float)raw_temperature / 65535.0;

    return true;
}

int get_fire_info(const char *chipname, int line_num) {
    struct gpiod_chip *chip;
    struct gpiod_line *line;

    // 打开GPIO芯片
    chip = gpiod_chip_open_by_name(chipname);
    if (!chip) {
        perror("Failed to open GPIO chip");
        return -1;
    }

    // 获取GPIO引脚
    line = gpiod_chip_get_line(chip, line_num);
    if (!line) {
        perror("Failed to get GPIO line");
        gpiod_chip_close(chip);
        return -1;
    }
    if (gpiod_line_request_input(line, "fire_sensor")) {
        perror("failed to request GPIO line as input");
        gpiod_line_release(line);
        gpiod_chip_close(chip);
    }
    int flag = gpiod_line_get_value(line);
    if (flag == 0) {
//        std::cout<<"no fire"<<std::endl;
        gpiod_line_release(line);
        gpiod_chip_close(chip);
        return 0;
    } else if (flag == 1) {
        std::cout<<"fire detected"<<std::endl;
        gpiod_line_release(line);
        gpiod_chip_close(chip);
        return 1;
    } else {
        std::cout<<"get fire info error"<<std::endl;
        gpiod_line_release(line);
        gpiod_chip_close(chip);
        return -1;
    }
}

//void turn_warn_on(const char *chipname, int line_num) {
//    struct gpiod_chip *chip;
//    struct gpiod_line *line;

//    // 打开GPIO芯片
//    chip = gpiod_chip_open_by_name(chipname);
//    if (!chip) {
//        perror("Failed to open GPIO chip");
//        return;
//    }

//    // 获取GPIO引脚
//    line = gpiod_chip_get_line(chip, line_num);
//    if (!line) {
//        perror("Failed to get GPIO line");
//        gpiod_chip_close(chip);
//        return;
//    }

//    if (gpiod_line_request_output(line, "warn-on", 0)) {
//        perror("Failed to request GPIO line as output");
//        gpiod_line_release(line);
//        gpiod_chip_close(chip);
//        return;
//    }

//    gpiod_line_release(line);
//    gpiod_chip_close(chip);
//}

CollentdataThread::CollentdataThread()
{

}
void CollentdataThread::run()
{
//    int temp_raw = 0;
//    int temp_offset = 0;
//    float temp_scale = 0;
//    int hum_raw = 0;
//    int hum_offset = 0;
//    float hum_scale = 0;
//    float tem_float =0;
//    float hum_float =0;
//    float ill_float =0;
//    const char *device1 ="iio:device0";//温湿度
//    const char *device2 ="iio:device1";//光照
    QString hum;
    QString tem;
    QString ill;

    while (1)
    {
//        /*read temp data*/
//        read_sysfs_int(device1, "in_temp_raw", &temp_raw);
//        read_sysfs_int(device1, "in_temp_offset", &temp_offset);
//        read_sysfs_float(device1, "in_temp_scale", &temp_scale);
//        tem_float =(temp_raw + temp_offset) * temp_scale / 1000;
//        tem =QString::number(tem_float,'f', 2);
//        read_sysfs_int(device1, "in_humidityrelative_raw", &hum_raw);
//        read_sysfs_int(device1, "in_humidityrelative_offset", &hum_offset);
//        read_sysfs_float(device1, "in_humidityrelative_scale", &hum_scale);
//        hum_float = (hum_raw + hum_offset) * hum_scale / 1000;
//        hum =QString::number(hum_float,'f', 2);
//        read_sysfs_float(device2, "in_illuminance_input", &ill_float);
//        ill =QString::number(ill_float,'f', 2);
//        emit send(tem,hum,ill);
//        sleep(2);
//        if (get_fire_info("gpiochip0", 14) == 1) {
//            std::cout<<"on fire"<<std::endl;
//            emit turn_warn_on("gpiochip0", 14);
//        }
        int file_i2c = init_i2c("/dev/i2c-2");
        if (file_i2c < 0) {
//            return 1;
        }

        // Send the measurement command
        if (!write_command(file_i2c, MEASURE_HIGH_PRECISION)) {
//            return 1;
        }
      const char* i2cBus = "/dev/i2c-3";

      // 初始化I2C总线和设备
      int file = initI2CBus(i2cBus, BH1750_address);
      if(file < 0) {
//        return 1;
      }

      // 打开传感器
      if(!writeI2C(file, BH1750_POWER_ON)){
//        return 1;
      }

      // 重置传感器
      if(!writeI2C(file, BH1750_RESET)){
//        return 1;
      }

      // 设置为连续高分辨率模式
      if(!writeI2C(file, BH1750_CONTINUOUS_HIGH_RES_MODE)){
//        return 1;
      }

      // 给传感器时间进行测量
      sleep(1);

      // 读取光照强度
      int lightLevel = readI2CLight(file);
      if(lightLevel < 0) {
//        return 1;
      }
      float temperature, humidity;
      read_temperature_humidity(file_i2c, temperature, humidity);
      tem = QString::number(temperature,'f', 2);
      hum = QString::number(humidity,'f', 2);
      // 转换为勒克斯（每单位1 lx）
      double lux = lightLevel / 1.2;
      ill = QString::number(lux,'f', 2);
      emit send(tem,hum,ill);
    }
}
//int CollentdataThread::read_sysfs_float(const char *device, const char *filename, float *val)
//{
//    int ret = 0;
//    FILE *sysfsfp;
//    char temp[128];
//    memset(temp, '0', 128);
//    ret = sprintf(temp, "/sys/bus/iio/devices/%s/%s", device, filename);
//    if (ret < 0)
//        goto error;
//    sysfsfp = fopen(temp, "r");
//    if (!sysfsfp)
//    {
//        ret = -errno;
//        goto error;
//    }
//    errno = 0;
//    if (fscanf(sysfsfp, "%f\n", val) != 1)
//    {
//        ret = errno ? -errno : -ENODATA;
//        if (fclose(sysfsfp))
//            perror("read_sysfs_float(): Failed to close dir");
//        goto error;
//    }
//    if (fclose(sysfsfp))
//        ret = -errno;
//error:
//    return ret;
//}
//int CollentdataThread::read_sysfs_int(const char *device, const char *filename, int *val)
//{
//    int ret = 0;
//    FILE *sysfsfp;
//    char temp[128];
//    memset(temp, '0', 128);
//    ret = sprintf(temp, "/sys/bus/iio/devices/%s/%s", device, filename);
//    if (ret < 0)
//        goto error;
//    sysfsfp = fopen(temp, "r");
//    if (!sysfsfp)
//    {
//        ret = -errno;
//        goto error;
//    }
//    errno = 0;
//    if (fscanf(sysfsfp, "%d\n", val) != 1)
//    {
//        ret = errno ? -errno : -ENODATA;
//        if (fclose(sysfsfp))
//            perror("read_sysfs_float(): Failed to close dir");
//        goto error;
//    }
//    if (fclose(sysfsfp))
//        ret = -errno;
//error:
//    return ret;
//}
