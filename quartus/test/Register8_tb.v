`timescale 1 ns/10 ps  // time-unit = 1 ns, precision = 10 ps


module Register8_tb();
reg clk;
wire [7:0] BUS;
reg A_LATCH;
reg A_ENABLE;
wire [7:0] A_REG;

reg [7:0] counter;

assign BUS[7:0] = A_LATCH ? counter : 8'bZZZZZZZZ;

Register8 dut(
    .clk(clk),
	.latch(A_LATCH),
	.enable(A_ENABLE),
	.BUS(BUS),
	.REG_OUT(A_REG)
);

initial begin
    clk = 0;
    counter = 0;
    A_LATCH = 0;
    A_ENABLE = 0;
    #10

    #5
    clk = 0;
    #5
    counter = 1;
    A_LATCH = 1;
    A_ENABLE = 0;
    #5
    clk = 1;

    #5
    clk = 0;
    #5
    counter = 0;
    A_LATCH = 0;
    A_ENABLE = 0;
    #5
    clk = 1;

    #5
    clk = 0;
    #5
    counter = 0;
    A_LATCH = 0;
    A_ENABLE = 1;
    #5
    clk = 1;

    #5
    $stop;
end

endmodule