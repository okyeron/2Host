## 2host BOM

| Mouser  | QTY | Part | Value | Package |
|-----|:--:|-----|-----|-----|
||2|R3 R7|220R|0805|
||4|R1 R2 R5 R6|47R|0805|
||2|R4 R8|470R|0805|
|80-C0805C104J5RACLR|2|C1 C2|100n|0805|
|833-1N4148W-TP|2|D1 D2|1N4148|SOD-123|                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                             
||2|OK1 OK2|6N138|SMD-8|                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                             
||2| |TEENSY_LC||
||1||male pin headers*||

## Build notes

* pin header can be whatever you have around. I get 1x40 headers in bulk from Tayda. Mouser has a 1x20 here [855-M20-9994045](https://www.mouser.com/ProductDetail/Harwin/M20-9994045?qs=%2Fha2pyFaduhF0xjSTgJg%252BrgY%252BmALgk%2FsNJjZomxYmF5SO6%252BK1RqInQ%3D%3D). Basically you need at least 2 1x3 headers to connect the Teensy's I also used a couple 1x2 headers as supports.

Don't bother soldering pin headers to all the pins on the Teensies. You only need the last three pins on the USB connector end. (shown with white markings on the board)

![<2host-top>](<2host-top.png>)
![<2host-top>](<2host-bottom.png>)