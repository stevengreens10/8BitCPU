module Buffer8(
    input EN,
    inout [7:0] IN,
    inout [7:0] OUT
);

assign OUT = EN ? IN : 8'bZZZZZZZZ;

endmodule