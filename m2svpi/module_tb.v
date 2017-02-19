/**********************************************************************
* $show_all_signals example 2 -- Verilog HDL test bench.
*
* Verilog test bench to test the $show_all_signals PLI application on
* a 1-bit adder modeled using gate primitives.
*
* For the book, "The Verilog PLI Handbook" by Stuart Sutherland
*  Copyright 1999 & 2001, Kluwer Academic Publishers, Norwell, MA, USA
*   Contact: www.wkap.il
*  Example copyright 1998, Sutherland HDL Inc, Portland, Oregon, USA
*   Contact: www.sutherland-hdl.com
*********************************************************************/

/*********************************************************************/
`timescale 1ns / 100ps
module top;

  reg clk, reset;
  reg write_en;
  reg read_en;

  // Inputs ----------------------------------
  // request
  reg rw_flag;
  reg[30:0] address;
  reg[31:0] data;

  wire[63:0] request;
  assign request={rw_flag,address,data};

  // identification
  reg[9:0] identification;

  // Outputs -----------------------------------
  wire[31:0] data_out;
  wire[9:0] identification_out;
  wire[9:0] next_id_available_out;
  wire full, empty;

  reg[256*8:0] string;

  initial
  begin
      string = "--si-sim detailed --si-config si-config \
  --mem-config mem-si-1 \
  --si-report si_report \
  --mem-report mem_report \
  --trace trace.gz \
  --mem-debug debug.mem \
  outM2S";
  end

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
  always
  begin
    #1  clk = ~clk; // Toggle clock every 5 ticks
  end

  always
  begin
      #50 write_en = 1;
      #0 identification = identification+1;
      #0 address=$urandom_range(255,0);
      #0 rw_flag=$urandom_range(1,0);
      #2 write_en = 0;
  end


  // Instance generator
  interface i1(
    .clk(clk),
    .reset(reset),
    
    .request_write_en(write_en),
    .request_in(request),
    .identification_in(identification),
    .full_flag(full),
    
    .served_read_en(read_en),
    .data_out(data_out),
    .identification_out(identification_out),
    .next_id_available_out(next_id_available_out),
    .empty_flag(empty)
    );

endmodule
/*********************************************************************/


/*** An RTL For the VPI communication ***/
//-----------------------------------------------------
// Design Name : interface
// File Name   : module_tb.v
// Function    : Interface to communicate to multi2sim
// Coder       : Nicolas Bohm Agostini
//-----------------------------------------------------
module interface (
	clk, reset, 
	request_write_en, request_in, identification_in, full_flag,
  served_read_en, data_out, identification_out, next_id_available_out, empty_flag);

  // Clock, reset
  input clk, reset;
  
  // Access request ports 
  // Derection ----------->
  input request_write_en;
  input [63:0] request_in;
  input [9:0] identification_in;
  output full_flag;
  // Subfields
  wire [73:0] request_data_in;
  assign request_data_in [73:0] = 15// TODO //{request_in[63:0],identification_in[9:0]};

  // Access served ports
  // Derection <-----------
  input served_read_en;
  output [31:0] data_out;
  output [9:0] identification_out;
  output [9:0] next_id_available_out;
  output empty_flag;
  //Subfields
  wire[51:0] served_data_out;
  assign data_out = served_data_out[51:20];
  assign identification_out = served_data_out[19:10];
  assign next_id_available_out = served_data_out[9:0];
  
  // Extra wires needed after fifo
  // Derection ----------->
  wire [73:0]data_req_out;
  wire empty_req_flag;
  
  // Derection <-----------
  wire [51:0]ret_data_serve_in;
  wire full_serve_flag;
  
  two_piped_fifos U1(
    .clk(clk),
    .rst(reset),

    .write_req_en(request_write_en),
    .data_req_in(request_data_in),
    .full_req_flag(full_flag),
    .read_req_en(0), // TODO
    .data_req_out(data_req_out),
    .empty_req_flag(empty_req_flag),

    .read_serve_en(served_read_en),
    .data_serve_out(served_data_out),
    .empty_serve_flag(empty_flag),
    .write_serve_en(0), // TODO
    .data_serve_in(ret_data_serve_in),
    .full_serve_flag(full_serve_flag)

  );
  
  // unpacking the signals to be used by multi2sim
  // reg rw_flag;
  // reg [30:0] address;
  // reg [31:0] data;
  // reg [9:0] id;
  // always@(*)
  // begin
  //   rw_flag=data_req_out[73];
  //   address=data_req_out[72:42];
  //   data=data_req_out[41:10]; 
  //   id= data_req_out[9:0];
  // end
  wire [30:0]address;
  wire [31:0]data;
  wire [9:0]id;
  //assign rw_flag=data_req_out[73];
  //assign address[30:0]=data_req_out[72:42];
  //assign data[31:0]=data_req_out[41:10];
  //assign id[9:0]= data_req_out[9:0];
  
  
  // packing the signals to be send back
  reg ret_data;
  reg ret_id;
  reg ret_next_id_available;
  assign ret_data_serve_in = {ret_data,ret_id,ret_next_id_available};


  // ----------------------------------------------------------------------
  // Logic to commit the accesses
  // Important signals
  // rw_flag, address, data, id, empty_flag
  //
  // Algorithm:
  // while the empty_flag is 0 do:
  //  commit rw_flag, address, data to a access table to be sent to multi2sim
  
  
  // ----------------------------------------------------------------------
  // Logic to return the accesses
  // Important signals
  // ret_data, ret_id, ret_next_id_available, full_flag
  //
  // Algorithm:
  // while the full_flag is 0 do:
  //  send ret_data, ret_id, ret_next_id_available back to the fifo


  // Other signals
  reg running;

  // State machine to initialize the m2s interface
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
    ret_data = 0;
    if (!reset)
    begin
      if(state==work_st)
      begin
        ret_data=$m2s_step;

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


//-----------------------------------------------------
// Design Name : two_piped_fifos
// File Name   : module_tb.v
// Function    : Encapsulate 2 FIFOS
// Coder       : Nicolas Bohm Agostini
//-----------------------------------------------------
module two_piped_fifos (
clk,
rst,

write_req_en,
data_req_in,
full_req_flag,
read_req_en,
data_req_out,
empty_req_flag,

read_serve_en,
data_serve_out,
empty_serve_flag,
write_serve_en,
data_serve_in,
full_serve_flag

);

  input clk, rst;

  // [request side]
  input write_req_en;
  input [73:0] data_req_in;
  output full_req_flag;
  input read_req_en;
  output [73:0] data_req_out;
  output empty_req_flag;

  // [m2s side]
  input read_serve_en;
  output [51:0]data_serve_out;
  output empty_serve_flag;
  input write_serve_en;
  input [51:0]data_serve_in;
  output full_serve_flag;
  
  // [request side]
  syn_fifo #(.DATA_WIDTH(74),.ADDR_WIDTH(2))REQUEST_FIFO(
  .clk      (clk)             , // Clock input
  .rst      (rst)           , // Active high reset
  .wr_cs    (1'b1)            , // Write chip select
  .rd_cs    (1'b1)            , // Read chip select
  .data_in  (data_req_in)     , // Data input   [request side]
  .rd_en    (read_req_en)     , // Read enable  [m2s side]
  .wr_en    (write_req_en)    , // Write Enable [request side]
  .data_out (data_req_out)    , // Data Output  [m2s side]
  .empty    (empty_req_flag)  , // FIFO empty   [request side]
  .full     (full_req_flag)     // FIFO full    [m2s side]
  ); 

  // [m2s side]
  syn_fifo #(.DATA_WIDTH(52),.ADDR_WIDTH(2))SERVE_FIFO(
  .clk      (clk)             , // Clock input
  .rst      (rst)           , // Active high reset
  .wr_cs    (1'b1)            , // Write chip select
  .rd_cs    (1'b1)            , // Read chip select
  .data_in  (data_serve_in)   , // Data input   [m2s side]
  .rd_en    (read_serve_en)   , // Read enable  [request side]
  .wr_en    (write_serve_en)  , // Write Enable [m2s side]
  .data_out (data_serve_out)  , // Data Output  [request side]
  .empty    (empty_serve_flag), // FIFO empty   [m2s side]
  .full     (full_serve_flag)   // FIFO full    [request side]
  ); 


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
assign data_0 = (cs_0 && oe_0 && !we_0) ? data_0_out : 8'bz; 

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
assign data_1 = (cs_1 && oe_1 && !we_1) ? data_1_out : 8'bz; 
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


`timescale 1ns/1ns
module tb_for_twofifo;

  reg clk,reset;

  wire [73:0]data_serve_in;
  assign data_serve_in [73:0]=3;
  reg write_serve_en;

  wire[73:0]data_serve_out;
  reg read_serve_en;

  wire empty_serve_flag,full_serve_flag;

  two_piped_fifos U1(
    .clk(clk),
    .rst(reset),

    .write_req_en(write_serve_en),
    .data_req_in(data_serve_in),
    .full_req_flag(full_serve_flag),
    .read_req_en(read_serve_en),
    .data_req_out(data_serve_out),
    .empty_req_flag(empty_serve_flag),

    .read_serve_en(),
    .data_serve_out(),
    .empty_serve_flag(),
    .write_serve_en(),
    .data_serve_in(),
    .full_serve_flag()

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

