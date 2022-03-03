## 2host BOM

| Mouser  | QTY | Part | Value | Package |
|-----|:--:|-----|-----|-----|
|603-RC0805FR-07220RL|2|R3 R7|220R|0805|
|AC0805FR-0747RL|4|R1 R2 R5 R6|47R|0805|
|652-CR0805FX-4700ELF|2|R4 R8|470R|0805|
|80-C0805C104J5RACLR|2|C1 C2|100n|0805|
|833-1N4148W-TP|2|D1 D2|1N4148|SOD-123|                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                             
|859-6N138S|2|OK1 OK2|6N138|SMD-8|                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                             
||2| |TEENSY_LC||
|855-M20-9994045|1||male pin headers*||

[Mouser Cart](https://www.mouser.com/ProjectManager/ProjectDetail.aspx?AccessID=799423ce4a)

Order Teensy's from [PJRC here](https://www.pjrc.com/store/teensylc.html)

## Build notes

* pin headers can be whatever you have around. I get 1x40 headers in bulk from Tayda. Mouser has a 1x20 here [855-M20-9994045](https://www.mouser.com/ProductDetail/Harwin/M20-9994045?qs=%2Fha2pyFaduhF0xjSTgJg%252BrgY%252BmALgk%2FsNJjZomxYmF5SO6%252BK1RqInQ%3D%3D). Basically you need at least 2 1x3 headers to connect each of the Teensies. I also used a couple 1x2 headers as supports.
* Don't bother soldering pin headers to all the pins on the Teensies. You only need the last three pins on the USB connector end. (shown with white markings on the board)

![<2host-top>](<2host-top.png>)
![<2host-top>](<2host-bottom.png>)

