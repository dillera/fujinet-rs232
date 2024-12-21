# FujiNet PC BIOS Specification

## Reference

### INT F5H - FujiNet Control

#### (AH = 0xFF) RESET FUJINET

| Register | Value |
|----------|-------|
| AH       | 0xFF  |

#### (AH = 0xFE) GET CURRENT SSID

| Register | Value                                                  |
|----------|--------------------------------------------------------|
| AH       | 0xFE                                                   |
| ES:BX    | Destination Segment (ES) and Offset (BX) for SSID Data |

The target buffer will be filled with 97 bytes of data:

| Offset | Length | Value                                       |
|--------|--------|---------------------------------------------|
| 0      | 33     | NULL terminated string containing WiFi SSID |
| 34     | 64     | NULL terminated string containing PSK       |

#### (AH = 0xFD) SCAN NETWORKS

| Register | Value |
|----------|-------|
| AH       | 0xFD  |

The number of found networks will be returned in AL.

#### (AH = 0xFC) GET SCAN RESULT

| Register | Value                                                      |
|----------|------------------------------------------------------------|
| AH       | 0xFC                                                       |
| AL       | An Index equal or less than # of networks returned by 0xFD |
| ES:BX    | Destination Segment (ES) and Offset (BX) for SSID Result   |

The target buffer will contain 34 bytes of data:

| Offset | Length | Value                                     |
| 0      | 33     | SSID as NULL terminated string            |
| 34     | 1      | RSSI strength (dBm from negative to zero) |

#### (AH = 0xFB) SET SSID AND CONNECT

| Register | Value                                         |
|----------|-----------------------------------------------|
| AH       | 0xFB                                          |
| ES:BX    | Pointer to SSID Information                   |
| CL       | 1 = Ask FujiNet to save SSID, 0 = Do not save |

The source buffer should contain the following data at ES:BX:

| Offset | Length | Value                                       |
|--------|--------|---------------------------------------------|
| 0      | 33     | NULL terminated string containing WiFi SSID |
| 34     | 64     | NULL terminated string containing PSK       |


#### (AH = 0xFA) GET WIFI STATUS

| Register | Value |
|----------|-------|
| AH       | 0xFA  |

The following WIFI status values are returned in AL:

| Value | Description                   |
|-------|-------------------------------|
| 0     | WIFI is idle.                 |
| 1     | No SSID available.            |
| 2     | Scan completed.               |
| 3     | Connected to SSID and active. |
| 4     | Most recent connect failed.   |
| 5     | WiFi Connection Lost          |
| 6     | WiFi Explicitly Disconnected  |

#### (AH = 0xF9) MOUNT HOST

| Register | Value                |
|----------|----------------------|
| AH       | 0xF9                 |
| AL       | Host slot # to mount |

#### (AH = 0xF8) MOUNT DEVICE IMAGE

| Register | Value                        |
|----------|------------------------------|
| AH       | 0xF8                         |
| CL       | Device Slot # to mount       |
| CH       | Mode $01 = Read, $02 = Write |

> **_NOTE:_** The device slots should be populated using both the WRITE DEVICE SLOTS and SET FILENAME FOR DEVICE SLOT commands, before using this command.

#### (AH = 0xF7) OPEN DIRECTORY

| Register | Value                                                                         |
|----------|-------------------------------------------------------------------------------|
| AH       | 0xF7                                                                          |
| AL       | Host slot to perform open directory on.                                       |
| ES:BX    | Pointer to 256 byte NULL terminated string containing desired directory path. |

#### (AH = 0xF6) READ NEXT DIRECTORY ENTRY

| Register | Value                                                                                       |
|----------|---------------------------------------------------------------------------------------------|
| AH       | 0xF6                                                                                        |
| AL       | Desired Length of returned directory entry.                                                 |
| ES:BX    | Pointer to destination for directory entry. Reserved space must be equal or greater than AL |
| CL       | If bit 7 (0x80 is set), additional file details are returned, add 9 bytes to AL.            |

* If bit 7 **is not** set, the next file name is returned.

* If bit 7 **is** set, additional file info is appended to the entry in the following format:

