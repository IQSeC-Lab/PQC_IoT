# Test Case: Raspberry pi

This test case is developed to simulate the communication between two raspberry pi's using PQC for authentication.
The server is always listening and the client is connecting and recconnecting.

## Requirements

- Two Raspberry PI
- a network switch
- Ethernet cables
- power suppplies
- (optional) A Laptop or desktop for monitoring and control.

## set up

1. Connect the raspberry pi's to the network switch using the ethernet cables
   1. ensure the swith is powered on.
2. Configure the IP addresses
   1. lets set a static IP for each one of them
   ```
   sudo nano /etc/dhcpcd.conf
   ```
   add these at the end of the file:
   ```
   interface eth0
   static ip_address=192.168.1.101/24
   static routers=192.168.1.1
   static domain_name_servers=192.168.1.1
   ```
3. Test connectivity

```
ping 192.168.1.102  # Run this from Raspberry Pi 1
ping 192.168.1.101  # Run this from Raspberry Pi 2
```

## Disclaimer

In this example:

- 101 is for the server
- 102 is the client
