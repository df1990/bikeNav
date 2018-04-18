
Connections:


| FUNC      | PIN  | MORPHO PIN | DIR | PIN  | FUNC |
|:-------------------------------------------------:|
| GPS                                               |
|:---------:|:----:|:----------:|:---:|:----:|:----:|
| USART3_TX | PC10 | CN7_1      | ->  | 10   | RX   |
| USART3_RX | PC11 | CN7_2      | <-  | 9    | TX   |
| LCD                                               |
|:---------:|:----:|:----------:|:---:|:----:|:----:|
| GPIO      | PB0  | CN7_34     | ->  | 7    | SCK  |
| GPIO      | PC0  | CN7_38     | ->  | 6    | DIN  |
| GPIO      | PC1  | CN7_36     | ->  | 5    | DC   |
| GPIO      | PC2  | CN7_35     | ->  | 4    | RST  |
| GPIO      | PC3  | CN7_37     | ->  | 3    | SCE  |
| uSD                                               |
|:---------:|:----:|:----------:|:---:|:----:|:----:|
| SPI2_MOSI | PB15 | CN10_26    | ->  |  ?   | DI   |
| SPI2_MISO | PB14 | CN10_28    | <-  |  ?   | DO   |
| SPI2_SCK  | PB13 | CN10_30    | ->  |  ?   | SCK  |
| GPIO      | PB1  | CN10_24    | ->  |  ?   | CS   |
| GPIO      | PB2  | CN10_22    | <-  |  ?   | CD   |
