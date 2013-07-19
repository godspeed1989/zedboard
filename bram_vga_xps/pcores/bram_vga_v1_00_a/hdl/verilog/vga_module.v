`include "vga_params.v"

// Display a solid rectangle
module vga_module
(
	input CLOCK_50,
	input [`D_WIDTH-1:0]  Red, Green, Blue,
	output [`D_WIDTH-1:0] VGA_R, VGA_G, VGA_B,
	output VGA_HS, VGA_VS
);

wire VGA_CLK = CLOCK_50;
wire [`P_WIDTH-1:0] X, Y;
wire valid;

vga_sync_module sync
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
	.Red(Red), .Green(Green), .Blue(Blue),
	.valid(valid)
);

endmodule
