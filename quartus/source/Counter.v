// 4 bit counter
// increments at falling clock
module Counter(
    input clk,
    input reset,
    input done,
    output [3:0] count
);

reg [3:0] count_r = 0;

always @(negedge clk) begin
    if(reset) begin
        count_r <= -1;
    end else if(done) begin
        count_r <= 0;
    end else begin
        count_r <= count + 1;
    end
end

assign count = count_r;

endmodule