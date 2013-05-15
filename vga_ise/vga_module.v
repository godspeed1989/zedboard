`include "vga_params.v"

module pll_module
(
	input i_clock100MHz,
	output reg o_clock50MHz
);

always @ (posedge i_clock100MHz)
	o_clock50MHz <= ~o_clock50MHz;

endmodule


//* Display a rectangle
module vga_module
(
	input CLOCK_100,
	output [`D_WIDTH-1:0] VGA_R, VGA_G, VGA_B,
	output VGA_HS, VGA_VS
);
wire VGA_CLK;
pll_module clk
(
	.i_clock100MHz(CLOCK_100),
	.o_clock50MHz(VGA_CLK)
);

wire [`P_WIDTH-1:0] X,Y;
wire valid;
sync_module sync
(
	.VGA_CLK(VGA_CLK),
	.VGA_HS(VGA_HS),
	.VGA_VS(VGA_VS),
	.X(X), .Y(Y),
	.valid(valid)
);

vga_control_module control
(
	.VGA_CLK(VGA_CLK),
	.VGA_R(VGA_R),
	.VGA_G(VGA_G),
	.VGA_B(VGA_B),
	.X(X), .Y(Y),
	.valid(valid)
);

endmodule
