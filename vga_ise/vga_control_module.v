`include "vga_params.v"

module vga_control_module
(
	input VGA_CLK,
	input [`P_WIDTH-1:0] X, Y,
	input valid,
	output [`D_WIDTH-1:0] VGA_R, VGA_G, VGA_B
);

reg rectangle;
always @ (posedge VGA_CLK)
	if(X > 100 && X < `H_ACT-100 && Y > 100 && Y < `V_ACT-100)
		rectangle <= 1'b0;
	else
		rectangle <= 1'b1;

assign VGA_R = valid && rectangle ? `D_WIDTH'b0000 : `D_WIDTH'b0;
assign VGA_G = valid && rectangle ? `D_WIDTH'b0000 : `D_WIDTH'b0;
assign VGA_B = valid && rectangle ? `D_WIDTH'b1111 : `D_WIDTH'b0;

endmodule
