module ledwater(
	input BTN,
	input CLK,
	output reg [7:0] LED
);

reg [31:0] cnt;
wire clk = CLK;
wire rst = ~BTN;

always @ (posedge clk or negedge rst)
begin
	if(rst == 1'b0)
	begin
		LED <= 8'b1000_0000;
		cnt <= 32'b0;
	end
	else
	begin
		if(cnt == 32'd25000000) begin
			cnt <= 32'b0;
			LED <= (LED>>1)|(LED<<7);
		end
		else begin
			cnt <= cnt + 32'd1;
		end
	end
end

endmodule
