
module user_logic
(
  bram_Rst,
  bram_Clk,
  bram_En,
  bram_WE,
  bram_Addr,
  bram_WrData,
  bram_RdData,
  vga_HS,
  vga_VS,
  vga_R,
  vga_G,
  vga_B,

  Bus2IP_Clk                     // Bus to IP clock
);

parameter C_S_AXI_DATA_WIDTH             = 32;
parameter C_S_AXI_ADDR_WIDTH             = 32;
parameter USER_VGA_DATA_WIDTH            = 4;

output                                     bram_Rst;
output                                     bram_Clk;
output                                     bram_En;
output     [C_S_AXI_DATA_WIDTH/8 - 1 : 0]  bram_WE;
output     [C_S_AXI_ADDR_WIDTH - 1 : 0]    bram_Addr;
output     [C_S_AXI_DATA_WIDTH - 1 : 0]    bram_WrData;
input      [C_S_AXI_DATA_WIDTH - 1 : 0]    bram_RdData;
output                                     vga_HS;
output                                     vga_VS;
output     [USER_VGA_DATA_WIDTH - 1 : 0]   vga_R;
output     [USER_VGA_DATA_WIDTH - 1 : 0]   vga_G;
output     [USER_VGA_DATA_WIDTH - 1 : 0]   vga_B;

input                                      Bus2IP_Clk;

//----------------------------------------------------------------------------
// Implementation
//----------------------------------------------------------------------------

  reg    [C_S_AXI_DATA_WIDTH - 1 : 0]   data_in;

  // USER logic implementation added here
  assign bram_Clk = Bus2IP_Clk;
  assign bram_En = 1'b1;
  assign bram_Rst = 1'b0;
  assign bram_Addr = 0;
  assign bram_WE = 1'b0;
  assign bram_WrData = 0;

  always @ ( posedge Bus2IP_Clk )
  begin
    data_in <= bram_RdData;
  end

  wire [USER_VGA_DATA_WIDTH-1 : 0] Red    = data_in[3:0];
  wire [USER_VGA_DATA_WIDTH-1 : 0] Green  = data_in[11:8];
  wire [USER_VGA_DATA_WIDTH-1 : 0] Blue   = data_in[19:16];
  vga_module vga
  (
    .CLOCK_50(Bus2IP_Clk),
    .VGA_R(vga_R),
    .VGA_G(vga_G),
    .VGA_B(vga_B),
    .VGA_HS(vga_HS),
    .VGA_VS(vga_VS),
    .Red(Red),
    .Green(Green),
    .Blue(Blue)
  );

endmodule

