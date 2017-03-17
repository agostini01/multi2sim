/**********************************************************************
* Created by Nicolas
*********************************************************************/

/*********************************************************************/
`timescale 1ns / 100ps
module top;

// Parameters---------------------------------------------------
parameter DATA_WIDTH = 32;
parameter ADDR_WIDTH = 31;
parameter REQ_WIDTH = 1 + ADDR_WIDTH + DATA_WIDTH; // request
parameter TID_WIDTH = 16;

//parameter ALL_FIFO_DEPTH = 8;
parameter FROM_DP_TO_VPI_DEPTH = 4;
parameter FROM_VPI_TO_DP_DEPTH = 4;

parameter DP_DATA_WIDTH  = TID_WIDTH+ REQ_WIDTH; // ID, Flag, Address, Data
parameter VPI_DATA_WIDTH  = TID_WIDTH + DATA_WIDTH; // ID, Data


// Inputs!
wire [DP_DATA_WIDTH-1:0]from_DP_data_in;
wire from_DP_wr_ctr;

wire[VPI_DATA_WIDTH-1:0]to_VPI_data_out;
reg to_VPI_rd_ctr;

wire to_VPI_empty,from_DP_full;

// Inputs ----------------------------------
reg clk,reset,write_en,read_en;
// request
reg rw_flag;
reg[ADDR_WIDTH-1:0] address;
reg[DATA_WIDTH-1:0] data;

wire[REQ_WIDTH-1:0] request;
assign request={rw_flag,address,data};

// identification
reg[TID_WIDTH-1:0] identification;

// Outputs ----------------------------------
wire[DATA_WIDTH-1:0] data_out;
wire[TID_WIDTH-1:0] identification_out;

// Control signals --------------------------
wire full, empty;

reg[256*8:0] string;

initial begin
	#0 clk = 0;
	#0 reset = 0;
	#0 write_en = 0;
	#0 read_en = 0;
	#0 address = 0;
	#0 rw_flag = 0;
	#0 data = 7;
	#0 identification =0;

	#5 reset = 1;
	#5 reset = 0;
	#10001 $finish;
end

// Stim generator
always begin
	#1  clk = ~clk; // Toggle clock every 5 ticks
end

always begin
	#50 write_en = 1;
	#0 identification = identification+1;
	#0 address=$urandom_range(255,0);
	#0 rw_flag=$urandom_range(1,0);
	#2 write_en = 0;
end


// Inputs From Amir side


  two_fifo_pipe U1(
	.clk(clk),				// Clock input
	.rst(reset),			// Active high reset

	// ------------------------------------>
	.A_data_in(request),			// Data input to vpi
	.A_wr_ctr(request_write_en),	// Write enable
	.A_full(full_flag),				// FIFO to vpi full

	.A_data_out(data_req_out), 		// Data output to vpi
	.A_rd_ctr(1'b0),				// Read Enable
	.A_empty(empty_req_flag),		// FIFO from dp empty

	// <------------------------------------
	.B_data_in(ret_data_serve_in),	// Data input to DP
	.B_wr_ctr(1'b0),				// Write Enable
	.B_full(full_serve_flag),		// FIFO empty

	.B_data_out(served_data_out),		// Data output to vpi
	.B_rd_ctr(served_read_en),		// Read Enable
	.B_empty(empty_flag)			// FIFO full
);

  // Instance generator
  interface_test i1(
    .clk(clk),
    .reset(reset),
    
    .request_write_en(write_en),
    .request_in(request),
    .identification_in(identification),
    .full_flag(full),
    
    .served_read_en(read_en),
    .data_out(data_out),
    .identification_out(identification_out),
    .empty_flag(empty)
    );

endmodule
/*********************************************************************/


/*** An RTL For the VPI communication ***/
//-----------------------------------------------------
// Design Name : interface_test
// File Name   : module_tb.v
// Function    : interface_test to communicate to multi2sim
// Coder       : Nicolas Bohm Agostini
//-----------------------------------------------------
module interface_test (
	clk, reset,
	read_ctr, data_in, empty_flag,
	write_ctr, data_out, full_flag
);
	


parameter DATA_WIDTH = 32;
parameter ADDR_WIDTH = 31;
parameter REQ_WIDTH = 1 + ADDR_WIDTH + DATA_WIDTH; // request
parameter TID_WIDTH = 16;

parameter FROM_DP_TO_VPI_DEPTH = 4;
parameter FROM_VPI_TO_DP_DEPTH = 4;

parameter DP_DATA_WIDTH  = TID_WIDTH+ REQ_WIDTH; // ID, Flag, Address, Data
parameter VPI_DATA_WIDTH  = TID_WIDTH + DATA_WIDTH; // ID, Data

  // Clock, reset
  input clk, reset;
  
  // Access request ports 
  // Direction ----------->
  output read_ctr;
  input [DP_DATA_WIDTH-1:0] data_in;
  input empty_flag;
  // Derection <-----------
  output write_ctr;
  output [VPI_DATA_WIDTH-1:0]data_out;
  input full_flag;


  // Subfields
  // Direction ----------->
  wire [TID_WIDTH-1:0] req_id;
  wire [REQ_WIDTH-1:0] req_data_in;
  wire req_flag; 
  wire [ADDR_WIDTH-1:0] req_address;
  wire [DATA_WIDTH-1:0] req_data;
  assign req_id       [TID_WIDTH-1:0]   = data_in[DP_DATA_WIDTH-1:REQ_WIDTH];
  assign req_data_in  [REQ_WIDTH-1:0]   = data_in[REQ_WIDTH-1:0];
  assign req_flag                       = req_data_in [REQ_WIDTH-1];
  assign req_address  [ADDR_WIDTH-1:0]  = req_data_in[REQ_WIDTH-1:DATA_WIDTH];
  assign req_data     [DATA_WIDTH-1:0]  = req_data_in [DATA_WIDTH-1:0];
  // Derection <-----------
  reg [TID_WIDTH-1:0] fetch_id;
  reg [DATA_WIDTH-1:0] fetch_data;
  assign data_out[VPI_DATA_WIDTH-1:0] = {fetch_id,fetch_data};

  
// BEHAVIOURAL

  // Other signals
  reg running;

  // State machine to initialize the m2s interface_test
  parameter init_st=0, reset_st=1, work_st=2, finish_st=3;
  reg [1:0] state;

  // Next state logic
  always @(posedge clk or posedge reset)
  begin : NEXT_STATE
    case (state)

      init_st:
       state = reset_st;

      reset_st:
        if (reset)
          state = reset_st;
        else
          state = work_st;

      work_st:
        if (reset)
          state = reset_st;
        else if (finish_st)
          state = finish_st;
        else
          state = work_st;

      finish_st:
        state = finish_st;

    endcase
  end


  // Request out

  initial begin
    $m2s_initialize;
    running=1;
  end

  always @ (posedge clk)
  begin : FLOW_LOGIC
    //ret_data = 0;
    if (!reset)
    begin
      if(state==work_st)
      begin
        //req_data=$m2s_step;

        if (1)//(rw_flag)
        begin
          //$m2s_access(1, 1, address);
        end
      end

      if(state==finish_st)
      begin
        if(running) // latch
        begin
          $m2s_finalize;
          running = 0;
        end
        else
          running = 1;
      end

    end
    else
      $m2s_reset;


  end

  // Must do write_en logic!!!!!!!

endmodule



