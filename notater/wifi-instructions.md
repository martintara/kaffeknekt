Nice! If your **ESP32 FireBeetle can connect to your phone's hotspot**, that means the hardware is good — and the issue is probably with the AP settings on the Raspberry Pi or router.

Let’s now **turn your Raspberry Pi 5 into an offline Wi-Fi hotspot** (no internet, just LAN so the ESP32 can talk to it).

---

### ✅ What we’re setting up:

- A **Wi-Fi Access Point** from your Raspberry Pi 5
    
- No internet needed
    
- The Pi will assign IPs using `dnsmasq`
    
- You can connect ESP32 devices to it
    

---

### 🧰 Step-by-step setup (tested on Raspberry Pi OS Bookworm):

#### 1. **Install required packages**

```bash
sudo apt update
sudo apt install hostapd dnsmasq netfilter-persistent iptables-persistent
```

Then disable the services (we’ll configure them first):

```bash
sudo systemctl stop hostapd
sudo systemctl stop dnsmasq
```

---

#### 2. **Set a static IP for wlan0**

Edit:

```bash
sudo nano /etc/dhcpcd.conf
```

Add to the end:

```conf
interface wlan0
    static ip_address=192.168.4.1/24
    nohook wpa_supplicant
```

---

#### 3. **Configure dnsmasq (DHCP server)**

Backup and edit config:

```bash
sudo mv /etc/dnsmasq.conf /etc/dnsmasq.conf.orig
sudo nano /etc/dnsmasq.conf
```

Paste:

```conf
interface=wlan0
dhcp-range=192.168.4.10,192.168.4.100,255.255.255.0,24h
```

---

#### 4. **Configure hostapd (Access Point)**

Create config:

```bash
sudo nano /etc/hostapd/hostapd.conf
```

Paste:

```conf
interface=wlan0
driver=nl80211
ssid=PiAP
hw_mode=g
channel=6
auth_algs=1
wmm_enabled=0
wpa=2
wpa_passphrase=81549300
wpa_key_mgmt=WPA-PSK
rsn_pairwise=CCMP
```

Then tell hostapd where to find it:

```bash
sudo nano /etc/default/hostapd
```

Uncomment and modify:

```conf
DAEMON_CONF="/etc/hostapd/hostapd.conf"
```

---

#### 5. **Enable services**

```bash
sudo systemctl unmask hostapd
sudo systemctl enable hostapd
sudo systemctl enable dnsmasq
```

---

#### 6. **Reboot**

```bash
sudo reboot
```

---

### 🎉 After reboot:

- Your Pi will broadcast the network `PiAP`
    
- ESP32 should be able to connect with password `81549300`
    
- Pi’s IP is `192.168.4.1`
    
- ESP32 will get IPs between `192.168.4.10` and `.100`
    

---

Let me know if you want to:

- Serve files or a web UI from the Pi
    
- Use `mosquitto` MQTT for local comms
    
- Automatically start logging sensor data
    

Happy to help!