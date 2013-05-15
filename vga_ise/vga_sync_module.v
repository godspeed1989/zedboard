module sync_module(
	input VGA_CLK,  //800x600
	output VGA_HS, VGA_VS, valid,
	output[10:0] X, Y //column and row addr signal
);
 
reg[10:0] Count_H;
always@(posedge VGA_CLK)
	if(Count_H==11'd1056)
        Count_H<=11'd0;
    else
        Count_H<=Count_H+1'b1;

reg[10:0] Count_V;
always@(posedge VGA_CLK)
    if(Count_V==11'd628)
        Count_V<=11'd0;
    else if(Count_H==11'd1056)
        Count_V<=Count_V+1'b1;
         
reg valid_r;
always@(posedge VGA_CLK)
    if((Count_H>216&&Count_H<1017)&&(Count_V>11'd27&&Count_V<11'd627))
        valid_r<=1'b1;
    else
        valid_r<=1'b0;
         
assign VGA_HS=(Count_H<=11'd128) ? 1'b0 : 1'b1;
assign VGA_VS=(Count_V<=11'd4) ? 1'b0 : 1'b1;
assign valid=valid_r;
 
assign X=valid_r ? Count_H-11'd217 : 11'd0;
assign Y=valid_r ? Count_V-11'd28 : 11'd0;
 
endmodule
