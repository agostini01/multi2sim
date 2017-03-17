onerror {resume}
quietly WaveActivateNextPane {} 0
add wave -noupdate -divider {Global Stim}
add wave -noupdate /interface_tb/clk
add wave -noupdate /interface_tb/reset
add wave -noupdate -divider {Input Port}
add wave -noupdate /interface_tb/read_ctr
add wave -noupdate /interface_tb/data_in
add wave -noupdate /interface_tb/empty_flag
add wave -noupdate -divider {Output Port}
add wave -noupdate /interface_tb/write_ctr
add wave -noupdate /interface_tb/data_out
add wave -noupdate -divider -height 35 Mem_Controller0
add wave -noupdate {/interface_tb/U1/mem_con_gen[0]/mem_con/MODULE_NUM}
add wave -noupdate -divider {Input Port}
add wave -noupdate /interface_tb/full_flag
add wave -noupdate {/interface_tb/U1/mem_con_gen[0]/mem_con/identification_in}
add wave -noupdate {/interface_tb/U1/mem_con_gen[0]/mem_con/rw}
add wave -noupdate {/interface_tb/U1/mem_con_gen[0]/mem_con/address}
add wave -noupdate {/interface_tb/U1/mem_con_gen[0]/mem_con/data}
add wave -noupdate -divider {Output Port}
add wave -noupdate {/interface_tb/U1/mem_con_gen[0]/mem_con/id_out}
add wave -noupdate {/interface_tb/U1/mem_con_gen[0]/mem_con/data_out}
add wave -noupdate -divider Internals
add wave -noupdate -divider -height 35 Mem_Controller1
add wave -noupdate {/interface_tb/U1/mem_con_gen[1]/mem_con/MODULE_NUM}
add wave -noupdate -divider {Input Port}
add wave -noupdate {/interface_tb/U1/mem_con_gen[1]/mem_con/identification_in}
add wave -noupdate {/interface_tb/U1/mem_con_gen[1]/mem_con/rw}
add wave -noupdate {/interface_tb/U1/mem_con_gen[1]/mem_con/address}
add wave -noupdate {/interface_tb/U1/mem_con_gen[1]/mem_con/data}
add wave -noupdate -divider {Output Port}
add wave -noupdate {/interface_tb/U1/mem_con_gen[1]/mem_con/id_out}
add wave -noupdate {/interface_tb/U1/mem_con_gen[1]/mem_con/data_out}
add wave -noupdate -divider Internals
TreeUpdate [SetDefaultTree]
WaveRestoreCursors {{Cursor 1} {257000 ps} 0}
quietly wave cursor active 1
configure wave -namecolwidth 411
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
WaveRestoreZoom {0 ps} {317100 ps}
