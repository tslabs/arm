# USB-AY

This device is an interface between PC and AY/YM sound chip via USB port.


## Communication protocol

### Packet

All data is sent in packets, one packet in either direction per USB transaction.
Each message normally contains payload, its length and a checksum.
Payload within packet contains one of more messages.

| Offset | Size | Description           |
| ------ | ---- | --------------------- |
| 0      | 2    | Length of payload (n) |
| 2      | n    | Payload               |
| n+2    | 2    | CRC16                 |


### Message

Messages sent from Host to Device are considered settings or commands.
Messages sent from Device to Host are considered reports or responses.
Each message contains its code and (optionally) additional data.

| Offset | Size | Description  |
| ------ | ---- | ------------ |
| 0      | 1    | Message code |
| 1      | n    | Message data |


### Message codes

#### Host-to-Device

| Code | Name                        | Description                                           |
| ---- | --------------------------- | ----------------------------------------------------- |
| 0    | NOP                         |                                                       |
| 1    | SET_TS                      | Set the timestamp                                     |
| 2    | WRITE_REGS                  | Write a set of registers                              |
| 3    | RESET_TS                    | Reset timestamp on Device to zero value               |
| 4    | SET_PSG_CLK_FREQ            | Set frequency of the PSG clock                        |
| 5    | SET_PSG_CLK_FREQ_RAW        | Set frequency of the PSG clock using raw PLL settings |
| 6    | SEL_PSG                     | Select active PSG chip                                |
| 7    | SET_MUX                     | Set Channel Mux                                       |


#### Device-to-Host

| Code | Name             | Description                             |
| ---- | ---------------- | --------------------------------------- |
| 0    | NOP              | No operation                            |
| 1    | GET_TS           | Get current Device timestamp            |


### Messages format

#### No Operation

No operation.

No message data.


#### Set Timestamp, Get Timestamp

Timestamp sent by Host will be used for all subsequent operations requiring a such. If a timestamp sent by Host is earlier than current Device timestamp, Device will perform following operations immediately, but eventual sound artefacts may appear.
Timestamp reported by Device is used to synchronize packets sending by Host.
Each tick is calculated as ... (TBD)

| Offset | Size | Description     |
| ------ | ---- | --------------- |
| 1      | 4    | Number of ticks |


#### Write Register(s)

Command is used to write a set of registers directly into the PSG.
All registers in a single command will be written starting from the timestamp set by the Set Timestamp command. Writing of a single register takes about 1 clock tick (TBD).

| Offset | Size | Description                      |
| ------ | ---- | -------------------------------- |
| 1      | 1    | Number of registers to write (n) |
| 2      | 1    | Register 1 number                |
| 3      | 1    | Register 1 value                 |
| ...    |      |                                  |
| n*2    | 1    | Register n number                |
| n*2+1  | 1    | Register n value                 |


#### Reset Timestamp

Resets Device internal clock ticks value to 0. Used for Host-Device timestamps synchronization.

No message data.


#### Set PSG Clock Frequency

Sets the frequency of the clock which is directly fed to the PSG using preset values for the PLL.

| Offset | Size | Description                       |
| ------ | ---- | --------------------------------- |
| 1      | 1    | Frequency value (see table below) |

Frequency values:

| Frequency value | Clock Freqency |
| --------------- | -------------- |
| 0               | 1,75           |
| 1               | 1              |
| 2               | 2              |
| 3               | 1,777777778    |
| 4               | 3,555555556    |
| 5               | 3,5            |


#### Set PSG Clock Frequency Raw

Sets the frequency of the clock which is directly fed to the PSG using raw PLL values.
This command allows setting custom frequencies for the PSG.

| Offset | Size | Description |
| ------ | ---- | ----------- |
| 1      | 1    | Div~RAW~    |
| 2      | 1    | Mul~RAW~    |
| 3      | 1    | ARR~RAW~    |

Formula to calculate clock frequency, MHz: Clock = 4 * Mul / Div / ARR

Div~RAW~ = Div - 1

ARR~RAW~ = ARR - 1

Mul~RAW~:
| Mul  | Mul~RAW~ | Mul  | Mul~RAW~ | Mul  | Mul~RAW~ |
| ---- | ----- | ---- | ---- | ---- | ---- |
| 8    | 0x06  | 11   | 0x09 | 14   | 0x0C |
| 9    | 0x07  | 12   | 0x0A | 16   | 0x0E |
| 10   | 0x08  | 13   | 0x0B | 20   | 0x0F |

Pre-calculated values for frequencies:

|Clock, MHz | Div  | Mul  | ARR  | Div~RAW~ | Mul~RAW~ | ARR~RAW~ |
|---------- | ---- | ---- | ---- | -------- | ----     | ----     |
|1,75       |16    |14    |2     |0x0F      |0x0C      |0x01      |
|1          |8     |8     |4     |0x07      |0x06      |0x03      |
|2          |8     |8     |2     |0x07      |0x06      |0x01      |
|1,777777778|9     |8     |2     |0x08      |0x06      |0x01      |
|3,555555556|9     |16    |2     |0x08      |0x0E      |0x01      |
|3,5        |8     |14    |2     |0x07      |0x0C      |0x01      |


#### Select active PSG chip

Selects PSG chip to be addressed in all subsequent operations.

| Offset | Size | Description        |
| ------ | ---- | ------------------ |
| 1      | 1    | Chip number (0, 1) |


#### Set Channel Mux

Set mapping two PSG ABC channels to left/right speaker channel.

| Offset | Size | Description        |
| ------ | ---- | ------------------ |
| 1      | 1    | Volume of PSG0 channel A in left stereo channel  |
| 2      | 1    | Volume of PSG0 channel A in right stereo channel |
| 3      | 1    | Volume of PSG0 channel B in left stereo channel  |
| 4      | 1    | Volume of PSG0 channel B in right stereo channel |
| 5      | 1    | Volume of PSG0 channel C in left stereo channel  |
| 6      | 1    | Volume of PSG0 channel C in right stereo channel |
| 7      | 1    | Volume of PSG1 channel A in left stereo channel  |
| 8      | 1    | Volume of PSG1 channel A in right stereo channel |
| 9      | 1    | Volume of PSG1 channel B in left stereo channel  |
| 10     | 1    | Volume of PSG1 channel B in right stereo channel |
| 11     | 1    | Volume of PSG1 channel C in left stereo channel  |
| 12     | 1    | Volume of PSG1 channel C in right stereo channel |


### Packet binary example

| Hex values  | Description                                |
| ----------- | ------------------------------------------ |
| 12 00       | Length of payload (18 bytes)               |
| 05          | Set PSG Clock Frequency Raw command        |
| 09          | Div value (9)                              |
| 08          | Mul value (8)                              |
| 01          | ARR value (1)                              |
| 03          | Reset Clock command                        |
| 01          | Set Timestamp command                      |
| 00 80 00 00 | Timestamp value (32768 ticks)              |
| 02          | Write Registers command                    |
| 03          | Number of registers to write (3 registers) |
| 08 0F       | Register 8, value 15                       |
| 09 10       | Register 9, value 16                       |
| 0A 08       | Register 10, value 8                       |
| xx xx       | CRC16                                      |

