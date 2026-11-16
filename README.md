# RIT IoT App (ESP8266 NodeMCU)

Android app (Capacitor) that controls NodeMCU projects over Wi-Fi.

## Get the APK
1. Upload all files of this folder to a new GitHub repository (keep the `.github` folder).
2. Open the repository's **Actions** tab and wait for **Build APK** to turn green (about 5 to 8 minutes).
   If it does not start, open **Build APK** and click **Run workflow**.
3. Open the finished run, scroll to **Artifacts**, download **rit-iot-app-apk**, and unzip it.
4. Copy `app-debug.apk` to your phone and install it (allow "Install unknown apps").

## Use it
Put the phone and NodeMCU on the same 2.4 GHz Wi-Fi, enter the NodeMCU IP from Serial Monitor, tap Connect.
Tap the i icon on any screen for wiring, full Arduino code and troubleshooting.
