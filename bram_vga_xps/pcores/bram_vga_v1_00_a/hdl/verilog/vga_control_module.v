`include "vga_params.v"

module vga_control_module
(
	input VGA_CLK,
	input [`P_WIDTH-1:0] X, Y,
	input valid,
	input [`D_WIDTH-1:0]  Red, Green, Blue,
	output [`D_WIDTH-1:0] VGA_R, VGA_G, VGA_B
);

reg rectangle;
always @ (posedge VGA_CLK)
	if(X > 100 && X < `H_ACT-100 && Y > 100 && Y < `V_ACT-100)
		rectangle <= 1'b1;
	else
		rectangle <= 1'b0;

//`define DEBUG
`ifdef DEBUG
assign VGA_R = valid && rectangle ? `D_WIDTH'b0 : `D_WIDTH'b0;
assign VGA_G = valid && rectangle ? `D_WIDTH'b0 : `D_WIDTH'b0;
assign VGA_B = valid && rectangle ? `D_WIDTH'b1100 : `D_WIDTH'b0;
`else
assign VGA_R = valid && rectangle ? Red : `D_WIDTH'b0;
assign VGA_G = valid && rectangle ? Green : `D_WIDTH'b0;
assign VGA_B = valid && rectangle ? Blue : `D_WIDTH'b0;
`endif

endmodule
