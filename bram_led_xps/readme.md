###Dual Port BRAM between PS & PL

*  **step 1** Add `bram_block` and `bram_ctrl`
*  **step 2** Disconnect PORTB between `bram_block` and `bram_ctrl`

![bram connection][connect]
[connect]: https://raw.github.com/godspeed1989/zedboard/master/bram_led_xps/bram_connect.png

*  **step 3** Add user peripheral (here is `bram_led`) and connect PORTB between `bram_block` and `bram_led`

![block RAM][bram]
[bram]: https://raw.github.com/godspeed1989/zedboard/master/bram_led_xps/bram.png
