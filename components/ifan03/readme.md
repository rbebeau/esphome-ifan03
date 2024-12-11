# Example

The following is an example YAML configuration for the ifan03 component


```yaml
external_components:
  - source: github://rbebeau/esphome-ifan03
    components: [ ifan03 ]
    #refresh: 0s

substitutions:
  name: dining-room-fan
  device_description: Sonoff iFan03
  friendly_name: Dining Room

esphome:
  name: ${device_name}
  friendly_name: ${friendly_name}

esp8266:
  board: esp8285

# Enable logging
logger:

# Enable Home Assistant API
api:
  encryption:
    key: "notagoodvalidkeychangeme"

ota:
  - platform: esphome
    password: !secret ota_password

wifi:
  ssid: !secret wifi_ssid
  password: !secret wifi_password

  # Enable fallback hotspot (captive portal) in case wifi connection fails
  ap:
    ssid: "Ifan3 Fallback Hotspot"
    password: "blahblahfallbackpassword"

captive_portal:

web_server:
  port: 80

output:
  - platform: gpio
    pin: GPIO10
    id: buzzer
    inverted: true

light:
  - platform: ifan03
    id: fan_light
    name: "${friendly_name} Light"

fan:
  - platform: ifan03
    id: the_fan
    name: "${friendly_name} Fan"
    # optional: allows you to disable the buzzer, enabled by default. 
    buzzer_enable: false 

switch:
  - platform: restart
    name: "${friendly_name} Restart"

binary_sensor:
  - platform: remote_receiver
    name: "Fan 0"
    id: remote_0
    raw:
      code: [-207, 104, -103, 104, -104, 103, -104, 207, -104, 103, -104, 104, -103, 104, -104, 103, -104, 105, -102, 104, -725, 104, -311, 103, -518, 104, -933, 103, -104, 104, -725, 104, -932, 104, -207, 207, -519]
    on_release:
      then:
        - fan.turn_off: the_fan
    internal: true
  - platform: remote_receiver
    id: remote_fan1
    raw:
      code: [-207, 104, -104, 103, -104, 104, -103, 207, -104, 104, -103, 104, -104, 103, -104, 104, -103, 104, -104, 103, -726, 103, -312, 103, -518, 104, -933, 103, -104, 104, -725, 104, -103, 104, -726, 103, -104, 311, -518]
    on_release:
      then:
        - fan.turn_on:
              id: the_fan
              speed: 1
    internal: true
  - platform: remote_receiver
    id: remote_fan2
    raw:
      code: [-208, 103, -104, 104, -103, 104, -103, 208, -103, 104, -104, 103, -104, 104, -103, 104, -104, 103, -104, 103, -726, 104, -310, 104, -518, 104, -933, 103, -104, 104, -725, 104, -207, 104, -622, 103, -416, 102, -415]
    on_release:
      then:
        - fan.turn_on:
              id: the_fan
              speed: 2
    internal: true
  - platform: remote_receiver
    id: remote_fan3
    raw:
      code: [-207, 104, -104, 103, -104, 104, -103, 208, -103, 104, -104, 103, -104, 104, -103, 104, -104, 103, -104, 103, -726, 104, -311, 104, -518, 103, -934, 103, -103, 104, -726, 103, -104, 207, -622, 104, -103, 104, -207, 104, -415]
    on_release:
      then:
        - fan.turn_on:
              id: the_fan
              speed: 3
    internal: true

  - platform: remote_receiver
    id: remote_light
    raw:
      code: [-207, 104, -103, 104, -104, 103, -104, 207, -104, 103, -104, 104, -103, 104, -103, 104, -104, 103, -104, 104, -725, 104, -311, 103, -518, 104, -933, 103, -104, 103, -726, 103, -311, 104, -518, 104, -207, 104, -103, 104, -414]
    on_release:
      then:
        - light.toggle: fan_light

remote_receiver:
  pin: GPIO3

button:
  - platform: template
    name: One beep
    on_press:
      - output.turn_on: buzzer
      - delay: 50ms
      - output.turn_off: buzzer
  - platform: template
    name: Two beep
    on_press:
      - output.turn_on: buzzer
      - delay: 50ms
      - output.turn_off: buzzer
      - delay: 50ms
      - output.turn_on: buzzer
      - delay: 50ms
      - output.turn_off: buzzer

```
