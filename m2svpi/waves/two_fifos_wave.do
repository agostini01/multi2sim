onerror {resume}
quietly WaveActivateNextPane {} 0
add wave -noupdate -divider {New Divider}
add wave -noupdate -divider {New Divider}
add wave -noupdate -divider {Global Stim}
add wave -noupdate /tb_for_two_fifos/clk
add wave -noupdate /tb_for_two_fifos/reset
add wave -noupdate -divider {Input Port}
add wave -noupdate /tb_for_two_fifos/from_DP_wr_ctr
add wave -noupdate /tb_for_two_fifos/from_DP_full
add wave -noupdate /tb_for_two_fifos/from_DP_data_in
add wave -noupdate -divider {Output Port}
add wave -noupdate /tb_for_two_fifos/to_VPI_rd_ctr
add wave -noupdate /tb_for_two_fifos/to_VPI_empty
add wave -noupdate /tb_for_two_fifos/to_VPI_data_out
add wave -noupdate -divider -height 35 {Request Fifo}
add wave -noupdate /tb_for_two_fifos/U1/REQUEST_FIFO/wr_cs
add wave -noupdate /tb_for_two_fifos/U1/REQUEST_FIFO/rd_cs
add wave -noupdate -divider {DP Port}
add wave -noupdate /tb_for_two_fifos/U1/REQUEST_FIFO/wr_en
add wave -noupdate /tb_for_two_fifos/U1/REQUEST_FIFO/full
add wave -noupdate /tb_for_two_fifos/U1/REQUEST_FIFO/data_in
add wave -noupdate -divider {VPI Port}
add wave -noupdate /tb_for_two_fifos/U1/REQUEST_FIFO/rd_en
add wave -noupdate /tb_for_two_fifos/U1/REQUEST_FIFO/empty
add wave -noupdate /tb_for_two_fifos/U1/REQUEST_FIFO/data_out
add wave -noupdate -divider Internals
add wave -noupdate /tb_for_two_fifos/U1/REQUEST_FIFO/DP_RAM/mem
add wave -noupdate -divider -height 35 {Serve Fifo}
add wave -noupdate /tb_for_two_fifos/U1/SERVE_FIFO/wr_cs
add wave -noupdate /tb_for_two_fifos/U1/SERVE_FIFO/rd_cs
add wave -noupdate -divider DP
add wave -noupdate /tb_for_two_fifos/U1/SERVE_FIFO/rd_en
add wave -noupdate /tb_for_two_fifos/U1/SERVE_FIFO/empty
add wave -noupdate /tb_for_two_fifos/U1/SERVE_FIFO/data_out
add wave -noupdate -divider VPI
add wave -noupdate /tb_for_two_fifos/U1/SERVE_FIFO/wr_en
add wave -noupdate /tb_for_two_fifos/U1/SERVE_FIFO/full
add wave -noupdate /tb_for_two_fifos/U1/SERVE_FIFO/data_in
add wave -noupdate -divider Internals
add wave -noupdate /tb_for_two_fifos/U1/SERVE_FIFO/DP_RAM/mem
TreeUpdate [SetDefaultTree]
WaveRestoreCursors {{Cursor 1} {691 ns} 0}
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
WaveRestoreZoom {0 ns} {315 ns}
