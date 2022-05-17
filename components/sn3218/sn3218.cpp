#include "esphome/core/log.h"
#include "esphome/core/helpers.h"
#include "sn3218.h"

namespace esphome {
    namespace sn3218 {
        #define SN3218_CH_ALL 0x3FFFF
        #define SN3218_CH_00 0x00001
        #define SN3218_CH_01 0x00002
        #define SN3218_CH_02 0x00004
        #define SN3218_CH_03 0x00008
        #define SN3218_CH_04 0x00010
        #define SN3218_CH_05 0x00020
        #define SN3218_CH_06 0x00040
        #define SN3218_CH_07 0x00080
        #define SN3218_CH_08 0x00100
        #define SN3218_CH_09 0x00200
        #define SN3218_CH_10 0x00400
        #define SN3218_CH_11 0x00800
        #define SN3218_CH_12 0x01000
        #define SN3218_CH_13 0x02000
        #define SN3218_CH_14 0x04000
        #define SN3218_CH_15 0x08000
        #define SN3218_CH_16 0x10000
        #define SN3218_CH_17 0x20000

        static const char *TAG = "sn3218";

        static const uint8_t SN3218_CMD_ENABLE_OUTPUT = 0x00;
        static const uint8_t SN3218_CMD_SET_PWM_VALUES = 0x01;
        static const uint8_t SN3218_CMD_ENABLE_LEDS = 0x13;
        static const uint8_t SN3218_CMD_UPDATE = 0x16;
        static const uint8_t SN3218_CMD_RESET = 0x17;

        void SN3218Component::setup() {
            ESP_LOGI(TAG, "Resetting device...");
            if(!this->write_byte(SN3218_CMD_RESET, 0x00)) {
                this->mark_failed();
            }

            delay(500);

            this->sn3218_enable();
            this->sn3218_enable_leds(SN3218_CH_ALL);

            this->loop();
        }

        void SN3218Component::loop() {
            if (this->update_) {
                for(uint8_t channel = 0; channel < 18; channel++) {
                    uint16_t value = this->output_value[channel];

                    this->sn3218_set(channel, value);
                    ESP_LOGI(TAG, "Channel %02u: value=%04u", channel, value);
                }

                this->sn3218_update();
                this->update_ = false;
            }
        }

        void SN3218Component::dump_config() {
            if (this->is_failed()) {
                ESP_LOGE(TAG, "Setting up SN3218 failed!");
            } else {
                ESP_LOGI(TAG, "Setting up SN3218 is successfully!");
            }
        }

        void SN3218Component::sn3218_enable() {
            if(!this->write_byte(SN3218_CMD_ENABLE_OUTPUT, 0x01)) {
                ESP_LOGE("sn3218", "enable error");
                this->mark_failed();
            }
        }

        void SN3218Component::sn3218_disable() {
            if(!this->write_byte(SN3218_CMD_ENABLE_OUTPUT, 0x00)) {
                ESP_LOGE("sn3218", "disable error");
                this->mark_failed();
            }
        }

        void SN3218Component::sn3218_reset() {
            if(!this->write_byte(SN3218_CMD_RESET, 0x00)) {
                ESP_LOGE("sn3218", "reset error");
                this->mark_failed();
            }
            else this->sn3218_enable();
        }

        void SN3218Component::sn3218_update() {
            if(!this->write_byte(SN3218_CMD_UPDATE, 0x00)) {
                ESP_LOGE("sn3218", "update error");
                this->mark_failed();
            }
        }

        void SN3218Component::sn3218_enable_leds(unsigned long enable_mask) {
            this->write_byte(SN3218_CMD_ENABLE_LEDS, enable_mask & 0x3F);
            this->write_byte(SN3218_CMD_ENABLE_LEDS + 1, (enable_mask >> 6) & 0x3F);
            this->write_byte(SN3218_CMD_ENABLE_LEDS + 2, (enable_mask >> 12) & 0x3F);
            this->sn3218_update();
        }

        void SN3218Component::sn3218_set(unsigned char chan, unsigned char val) {
            this->write_byte(SN3218_CMD_SET_PWM_VALUES + chan, val);
        }

        SN3218Channel *SN3218Component::create_channel(uint8_t channel) {
            return new SN3218Channel(this, channel);
        }

        void SN3218Channel::write_state(float state) {
            uint8_t value = static_cast<uint8_t>(roundf(state * 255));
            this->parent_->set_output_value_(this->channel_, value);
        }

    }  // namespace sn3218
}  // namespace esphome
