#include "ifan03.h"
//#include "esphome/components/fan/fan_helpers.h"
#include "esphome/core/log.h"
#include "esphome.h"

namespace esphome {
namespace ifan03 {
#define buzzer 10
#define relay_1 14
#define relay_2 12
#define relay_3 15
#define TAG "IFAN03"
int target_fan_speed;
int start_time_offset;
void IFan03::setup() {
  pinMode(buzzer, 0x01);
  pinMode(relay_1, 0x01);
  pinMode(relay_2, 0x01);
  pinMode(relay_3, 0x01);

  // Initialize buzzer to stop errant beeping due to non-initialized pin
  digitalWrite(buzzer, HIGH);

  auto restore = this->restore_state_();
  if (restore.has_value()) {
    restore->apply(*this);
    this->current_speed = this->speed;
    this->state_ = this->state;
    this->write_state_();
  }
}
void IFan03::dump_config() { LOG_FAN("", "IFan03", this); }
fan::FanTraits IFan03::get_traits() { return fan::FanTraits(false, true, false, 3); }
void IFan03::control(const fan::FanCall &call) {
  if (call.get_state().has_value())
    this->state = *call.get_state();
  if (call.get_speed().has_value())
    this->speed = *call.get_speed();

  this->write_state_();
  this->publish_state();
}
void IFan03::write_state_() {
  int local_speed = static_cast<int>(this->speed);
  ESP_LOGD(TAG, "State: %s, Speed: %i ",this->state ? "ON" : "OFF", local_speed);
  if (!this->state)
    set_off();
  if (this->state)
    do_speed(local_speed);
}  // write_state_

void IFan03::do_speed(const int lspeed){
    switch (lspeed) {
      case 1:
         // low speed
        set_low();
        break;
      case 2:
        // medium speed
        set_med();
        break;
      case 3:
          // high speed
        set_high();
        break;

      default:
        set_off();
        break;
    }
}
void IFan03::set_off() {
  this->state = 0;
  digitalWrite(relay_1, LOW);
  digitalWrite(relay_2, LOW);
  digitalWrite(relay_3, LOW);
  long_beep();
}
void IFan03::set_low() {
  digitalWrite(relay_1, HIGH);  // Start at high speed for 2 seconds to prevent fan stall
  digitalWrite(relay_2, LOW);
  digitalWrite(relay_3, HIGH);
  beep();
  delay(2000);
  digitalWrite(relay_1, HIGH);
  digitalWrite(relay_2, LOW);
  digitalWrite(relay_3, LOW);
}
void IFan03::set_med() {
  digitalWrite(relay_1, HIGH);
  digitalWrite(relay_2, HIGH);
  digitalWrite(relay_3, LOW);
  beep(2);
}
void IFan03::set_high() {
  digitalWrite(relay_1, HIGH);
  digitalWrite(relay_2, LOW);
  digitalWrite(relay_3, HIGH);

  beep(3);
}
void IFan03::beep(int num) {
  if (!this->buzzer_enable_)
    return;
  for (int i = 0; i < num; i++) {
    digitalWrite(buzzer, LOW);
    delay(50);
    digitalWrite(buzzer, HIGH);
    delay(50);
  }
}
void IFan03::long_beep(int num) {
  if (!this->buzzer_enable_)
    return;
  for (int i = 0; i < num; i++) {
    digitalWrite(buzzer, LOW);
    delay(500);
    digitalWrite(buzzer, HIGH);
    delay(500);
  }
}

}  // namespace ifan03

}  // namespace esphome
