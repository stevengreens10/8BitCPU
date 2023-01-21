`timescale 1 ns/10 ps  // time-unit = 1 ns, precision = 10 ps

module CPU_tb();
reg clk = 0;
reg reset = 0;
reg [7:0] DATA_IN = 0;
wire [7:0] ADDR_OUT;

CPU CPU(
    .clk(clk),
    .reset(reset),
    .DATA_IN_BUS(DATA_IN),
    .ADDR_OUT_BUS(ADDR_OUT)
);

task assertEQ;
    input [7:0] val1;
    input [7:0] val2;
    begin
        if(val1 != val2) begin
            $display("ERROR:Assertion failed at time %0t. 0x%0h != 0x%0h", $time, val1, val2);
            $stop;
        end
    end
endtask

// CLOCK PULSE WIDTH = 10
// CLOCK PERIOD = 20

initial begin

    // Reset (1 cycle)
    reset = 1;
    clk = 0;
    DATA_IN = 0;
    #5
    clk = 1;
    #5
    reset = 0;

    // NOP (2 cycles)
    #5
    clk = 0;
    #5
    assertEQ(ADDR_OUT, 0);
    DATA_IN = 0;
    #5
    clk = 1;
    #10
    clk = 0;
    #10
    clk = 1;

    // ADD A, 2 (2 cycles)
    #10
    clk = 0;
    #5
    assertEQ(ADDR_OUT, 1);
    DATA_IN = 1;
    #5
    clk = 1;
    #10
    clk = 0;
    #5
    assertEQ(ADDR_OUT, 2);
    DATA_IN = 2;
    #5
    clk = 1;

    #5
    assertEQ(CPU.A_REG, 2);

    // LOD A, &42 (3 cycles)
    // (&42 = 69)
    #5
    clk = 0;
    #5
    assertEQ(ADDR_OUT, 3);
    DATA_IN = 2;
    #5
    clk = 1;

    #10
    clk = 0;
    #5
    assertEQ(ADDR_OUT, 4);
    DATA_IN = 42;
    #5
    clk = 1;

    #10
    clk = 0;
    #5
    assertEQ(ADDR_OUT, 42);
    DATA_IN = 69;
    #5
    clk = 1;

    #5
    assertEQ(CPU.A_REG, 69);

    // ADD A, 5 (2 cycles)
    #5
    clk = 0;
    #5
    assertEQ(ADDR_OUT, 5);
    DATA_IN = 1;
    #5
    clk = 1;
    #10
    clk = 0;
    #5
    DATA_IN = 5;
    #5
    clk = 1;

    #5
    assertEQ(CPU.A_REG, 74);
    
    #5
    clk = 0;
    $stop;
end

endmodule