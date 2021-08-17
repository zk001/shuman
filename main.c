#include "../../drivers.h"
#include "../../genfsk_ll/genfsk_ll.h"
#include "../../common/mempool.h"
#include "key.h"
#include "rf.h"
#include "app.h"
#include "led.h"
#include "dc.h"
#include "n_timer.h"
#include "board.h"
#include "mac_id.h"
#include "gpio_key.h"
#include "gpio_led.h"
#include "power_saving.h"
#include "ssd1306_oled.h"
#include "wakeup.h"
#include "i2c_gpio_set.h"

static u32 display_wait_time;
MEMPOOL_DECLARE(KEY_EVENT_POOL, KEY_EVENT_POOL_MEM, sizeof(mem_block_t) + sizeof(event_handler_t), MAX_EVENT);
_attribute_data_retention_ key_map_t key_arry[MAX_GPIO_KEYS] = {
  {ROW0, COL0, IS_WAKE_UP},
  {ROW0, COL1, IS_WAKE_UP},
  {ROW0, COL2, IS_WAKE_UP},
  {ROW0, COL3, IS_WAKE_UP},
  {ROW2, COL1, IS_WAKE_UP},
  {ROW2, COL2, IS_WAKE_UP},
  {ROW1, COL0, IS_WAKE_UP},
  {ROW1, COL1, IS_WAKE_UP},
  {ROW1, COL2, IS_WAKE_UP},
  {ROW1, COL3, IS_WAKE_UP},
  {ROW2, COL0, IS_WAKE_UP},
};

const key_type_t key_enum_arry[MAX_KEYS] = {
  {KEY0,  MECHANICAL_KEY, gpio_key_init, gpio_key_low_scan, gpio_stuck_key_low_scan},
  {KEY1,  MECHANICAL_KEY, gpio_key_init, gpio_key_low_scan, gpio_stuck_key_low_scan},
  {KEY2,  MECHANICAL_KEY, gpio_key_init, gpio_key_low_scan, gpio_stuck_key_low_scan},
  {KEY3,  MECHANICAL_KEY, gpio_key_init, gpio_key_low_scan, gpio_stuck_key_low_scan},
  {KEY4,  MECHANICAL_KEY, gpio_key_init, gpio_key_low_scan, gpio_stuck_key_low_scan},
  {KEY5,  MECHANICAL_KEY, gpio_key_init, gpio_key_low_scan, gpio_stuck_key_low_scan},
  {KEY6,  MECHANICAL_KEY, gpio_key_init, gpio_key_low_scan, gpio_stuck_key_low_scan},
  {KEY7,  MECHANICAL_KEY, gpio_key_init, gpio_key_low_scan, gpio_stuck_key_low_scan},
  {KEY8,  MECHANICAL_KEY, gpio_key_init, gpio_key_low_scan, gpio_stuck_key_low_scan},
  {KEY9,  MECHANICAL_KEY, gpio_key_init, gpio_key_low_scan, gpio_stuck_key_low_scan},
  {KEY10, MECHANICAL_KEY, gpio_key_init, gpio_key_low_scan, gpio_stuck_key_low_scan},
};

static bool normal_scan;

int main(void)
{
  blc_pm_select_internal_32k_crystal();

  cpu_wakeup_init();

  clock_init(SYS_CLK_24M_Crystal);

  gpio_init(1);

  dc_power_on();

  if(!is_wakeup_from_sleep()){
    gpio_key_alloc(key_arry, MAX_GPIO_KEYS);
    register_key(key_enum_arry, MAX_KEYS);
  }

  if(!is_wakeup_from_sleep())
    key_init();
  else
    key_wakeup_init();

  key_process(NULL);//for wake up fast scan

  initial_lcd();

  if(!is_wakeup_from_sleep())
    id_init();

  mempool_init(&KEY_EVENT_POOL ,&KEY_EVENT_POOL_MEM[0], sizeof(mem_block_t) + sizeof(event_handler_t), MAX_EVENT);

  app_init();

  ev_on_timer(key_process, NULL, KEY_PROCESS_TIME);

  if(!is_wakeup_from_sleep())
    idle_time_for_sleep(SLEEP_WAIT_TIME);

  reload_sys_time();

  display_wait_time = clock_time();

  rf_8359_set_tx();

  while(1){
    if(normal_scan || !is_wakeup_from_sleep()){
      poll_key_event();
      ev_process_timer();
    }

    if(is_wakeup_from_sleep()){
      if(n_clock_time_exceed(display_wait_time, 100000)){//100ms
        key_process(NULL);
        poll_key_event();
        normal_scan = 1;
      }
    }

    if(poll_idle_time()){
      gpio_key_sleep_setup();
      clear_screen();
      i2c_gpio_set_deepsleep(SSD1306_I2C_SDA, SSD1306_I2C_CLK);
      set_wakeup_flag();
      dc_shutdown();
      cpu_sleep_wakeup(DEEPSLEEP_MODE_RET_SRAM_LOW32K, PM_WAKEUP_PAD, 0);
    }
  }
}
