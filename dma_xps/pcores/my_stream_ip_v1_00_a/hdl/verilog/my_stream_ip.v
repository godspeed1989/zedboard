//----------------------------------------------------------------------------
// my_stream_ip - module
//----------------------------------------------------------------------------
// Filename:          my_stream_ip
// Version:           1.00.a
// Description:       Example Axi Streaming core (Verilog).
// Verilog Standard:  Verilog-2001
//////////////////////////////////////////////////////////////////////////////

module my_stream_ip 
	(
		// DO NOT EDIT BELOW THIS LINE ////////////////////
		ACLK,
		ARESETN,
		S_AXIS_TREADY,
		S_AXIS_TDATA,
		S_AXIS_TLAST,
		S_AXIS_TVALID,
		M_AXIS_TVALID,
		M_AXIS_TDATA,
		M_AXIS_TLAST,
		M_AXIS_TREADY,
		M_AXIS_TKEEP
		// DO NOT EDIT ABOVE THIS LINE ////////////////////
	);

input                          ACLK;    // Synchronous clock
input                          ARESETN; // System reset, active low
// slave in interface
output                         S_AXIS_TREADY;  // Ready to accept data in
input      [31 : 0]            S_AXIS_TDATA;   // Data in
input                          S_AXIS_TLAST;   // Optional data in qualifier
input                          S_AXIS_TVALID;  // Data in is valid
// master out interface
output                         M_AXIS_TVALID;  // Data out is valid
output     [31 : 0]            M_AXIS_TDATA;   // Data Out
output                         M_AXIS_TLAST;   // Optional data out qualifier
input                          M_AXIS_TREADY;  // Connected slave device is ready to accept data out
output     [3:0]               M_AXIS_TKEEP;   // Keep which byte of the out data

//----------------------------------------
// Implementation Section
//----------------------------------------
// In this section, we povide an example implementation of MODULE my_stream_ip
// that does the following:
//
// 1. Read all inputs
// 2. Add each input to the contents of register 'sum' which
//    acts as an accumulator
// 3. After all the inputs have been read, write out the
//    content of 'sum' into the output stream NUMBER_OF_OUTPUT_WORDS times
//
// You will need to modify this example for
// MODULE my_stream_ip to implement your coprocessor

   // Total number of input data.
   localparam NUMBER_OF_INPUT_WORDS  = 8;

   // Total number of output data
   localparam NUMBER_OF_OUTPUT_WORDS = 8;

   // Define the states of state machine
   localparam Idle  = 3'b100;
   localparam Read_Inputs = 3'b010;
   localparam Write_Outputs  = 3'b001;

   reg [2:0] state;

   // Accumulator to hold sum of inputs read at any point in time
   reg [31:0] sum;

   // Counters to store the number inputs read & outputs written
   reg [NUMBER_OF_INPUT_WORDS - 1:0] nr_of_reads;
   reg [NUMBER_OF_OUTPUT_WORDS - 1:0] nr_of_writes;

   // CAUTION:
   // The sequence in which data are read in should be
   // consistent with the sequence they are written

   assign S_AXIS_TREADY = (state == Read_Inputs);
   assign M_AXIS_TVALID = (state == Write_Outputs);

   assign M_AXIS_TDATA = sum;
   assign M_AXIS_TLAST = 1'b0;
   assign M_AXIS_TKEEP = 4'b1111;

   always @(posedge ACLK) 
   begin

      /****** Synchronous reset (active low) ******/
      if (!ARESETN)
        begin
           // CAUTION: make sure your reset polarity is consistent with the
           // system reset polarity
           state        <= Idle;
           nr_of_reads  <= 0;
           nr_of_writes <= 0;
           sum          <= 0;
        end
      /************** state machines **************/
      else
        case (state)

          Idle:
            if (S_AXIS_TVALID == 1)
            begin
              state       <= Read_Inputs;
              nr_of_reads <= NUMBER_OF_INPUT_WORDS - 1;
              sum         <= 0;
            end

          Read_Inputs:
            if (S_AXIS_TVALID == 1) 
            begin
              // --- Coprocessor function happens below --- //
              sum  <=  sum + S_AXIS_TDATA;
              // --- Coprocessor function happens above --- //
              if (nr_of_reads == 0)
                begin
                  state        <= Write_Outputs;
                  nr_of_writes <= NUMBER_OF_OUTPUT_WORDS - 1;
                end
              else
                nr_of_reads <= nr_of_reads - 1;
            end

          Write_Outputs:
            if (M_AXIS_TREADY == 1) 
            begin
              if (nr_of_writes == 0) 
                 state <= Idle;
              else
                 nr_of_writes <= nr_of_writes - 1;
            end
        endcase
   end

endmodule
