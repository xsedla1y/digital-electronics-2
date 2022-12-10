#ifndef ADC_H
#define ADC_H

#define adc_internal_ref() ADMUX |= (1 << REFS0);

#define adc_enable() ADCSRA |= (1 << ADEN);

#define adc_prescaler_128() ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);

#define adc_enable_interrupt() ADCSRA |= (1 << ADIE);

#define adc_start_conversion() ADCSRA |= (1 << ADSC);

#define adc_select_channel(ch) ADMUX &= 0xF0; ADMUX |= ch & 0x0F;

#define adc_get_current_channel() ADMUX & 0x0F;

#endif