
module my_nios (
	clk_clk,
	i2c_sda_in,
	i2c_scl_in,
	i2c_sda_oe,
	i2c_scl_oe,
	pio0_export,
	pio1_export,
	reset_reset_n,
	reset_request_reset,
	sdram_addr,
	sdram_ba,
	sdram_cas_n,
	sdram_cke,
	sdram_cs_n,
	sdram_dq,
	sdram_dqm,
	sdram_ras_n,
	sdram_we_n,
	spi_MISO,
	spi_MOSI,
	spi_SCLK,
	spi_SS_n);	

	input		clk_clk;
	input		i2c_sda_in;
	input		i2c_scl_in;
	output		i2c_sda_oe;
	output		i2c_scl_oe;
	output	[7:0]	pio0_export;
	input	[7:0]	pio1_export;
	input		reset_reset_n;
	output		reset_request_reset;
	output	[11:0]	sdram_addr;
	output	[1:0]	sdram_ba;
	output		sdram_cas_n;
	output		sdram_cke;
	output		sdram_cs_n;
	inout	[15:0]	sdram_dq;
	output	[1:0]	sdram_dqm;
	output		sdram_ras_n;
	output		sdram_we_n;
	input		spi_MISO;
	output		spi_MOSI;
	output		spi_SCLK;
	output		spi_SS_n;
endmodule