| Byte | Item             | Description                               |
|------|------------------|-------------------------------------------|
| 0x00 | MODIFIED_YEAR    | File modified date-time, years since 1970 |
| 0x01 | MODIFIED_MONTH   | File modified date-time, month (1-12)     |
| 0x02 | MODIFIED_DAY     | File modified date-time, day (1-31)       |
| 0x03 | MODIFIED_HOUR    | File modified date-time, hour (0-23)      |
| 0x04 | MODIFIED_MINUTE  | File modified date-time, minute (0-59)    |
| 0x05 | MODIFIED_SECONDS | File modified date-time, seconds (0-59)   |
| 0x06 | FILE_SIZE_LO     | File size, lo byte                        |
| 0x07 | FILE_SIZE_HI     | File size, hi byte                        |
| 0x08 | FILE_FLAGS       | One or more file flags (see below)        |
| 0x09 | FILE_TYPE        | One of the file types (see below)         |

File Flags

| Flag     | Value | Description                                   |
|----------|-------|-----------------------------------------------|
| FF_DIR   | 0x01  | Entry is directory.                           |
| FF_TRUNC | 0x02  | Entry was truncated to fit requested AL size. |

File Types

| Type       | Value | Description                       |
|------------|-------|-----------------------------------|
| FT_UNKNOWN | 0x00  | File type could not be determined |
| FT_ATR     | 0x01  | ATR disk image format             |
| FT_ATX     | 0x02  | ATX disk image format             |
| FT_XEX     | 0x03  | XEX disk image format             |

TODO: Add more disk types, we never added more, oops.

#### (AH = 0xF5) CLOSE DIRECTORY

| Register | Value                    |
|----------|--------------------------|
| AH       | 0xF5                     |
| AL       | Host slot to close (0-7) |

#### (AH = 0xF4) READ HOST SLOTS

| Register | Value                                                                     |
|----------|---------------------------------------------------------------------------|
| AH       | 0xF4                                                                      |
| ES:BX    | Pointer to a 256 byte buffer to hold all 8 hosts, 32 characters per host. |

#### (AH = 0xF3) WRITE HOST SLOTS

| Register | Value                                                                       |
|----------|-----------------------------------------------------------------------------|
| AH       | 0xF3                                                                        |
| ES:BX    | Pointer to a 256 byte buffer to holding all 8 hosts, 32 characters per host |

#### (AH = 0xF2) READ DEVICE SLOTS

| Register | Value                                          |
|----------|------------------------------------------------|
| AH       | 0xF2                                           |
| ES:BX    | Pointer to 304 byte buffer in the format below |

The 304 byte buffer, is an array of 8 device slots, with the following format:

| Offset | Size | Description                     |
|--------|------|---------------------------------|
| 0      | 1    | Host slot (0-7)                 |
| 1      | 1    | Mode (0 = Read Only, 1 = write) |
| 2      | 36   | Displayed filename              |

#### (AH = 0xF1) WRITE DEVICE SLOTS

| Register | Value                                          |
|----------|------------------------------------------------|
| AH       | 0xF1                                           |
| ES:BX    | Pointer to 304 byte buffer in the format below |

The 304 byte buffer, is an array of 8 device slots, with the following format:

| Offset | Size | Description                     |
|--------|------|---------------------------------|
| 0      | 1    | Host slot (0-7)                 |
| 1      | 1    | Mode (0 = Read Only, 1 = write) |
| 2      | 36   | Displayed filename              |

#### (AH = 0xF0) UDP STREAMING MODE

| Register | Value                                                           |
|----------|-----------------------------------------------------------------|
| AH       | 0xF0                                                            |
| ES:BX    | Pointer to 64 byte NULL terminated string containing host name. |
| CX       | UDP Port number (0-65535)                                       |

#### (AH = 0xEA) GET WIFI ENABLED

| Register | Value |
|----------|-------|
| AH       | 0xEA  |

Return value in AL, 0 = Disabled, 1 = Enabled.

#### (AH = 0xE9) UNMOUNT DEVICE IMAGE

| Register | Value                        |
|----------|------------------------------|
| AH       | 0xE9                         |
| AL       | Device slot to unmount (0-7) |

#### (AH = 0xE8) GET ADAPTER CONFIGURATION

| Register | Value                                                                      |
|----------|----------------------------------------------------------------------------|
| AH       | 0xE8                                                                       |
| ES:BX    | Pointer to 140 byte data structure to hold the AdapterConfig defined below |

AdapterConfig Structure:

| Offset | Size | Description                               |
|--------|------|-------------------------------------------|
| 0      | 33   | The current SSID                          |
| 34     | 64   | The current host name                     |
| 97     | 4    | Local IPv4 Address                        |
| 101    | 4    | Gateway IPv4 Address                      |
| 105    | 4    | IPv4 Network Mask                         |
| 109    | 4    | DNS IPv4 Address                          |
| 113    | 6    | MAC address                               |
| 119    | 6    | BSSID                                     |
| 125    | 15   | Fujinet version as NULL terminated string |

