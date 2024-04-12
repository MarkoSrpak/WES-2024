# WES-2024
Workshop on Embedded Systems - Hackaton

## How to start

1. Download with 
```
git clone https://github.com/MarkoSrpak/WES-2024.git
```
or with 
```
git clone git@github.com:MarkoSrpak/WES-2024.git
```
if you are contributor.

2. **IMPORTANT** never push on main branch, immediately do
```
git checkout develop
```

3. Use
```
git submodule update --init --recursive
```
(**NOTE**: DON'T USE EDUROAM)

4. Position to components/lvgl_esp32_drivers using 
```
cd components/lvgl_esp32_drivers
```

5. Apply a patch using 
```
git apply ../lvgl_esp32_drivers_8-3.patch
```

6. Go back to main folder 
```
cd ../../
```

7. You can now build and flash the device.
