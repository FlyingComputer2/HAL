# HAL
Provides interfaces to access hardware:
- ADC
- GPIO
- I2C
- PWM
- SPI
- UART

## Implementations
- halserver - Framework to implement hal remotely, can be used for device simulation.
- halclient - Implement hal through halserver
- hallinux - implement hal through linux
- halduino - implement hal through arduino

## Simulated Devices (simdev)
Uses halserver to simulate devices

Devices:
- [ ] rtctestdev
- [ ] icm42688p
- [ ] neo6m
- [ ] bno055
- [ ] bmp180
- [ ] ms4515do

## Drivers
Uses hal to implement drives

Devices:
- [ ] rtctestdev
- [ ] icm42688p
- [ ] neo6m
- [ ] bno055
- [ ] bmp180
- [ ] ms4515do
