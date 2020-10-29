/* Hardware test for Arrow CYC1000
 *	
 * Instructions:
 * 
 * 1. Compile logic part
 *    - Regenerate Qsys source code if necessary.
 *    - If you make changes on the Qsys, you will have to generate BSP again in Eclipse tools
 *
 * 2. Convert programming files (.SOF --> .JIC for EPCQ16)
 *		- Recommend to "burn" the logic part in non-volatile memory of the board so it is saved
 *      when power is off. Then we will focus only on Eclipse and treat the board as an "Arduino"
 *		- Go to File -> Convert Programming Files -> Open conversion setup data -> jic_generate.cof
 *    - Select JIC file on programmer & burn it to the board
 *
 *	3. RESTART THE BOARD. This must be a "hard restart" or unplug & plug back the board to force 
 *    reload of configuration memory. NIOS CPU WON'T START UP WITHOUT THIS
 *
 * 4. Tools -> Nios II Software build tools for Eclipse
 *		- Select as workspace the same directory of this Quartus project
 *		
 *
 */

module top (
	input  CLK12M,
	output [7:0] LED,
	input  USER_BTN,
	input  CLK_X,
	input  SEN_INT1,
	input  SEN_INT2,
	inout  SEN_SDI,
	inout  SEN_SPC,
	input  SEN_SDO,
	output SEN_CS,
	output CKE,
	output RAS,
	output WE,
	output CS,
	output CAS,
	input D11_R,
	input D12_R,
	input AIN_X,
	input ADBUS_4,
	input ADBUS_7,
	input [7:0]	PIO,
	input [5:0]	BDBUS,
	inout [15:0] DQ,
	output [1:0] DQM,
	output [11:0] A,
	output [1:0] BA,
	inout [14:0] D,
	input [7:0] AIN,
	output MEM_CLK,
	
   inout  SEN_SCL,
   inout  SEN_SDA
);
wire nreset;
assign nreset = USER_BTN;

wire CLK48M;

// PLL generate 48MHz clock
//  Freq. of c0 and c1 are the same (48MHz), but the phase of c0 (MEM_CLK) is shifted
//  so that edges occur in the middle of the valid signal window.
//  See the section "Clock, PLL and Timing Considerations" of the "SDRAM Controller Core" document:
//  https://www.intel.cn/content/dam/altera-www/global/zh_CN/pdfs/literature/hb/nios2/n2cpu_nii51005.pdf
my_pll pll0(
	.areset(~nreset),
	.inclk0(CLK12M),
	.c0(MEM_CLK),
	.c1(CLK48M),
	.locked()
);

wire [7:0] nios_leds;
wire [7:0] nios_pio0;
wire reset_request;

wire scl_i;
wire sda_i;
wire i2c_scl_oe;
wire i2c_sda_oe;


my_nios u0 (
  .clk_clk					(CLK48M),
  .reset_reset_n			(nreset & ~reset_request),
  .reset_request_reset	(reset_request),
  .pio0_export				(nios_pio0),
  .pio1_export				(nios_leds),
  .sdram_addr				(A),
  .sdram_ba					(BA),
  .sdram_cas_n				(CAS),
  .sdram_cke				(CKE),
  .sdram_cs_n				(CS),
  .sdram_dq					(DQ),
  .sdram_dqm				(DQM),
  .sdram_ras_n				(RAS),
  .sdram_we_n				(WE),

  .spi_MISO             (SEN_SDO),
  .spi_MOSI             (SEN_SDI),
  .spi_SCLK             (SEN_SPC),
  .spi_SS_n					(SEN_CS),
  
  .i2c_sda_in				(sda_i),
  .i2c_scl_in				(scl_i),
  .i2c_sda_oe				(i2c_sda_oe),
  .i2c_scl_oe           (i2c_scl_oe)
);

// Leds (R, Y, G)
assign D[14:12] = nios_pio0[2:0];

// i2C => CO2 sensor
assign D[8] = i2c_scl_oe ? 1'b0:  1'bz;
assign scl_i = D[8];

assign D[9] = i2c_sda_oe ? 1'b0 : 1'bz;
assign sda_i = D[9];

// built in LEDS
assign LED[7] = 1'b0; // nios_leds;

endmodule