#### (AH = 0xE7) CREATE NEW DISK IMAGE

| Register | Value                                       |
|----------|---------------------------------------------|
| AH       | 0xE7                                        |
| ES:BX    | Pointer to 262 byte buffer specified below: |

The new disk buffer is in the following format:

| Offset | Size | Description                                                     |
|--------|------|-----------------------------------------------------------------|
| 0      | 2    | Number of sectors (0-65535)                                     |
| 2      | 2    | Sector size (512)                                               |
| 4      | 1    | Host Slot (0-7)                                                 |
| 5      | 1    | Device slot (0-7)                                               |
| 6      | 256  | NULL terminated string containing full path to filename on host |

TODO: change size of number of sectors to 4 bytes.

#### (AH = 0xE6) UNMOUNT HOST

| Register | Value           |
|----------|-----------------|
| AH       | 0xE6            |
| AL       | Host Slot (0-7) |

#### (AH = 0xE5) GET DIRECTORY POSITION

| Register | Value |
|----------|-------|
| AH       | 0xE5  |

The directory position (0-65535) is returned in AX.

#### (AH = 0xE4) SET DIRECTORY POSITION

| Register | Value                                |
|----------|--------------------------------------|
| AH       | 0xE4                                 |
| CX       | Desired directory position (0-65535) |

#### (AH = 0xE3) SET HSIO INDEX (NOT IMPLEMENTED)

| Register | Value                |
|----------|----------------------|
| AH       | 0xE3                 |
| AL       | High speed SIO index |

#### (AH = 0xE2) SET FILENAME FOR DEVICE SLOT

| Register | Value                                                                     |
|----------|---------------------------------------------------------------------------|
| AH       | 0xE2                                                                      |
| AL       | Desired Device Slot (0-7)                                                 |
| ES:BX    | Pointer to NULL terminated 256 byte string containing full path.          |
| CL       | The desired host slot (0-7)                                               |
| CH       | Bits 0-3 specify mode (0=read, 1=write), Bits 4-7 specify host slot (0-7) |

#### (AH = 0xE1) SET HOST PREFIX

| Register | Value                                                                      |
|----------|----------------------------------------------------------------------------|
| AH       | 0xE1                                                                       |
| AL       | Host slot (0-7)                                                            |
| ES:BX    | Pointer to NULL terminated 256 byte string containing the new host prefix. |

#### (AH = 0xE0) GET HOST PREFIX

| Register | Value                                                                     |
|----------|---------------------------------------------------------------------------|
| AH       | 0xE1                                                                      |
| AL       | Host slot (0-7)                                                           |
| ES:BX    | Pointer to NULL terminated 256 byte string to contain current host prefix |

#### (AH = 0xDF) SET EXTERNAL SIO CLOCK RATE (NOT IMPLEMENTED)

| Register | Value                          |
|----------|--------------------------------|
| AH       | 0xDF                           |
| CX       | 16-bit value, LSB, rate in kHz |

#### (AH = 0xDE) WRITE APP KEY

| Register | Value                                                                           |
|----------|---------------------------------------------------------------------------------|
| AH       | 0xDE                                                                            |
| ES:BX    | Pointer to buffer to write to FujiNet's App Key storage. Assumed to be CX bytes |
| CX       | Length of data to store.                                                        |

#### (AH = 0xDD) READ APP KEY

| Register | Value                                                                                                            |
|----------|------------------------------------------------------------------------------------------------------------------|
| AH       | 0xDD                                                                                                             |
| ES:BX    | Pointer to buffer in which to store the AppKey received from FujiNet app key storage. First two bytes are length |

#### (AH = 0xDC) OPEN APP KEY

| Register | Value                       |
|----------|-----------------------------|
| AH       | 0xDC                        |
| AL       | open Mode (0=read, 1=write) |
| BL       | Key ID                      |
| BH       | App ID                      |
| CX       | Creator ID                  |

The app key registry is here: https://github.com/FujiNetWIFI/fujinet-firmware/wiki/SIO-Command-$DC-Open-App-Key

#### (AH = 0xDB) CLOSE APP KEY

| Register | Value |
|----------|-------|
| AH       | 0xDB  |

#### (AH = 0xDA) GET DEVICE FILENAME

