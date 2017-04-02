`include "util/array_pack_unpack.v"

module interface (
	clk, reset,
	read_ctr_pack, data_in_pack, empty_flag_pack,
	write_ctr_pack, data_out_pack, full_flag_pack
);

// Parameters---------------------------------------------------
parameter CONTROLLERS_WIDTH = 2;

parameter DATA_WIDTH = 32 ;
parameter ADDR_WIDTH = 31 ;
parameter TID_WIDTH = 16;

parameter REQ_WIDTH = 1 + ADDR_WIDTH + DATA_WIDTH; // request_in

parameter DP_DATA_WIDTH  = TID_WIDTH+ REQ_WIDTH; // ID, Flag, Address, Data
parameter VPI_DATA_WIDTH  = TID_WIDTH + DATA_WIDTH; // ID, Data

// Clock, reset
input clk, reset;

// Access request ports 
// Direction ----------->
output	[CONTROLLERS_WIDTH-1:0] read_ctr_pack;
input	[(DP_DATA_WIDTH*CONTROLLERS_WIDTH)-1:0] data_in_pack;
input	[CONTROLLERS_WIDTH-1:0]empty_flag_pack;
// Derection <-----------
output	[CONTROLLERS_WIDTH-1:0]write_ctr_pack;
output	[(VPI_DATA_WIDTH*CONTROLLERS_WIDTH)-1:0]data_out_pack;
input	[CONTROLLERS_WIDTH-1:0]full_flag_pack;


// Unpacking the ports
// Direction ----------->
genvar pk_idx;
genvar unpk_idx;
wire [1-1:0]read_ctr						[0:CONTROLLERS_WIDTH-1]; // Output
`PACK_ARRAY(pk_idx,1,CONTROLLERS_WIDTH,read_ctr,read_ctr_pack)

wire [DP_DATA_WIDTH-1:0] data_in	[0:CONTROLLERS_WIDTH-1]; // Input
`UNPACK_ARRAY(unpk_idx,DP_DATA_WIDTH,CONTROLLERS_WIDTH,data_in,data_in_pack)

wire [1-1:0]empty_flag						[0:CONTROLLERS_WIDTH-1]; // Input
`UNPACK_ARRAY(unpk_idx,1,CONTROLLERS_WIDTH,empty_flag,empty_flag_pack)

// Derection <-----------
wire [1-1:0]write_ctr						[0:CONTROLLERS_WIDTH-1]; // Output
`PACK_ARRAY(pk_idx,1,CONTROLLERS_WIDTH,write_ctr,write_ctr_pack)

wire [VPI_DATA_WIDTH-1:0]data_out	[0:CONTROLLERS_WIDTH-1]; // Output
`PACK_ARRAY(unpk_idx,VPI_DATA_WIDTH,CONTROLLERS_WIDTH,data_out,data_out_pack)

wire [1-1:0]full_flag						[0:CONTROLLERS_WIDTH-1]; // Input
`UNPACK_ARRAY(unpk_idx,1,CONTROLLERS_WIDTH,full_flag,full_flag_pack)

// Generating the mem_controller instances
genvar mem_idx;
generate for (mem_idx=0; mem_idx<(CONTROLLERS_WIDTH); mem_idx=mem_idx+1) 
begin:mem_con_gen
mem_controller #(.MODULE_NUM(mem_idx),.MODULE_WIDTH(CONTROLLERS_WIDTH)) mem_con
(
.clk(clk),
.reset(reset),

.read_ctr(read_ctr[mem_idx]), 			// rd_en
.incoming_data(data_in[mem_idx]), // data_out
.empty_signal(empty_flag[mem_idx]), 	//empty

.write_ctr(write_ctr[mem_idx]), 		//wr_en
.outgoing_data(data_out[mem_idx]), // data_in
.full_signal(full_flag[mem_idx]) 		//full
);

end endgenerate

endmodule

`timescale 1ns / 100ps
module interface_tb ();
// Parameters---------------------------------------------------
parameter CONTROLLERS_WIDTH = 1;

parameter DATA_WIDTH = 32 ;
parameter ADDR_WIDTH = 31 ;
parameter TID_WIDTH = 16;

parameter REQ_WIDTH = 1 + ADDR_WIDTH + DATA_WIDTH; // request_in

parameter DP_DATA_WIDTH  = TID_WIDTH+ REQ_WIDTH; // ID, Flag, Address, Data
parameter VPI_DATA_WIDTH  = TID_WIDTH + DATA_WIDTH; // ID, Data


reg clk, reset;

wire	[CONTROLLERS_WIDTH-1:0]read_ctr_pack;
wire	[(DP_DATA_WIDTH*CONTROLLERS_WIDTH)-1:0]data_in_pack;
wire	[CONTROLLERS_WIDTH-1:0]empty_flag_pack;

