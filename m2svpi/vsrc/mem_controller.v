//-----------------------------------------------------
// Design Name : mem_controller
// File Name   : mem_controller.v
// Function    : Table to keep track of the id
// Coder       : Nicolas Bohm Agostini
//-----------------------------------------------------
module mem_controller (
clk,
reset,

read_ctr, // rd_en
incoming_data, // data_out
empty_signal, //empty

write_ctr, //wr_en
outgoing_data, // data_in
full_signal //full
);

// Parameters---------------------------------------------------
parameter MODULE_NUM = 0;
parameter DATA_WIDTH = 32 ;
parameter ADDR_WIDTH = 31 ;
parameter TID_WIDTH = 16;

parameter REQ_WIDTH = 1 + ADDR_WIDTH + DATA_WIDTH; // request_in

parameter DP_DATA_WIDTH  = TID_WIDTH+ REQ_WIDTH; // ID, Flag, Address, Data
parameter VPI_DATA_WIDTH  = TID_WIDTH + DATA_WIDTH; // ID, Data

// Ports -------------------------------------------------------
// Input ouput
input clk, reset, empty_signal, full_signal;
output reg read_ctr;
output reg write_ctr;
input [DP_DATA_WIDTH-1:0] incoming_data;
output[VPI_DATA_WIDTH-1:0] outgoing_data;

// Internals ---------------------------------------------------
wire [REQ_WIDTH-1:0] request_in;
wire [TID_WIDTH-1:0] identification_in;
wire rw;
wire [ADDR_WIDTH-1:0]address;
wire [DATA_WIDTH-1:0]data;

assign identification_in [TID_WIDTH-1:0] = incoming_data[DP_DATA_WIDTH-1:REQ_WIDTH]; // ID
assign request_in [REQ_WIDTH-1:0] = incoming_data[REQ_WIDTH-1:0];
assign rw = request_in[REQ_WIDTH-1]; // Flag
assign address[ADDR_WIDTH-1:0] = request_in[ADDR_WIDTH + DATA_WIDTH-1:DATA_WIDTH]; // Address
assign data [DATA_WIDTH-1:0] = request_in[DATA_WIDTH-1:0]; // Data

reg [REQ_WIDTH-1:0]id_out;
reg [DATA_WIDTH-1:0]data_out;
assign outgoing_data [VPI_DATA_WIDTH-1:0] = {id_out[REQ_WIDTH-1:0], data_out[DATA_WIDTH-1:0]};

// This overhead is necessary because it is complicated to
// translate the value from a net to a c object
// must be changed in te futre
reg module_number = MODULE_NUM;
reg [1:0]access_type;
reg [ADDR_WIDTH-1:0]access_address;
reg [DATA_WIDTH-1:0]access_data;
reg [TID_WIDTH-1:0]access_identifier;
initial begin
	if(rw)
		access_type=1;
	else
		access_type=2;
	access_address=address;
	access_data=data;
	access_identifier=identification_in;
end
always@(rw,address,data) begin
	if(rw)
		access_type=1;
	else
		access_type=2;
	access_address=address;
	access_data=data;
	access_identifier=identification_in;
end

always@(posedge clk,posedge reset)
begin
  if(reset) 
  begin
    data_out=0;
    id_out=0;
  end
  else
  begin
    if (!empty_signal) begin
      read_ctr = 1;
      $m2s_access(MODULE_NUM,
		access_type,
		access_address,
		access_data,
		access_identifier); // mod, type, address, data, identifier
    end
    else
      read_ctr = 0;
  end
end

// Algorithm
//

endmodule


//-----------------------------------------------------
// Design Name : mem_controller_tb
// File Name   : mem_controller_tb.v
// Function    : Testbench
// Coder       : Nicolas Bohm Agostini
//-----------------------------------------------------
`timescale 1ns / 100ps
module mem_controller_tb;

// Parameters---------------------------------------------------
parameter DATA_WIDTH = 32 ;
parameter ADDR_WIDTH = 31 ;
parameter TID_WIDTH = 16;

parameter REQ_WIDTH = 1 + ADDR_WIDTH + DATA_WIDTH; // request_in

parameter DP_DATA_WIDTH  = TID_WIDTH+ REQ_WIDTH; // ID, Flag, Address, Data
parameter VPI_DATA_WIDTH  = TID_WIDTH + DATA_WIDTH; // ID, Data

// Signals---------------------------------------------------
reg string;
initial begin
	  string = "--si-sim detailed --si-config si-config \
	--mem-config mem-si-1 \
	--si-report si_report \
	--mem-report mem_report \
	--trace trace.gz \
	--mem-debug debug.mem \
	--mem-sim-vpi \
	outM2S";
end

reg clk,reset;
reg empty_flag,full_flag;
wire read_ctr,write_ctr;


reg  [TID_WIDTH-1:0] identification_in;
reg rw_flag_in;
reg  [ADDR_WIDTH-1:0] address_in;
reg  [DATA_WIDTH-1:0] data_in;
wire [DP_DATA_WIDTH-1:0] input_data;
assign input_data = {identification_in,rw_flag_in,address_in,data_in};

wire [VPI_DATA_WIDTH-1:0] outgoing_data ;



initial begin
	#0 clk = 0;
	#0 reset = 0;
	#0 empty_flag = 1;
	#0 full_flag = 0;
	#0 address_in = 0;
	#0 rw_flag_in = 1;
	#0 data_in = 7;
	#0 identification_in =0;

	#5 reset = 1;
	#5 reset = 0;
	#0 empty_flag = 0;
	#10001 $m2s_finalize;
	#10001 $finish;
end

reg running;
initial begin
	$m2s_initialize;
	running=1;
end


reg [DATA_WIDTH-1:0] req_data;
always begin
	#1  clk = ~clk;
	#0	req_data=$m2s_step;
end

always@(posedge clk) begin
		#0 identification_in = identification_in+1;
		#0 address_in=$urandom_range(255,0);
		#0 rw_flag_in=$urandom_range(2,1);
end

mem_controller U1 (
.clk(clk),
.reset(reset),

.read_ctr(read_ctr),
.incoming_data(input_data),
.empty_signal(empty_flag),

.write_ctr(write_ctr),
.outgoing_data(outgoing_data),
.full_signal(full_flag)
);

endmodule