| Register | Value                                  |
|----------|----------------------------------------|
| AH       | 0xDA                                   |
| AL       | Device Slot (0-7)                      |
| ES:BX    | Pointer to 256 byte area to hold path. |

#### (AH = 0xD9) ENABLE OR DISABLE CONFIG IN DEVCE SLOT 1

| Register | Value                   |
|----------|-------------------------|
| AH       | 0xD9                    |
| AL       | 1 = Enable, 0 = Disable |

#### (AH = 0xD8) COPY FILE FROM ONE HOST SLOT, TO ANOTHER

| Register | Value                                                                    |
|----------|--------------------------------------------------------------------------|
| AH       | 0xD8                                                                     |
| ES:BX    | Pointer to 256 byte NULL terminated string containing copy specification |
| CL       | Source host slot (0-7)                                                   |
| CH       | Destination host slot (0-7)                                              |

The copy specification is a source path, and destination path seperated by the | character, e.g.

``` text
/sourcefolder/sourcefile.img|/destfolder/destfile.img
```

#### (AH = 0xD7) MOUNT ALL

| Register | Value |
|----------|-------|
| AH       | 0xD7  |

#### (AH = 0xD6) SET BOOT MODE

| Register | Value                 |
|----------|-----------------------|
| AH       | 0xD6                  |
| AL       | Boot mode, see below. |

Boot Mode:

| Mode | Description              |
|------|--------------------------|
| 0    | Boot into CONFIG         |
| 1    | Boot into mount-and-boot |
| 2    | Boot into Game Lobby     |

#### (AH = 0xD5) ENABLE DEVICE

| Register | Value     |
|----------|-----------|
| AH       | 0xD5      |
| AL       | Device ID |

#### (AH = 0xD4) DISABLE DEVICE

| Register | Value     |
|----------|-----------|
| AH       | 0xD4      |
| AL       | Device ID |

#### (AH = 0xD3) RANDOM NUMBER (NOT IMPLEMENTED)

| Register | Value |
|----------|-------|
| AH       | 0xD3  |

TODO: Implement in firmware

Random integer is returned in AX (0-65535)

#### (AH = 0xD2) GET TIME (NOT IMPLEMENTED)

| Register | Value |
|----------|-------|
| AH       | 0xD2  |

TODO: Implement in firmware

#### (AH = 0xD1) DEVICE ENABLE STATUS (NOT IMPLEMENTED)

| Register | Value |
|----------|-------|
| AH       | 0xD1  |

TODO: Implement in Firmware

#### (AH = 0xD0) BASE64 ENCODE INPUT

| Register | Value                                                    |
|----------|----------------------------------------------------------|
| AH       | 0xD0                                                     |
| ES:BX    | Pointer to buffer of data to feed into base64 generator. |
| CX       | Length of buffer (0-65535)                               |

#### (AH = 0xCF) BASE64 ENCODE COMPUTE

| Register | Value |
|----------|-------|
| AH       | 0xCF  |

#### (AH = 0xCE) BASE64 ENCODE LENGTH

| Register | Value |
|----------|-------|
| AH       | 0xCE  |

Total length of computed BASE64 data is returned in BX = High 16 bits, AX = Low 16 bits.

#### (AH = 0xCD) BASE64 ENCODE OUTPUT

| Register | Value                                            |
|----------|--------------------------------------------------|
| AH       | 0xCD                                             |
| ES:BX    | Pointer to destination buffer for BASE64 output. |

#### (AH = 0xCC) BASE64 DECODE INPUT

| Register | Value                                                    |
|----------|----------------------------------------------------------|
| AH       | 0xCC                                                     |
| ES:BX    | Pointer to buffer of data to feed into base64 generator. |
| CX       | Length of buffer (0-65535)                               |

#### (AH = 0xCB) BASE64 DECODE COMPUTE

| Register | Value |
|----------|-------|
| AH       | 0xCB  |

#### (AH = 0xCA) BASE64 ENCODE LENGTH

| Register | Value |
|----------|-------|
| AH       | 0xCA  |

Total length of decoded BASE64 data is returned in BX = High 16 bits, AX = Low 16 bits.

#### (AH = 0xC9) BASE64 ENCODE OUTPUT

| Register | Value                                           |
|----------|-------------------------------------------------|
| AH       | 0xC9                                            |
| ES:BX    | Pointer to destination buffer for binary output |

#### (AH = 0xC8) HASH INPUT

