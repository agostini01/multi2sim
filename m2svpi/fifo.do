onerror {resume}
quietly WaveActivateNextPane {} 0
add wave -noupdate /tb_for_fifo/clk
add wave -noupdate /tb_for_fifo/reset
add wave -noupdate /tb_for_fifo/data_serve_in
add wave -noupdate /tb_for_fifo/write_serve_en
add wave -noupdate /tb_for_fifo/data_serve_out
add wave -noupdate /tb_for_fifo/read_serve_en
add wave -noupdate /tb_for_fifo/empty_serve_flag
add wave -noupdate /tb_for_fifo/full_serve_flag
add wave -noupdate /tb_for_fifo/test_fifo/DP_RAM/address_0
add wave -noupdate /tb_for_fifo/test_fifo/DP_RAM/cs_0
add wave -noupdate /tb_for_fifo/test_fifo/DP_RAM/we_0
add wave -noupdate /tb_for_fifo/test_fifo/DP_RAM/oe_0
add wave -noupdate /tb_for_fifo/test_fifo/DP_RAM/address_1
add wave -noupdate /tb_for_fifo/test_fifo/DP_RAM/cs_1
add wave -noupdate /tb_for_fifo/test_fifo/DP_RAM/we_1
add wave -noupdate /tb_for_fifo/test_fifo/DP_RAM/oe_1
add wave -noupdate /tb_for_fifo/test_fifo/DP_RAM/data_0
add wave -noupdate /tb_for_fifo/test_fifo/DP_RAM/data_1
add wave -noupdate /tb_for_fifo/test_fifo/DP_RAM/data_0_out
add wave -noupdate /tb_for_fifo/test_fifo/DP_RAM/data_1_out
add wave -noupdate -expand /tb_for_fifo/test_fifo/DP_RAM/mem
TreeUpdate [SetDefaultTree]
WaveRestoreCursors {{Cursor 1} {737100 ps} 0}
quietly wave cursor active 1
configure wave -namecolwidth 302
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
configure wave -timelineunits ps
update
WaveRestoreZoom {601 ns} {981700 ps}
