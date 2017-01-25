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
`timescale 1ns / 1ns
module top;

  reg clk, reset;
  reg access_en;

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


  // Clock generator
  always
  begin
    #1  clk = ~clk; // Toggle clock every 5 ticks
  end

  initial begin
    #0 reset = 0;
    #0 access_en = 0;
    #0 address = 0;
    #0 rw_flag = 0;

    #5 reset = 1;
    #10 reset = 0;
    #10001 $finish;
  end

  // Stim generator
  always
  begin
    #1  clk = ~clk; // Toggle clock every 5 ticks
  end

  always
  begin
    #50 access_en = 1;
    #0 identification = 0;
    #1 access_en = 0;
  end

  always
  begin
    #1 address=$urandom_range(255,0);
    #1 rw_flag=$urandom_range(1,0);
  end

  // Instance generator
  interface i1(
    .clk(clk),
    .reset(reset),
    .access_en(access_en),
    .request_in(request),
    .identification_in(identification),
    .data_out(data_out),
    .identification_out(identification_out),
    .next_id_available_out(next_id_available_out)
    );

endmodule
/*********************************************************************/


/*** An RTL For the VPI communication ***/
`timescale 1ns / 1ns
module interface (clk, reset, access_en, request_in, identification_in,
                  data_out, identification_out, next_id_available_out);

  // Inputs
  input clk, reset, access_en;
  input [63:0] request_in;
  input [9:0] identification_in;

  // Outputs
  output [31:0] data_out;
  output [9:0] identification_out;
  output [9:0] next_id_available_out;

  // Subfields
  reg rw_flag;
  reg [30:0] address;
  reg [31:0] data;

  always @ ( * ) begin
    rw_flag=request_in[63];
    address=request_in[62:32];
    data=request_in[31:0];  
  end


  // Other signals
  wire access_en;
  reg  [32:0] result;
  reg running;

  // State machine to initialize the m2s interface
  parameter init_st=0, reset_st=1, work_st=2, finish_st=3;
  reg [1:0] state;

  // Next state logic
  always @(posedge clk or posedge reset)
  begin
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
  begin
    result = 0;
    if (reset)

      if(state==work_st)
      begin
        result=$m2s_step;

        if (access_en)
        begin
          $m2s_access(1, 1, address);
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


    else
      $m2s_reset;


  end

  // Must do access_en logic!!!!!!!

endmodule
/*********************************************************************/