| Register | Value                                   |
|----------|-----------------------------------------|
| AH       | 0xC8                                    |
| ES:BX    | Pointer to source buffer for hash input |
| CX       | Length in bytes (0-65535                |

#### (AH = 0xC7) HASH COMPUTE

| Register | Value                 |
|----------|-----------------------|
| AH       | 0xC7                  |
| AL       | Hash type, see below. |

| Hash Type | Description |
|-----------|-------------|
| 0         | MD5         |
| 1         | SHA1        |
| 2         | SHA256      |
| 3         | SHA512      |

#### (AH = 0xC6) HASH LENGTH

| Register | Value                                         |
|----------|-----------------------------------------------|
| AH       | 0xC6                                          |
| AL       | Type of data to retrieve: 0 = binary, 1 = HEX |

Total length of resulting hash is returned in AL

#### (AH = 0xC5) HASH OUTPUT

| Register | Value                                          |
|----------|------------------------------------------------|
| AH       | 0xC5                                           |
| AL       | Type of data to retrieve: 0 = binary, 1 = HEX  |
| ES:BX    | Pointer to destination buffer for hash output. |



#### (AH = 0x00) GET ERROR ADDRESS

| Register | Value |
|----------|-------|
| AH       | 0x00  |

Returns far pointer to the address of the error byte in ES:BX

### INT F6H - NETWORK ADAPTER

#### (AH = 0x00) GET STATUS ADDRESS

| Register | Value |
|----------|-------|
| AH       | 0x00  |

Returns far pointer to the address of the status bytes in ES:BX

The Status bytes are defined as follows

| Offset | Length | Description                       |
|--------|--------|-----------------------------------|
| 0      | 2      | Number of bytes waiting (0-65535) |
| 2      | 1      | Connected? (1 = true, 0 = false)  |
| 3      | 1      | Error code                        |

#### (AH = 0x20) - RENAME FILE

| Register | Value                                                               |
|----------|---------------------------------------------------------------------|
| AH       | 0x20                                                                |
| AL       | Unit Number (0-3)                                                   |
| ES:BX    | Pointer to 256 byte NULL terminated string with URL, example below. |

``` txt
N:SMB://WINSHARE/PUBLIC/OLDFILE.TXT,NEWFILE.TXT
```

#### (AH - 0x21) - DELETE FILE

| Register | Value                                   |
|----------|-----------------------------------------|
| AH       | 0x21                                    |
| AL       | Unit Number (0-3)                       |
| ES:BX    | Pointer to 256 byte NULL terminated URL |

#### (AH - 0x25) - SEEK IN FILE

| Register | Value                   |
|----------|-------------------------|
| AH       | 0x25                    |
| AL       | Unit Number (0-3)       |
| BX       | LO 16-bit byte position |
| CX       | HI 16-bit byte position |

#### (AH - 0x26) - TELL IN FILE

| Register | Value             |
|----------|-------------------|
| AH       | 0x25              |
| AL       | Unit Number (0-3) |

Return:

| Register | Value                   |
|----------|-------------------------|
| BX       | LO 16-bit byte position |
| CX       | HI 16-bit byte position |

#### (AH - 0x2A) - MAKE DIRECTORY

| Register | Value                                   |
|----------|-----------------------------------------|
| AH       | 0x2A                                    |
| AL       | Unit Number 0-3)                        |
| ES:BX    | Pointer to 256 byte NULL terminated URL |

#### (AH - 0x2B) - REMOVE DIRECTORY

| Register | Value                                   |
|----------|-----------------------------------------|
| AH       | 0x2B                                    |
| AL       | Unit Number (0-3)                       |
| ES:BX    | Pointer to 256 byte NULL terminated URL |

#### (AH - 0x2C) - CHANGE DIRECTORY

| Register | Value                                   |
|----------|-----------------------------------------|
| AH       | 0x2C                                    |
| AL       | Unit Number (0-3)                       |
| ES:BX    | Pointer to 256 byte NULL terminated URL |

#### (AH - 0x30) - GET CURRENT DIRECTORY

| Register | Value                                  |
|----------|----------------------------------------|
| AH       | 0x30                                   |
| AL       | Unit Number (0-3)                      |
| ES:BX    | Pointer to buffer for path (256 bytes) |

#### (AH = 0x41 'A') - ACCEPT CONNECTION

Accepts connection from listening socket.

| Register | Value             |
|----------|-------------------|
| AH       | 0x41 'A'          |
| AL       | Unit Number (0-3) |

#### (AH = 0x43 'C') - CLOSE CONNECTION

