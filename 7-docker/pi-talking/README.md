# Test Case: Raspberry Pi Communication with PQC

This test case simulates secure communication between two Raspberry Pi devices using **Post-Quantum Cryptography (PQC)** for authentication. One Raspberry Pi acts as a server that continuously listens for incoming connections, while the other operates as a client that connects and reconnects.

## Requirements

- 2× Raspberry Pi (with Ethernet support)
- 1× Network switch
- 2× Ethernet cables
- 2× Power supplies
- _(Optional)_ Laptop or desktop for monitoring and control

## Network Setup

1. **Connect Hardware**

   - Use Ethernet cables to connect both Raspberry Pi devices to the network switch.
   - Ensure the switch is powered on.

2. **Assign Static IP Addresses**

   - using nmcli do the following:
     ```bash
     enter nmcli command
     ```

3. **Verify Connectivity**
   - On **Raspberry Pi 1** (assumed to be the server):
     ```bash
     ping 192.168.1.102
     ```
   - On **Raspberry Pi 2** (the client):
     ```bash
     ping 192.168.1.101
     ```

## Sniffer

For the sniffer, we used ettercap to do arp spoofing to redirect the packages incoming from the client to the server.

## Device Roles

- `192.168.1.101` → **Server**
- `192.168.1.102` → **Client**
- `192.168.1.102` → **Eve**

## Notes

- If the raspberry pi cannot ping the other one, check the IP with `ip a ` and check that has the correct IP assigned. If not, then re-set the IP and check again.
