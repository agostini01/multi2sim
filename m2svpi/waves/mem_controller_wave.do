onerror {resume}
quietly virtual signal -install /mem_controller_tb { /mem_controller_tb/outgoing_data[31:0]} data_output
quietly virtual signal -install /mem_controller_tb { /mem_controller_tb/outgoing_data[47:32]} id_output
quietly virtual signal -install /mem_controller_tb {/mem_controller_tb/input_data[79]  } read_write_flag
quietly virtual signal -install /mem_controller_tb { /mem_controller_tb/input_data[31:0]} data_input
quietly virtual signal -install /mem_controller_tb { /mem_controller_tb/input_data[62:32]} Address_Input
quietly virtual signal -install /mem_controller_tb { /mem_controller_tb/input_data[62:32]} address_input
quietly virtual signal -install /mem_controller_tb {/mem_controller_tb/input_data[63]  } read_write_flag_input
quietly virtual signal -install /mem_controller_tb { /mem_controller_tb/input_data[79:64]} id_input
quietly WaveActivateNextPane {} 0
add wave -noupdate -divider {Global Stim}
add wave -noupdate /mem_controller_tb/clk
add wave -noupdate /mem_controller_tb/reset
add wave -noupdate -divider {Input Port}
add wave -noupdate /mem_controller_tb/empty_flag
add wave -noupdate /mem_controller_tb/read_ctr
add wave -noupdate -radix decimal /mem_controller_tb/id_input
add wave -noupdate /mem_controller_tb/read_write_flag_input
add wave -noupdate -radix unsigned /mem_controller_tb/data_input
add wave -noupdate -radix unsigned /mem_controller_tb/address_input
add wave -noupdate -radix hexadecimal /mem_controller_tb/input_data
add wave -noupdate -divider {Output Port}
add wave -noupdate /mem_controller_tb/full_flag
add wave -noupdate /mem_controller_tb/write_ctr
add wave -noupdate -radix decimal /mem_controller_tb/id_output
add wave -noupdate -radix unsigned /mem_controller_tb/data_output
add wave -noupdate -radix hexadecimal /mem_controller_tb/outgoing_data
add wave -noupdate -divider -height 35 Mem_Controller
add wave -noupdate -radix unsigned /mem_controller_tb/U1/MODULE_NUM
add wave -noupdate -divider {Input Port}
add wave -noupdate /mem_controller_tb/U1/empty_signal
add wave -noupdate /mem_controller_tb/U1/read_ctr
add wave -noupdate -radix hexadecimal /mem_controller_tb/U1/incoming_data
add wave -noupdate -divider {Output Port}
add wave -noupdate /mem_controller_tb/U1/full_signal
add wave -noupdate /mem_controller_tb/U1/write_ctr
add wave -noupdate -radix hexadecimal -childformat {{{/mem_controller_tb/U1/outgoing_data[47]} -radix hexadecimal} {{/mem_controller_tb/U1/outgoing_data[46]} -radix hexadecimal} {{/mem_controller_tb/U1/outgoing_data[45]} -radix hexadecimal} {{/mem_controller_tb/U1/outgoing_data[44]} -radix hexadecimal} {{/mem_controller_tb/U1/outgoing_data[43]} -radix hexadecimal} {{/mem_controller_tb/U1/outgoing_data[42]} -radix hexadecimal} {{/mem_controller_tb/U1/outgoing_data[41]} -radix hexadecimal} {{/mem_controller_tb/U1/outgoing_data[40]} -radix hexadecimal} {{/mem_controller_tb/U1/outgoing_data[39]} -radix hexadecimal} {{/mem_controller_tb/U1/outgoing_data[38]} -radix hexadecimal} {{/mem_controller_tb/U1/outgoing_data[37]} -radix hexadecimal} {{/mem_controller_tb/U1/outgoing_data[36]} -radix hexadecimal} {{/mem_controller_tb/U1/outgoing_data[35]} -radix hexadecimal} {{/mem_controller_tb/U1/outgoing_data[34]} -radix hexadecimal} {{/mem_controller_tb/U1/outgoing_data[33]} -radix hexadecimal} {{/mem_controller_tb/U1/outgoing_data[32]} -radix hexadecimal} {{/mem_controller_tb/U1/outgoing_data[31]} -radix hexadecimal} {{/mem_controller_tb/U1/outgoing_data[30]} -radix hexadecimal} {{/mem_controller_tb/U1/outgoing_data[29]} -radix hexadecimal} {{/mem_controller_tb/U1/outgoing_data[28]} -radix hexadecimal} {{/mem_controller_tb/U1/outgoing_data[27]} -radix hexadecimal} {{/mem_controller_tb/U1/outgoing_data[26]} -radix hexadecimal} {{/mem_controller_tb/U1/outgoing_data[25]} -radix hexadecimal} {{/mem_controller_tb/U1/outgoing_data[24]} -radix hexadecimal} {{/mem_controller_tb/U1/outgoing_data[23]} -radix hexadecimal} {{/mem_controller_tb/U1/outgoing_data[22]} -radix hexadecimal} {{/mem_controller_tb/U1/outgoing_data[21]} -radix hexadecimal} {{/mem_controller_tb/U1/outgoing_data[20]} -radix hexadecimal} {{/mem_controller_tb/U1/outgoing_data[19]} -radix hexadecimal} {{/mem_controller_tb/U1/outgoing_data[18]} -radix hexadecimal} {{/mem_controller_tb/U1/outgoing_data[17]} -radix hexadecimal} {{/mem_controller_tb/U1/outgoing_data[16]} -radix hexadecimal} {{/mem_controller_tb/U1/outgoing_data[15]} -radix hexadecimal} {{/mem_controller_tb/U1/outgoing_data[14]} -radix hexadecimal} {{/mem_controller_tb/U1/outgoing_data[13]} -radix hexadecimal} {{/mem_controller_tb/U1/outgoing_data[12]} -radix hexadecimal} {{/mem_controller_tb/U1/outgoing_data[11]} -radix hexadecimal} {{/mem_controller_tb/U1/outgoing_data[10]} -radix hexadecimal} {{/mem_controller_tb/U1/outgoing_data[9]} -radix hexadecimal} {{/mem_controller_tb/U1/outgoing_data[8]} -radix hexadecimal} {{/mem_controller_tb/U1/outgoing_data[7]} -radix hexadecimal} {{/mem_controller_tb/U1/outgoing_data[6]} -radix hexadecimal} {{/mem_controller_tb/U1/outgoing_data[5]} -radix hexadecimal} {{/mem_controller_tb/U1/outgoing_data[4]} -radix hexadecimal} {{/mem_controller_tb/U1/outgoing_data[3]} -radix hexadecimal} {{/mem_controller_tb/U1/outgoing_data[2]} -radix hexadecimal} {{/mem_controller_tb/U1/outgoing_data[1]} -radix hexadecimal} {{/mem_controller_tb/U1/outgoing_data[0]} -radix hexadecimal}} -subitemconfig {{/mem_controller_tb/U1/outgoing_data[47]} {-radix hexadecimal} {/mem_controller_tb/U1/outgoing_data[46]} {-radix hexadecimal} {/mem_controller_tb/U1/outgoing_data[45]} {-radix hexadecimal} {/mem_controller_tb/U1/outgoing_data[44]} {-radix hexadecimal} {/mem_controller_tb/U1/outgoing_data[43]} {-radix hexadecimal} {/mem_controller_tb/U1/outgoing_data[42]} {-radix hexadecimal} {/mem_controller_tb/U1/outgoing_data[41]} {-radix hexadecimal} {/mem_controller_tb/U1/outgoing_data[40]} {-radix hexadecimal} {/mem_controller_tb/U1/outgoing_data[39]} {-radix hexadecimal} {/mem_controller_tb/U1/outgoing_data[38]} {-radix hexadecimal} {/mem_controller_tb/U1/outgoing_data[37]} {-radix hexadecimal} {/mem_controller_tb/U1/outgoing_data[36]} {-radix hexadecimal} {/mem_controller_tb/U1/outgoing_data[35]} {-radix hexadecimal} {/mem_controller_tb/U1/outgoing_data[34]} {-radix hexadecimal} {/mem_controller_tb/U1/outgoing_data[33]} {-radix hexadecimal} {/mem_controller_tb/U1/outgoing_data[32]} {-radix hexadecimal} {/mem_controller_tb/U1/outgoing_data[31]} {-radix hexadecimal} {/mem_controller_tb/U1/outgoing_data[30]} {-radix hexadecimal} {/mem_controller_tb/U1/outgoing_data[29]} {-radix hexadecimal} {/mem_controller_tb/U1/outgoing_data[28]} {-radix hexadecimal} {/mem_controller_tb/U1/outgoing_data[27]} {-radix hexadecimal} {/mem_controller_tb/U1/outgoing_data[26]} {-radix hexadecimal} {/mem_controller_tb/U1/outgoing_data[25]} {-radix hexadecimal} {/mem_controller_tb/U1/outgoing_data[24]} {-radix hexadecimal} {/mem_controller_tb/U1/outgoing_data[23]} {-radix hexadecimal} {/mem_controller_tb/U1/outgoing_data[22]} {-radix hexadecimal} {/mem_controller_tb/U1/outgoing_data[21]} {-radix hexadecimal} {/mem_controller_tb/U1/outgoing_data[20]} {-radix hexadecimal} {/mem_controller_tb/U1/outgoing_data[19]} {-radix hexadecimal} {/mem_controller_tb/U1/outgoing_data[18]} {-radix hexadecimal} {/mem_controller_tb/U1/outgoing_data[17]} {-radix hexadecimal} {/mem_controller_tb/U1/outgoing_data[16]} {-radix hexadecimal} {/mem_controller_tb/U1/outgoing_data[15]} {-radix hexadecimal} {/mem_controller_tb/U1/outgoing_data[14]} {-radix hexadecimal} {/mem_controller_tb/U1/outgoing_data[13]} {-radix hexadecimal} {/mem_controller_tb/U1/outgoing_data[12]} {-radix hexadecimal} {/mem_controller_tb/U1/outgoing_data[11]} {-radix hexadecimal} {/mem_controller_tb/U1/outgoing_data[10]} {-radix hexadecimal} {/mem_controller_tb/U1/outgoing_data[9]} {-radix hexadecimal} {/mem_controller_tb/U1/outgoing_data[8]} {-radix hexadecimal} {/mem_controller_tb/U1/outgoing_data[7]} {-radix hexadecimal} {/mem_controller_tb/U1/outgoing_data[6]} {-radix hexadecimal} {/mem_controller_tb/U1/outgoing_data[5]} {-radix hexadecimal} {/mem_controller_tb/U1/outgoing_data[4]} {-radix hexadecimal} {/mem_controller_tb/U1/outgoing_data[3]} {-radix hexadecimal} {/mem_controller_tb/U1/outgoing_data[2]} {-radix hexadecimal} {/mem_controller_tb/U1/outgoing_data[1]} {-radix hexadecimal} {/mem_controller_tb/U1/outgoing_data[0]} {-radix hexadecimal}} /mem_controller_tb/U1/outgoing_data
add wave -noupdate -divider Internals
add wave -noupdate -radix unsigned /mem_controller_tb/U1/module_number
add wave -noupdate -radix unsigned -childformat {{{/mem_controller_tb/U1/access_type[1]} -radix unsigned} {{/mem_controller_tb/U1/access_type[0]} -radix unsigned}} -subitemconfig {{/mem_controller_tb/U1/access_type[1]} {-radix unsigned} {/mem_controller_tb/U1/access_type[0]} {-radix unsigned}} /mem_controller_tb/U1/access_type
add wave -noupdate -radix unsigned /mem_controller_tb/U1/access_address
add wave -noupdate /mem_controller_tb/U1/access_data
add wave -noupdate /mem_controller_tb/U1/access_identifier
TreeUpdate [SetDefaultTree]
WaveRestoreCursors {{Cursor 1} {0 ps} 0}
quietly wave cursor active 1
configure wave -namecolwidth 312
configure wave -valuecolwidth 100
configure wave -justifyvalue left
configure wave -signalnamewidth 0
configure wave -snapdistance 10
configure wave -datasetprefix 0
configure wave -rowmargin 4
configure wave -childrowmargin 2
configure wave -gridoffset 0
configure wave -gridperiod 1
configure wave -griddelta 40
configure wave -timeline 0
configure wave -timelineunits ns
update
WaveRestoreZoom {0 ps} {315 ns}
