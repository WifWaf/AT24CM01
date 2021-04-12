#include "AT24CM01.h"
#include "esp_log.h"
#include "driver/i2c.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define DEBUG 1

#define TAG_AT24 "AT24CM01"

#define I2C_MASTER_SCL_IO 22        /*!< gpio number for I2C master clock */
#define I2C_MASTER_SDA_IO 21        /*!< gpio number for I2C master data  */
#define I2C_MASTER_FREQ_HZ 400000   /*!< I2C master clock frequency */
#define I2C_MASTER_TX_BUF_DISABLE 0 /*!< I2C master doesn't need buffer */
#define I2C_MASTER_RX_BUF_DISABLE 0 /*!< I2C master doesn't need buffer */

#define ACK_CHECK_EN 0x1  /*!< I2C master will check ack from slave*/
#define ACK_CHECK_DIS 0x0 /*!< I2C master will not check ack from slave */
#define ACK_VAL 0x0       /*!< I2C ack value */
#define NACK_VAL 0x1      /*!< I2C nack value */


bool AT24CM01begin()
{
  if(AT24CM01I2CSetup() != ESP_OK)
    return false;

  return true;
}

uint8_t AT24CM01I2CSetup()
{
  int i2c_master_port = 0;
  
  i2c_config_t conf;
  conf.mode = I2C_MODE_MASTER;
  conf.sda_io_num = I2C_MASTER_SDA_IO;
  conf.sda_pullup_en = GPIO_PULLUP_ENABLE;
  conf.scl_io_num = I2C_MASTER_SCL_IO;
  conf.scl_pullup_en = GPIO_PULLUP_ENABLE;
  conf.master.clk_speed = I2C_MASTER_FREQ_HZ;  
  i2c_param_config(i2c_master_port, &conf);

  return i2c_driver_install(i2c_master_port, conf.mode,
                            I2C_MASTER_RX_BUF_DISABLE,
                            I2C_MASTER_TX_BUF_DISABLE, 0); 
}

uint8_t AT24CM01read(uint32_t address)
{
  uint8_t rBuff = 0, *prBuff;
  prBuff = &rBuff;

  uint8_t DAdr = (uint8_t)((AT24CM01_WORD_ADR << 16 | address) >> 16);

  ESP_LOGI(TAG_AT24, "<AT24CM01read>\tDevice Address\t%#03x", DAdr >> 1);

  i2c_cmd_handle_t cmd = i2c_cmd_link_create();

  ESP_ERROR_CHECK(i2c_master_start(cmd));
  ESP_ERROR_CHECK(i2c_master_write_byte(cmd, DAdr | I2C_MASTER_WRITE, ACK_CHECK_EN));  
  ESP_ERROR_CHECK(i2c_master_write_byte(cmd, (uint8_t)((address & 0xFFFF) >> 8), ACK_CHECK_EN));
  ESP_ERROR_CHECK(i2c_master_write_byte(cmd, (uint8_t)(address & 0xFF), ACK_CHECK_EN)); 
  //ESP_ERROR_CHECK(i2c_master_stop(cmd));
  
  ESP_ERROR_CHECK(i2c_master_start(cmd));
  ESP_ERROR_CHECK(i2c_master_write_byte(cmd, DAdr | I2C_MASTER_WRITE, ACK_CHECK_EN));
  ESP_ERROR_CHECK(i2c_master_read_byte(cmd, prBuff, ACK_VAL));
  ESP_ERROR_CHECK(i2c_master_stop(cmd));

  ESP_ERROR_CHECK(i2c_master_cmd_begin(0, cmd, (2000 / portTICK_RATE_MS)));
  i2c_cmd_link_delete(cmd);
  
  ESP_LOGI(TAG_AT24, "<AT24CM01read>\Value Returned\t%#03x", *prBuff);

  return 0;
}

