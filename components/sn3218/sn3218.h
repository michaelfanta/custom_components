#pragma once

#include "esphome.h"
#include "esphome/core/component.h"
#include "esphome/components/output/float_output.h"
#include "esphome/components/i2c/i2c.h"

namespace esphome {
    namespace sn3218 {
        class SN3218Component;

        class SN3218Channel : public output::FloatOutput {
            public:
                SN3218Channel(SN3218Component *parent, uint8_t channel) : parent_(parent), channel_(channel) {}

            protected:
                void write_state(float state) override;

            SN3218Component *parent_;
            uint8_t channel_;
        };

        class SN3218Component : public i2c::I2CDevice, public Component {
            public:
                SN3218Channel *create_channel(uint8_t channel);

                void setup() override;
                void loop() override;
                void dump_config() override;

            private:
                void sn3218_enable();
                void sn3218_disable();
                void sn3218_reset();
                void sn3218_update();
                void sn3218_enable_leds(unsigned long enable_mask);
                void sn3218_set(unsigned char chan, unsigned char val);

            protected:
                friend SN3218Channel;

                void set_output_value_(uint8_t channel, uint8_t value) {
                    if (this->output_value[channel] != value) {
                        this->output_value[channel] = value;
                        this->update_ = true;
                    }
                }

                uint8_t output_value[18] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
                bool update_ = false;
        };

    } // namespace sn3218
} // namespace esphome