wire	[CONTROLLERS_WIDTH-1:0]write_ctr_pack;
wire	[(VPI_DATA_WIDTH*CONTROLLERS_WIDTH)-1:0]data_out_pack;
wire	[CONTROLLERS_WIDTH-1:0]full_flag_pack;

// instances
interface #(.CONTROLLERS_WIDTH(CONTROLLERS_WIDTH)) U1 (
	clk, reset,
	read_ctr_pack, data_in_pack, empty_flag_pack,
	write_ctr_pack, data_out_pack, full_flag_pack
);

// Unpacking the ports
// Direction ----------->
genvar pk_idx;
genvar unpk_idx;
wire [1-1:0]read_ctr						[0:CONTROLLERS_WIDTH-1]; // wire
`UNPACK_ARRAY(pk_idx,1,CONTROLLERS_WIDTH,read_ctr,read_ctr_pack)

reg [DP_DATA_WIDTH-1:0] data_in	[0:CONTROLLERS_WIDTH-1]; // reg
`PACK_ARRAY(unpk_idx,DP_DATA_WIDTH,CONTROLLERS_WIDTH,data_in,data_in_pack)

reg [1-1:0]empty_flag						[0:CONTROLLERS_WIDTH-1]; // reg
`PACK_ARRAY(unpk_idx,1,CONTROLLERS_WIDTH,empty_flag,empty_flag_pack)

// Derection <-----------
wire [1-1:0]write_ctr						[0:CONTROLLERS_WIDTH-1]; // wire
`UNPACK_ARRAY(pk_idx,1,CONTROLLERS_WIDTH,write_ctr,write_ctr_pack)

wire [VPI_DATA_WIDTH-1:0]data_out	[0:CONTROLLERS_WIDTH-1]; // wire
`UNPACK_ARRAY(unpk_idx,VPI_DATA_WIDTH,CONTROLLERS_WIDTH,data_out,data_out_pack)

reg [1-1:0]full_flag						[0:CONTROLLERS_WIDTH-1]; // reg
`PACK_ARRAY(unpk_idx,1,CONTROLLERS_WIDTH,full_flag,full_flag_pack)


reg finishing_sim_flag;
integer i;

initial begin
	#0 finishing_sim_flag = 0;
	#0 clk = 0;
	#0 reset = 0;

	for (i = 0; i < CONTROLLERS_WIDTH; i = i + 1)
		#0 empty_flag[i] = 1;

	for (i = 0; i < CONTROLLERS_WIDTH; i = i + 1)
		#0 full_flag[i] = 0;

	for (i = 0; i < CONTROLLERS_WIDTH; i = i + 1)  	// identification
		#0 data_in[i][TID_WIDTH+REQ_WIDTH-1:REQ_WIDTH] = 0;

	for (i = 0; i < CONTROLLERS_WIDTH; i = i + 1)
		#0 data_in[i][REQ_WIDTH-1] = 1; // rw_flag
		
	for (i = 0; i < CONTROLLERS_WIDTH; i = i + 1)  	// address
		#0 data_in[i][ADDR_WIDTH + DATA_WIDTH-1:DATA_WIDTH] = 0;

	for (i = 0; i < CONTROLLERS_WIDTH; i = i + 1)  	// data
		#0 data_in[i][DATA_WIDTH-1:0] = 15;


	#5 reset = 1;
	#5 reset = 0;
	for (i = 0; i < CONTROLLERS_WIDTH; i = i + 1)
		#0 empty_flag[i] = 0;

	#100 finishing_sim_flag = 1;
	for (i = 0; i < CONTROLLERS_WIDTH; i = i + 1)
		#0 empty_flag[i] = 1;
	#0 $m2s_finalize;
	#500 $finish;
end

//[TID_WIDTH+ 1 + ADDR_WIDTH + DATA_WIDTH]

reg running;
initial begin
	$m2s_initialize;
	running=1;
end


always begin
	#1  clk = ~clk;
end

always@(posedge clk) begin
	$m2s_step;
end

always@(posedge clk) begin
	if (finishing_sim_flag==0) begin
		for (i = 0; i < CONTROLLERS_WIDTH; i = i + 1) begin
			#0 data_in[i][TID_WIDTH+REQ_WIDTH-1:REQ_WIDTH] = 
					data_in[i][TID_WIDTH+REQ_WIDTH-1:REQ_WIDTH] + 1; // identification[i]
			#0 data_in[i][REQ_WIDTH-1] = $urandom_range(1,0); 		// rw_flag[i]
			#0 data_in[i][ADDR_WIDTH + DATA_WIDTH-1:DATA_WIDTH] = $urandom_range(255,0); // Address[i]
		end
	end
end

endmodule
