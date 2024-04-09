# WES-2024
Workshop on Embedded Systems - Hackaton

## How to start

1. Download with `git clone https://github.com/MarkoSrpak/WES-2024.git`
of with `git clone git@github.com:MarkoSrpak/WES-2024.git` if you are contributor.

2. Use `git submodule update --init --recursive` (NOTE: DON'T USE EDUROAM)

3. Position to components/lvgl_esp32_drivers using `cd components/lvgl_esp32_drivers`

4. Apply a patch using `git apply ../lvgl_esp32_drivers_8-3.patch`

5. Go back to main folder `cd ../../`

6. You can now build and flash the device.
