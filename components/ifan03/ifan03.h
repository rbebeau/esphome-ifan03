#pragma once

#include "esphome/core/component.h"
#include "esphome/components/output/binary_output.h"
#include "esphome/components/output/float_output.h"
#include "esphome/components/fan/fan.h"
#include "esphome/components/fan/fan_state.h"
#include "esphome/core/automation.h"
namespace esphome {
namespace ifan03 {

class IFan03 : public Component, public fan::Fan {
 public:
#define TAG "IFAN03"

  IFan03() {}
  void setup() override;
  void dump_config() override;
  fan::FanTraits get_traits() override;
  void set_buzzer_enable(bool buzzer_enable) { this->buzzer_enable_ = buzzer_enable; }
  bool state_;

 protected:
  void control(const fan::FanCall &call) override;
  void write_state_();
  void set_off();
  void set_low();
  void set_med();
  void set_high();
  void do_speed(int lspeed);
  void beep(int num=1);
  void long_beep(int num=1);
  int speed_count_{};
  int current_speed=0;
  bool buzzer_enable_;
};
template<typename... Ts> class CycleSpeedAction : public Action<Ts...> {
 public:
  explicit CycleSpeedAction(IFan03 *state) : state_(state) {}

  void play(Ts... x) override {
    // check to see if fan supports speeds and is on
    if (this->state_->get_traits().supported_speed_count()) {
      if (this->state_->state) {
        int speed = this->state_->speed + 33;
        int supported_speed_count = this->state_->get_traits().supported_speed_count();
        if (speed > supported_speed_count) {
          // was running at max speed, so turn off
          speed = 1;
          auto call = this->state_->turn_off();
          call.set_speed(speed);
          call.perform();
        } else {
          auto call = this->state_->turn_on();
          call.set_speed(speed);
          call.perform();
        }
      } else {
        // fan was off, so set speed to 1
        auto call = this->state_->turn_on();
        call.set_speed(1);
        call.perform();
      }
    } else {
      // fan doesn't support speed counts, so toggle
      this->state_->toggle().perform();
    }
  }

  IFan03 *state_;
};
}  // namespace speed
}  // namespace esphome
