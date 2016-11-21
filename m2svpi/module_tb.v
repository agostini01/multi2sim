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
`timescale 1ns / 1ns
module top;
tri  [1:0] results;
integer    test;
real       foo;
time       bar;
reg  [32:0] result;
reg         a, b;
reg[256*8:0] string;

  addbit i1 (test[0], test[1], test[2], results[0], results[1]);

  initial
  begin

  test = 3'b000;
  foo = 3.14;
  bar = 0;
  bar[63:60] = 4'hF;
  bar[35:32] = 4'hA;
  bar[31:28] = 4'hC;
  bar[03:00] = 4'hE;

// Removed this section because of  the string output
//      #10 $show_all_signals;
//      begin: local
//      reg  foobar;
//      #10 test = 3'b010;
//      #10 $show_all_signals;
//      #10 $show_all_signals(top.i1);
//      end
      //      #10 $stop;
      #10 $finish;
      end

initial
    begin
    a = 1;
    b = 0;
    string = "--si-sim detailed --si-config si-config \
--mem-config mem-si-1 \
--si-report si_report \
--mem-report mem_report \
--trace trace.gz \
--mem-debug debug.mem \
outM2S";

    $display("%s", string);

    #1 $m2s_initialize;
    #1 $m2s_reset;
    #1 result = $m2s_step();
	$m2s_access(1, 0, 8'hAA);
    #1 result = $m2s_step();
	$m2s_access(1, 0, 8'hAA);
    #1 $m2s_finalize;
    end
endmodule




/*** An RTL level 1 bit adder model ***/
`timescale 1ns / 1ns
module addbit (a, b, ci, sum, co);
input  a, b, ci;
output sum, co;

  wire  a, b, ci;
  reg   sum, co;

  always @(a or b or ci)
  {co, sum} = a + b + ci;

  //always @(sum)
  //$show_all_signals();

endmodule
/*********************************************************************/

