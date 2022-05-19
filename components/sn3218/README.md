# SN3218 output component for ESPHome

***(Tested on ESP32-F)***

Pimoroni SN3218 - https://shop.pimoroni.com/products/18-channel-8-bit-pwm-led-driver-w-i2c-interface



```yaml
# example configuration:

sn3218:
  id: sn3218_1
  address: 0x54

i2c:
  sda: 12
  scl: 13

output:
  - platform: sn3218
    channel: 0
    id: 'sn3218_channel_0'

light:
  - platform: sn3218
    output: sn3218_channel_0
    name: SN3218 Light 0

OR

switch:
  - platform: output
    output: sn3218_channel_0
    name: SN3218 Channel 0
    id: sn3218_switch_channel_0
```
