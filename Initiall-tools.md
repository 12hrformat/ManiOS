Mani Linux — Security Tool Implementation Difficulty

🟢 Tier 1 — Easy

Tools that should generally work immediately after installation.

- Nmap
- Netcat
- Tcpdump
- Dig / DNS tools
- Curl
- Wget
- FFUF
- SQLmap
- Nikto
- Nuclei
- WhatWeb
- Hydra
- John the Ripper
- Hashcat (CPU mode)
- YARA

---

🟡 Tier 2 — Moderate

Tools that are relatively straightforward to install but have additional dependencies, configuration, or environment requirements.

- Masscan
- Amass
- Subfinder
- HTTPX
- Naabu
- Gobuster
- Feroxbuster
- Impacket
- NetExec
- SearchSploit
- Metasploit Framework
- Radare2
- Rizin
- Binwalk
- Volatility

---

🟠 Tier 3 — Hardware / Environment Dependent

Tools that require specific hardware, drivers, interfaces, permissions, or external environments.

- Aircrack-ng
- Kismet
- Wifite
- Bettercap
- Mitmproxy
- Evil-WinRM
- Kerbrute
- BloodHound Python

Aircrack-ng and related wireless tools require proper wireless hardware, drivers, monitor-mode support, and appropriate interface capabilities.

Active Directory tools require an AD/Windows environment for meaningful testing.

---

🔴 Tier 4 — GUI / Later Mani Environment

These should be integrated after the Mani GUI environment is available.

- Ghidra
- Burp Suite
- Wireshark GUI
- Cutter
- BloodHound GUI
- Autopsy

CLI alternatives or components can still be available earlier where applicable, such as:

- Tshark instead of the Wireshark GUI
- Radare2/Rizin instead of Cutter
- BloodHound Python collectors for AD work

---

Initial Mani Security Toolset

Easy

- Nmap
- Curl
- Wget
- Netcat
- Tcpdump
- FFUF
- SQLmap
- Nuclei
- Hashcat
- John the Ripper
- YARA

Moderate

- Masscan
- Amass
- Subfinder
- HTTPX
- Gobuster
- Feroxbuster
- Impacket
- NetExec
- SearchSploit
- Metasploit
- Radare2
- Binwalk
- Volatility

Hardware / Environment

- Aircrack-ng
- Kismet
- Wifite
- Bettercap
- BloodHound

Reporting

- DragonRepo

GUI / Later

- Ghidra
- Burp Suite
- Wireshark GUI
- Cutter
- BloodHound GUI
- Autopsy

---

Tool Testing Standard

Every preinstalled tool should pass three stages:

1. INSTALLED
2. LAUNCHES
3. FUNCTIONALLY TESTED

Example:

Nmap:

- [x] Installed
- [x] Launches
- [x] Network access works
- [x] Successfully tested against a lab target

Aircrack-ng:

- [x] Installed
- [x] Launches
- [ ] Wireless interface detected
- [ ] Monitor mode tested
- [ ] Lab wireless testing completed

A tool should not be considered fully supported by Mani merely because its package installs successfully.
