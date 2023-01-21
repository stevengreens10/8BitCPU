`include "./ALU_ops.v"

module ALU(
    input [7:0] ACC_IN,
    input [7:0] DATA_IN,
    input [3:0] op,

    output [7:0] ACC_OUT
);

reg [7:0] RESULT;

assign ACC_OUT[7:0] = RESULT[7:0];

always @(*) begin
    if (op == `OP_NOP) begin
        RESULT <= ACC_IN;
    end else if (op == `OP_ADD) begin
        RESULT <= ACC_IN + DATA_IN;
    end else if (op == `OP_SUB) begin
        RESULT <= ACC_IN - DATA_IN;
    end else if (op == `OP_AND) begin
        RESULT <= ACC_IN & DATA_IN;
    end else if (op == `OP_NOT) begin
        RESULT <= ~ACC_IN;
    end else if (op == `OP_OR) begin
        RESULT <= ACC_IN | DATA_IN;
    end
end


endmodule