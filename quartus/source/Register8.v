module Register8(
    input clk,
    input reset,
    input latch,
    input enable,
    input inc,
    input inBusSelect,
    input outBusSelect,
    inout [7:0] BUSA,
    inout [7:0] BUSB,
    output [7:0] REG_OUT
);

reg [7:0] value = 0;

always @(posedge clk) begin
    if(reset) begin
        value <= 0;
    end else begin
        if(latch) begin
            value <= inBusSelect == 0 ? BUSA : BUSB;
        end
        if(inc) begin
            value <= value + 1;
        end
    end
end

assign REG_OUT[7:0] = value;
assign BUSA[7:0] = outBusSelect == 0 ? (enable ? value : 8'bZZZZZZZZ) : 8'bZZZZZZZZ;
assign BUSB[7:0] = outBusSelect == 1 ? (enable ? value : 8'bZZZZZZZZ) : 8'bZZZZZZZZ;

endmodule