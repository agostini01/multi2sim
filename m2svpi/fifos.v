`timescale 1ns/1ns
module tb_for_two_fifos;
parameter DATA_WIDTH = 32;
parameter ADDR_WIDTH = 31;
parameter REQ_WIDTH = 1 + ADDR_WIDTH + DATA_WIDTH; // request
parameter TID_WIDTH = 16;

//parameter ALL_FIFO_DEPTH = 8;
parameter FROM_DP_TO_VPI_DEPTH = 4;
parameter FROM_VPI_TO_DP_DEPTH = 4;

parameter DP_DATA_WIDTH  = TID_WIDTH+ REQ_WIDTH; // ID, Flag, Address, Data
parameter VPI_DATA_WIDTH  = TID_WIDTH + DATA_WIDTH; // ID, Data

// Internals
reg clk,reset;

reg [DP_DATA_WIDTH-1:0]from_DP_data_in;
reg from_DP_wr_ctr;

wire[DP_DATA_WIDTH-1:0]to_VPI_data_out;
reg to_VPI_rd_ctr;

wire to_VPI_empty,from_DP_full;

two_fifo_pipe U1(
	.clk(clk),				// Clock input
	.rst(reset),			// Active high reset

	// ------------------------------------>
	.from_DP_data_in(from_DP_data_in),	// Data input to vpi
	.from_DP_wr_ctr(from_DP_wr_ctr),	// Write enable
	.from_DP_full(from_DP_full),			// FIFO to vpi full

	.to_VPI_data_out(to_VPI_data_out), 	// Data output to vpi
	.to_VPI_rd_ctr(to_VPI_rd_ctr),		// Read Enable
	.to_VPI_empty(to_VPI_empty),		// FIFO from dp empty

	// <------------------------------------
	.from_VPI_data_in(),	// Data input to DP
	.from_VPI_wr_ctr(),	// Write Enable
	.from_VPI_full(),		// FIFO empty

	.to_DP_data_out(),		// Data output to vpi
	.to_DP_rd_ctr(),		// Read Enable
	.to_DP_empty()			// FIFO full
);
  

initial
begin
  clk=0;
  reset=0;
  from_DP_wr_ctr=0;
  to_VPI_rd_ctr=0;
  from_DP_data_in=3;
  #5  reset=1;
    #5  reset=0;
  #1000 $finish;
end

always
  #1 clk = !clk;

always
begin
  #50 from_DP_wr_ctr=1;
  #10 from_DP_wr_ctr=0;
end

//always
//  #10 data_serve_in = data_serve_in+3;
  
always
begin
  #100 to_VPI_rd_ctr=1;
  #10 to_VPI_rd_ctr=0;
end
  endmodule


module two_fifo_pipe (

clk,				// Clock input
rst,				// Active high reset

// ------------------------------------>
from_DP_data_in,	// Data input to vpi
from_DP_wr_ctr,		// Write enable
from_DP_full,			// FIFO to vpi full

to_VPI_data_out, 	// Data output to vpi
to_VPI_rd_ctr,		// Read Enable
to_VPI_empty,		// FIFO from dp empty

// <------------------------------------
from_VPI_data_in,	// Data input to DP
from_VPI_wr_ctr,	// Write Enable
from_VPI_full,		// FIFO empty

to_DP_data_out,		// Data output to vpi
to_DP_rd_ctr,		// Read Enable
to_DP_empty			// FIFO full

);



parameter DATA_WIDTH = 32;
parameter ADDR_WIDTH = 31;
parameter REQ_WIDTH = 1 + ADDR_WIDTH + DATA_WIDTH; // request
parameter TID_WIDTH = 16;

//parameter ALL_FIFO_DEPTH = 8;
parameter FROM_DP_TO_VPI_DEPTH = 4;
parameter FROM_VPI_TO_DP_DEPTH = 4;

parameter DP_DATA_WIDTH  = TID_WIDTH+ REQ_WIDTH; // ID, Flag, Address, Data
parameter VPI_DATA_WIDTH  = TID_WIDTH + DATA_WIDTH; // ID, Data


input clk, rst;

// [DP side]
input [DP_DATA_WIDTH-1:0] from_DP_data_in;
input from_DP_wr_ctr;
output from_DP_full;
output [VPI_DATA_WIDTH-1:0] to_DP_data_out;
input to_DP_rd_ctr;
output to_DP_empty;

// [VPI side]
input to_VPI_rd_ctr;
output [DP_DATA_WIDTH-1:0]to_VPI_data_out;
output to_VPI_empty;
input [VPI_DATA_WIDTH-1:0]from_VPI_data_in;
input from_VPI_wr_ctr;
output from_VPI_full;

// [From DP side]
syn_fifo #(.DATA_WIDTH(DP_DATA_WIDTH),.ADDR_WIDTH(FROM_DP_TO_VPI_DEPTH))REQUEST_FIFO(
.clk      (clk)				, // Clock input
.rst      (rst)				, // Active high reset
.wr_cs    (1'b1)			, // Write chip select
.rd_cs    (1'b1)			, // Read chip select
.data_in  (from_DP_data_in)	, // Data input   [DP side]
.rd_en    (to_DP_rd_ctr)	, // Read enable  [VPI side]
.wr_en    (from_DP_wr_ctr)	, // Write Enable [DP side]
.data_out (to_VPI_data_out)	, // Data Output  [VPI side]
.empty    (to_VPI_empty)		, // FIFO empty   [DP side]
.full     (from_DP_full)		  // FIFO full    [VPI side]
); 

// [From VPI side]
syn_fifo #(.DATA_WIDTH(VPI_DATA_WIDTH),.ADDR_WIDTH(FROM_VPI_TO_DP_DEPTH))SERVE_FIFO(
.clk      (clk)					, // Clock input
.rst      (rst)					, // Active high reset
.wr_cs    (1'b1)				, // Write chip select
.rd_cs    (1'b1)				, // Read chip select
.data_in  (from_VPI_data_in)	, // Data input   [VPI side]
.rd_en    (to_DP_rd_ctr)		, // Read enable  [DP side]
.wr_en    (from_VPI_wr_ctr)		, // Write Enable [VPI side]
.data_out (to_DP_data_out)		, // Data Output  [DP side]
.empty    (to_DP_empty)			, // FIFO empty   [VPI side]
.full     (from_VPI_full)			  // FIFO full    [DP side]
); 


endmodule

`timescale 1ns/1ns
module tb_for_fifo;
  reg clk,reset;

  wire [7:0]data_serve_in;
  assign data_serve_in [7:0]=3;
  reg write_serve_en;

  wire[7:0]data_serve_out;
  reg read_serve_en;

  wire empty_serve_flag,full_serve_flag;

  syn_fifo #(.DATA_WIDTH(8),.ADDR_WIDTH(8))test_fifo(
  .clk      (clk)             , // Clock input
  .rst      (reset)           , // Active high reset
  .wr_cs    (1'b0)            , // Write chip select
  .rd_cs    (1'b0)            , // Read chip select
  .data_in  (data_serve_in)   , // Data input   [m2s side]
  .rd_en    (read_serve_en)   , // Read enable  [request side]
  .wr_en    (write_serve_en)  , // Write Enable [m2s side]
  .data_out (data_serve_out)  , // Data Output  [request side]
  .empty    (empty_serve_flag), // FIFO empty   [m2s side]
  .full     (full_serve_flag)   // FIFO full    [request side]
  ); 
  
  
  
initial
begin
  clk=0;
  reset=0;
  write_serve_en=0;
  read_serve_en=0;
  //data_serve_in=0;
  #5  reset=1;
    #5  reset=0;
  #1000 $finish;
end

always
  #1 clk = !clk;

always
begin
  #50 write_serve_en=1;
  #10 write_serve_en=0;
end

//always
//  #10 data_serve_in = data_serve_in+3;
  
always
begin
  #100 read_serve_en=1;
  #10 read_serve_en=0;
end

endmodule

/*********************************************************************/
// The following designs were adapted from asic word
//-----------------------------------------------------
// Design Name : ram_dp_ar_aw
// File Name   : ram_dp_ar_aw.v
// Function    : Asynchronous read write RAM
// Coder       : Deepak Kumar Tala
//-----------------------------------------------------
module ram_dp_ar_aw (
address_0 , // address_0 Input
data_0    , // data_0 bi-directional
cs_0      , // Chip Select
we_0      , // Write Enable/Read Enable
oe_0      , // Output Enable
address_1 , // address_1 Input
data_1    , // data_1 bi-directional
cs_1      , // Chip Select
we_1      , // Write Enable/Read Enable
oe_1        // Output Enable
); 

parameter DATA_WIDTH = 8 ;
parameter ADDR_WIDTH = 8 ;
parameter RAM_DEPTH = 1 << ADDR_WIDTH;

//--------------Input Ports----------------------- 
input [ADDR_WIDTH-1:0] address_0 ;
input cs_0 ;
input we_0 ;
input oe_0 ; 
input [ADDR_WIDTH-1:0] address_1 ;
input cs_1 ;
input we_1 ;
input oe_1 ; 

//--------------Inout Ports----------------------- 
input [DATA_WIDTH-1:0] data_0 ; 
output [DATA_WIDTH-1:0] data_1 ;

//--------------Internal variables---------------- 
reg [DATA_WIDTH-1:0] data_0_out ; 
reg [DATA_WIDTH-1:0] data_1_out ;
reg [DATA_WIDTH-1:0] mem [0:RAM_DEPTH-1];

//--------------Code Starts Here------------------ 
// Memory Write Block 
// Write Operation : When we_0 = 1, cs_0 = 1
always @ (address_0 or cs_0 or we_0 or data_0
or address_1 or cs_1 or we_1 or data_1)
begin : MEM_WRITE
  if ( cs_0 && we_0 ) begin
     mem[address_0] <= data_0;
  end else if  (cs_1 && we_1) begin
     mem[address_1] <= data_1;
  end
end

// Tri-State Buffer control 
// output : When we_0 = 0, oe_0 = 1, cs_0 = 1
assign data_0 = (cs_0 && oe_0 && !we_0) ? data_0_out : {DATA_WIDTH{1'bz}}; 

// Memory Read Block 
// Read Operation : When we_0 = 0, oe_0 = 1, cs_0 = 1
always @ (address_0 or cs_0 or we_1 or oe_0)
begin : MEM_READ_0
  if (cs_0 && !we_0 && oe_0) begin
    data_0_out <= mem[address_0]; 
  end else begin
    data_0_out <= 0; 
  end
end 

//Second Port of RAM
// Tri-State Buffer control 
// output : When we_0 = 0, oe_0 = 1, cs_0 = 1
assign data_1 = (cs_1 && oe_1 && !we_1) ? data_1_out : {DATA_WIDTH{1'bz}}; 
// Memory Read Block 1 
// Read Operation : When we_1 = 0, oe_1 = 1, cs_1 = 1
always @ (address_1 or cs_1 or we_1 or oe_1)
begin : MEM_READ_1
  if (cs_1 && !we_1 && oe_1) begin
    data_1_out <= mem[address_1]; 
  end else begin
    data_1_out <= 0; 
  end
end

endmodule // End of Module ram_dp_ar_aw

//-----------------------------------------------------
// Design Name : syn_fifo
// File Name   : syn_fifo.v
// Function    : Synchronous (single clock) FIFO
// Coder       : Deepak Kumar Tala
//-----------------------------------------------------
module syn_fifo (
clk      , // Clock input
rst      , // Active high reset
wr_cs    , // Write chip select
rd_cs    , // Read chip select
data_in  , // Data input
rd_en    , // Read enable
wr_en    , // Write Enable
data_out , // Data Output
empty    , // FIFO empty
full       // FIFO full
);    
 
// FIFO constants
parameter DATA_WIDTH = 8;
parameter ADDR_WIDTH = 8;
parameter RAM_DEPTH = (1 << ADDR_WIDTH);
// Port Declarations
input clk ;
input rst ;
input wr_cs ;
input rd_cs ;
input rd_en ;
input wr_en ;
input [DATA_WIDTH-1:0] data_in ;
output full ;
output empty ;
output [DATA_WIDTH-1:0] data_out ;

//-----------Internal variables-------------------
reg [ADDR_WIDTH-1:0] wr_pointer;
reg [ADDR_WIDTH-1:0] rd_pointer;
reg [ADDR_WIDTH :0] status_cnt;
reg [DATA_WIDTH-1:0] data_out ;
wire [DATA_WIDTH-1:0] data_ram ;

//-----------Variable assignments---------------
assign full = (status_cnt == (RAM_DEPTH-1));
assign empty = (status_cnt == 0);

//-----------Code Start---------------------------
always @ (posedge clk or posedge rst)
begin : WRITE_POINTER
  if (rst) begin
    wr_pointer <= 0;
  end else if (wr_cs && wr_en ) begin
    wr_pointer <= wr_pointer + 1;
  end
end

always @ (posedge clk or posedge rst)
begin : READ_POINTER
  if (rst) begin
    rd_pointer <= 0;
  end else if (rd_cs && rd_en ) begin
    rd_pointer <= rd_pointer + 1;
  end
end

always  @ (posedge clk or posedge rst)
begin : READ_DATA
  if (rst) begin
    data_out <= 0;
  end else if (rd_cs && rd_en ) begin
    data_out <= data_ram;
  end
end

always @ (posedge clk or posedge rst)
begin : STATUS_COUNTER
  if (rst) begin
    status_cnt <= 0;
  // Read but no write.
  end else if ((rd_cs && rd_en) && !(wr_cs && wr_en) 
                && (status_cnt != 0)) begin
    status_cnt <= status_cnt - 1;
  // Write but no read.
  end else if ((wr_cs && wr_en) && !(rd_cs && rd_en) 
               && (status_cnt != RAM_DEPTH)) begin
    status_cnt <= status_cnt + 1;
  end
end 
   
ram_dp_ar_aw #(DATA_WIDTH,ADDR_WIDTH)DP_RAM (
.address_0 (wr_pointer) , // address_0 input 
.data_0    (data_in)    , // data_0 bi-directional
.cs_0      (wr_cs)      , // chip select
.we_0      (wr_en)      , // write enable
.oe_0      (1'b0)       , // output enable
.address_1 (rd_pointer) , // address_q input
.data_1    (data_ram)   , // data_1 bi-directional
.cs_1      (rd_cs)      , // chip select
.we_1      (1'b0)       , // Read enable
.oe_1      (rd_en)        // output enable
);     

endmodule
