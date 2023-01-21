`include "./ALU_ops.v"
module InstructionDecoder(
    input clk,
    input reset,
    input [7:0] INST,

	output DATA_IN,
    output DATA_OUT,

    output [3:0] ALU_OP,

	output A_LATCH,
	output A_ENABLE,
    output A_READ_ALU,

    output B_LATCH,
	output B_ENABLE,

	output IP_LATCH,
	output IP_ENABLE,
	output IP_INC,

	output IR_LATCH,

	output ADDR_LATCH,

	output D_LATCH,
    output D_ENABLE,
	output D_INC,
	output D_IN_SELECT,
	output D_OUT_SELECT
);

// ==== REGS/ASSIGNMENTS ====
reg DATA_IN_R = 0;
assign DATA_IN = DATA_IN_R;

reg DATA_OUT_R = 0;
assign DATA_OUT = DATA_OUT_R;


reg [3:0] ALU_OP_R = 0;
assign ALU_OP[3:0] = ALU_OP_R[3:0];

reg A_LATCH_R = 0;
assign A_LATCH = A_LATCH_R;

reg A_ENABLE_R = 0;
assign A_ENABLE = A_ENABLE_R;

reg A_READ_ALU_R = 0;
assign A_READ_ALU = A_READ_ALU_R;


reg B_LATCH_R = 0;
assign B_LATCH = B_LATCH_R;

reg B_ENABLE_R = 0;
assign B_ENABLE = B_ENABLE_R;


reg IP_LATCH_R = 0;
assign IP_LATCH = IP_LATCH_R;

reg IP_ENABLE_R = 0;
assign IP_ENABLE = IP_ENABLE_R;

reg IP_INC_R = 0;
assign IP_INC = IP_INC_R;


reg IR_LATCH_R = 0;
assign IR_LATCH = IR_LATCH_R;

reg ADDR_LATCH_R = 0;
assign ADDR_LATCH = ADDR_LATCH_R;

reg D_LATCH_R = 0;
assign D_LATCH = D_LATCH_R;

reg D_ENABLE_R = 0;
assign D_ENABLE = D_ENABLE_R;

reg D_INC_R = 0;
assign D_INC = D_INC_R;

reg D_IN_SELECT_R = 0;
assign D_IN_SELECT = D_IN_SELECT_R;

reg D_OUT_SELECT_R = 0;
assign D_OUT_SELECT = D_OUT_SELECT_R;

// Indicates an instruction is complete
reg DONE_R = 0;

wire [3:0] CYCLE;

Counter counter(
    .clk(clk),
    .reset(reset),
    .done(DONE_R),
    .count(CYCLE)
);

task resetSignals;
begin
    DATA_IN_R <= 0;
    DATA_OUT_R <= 0;
    ALU_OP_R <= 0;
    A_LATCH_R <= 0;
    A_ENABLE_R <= 0;
    A_READ_ALU_R <= 0;
    B_LATCH_R <= 0;
    B_ENABLE_R <= 0;
    IP_LATCH_R <= 0;
    IP_ENABLE_R <= 0;
    IP_INC_R <= 0;
    IR_LATCH_R <= 0;
    ADDR_LATCH_R <= 0;
    D_LATCH_R <= 0;
    D_ENABLE_R <= 0;
    D_INC_R <= 0;
    D_IN_SELECT_R <= 0;
    D_OUT_SELECT_R <= 0;

    DONE_R <= 0;
end
endtask

// On every cycle
// Set signals for instruction at that cycle
always @(CYCLE) begin
    resetSignals();
    if (reset == 0) begin
        if(CYCLE == 0) begin
            // Fetch
            DATA_IN_R <= 1;
            IP_ENABLE_R <= 1;
            IP_INC_R <= 1;
            IR_LATCH_R <= 1;
            ADDR_LATCH_R <= 1;
        end else begin
            if(INST == 0) begin /* NOP */
                nop();
            end else if(INST == 1) begin /* ADD A, imm8 */
                addAImm();
            end else if(INST == 2) begin /* LOD A, addr8 */
                lodA();
            end else if(INST == 3) begin /* MOV A, B */
                movAB();
            end else if(INST == 4) begin /* MOV B, A */
                movBA();
            end else if(INST == 5) begin /* WRITE imm8 */
                writeImm();
            end
        end
    end
end

// ===== UTIL =====
task fetchByte;
begin
    // Put IP onto address bus
    IP_ENABLE_R <= 1;
    ADDR_LATCH_R <= 1;

    // Read from data bus
    DATA_IN_R <= 1;

    // Increment IP
    IP_INC_R <= 1;
end
endtask

// ====== INSTRUCTIONS =======

// 0x00
task nop;
begin
    DONE_R <= 1;
end
endtask

// 0x01
task addAImm;
begin
    if(CYCLE == 1) begin
        fetchByte();
        ALU_OP_R <= `OP_ADD;
        A_READ_ALU_R <= 1;
        A_LATCH_R <= 1;

        DONE_R <= 1;
    end
end
endtask

// 0x02
task lodA;
begin
    if(CYCLE == 1) begin
        fetchByte();
        // Latch mem byte
        D_IN_SELECT_R <= 0; 
        D_LATCH_R <= 1;
    end else if(CYCLE == 2) begin
        // Write latched address to ADDR bus
        D_OUT_SELECT_R <= 1; /* Output to ADDR bus */
        D_ENABLE_R <= 1;
        ADDR_LATCH_R <= 1;

        // Latch DATA_IN to A
        DATA_IN_R <= 1;
        A_LATCH_R <= 1;
        DONE_R <= 1;
    end
end
endtask

// 0x03
task movAB;
begin
    A_LATCH_R <= 1;
    B_ENABLE_R <= 1;
    DONE_R <= 1;
end
endtask

// 0x04
task movBA;
begin
    B_LATCH_R <= 1;
    A_ENABLE_R <= 1;
    DONE_R <= 1;
end
endtask

// 0x05
task writeImm;
begin
    if(CYCLE == 1) begin
        fetchByte();
        DATA_OUT_R <= 1;
        DONE_R <= 1;
    end
end
endtask

endmodule