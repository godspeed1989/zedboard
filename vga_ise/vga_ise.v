module vga_ise(i_clk100MHz, o_hsync, o_vsync,
               o_red, o_green, o_blue);
input        i_clk100MHz;
output       o_hsync;
output       o_vsync;
output [3:0] o_red;
output [3:0] o_green;
output [3:0] o_blue;

	vga vga(i_clk100MHz,
			o_hsync, o_vsync,
			o_red, o_green, o_blue);

endmodule

module vga(i_clk100MHz, o_hsync, o_vsync,
           o_red, o_green, o_blue);
input               i_clk100MHz;
output  reg         o_hsync;
output  reg         o_vsync;
output  reg   [3:0] o_red;
output  reg   [3:0] o_green;
output  reg   [3:0] o_blue;
// internal registers
reg clk50Mhz;
reg clk25Mhz;
reg [11:0] H_Cont;
reg [11:0] V_Cont;
reg data_o_enable;

// Horizontal	Parameter
parameter	H_FRONT	=	16;
parameter	H_SYNC	=	96;
parameter	H_BACK	=	48;
parameter	H_ACT	=	640;
parameter	H_BLANK	=	H_FRONT+H_SYNC+H_BACK;
parameter	H_TOTAL	=	H_FRONT+H_SYNC+H_BACK+H_ACT;
// Vertical Parameter
parameter	V_FRONT	=	10;
parameter	V_SYNC	=	2;
parameter	V_BACK	=	33;
parameter	V_ACT	=	480;
parameter	V_BLANK	=	V_FRONT+V_SYNC+V_BACK;
parameter	V_TOTAL	=	V_FRONT+V_SYNC+V_BACK+V_ACT;

// clock generate
always @ (posedge i_clk100MHz)
begin
	clk50Mhz = ~clk50Mhz;
	if(clk50Mhz == 1'b1)
		clk25Mhz = ~clk25Mhz;
end

// Horizontal Generator: Refer to the pixel clock
always @ (posedge i_clk100MHz)
begin
	if(clk25Mhz == 1'b1)
	begin
		if(H_Cont < H_TOTAL-1)
			H_Cont <= H_Cont + 1'b1;
		else
			H_Cont <= 0;
		//	Horizontal Sync
		if(H_Cont == H_FRONT-1)			// Front porch end
			o_hsync <= 1'b0;
		if(H_Cont == H_FRONT+H_SYNC-1)	// Sync pulse end
			o_hsync <= 1'b1;
	end
end

// Vertical Generator: Refer to the horizontal sync
always @ (posedge o_hsync)
begin
	if(V_Cont < V_TOTAL-1)
		V_Cont <= V_Cont+1'b1;
	else
		V_Cont <= 0;
		//	Vertical Sync
		if(V_Cont == V_FRONT-1)			// Front porch end
			o_vsync <= 1'b0;
		if(V_Cont == V_FRONT+V_SYNC-1)	// Sync pulse end
			o_vsync <= 1'b1;
end

// data out enable signal
always @ (H_Cont or V_Cont)
begin
	if( (H_Cont >= H_BACK+H_SYNC && H_Cont < H_BACK+H_SYNC+H_ACT) &&
	    (V_Cont >= V_BACK+H_SYNC && H_Cont < V_BACK+V_SYNC+V_ACT+V_FRONT) )
		data_o_enable = 1'b1;
	else
		data_o_enable = 1'b0;
end

// data out
always @ (data_o_enable)
begin
	if(data_o_enable == 1'b1) begin
		if(H_Cont < H_BACK+H_SYNC+H_ACT/2) begin  // left part red
			o_red    <= 4'b1111;
			o_green  <= 4'b0000;
			o_blue   <= 4'b0000;
		end else begin //right part green
			o_red    <= 4'b0000;
			o_green  <= 4'b1111;
			o_blue   <= 4'b0000;
		end
	end else begin
		o_red    <= 4'b0000;
		o_green  <= 4'b0000;
		o_blue   <= 4'b0000;
	end
end

endmodule

