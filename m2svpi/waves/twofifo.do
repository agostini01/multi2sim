onerror {resume}
quietly WaveActivateNextPane {} 0
add wave -noupdate /tb_for_twofifo/U1/clk
add wave -noupdate /tb_for_twofifo/U1/rst
add wave -noupdate /tb_for_twofifo/U1/write_req_en
add wave -noupdate /tb_for_twofifo/U1/data_req_in
add wave -noupdate /tb_for_twofifo/U1/full_req_flag
add wave -noupdate /tb_for_twofifo/U1/read_req_en
add wave -noupdate /tb_for_twofifo/U1/data_req_out
add wave -noupdate /tb_for_twofifo/U1/empty_req_flag
add wave -noupdate /tb_for_twofifo/U1/read_serve_en
add wave -noupdate /tb_for_twofifo/U1/data_serve_out
add wave -noupdate /tb_for_twofifo/U1/empty_serve_flag
add wave -noupdate /tb_for_twofifo/U1/write_serve_en
add wave -noupdate /tb_for_twofifo/U1/data_serve_in
add wave -noupdate /tb_for_twofifo/U1/full_serve_flag
add wave -noupdate -divider {New Divider}
add wave -noupdate /tb_for_twofifo/U1/REQUEST_FIFO/clk
add wave -noupdate /tb_for_twofifo/U1/REQUEST_FIFO/rst
add wave -noupdate /tb_for_twofifo/U1/REQUEST_FIFO/wr_cs
add wave -noupdate /tb_for_twofifo/U1/REQUEST_FIFO/rd_cs
add wave -noupdate /tb_for_twofifo/U1/REQUEST_FIFO/rd_en
add wave -noupdate /tb_for_twofifo/U1/REQUEST_FIFO/wr_en
add wave -noupdate /tb_for_twofifo/U1/REQUEST_FIFO/data_in
add wave -noupdate /tb_for_twofifo/U1/REQUEST_FIFO/full
add wave -noupdate /tb_for_twofifo/U1/REQUEST_FIFO/empty
add wave -noupdate /tb_for_twofifo/U1/REQUEST_FIFO/data_out
add wave -noupdate /tb_for_twofifo/U1/REQUEST_FIFO/wr_pointer
add wave -noupdate /tb_for_twofifo/U1/REQUEST_FIFO/rd_pointer
add wave -noupdate /tb_for_twofifo/U1/REQUEST_FIFO/status_cnt
add wave -noupdate /tb_for_twofifo/U1/REQUEST_FIFO/data_ram
add wave -noupdate /tb_for_twofifo/U1/REQUEST_FIFO/DP_RAM/address_0
add wave -noupdate /tb_for_twofifo/U1/REQUEST_FIFO/DP_RAM/cs_0
add wave -noupdate /tb_for_twofifo/U1/REQUEST_FIFO/DP_RAM/we_0
add wave -noupdate /tb_for_twofifo/U1/REQUEST_FIFO/DP_RAM/oe_0
add wave -noupdate /tb_for_twofifo/U1/REQUEST_FIFO/DP_RAM/address_1
add wave -noupdate /tb_for_twofifo/U1/REQUEST_FIFO/DP_RAM/cs_1
add wave -noupdate /tb_for_twofifo/U1/REQUEST_FIFO/DP_RAM/we_1
add wave -noupdate /tb_for_twofifo/U1/REQUEST_FIFO/DP_RAM/oe_1
add wave -noupdate /tb_for_twofifo/U1/REQUEST_FIFO/DP_RAM/data_0
add wave -noupdate /tb_for_twofifo/U1/REQUEST_FIFO/DP_RAM/data_1
add wave -noupdate /tb_for_twofifo/U1/REQUEST_FIFO/DP_RAM/data_0_out
add wave -noupdate /tb_for_twofifo/U1/REQUEST_FIFO/DP_RAM/data_1_out
add wave -noupdate -expand /tb_for_twofifo/U1/REQUEST_FIFO/DP_RAM/mem
add wave -noupdate -divider {New Divider}
add wave -noupdate /tb_for_twofifo/U1/SERVE_FIFO/clk
add wave -noupdate /tb_for_twofifo/U1/SERVE_FIFO/rst
add wave -noupdate /tb_for_twofifo/U1/SERVE_FIFO/wr_cs
add wave -noupdate /tb_for_twofifo/U1/SERVE_FIFO/rd_cs
add wave -noupdate /tb_for_twofifo/U1/SERVE_FIFO/rd_en
add wave -noupdate /tb_for_twofifo/U1/SERVE_FIFO/wr_en
add wave -noupdate /tb_for_twofifo/U1/SERVE_FIFO/data_in
add wave -noupdate /tb_for_twofifo/U1/SERVE_FIFO/full
add wave -noupdate /tb_for_twofifo/U1/SERVE_FIFO/empty
add wave -noupdate /tb_for_twofifo/U1/SERVE_FIFO/data_out
add wave -noupdate /tb_for_twofifo/U1/SERVE_FIFO/wr_pointer
add wave -noupdate /tb_for_twofifo/U1/SERVE_FIFO/rd_pointer
add wave -noupdate /tb_for_twofifo/U1/SERVE_FIFO/status_cnt
add wave -noupdate /tb_for_twofifo/U1/SERVE_FIFO/data_ram
TreeUpdate [SetDefaultTree]
WaveRestoreCursors {{Cursor 1} {117500 ps} 0}
quietly wave cursor active 1
configure wave -namecolwidth 321
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
WaveRestoreZoom {0 ps} {210 ns}
