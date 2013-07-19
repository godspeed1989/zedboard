
library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_arith.all;
use ieee.std_logic_unsigned.all;

entity bram_vga is
  generic
  (
    C_S_AXI_DATA_WIDTH             : integer              := 32;
    C_S_AXI_ADDR_WIDTH             : integer              := 32;
    C_SLV_AWIDTH                   : integer              := 32;
    C_SLV_DWIDTH                   : integer              := 32;
    VGA_DATA_WIDTH                 : integer              := 4
  );
  port
  (
    bram_Rst                : out   std_logic;
    bram_Clk                : out   std_logic;
    bram_En                 : out   std_logic;
    bram_WE                 : out   std_logic_vector (C_S_AXI_DATA_WIDTH/8 - 1 downto 0);
    bram_Addr               : out   std_logic_vector (C_S_AXI_ADDR_WIDTH - 1 downto 0);
    bram_WrData             : out   std_logic_vector (C_S_AXI_DATA_WIDTH - 1 downto 0);
    bram_RdData             : in    std_logic_vector (C_S_AXI_DATA_WIDTH - 1 downto 0);
    vga_HS                  : out   std_logic;
    vga_VS                  : out   std_logic;
    vga_R                   : out   std_logic_vector (VGA_DATA_WIDTH - 1 downto 0);
    vga_G                   : out   std_logic_vector (VGA_DATA_WIDTH - 1 downto 0);
    vga_B                   : out   std_logic_vector (VGA_DATA_WIDTH - 1 downto 0);

	S_AXI_ACLK              : in    std_logic
  );

end entity bram_vga;

------------------------------------------------------------------------------
-- Architecture section
------------------------------------------------------------------------------

architecture IMP of bram_vga is

  signal ipif_Bus2IP_Clk                : std_logic;

  -- Declaration for verilog user logic, not implementation
  component user_logic is
    generic
    (
	  USER_VGA_DATA_WIDTH               : integer              := 4
	);
	port
    (
      -- ADD USER PORTS BELOW THIS LINE ------------------
      bram_Rst                : out   std_logic;
      bram_Clk                : out   std_logic;
      bram_En                 : out   std_logic;
      bram_WE                 : out   std_logic_vector (C_S_AXI_DATA_WIDTH/8 - 1 downto 0);
      bram_Addr               : out   std_logic_vector (C_S_AXI_ADDR_WIDTH - 1 downto 0);
      bram_WrData             : out   std_logic_vector (C_S_AXI_DATA_WIDTH - 1 downto 0);   
      bram_RdData             : in    std_logic_vector (C_S_AXI_DATA_WIDTH - 1 downto 0);      
      vga_HS                  : out   std_logic;
      vga_VS                  : out   std_logic;
      vga_R                   : out   std_logic_vector (VGA_DATA_WIDTH - 1 downto 0);
      vga_G                   : out   std_logic_vector (VGA_DATA_WIDTH - 1 downto 0);
      vga_B                   : out   std_logic_vector (VGA_DATA_WIDTH - 1 downto 0);

      Bus2IP_Clk              : in    std_logic
    );
  end component user_logic;

begin

  ipif_Bus2IP_Clk <= S_AXI_ACLK;
  ------------------------------------------
  -- instantiate User Logic
  ------------------------------------------
  USER_LOGIC_I : component user_logic
    generic map
    (
      USER_VGA_DATA_WIDTH            => VGA_DATA_WIDTH
    )
    port map
    (
      bram_Rst                       => bram_Rst,
      bram_Clk                       => bram_Clk,
      bram_En                        => bram_En,
      bram_WE                        => bram_WE,
      bram_Addr                      => bram_Addr,
      bram_WrData                    => bram_WrData,
      bram_RdData                    => bram_RdData,
      vga_HS                         => vga_HS,
      vga_VS                         => vga_VS,
      vga_R                          => vga_R,
      vga_G                          => vga_G,
      vga_B                          => vga_B,

      Bus2IP_Clk                     => ipif_Bus2IP_Clk
    );

end IMP;

