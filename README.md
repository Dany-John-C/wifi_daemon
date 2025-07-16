# Wi-Fi Daemon

A lightweight Linux daemon written in C that monitors nearby Wi-Fi networks using nmcli, identifies the strongest network based on signal strength, logs the result to a file, and sends desktop notifications every 10 seconds.

---

## 📦 Features

- Runs as a background daemon process
- Uses nmcli to scan available Wi-Fi networks
- Selects the strongest network based on signal strength
- Sends desktop notifications using notify-send
- Logs results to /tmp/wifi_daemon.log
- Gracefully stops on SIGINT or SIGTERM

---

## 🛠️ Requirements

- Linux OS with nmcli (NetworkManager CLI)
- notify-send (from libnotify-bin)

Install required tools:
bash
sudo apt update
sudo apt install network-manager libnotify-bin


---

## ⚙️ Compilation

To compile the daemon:

bash
gcc -o wifi_daemon wifi_daemon.c


---

## 🚀 Usage

To run the daemon in the background:

bash
./wifi_daemon &


You’ll start receiving desktop notifications about the best available Wi-Fi network every 10 seconds.

---

## 📄 Log Output

Logs are written to:
/tmp/wifi_daemon.log


Each entry includes the strongest detected network name (SSID) and signal strength at that time.

---

## 🛑 Stop the Daemon

You can stop the daemon with:

bash
pkill wifi_daemon


Or:
bash
kill <PID>


---

## ⚠️ Note

- This is a basic daemon written in C, mainly for educational or personal use.
- For production use, consider enhancements such as dynamic configuration, better error handling, and support for system service managers like systemd.
