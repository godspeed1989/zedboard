###Dual Port BRAM between PS & PL and VGA show

*  **step 1** Add `bram_block` and `bram_ctrl` cores
*  **step 2** Disconnect PORTB between `bram_block` and `bram_ctrl`
*  **step 3** Add user peripheral(here is `bram_vga`), connect PORTB between `bram_block` and `bram_vga`

![block RAM][bram]
[bram]: https://raw.github.com/godspeed1989/zedboard/master/bram_vga_xps/bram_vga.png
