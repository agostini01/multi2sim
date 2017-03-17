`ifndef ARRAY_PACK_UNPACK_V
`ifdef PACK_ARRAY
$finish; // macro PACK_ARRAY already exists. refusing to redefine.
`endif
`ifdef UNPACK_ARRAY
$finish; // macro UNPACK_ARRAY already exists. refusing to redefine.
`endif

`ifdef PACK_REG_ARRAY
$finish; // macro PACK_REG_ARRAY already exists. refusing to redefine.
`endif
`ifdef UNPACK_REG_ARRAY
$finish; // macro UNPACK_REG_ARRAY already exists. refusing to redefine.
`endif

`define ARRAY_PACK_UNPACK_V 1

// Must add the genvars outside of this
`define PACK_ARRAY(pk_idx,PK_WIDTH,PK_LEN,PK_SRC,PK_DEST)  generate for (pk_idx=0; pk_idx<(PK_LEN); pk_idx=pk_idx+1) begin assign PK_DEST[((PK_WIDTH)*pk_idx+((PK_WIDTH)-1)):((PK_WIDTH)*pk_idx)] = PK_SRC[pk_idx][((PK_WIDTH)-1):0]; end endgenerate

`define UNPACK_ARRAY(unpk_idx,PK_WIDTH,PK_LEN,PK_DEST,PK_SRC) generate for (unpk_idx=0; unpk_idx<(PK_LEN); unpk_idx=unpk_idx+1) begin assign PK_DEST[unpk_idx][((PK_WIDTH)-1):0] = PK_SRC[((PK_WIDTH)*unpk_idx+(PK_WIDTH-1)):((PK_WIDTH)*unpk_idx)]; end endgenerate

// Must add the genvars outside of this
`define PACK_REG_ARRAY(pk_idx,PK_WIDTH,PK_LEN,PK_SRC,PK_DEST)  generate for (pk_idx=0; pk_idx<(PK_LEN); pk_idx=pk_idx+1) begin always@(PK_SRC) PK_DEST[((PK_WIDTH)*pk_idx+((PK_WIDTH)-1)):((PK_WIDTH)*pk_idx)] = PK_SRC[pk_idx][((PK_WIDTH)-1):0]; end endgenerate

`define UNPACK_REG_ARRAY(unpk_idx,PK_WIDTH,PK_LEN,PK_DEST,PK_SRC) generate for (unpk_idx=0; unpk_idx<(PK_LEN); unpk_idx=unpk_idx+1) begin always@(PK_SRC) PK_DEST[unpk_idx][((PK_WIDTH)-1):0] = PK_SRC[((PK_WIDTH)*unpk_idx+(PK_WIDTH-1)):((PK_WIDTH)*unpk_idx)]; end endgenerate

`endif
