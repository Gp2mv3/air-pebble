	component my_nios is
		port (
			clk_clk             : in    std_logic                     := 'X';             -- clk
			i2c_sda_in          : in    std_logic                     := 'X';             -- sda_in
			i2c_scl_in          : in    std_logic                     := 'X';             -- scl_in
			i2c_sda_oe          : out   std_logic;                                        -- sda_oe
			i2c_scl_oe          : out   std_logic;                                        -- scl_oe
			pio0_export         : out   std_logic_vector(7 downto 0);                     -- export
			pio1_export         : in    std_logic_vector(7 downto 0)  := (others => 'X'); -- export
			reset_reset_n       : in    std_logic                     := 'X';             -- reset_n
			reset_request_reset : out   std_logic;                                        -- reset
			sdram_addr          : out   std_logic_vector(11 downto 0);                    -- addr
			sdram_ba            : out   std_logic_vector(1 downto 0);                     -- ba
			sdram_cas_n         : out   std_logic;                                        -- cas_n
			sdram_cke           : out   std_logic;                                        -- cke
			sdram_cs_n          : out   std_logic;                                        -- cs_n
			sdram_dq            : inout std_logic_vector(15 downto 0) := (others => 'X'); -- dq
			sdram_dqm           : out   std_logic_vector(1 downto 0);                     -- dqm
			sdram_ras_n         : out   std_logic;                                        -- ras_n
			sdram_we_n          : out   std_logic;                                        -- we_n
			spi_MISO            : in    std_logic                     := 'X';             -- MISO
			spi_MOSI            : out   std_logic;                                        -- MOSI
			spi_SCLK            : out   std_logic;                                        -- SCLK
			spi_SS_n            : out   std_logic                                         -- SS_n
		);
	end component my_nios;

	u0 : component my_nios
		port map (
			clk_clk             => CONNECTED_TO_clk_clk,             --           clk.clk
			i2c_sda_in          => CONNECTED_TO_i2c_sda_in,          --           i2c.sda_in
			i2c_scl_in          => CONNECTED_TO_i2c_scl_in,          --              .scl_in
			i2c_sda_oe          => CONNECTED_TO_i2c_sda_oe,          --              .sda_oe
			i2c_scl_oe          => CONNECTED_TO_i2c_scl_oe,          --              .scl_oe
			pio0_export         => CONNECTED_TO_pio0_export,         --          pio0.export
			pio1_export         => CONNECTED_TO_pio1_export,         --          pio1.export
			reset_reset_n       => CONNECTED_TO_reset_reset_n,       --         reset.reset_n
			reset_request_reset => CONNECTED_TO_reset_request_reset, -- reset_request.reset
			sdram_addr          => CONNECTED_TO_sdram_addr,          --         sdram.addr
			sdram_ba            => CONNECTED_TO_sdram_ba,            --              .ba
			sdram_cas_n         => CONNECTED_TO_sdram_cas_n,         --              .cas_n
			sdram_cke           => CONNECTED_TO_sdram_cke,           --              .cke
			sdram_cs_n          => CONNECTED_TO_sdram_cs_n,          --              .cs_n
			sdram_dq            => CONNECTED_TO_sdram_dq,            --              .dq
			sdram_dqm           => CONNECTED_TO_sdram_dqm,           --              .dqm
			sdram_ras_n         => CONNECTED_TO_sdram_ras_n,         --              .ras_n
			sdram_we_n          => CONNECTED_TO_sdram_we_n,          --              .we_n
			spi_MISO            => CONNECTED_TO_spi_MISO,            --           spi.MISO
			spi_MOSI            => CONNECTED_TO_spi_MOSI,            --              .MOSI
			spi_SCLK            => CONNECTED_TO_spi_SCLK,            --              .SCLK
			spi_SS_n            => CONNECTED_TO_spi_SS_n             --              .SS_n
		);