Closes the network connection. Does nothing if the network device is not opened.

| Register | Value    |
|----------|----------|
| AH       | 0x43 'C' |

#### (AH - 0x44 'D') - CHANGE UDP DESTINATION ADDRESS

Changes destination address for the next UDP packet.

| Register | Value                            |
|----------|----------------------------------|
| AH       | 0x44 'D'                         |
| AL       | Unit Number (0-3)                |
| ES:BX    | 256 byte NULL terminated UDP URL |

#### (AH = 0x4C 'M') - HTTP CHANNEL MODE

Changes current HTTP channel mode

| Register | Value             |
|----------|-------------------|
| AH       | 0x4C 'M'          |
| AL       | Unit Number (0-3) |
| CL       | HTTP Channel Mode |

HTTP Channel Modes:

| CL | Description     |
|----|-----------------|
| 0  | BODY            |
| 1  | COLLECT HEADERS |
| 2  | GET HEADERS     |
| 3  | SET HEADERS     |
| 4  | SET POST DATA   |

For more information, see this page:
https://github.com/FujiNetWIFI/fujinet-firmware/wiki/HTTP-Set-Channel-Mode

#### (AH = 0x4E 'O') - OPEN CONNECTION

Opens the network connection. 

| Register | Value                                   |
|----------|-----------------------------------------|
| AH       | 0x4F 'O'                                |
| AL       | Unit Number (0-3)                       |
| ES:BX    | 256 byte NULL terminated string for URL |
| CL       | Mode                                    |
| CH       | Translation (not used)                  |

for more info see:
https://github.com/FujiNetWIFI/fujinet-firmware/wiki/N%3A-SIO-Command-%27O%27---Open

#### (AH = 0x50 'P') - PARSE JSON

Read open connection and parse JSON document to be used by JSON QUERY.

| Register | Value             |
|----------|-------------------|
| AH       | 0x50 'P'          |
| AL       | Unit Number (0-3) |

#### (AH - 0x51 'Q') - QUERY JSON

| Register | Value                                                  |
|----------|--------------------------------------------------------|
| AH       | 0x51 'Q'                                               |
| AL       | Unit Number (0-3)                                      |
| ES:BX    | 256 byte NULL terminated buffer with JSON Query String |

#### (AH = 0x52 'R') - READ FROM CONNECTION

Read from network connection

| Register | Value                            |
|----------|----------------------------------|
| AH       | 0x52 'R'                         |
| AL       | Unit Number (0-3)                |
| ES:BX    | Destination buffer for read data |
| CX       | Length of data                   |

#### (AH = 0x53 'S') - EXPLICITLY GET STATUS

| Register | Value                                |
|----------|--------------------------------------|
| AH       | 0x53 'S'                             |
| AL       | Unit Number (0-3)                    |
| ES:BX    | Destination for 4 byte status buffer |

Status Buffer format:

| Offset | Length | Description                     |
|--------|--------|---------------------------------|
| 0      | 2      | # of bytes waiting (0-65535)    |
| 2      | 1      | 1 = connected, 0 = disconnected |
| 3      | 1      | Error code                      |

#### (AH = 0x57 'W') - WRITE TO CONNECTION

Write to network connection

| Register | Value                           |
|----------|---------------------------------|
| AH       | 0x57 'W'                        |
| AL       | Unit Number (0-3)               |
| ES:BX    | Buffer containing data to write |
| CX       | Length of data                  |

#### (AH = 0x63 'c') - CLOSE CLIENT CONNECTION

Closes a connection previously 'A'ccepted.

| Register | Value             |
|----------|-------------------|
| AH       | 0x63 'c'          |
| AL       | Unit Number (0-3) |

#### (AH - 0xFC) - SET CHANNEL MODE

| Register | Value                |
| AH       | 0xFC                 |
| AL       | Unit Number (0-3)    |
| CL       | 0 = Normal, 1 = JSON |

#### (AH - 0xFD) - SET USER NAME

| Register | Value                                                            |
|----------|------------------------------------------------------------------|
| AH       | 0xFD                                                             |
| AL       | Unit Number (0-3)                                                |
| ES:BX    | Pointer to 256 byte buffer (NULL terminated)containing username. |

#### (AH - 0xFE) - SET PASSWORD

| Register | Value                                                            |
|----------|------------------------------------------------------------------|
| AH       | 0xFE                                                             |
| AL       | Unit Number (0-3)                                                |
| ES:BX    | Pointer to 256 byte buffer (NULL terminated)containing username. |
