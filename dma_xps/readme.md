###Dual Port BRAM between PS & PL and VGA show

*  **step 1** Add `axi_dma` IP core, disable `Scatter Gather`
![axi dma][nosg]
[nosg]: https://raw.github.com/godspeed1989/zedboard/master/dma_xps/nosg.png

*  **step 2** Fix a bug (below XPS 14.5), Zedboard has 512MB RAM(0x0000_0000~0x1FFF_FFFF)
![xps bug][hp0]
[hp0]: https://raw.github.com/godspeed1989/zedboard/master/dma_xps/hp0.png

*  **step 3** Add user peripheral(here is `my_stream_ip`)

*  **step 4** Connect `my_stream_ip:M_AXIS` and `S_AXIS_S2MM`
*  **step 5** Connect `my_stream_ip:M_AXIS` and `M_AXIS_MM2S`
![my stream ip][connect]
[connect]: https://raw.github.com/godspeed1989/zedboard/master/dma_xps/connect.png

*  **step 6** Setup clocks in the `Ports` tab
![setup clock][clock]
[clock]: https://raw.github.com/godspeed1989/zedboard/master/dma_xps/clk.png
