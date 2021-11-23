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
#include "matrix_gpio_key.h"
#include "gpio_led.h"
#include "power_saving.h"
#include "ssd1306_oled.h"
#include "wakeup.h"
#include "i2c_gpio_set.h"
#include "prevent_system_crash.h"
#include "display.h"
#include "my_display.h"

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
  {KEY0,  MECHANICAL_KEY, gpio_key_init, gpio_key_low_scan, gpio_key_enable_sleep, gpio_key_disable_sleep},
  {KEY1,  MECHANICAL_KEY, gpio_key_init, gpio_key_low_scan, gpio_key_enable_sleep, gpio_key_disable_sleep},
  {KEY2,  MECHANICAL_KEY, gpio_key_init, gpio_key_low_scan, gpio_key_enable_sleep, gpio_key_disable_sleep},
  {KEY3,  MECHANICAL_KEY, gpio_key_init, gpio_key_low_scan, gpio_key_enable_sleep, gpio_key_disable_sleep},
  {KEY4,  MECHANICAL_KEY, gpio_key_init, gpio_key_low_scan, gpio_key_enable_sleep, gpio_key_disable_sleep},
  {KEY5,  MECHANICAL_KEY, gpio_key_init, gpio_key_low_scan, gpio_key_enable_sleep, gpio_key_disable_sleep},
  {KEY6,  MECHANICAL_KEY, gpio_key_init, gpio_key_low_scan, gpio_key_enable_sleep, gpio_key_disable_sleep},
  {KEY7,  MECHANICAL_KEY, gpio_key_init, gpio_key_low_scan, gpio_key_enable_sleep, gpio_key_disable_sleep},
  {KEY8,  MECHANICAL_KEY, gpio_key_init, gpio_key_low_scan, gpio_key_enable_sleep, gpio_key_disable_sleep},
  {KEY9,  MECHANICAL_KEY, gpio_key_init, gpio_key_low_scan, gpio_key_enable_sleep, gpio_key_disable_sleep},
  {KEY10, MECHANICAL_KEY, gpio_key_init, gpio_key_low_scan, gpio_key_enable_sleep, gpio_key_disable_sleep},
};

static bool normal_scan;
u8 key_event_occured;
extern void long_dachong_display_kai ();

int main(void)
{
start:
  blc_pm_select_internal_32k_crystal ();

  cpu_wakeup_init ();

  clock_init (SYS_CLK_24M_Crystal);

  gpio_init (1);

  prevent_system_crash ();

  dc_power_on ();

  if (!is_wakeup_from_sleep()) {
    gpio_key_alloc (key_arry, MAX_GPIO_KEYS);
    register_key (key_enum_arry, MAX_KEYS);
  }

  if (!is_wakeup_from_sleep ())
    key_init ();
  else
    key_wakeup_init ();

  key_process (NULL);//for wake up fast scan

  initial_lcd  ();

  if (!is_wakeup_from_sleep())
    id_init ();

  if (!is_wakeup_from_sleep()) {
    mempool_init(&KEY_EVENT_POOL, &KEY_EVENT_POOL_MEM[0], sizeof(mem_block_t) + sizeof(event_handler_t), MAX_EVENT);
    app_init();
  }

  idle_time_for_sleep (SLEEP_WAIT_TIME);

  reload_sys_time ();

  display_wait_time = clock_time ();

  rf_8359_set_tx ();

  while (1) {

    if (normal_scan || !is_wakeup_from_sleep ()) {
      poll_key_event ();
      key_event_occured = key_process (NULL);
    }

    if (is_wakeup_from_sleep()) {
      if (n_clock_time_exceed (display_wait_time, 100000)) {//100ms
    	key_event_occured = key_process (NULL);
    	poll_key_event ();
        clr_wakeup_flag ();
        normal_scan = 1;
      }
    }

    if ((!is_led_on || poll_idle_time()) && !key_event_occured && !is_wakeup_from_sleep()) {
      gpio_key_sleep_setup ();
      clear_screen ();
      i2c_gpio_set_deepsleep (SSD1306_I2C_SDA, SSD1306_I2C_CLK);
      set_wakeup_flag ();
      dc_shutdown ();
      cpu_sleep_wakeup (DEEPSLEEP_MODE_RET_SRAM_LOW32K, PM_WAKEUP_PAD, 0);
      goto start;
    }
  }
}
