module counter_fsm (clock, reset, count);

  input   clock,reset;
  output  count[3:0];

  wire    clock,reset;

  parameter SIZE    = 5;
  parameter TWELVE  = 4'b1100;
  parameter THREE   = 4'b0011;
  parameter ONE     = 4'b0001;
  parameter FIVE    = 4'b0101;
  parameter TEN     = 4'b1010;


  reg [SIZE-1:0]  state;

  always @ (posedge clock)
  begin
    if (reset == 1'b1) begin
      count <= 4'b0000;
    end
    else begin
      count <= count + 1;
         case (state)
              TWELVE:
                    if (count == TWELVE)
                       count <= 4'b0000;
                       state <= THREE;
              THREE:
                    if (count == THREE)
                       count <= 4'b0000;
                       state <= ONE;
              ONE:
                    if (count == ONE)
                       count <= 4'b0000;
                       state <= FIVE;
              FIVE:
                    if (count == FIVE)
                       count <= 4'b0000;
                       state <= TEN;
              TEN:
                    if (count == TEN)
                       count <= 4'b0000;
                       state <= TWELVE;
              default:
                   out <= 4'b0000;
                   state <= TWELVE;
         endcase
      end //Else
    end //Always

endmodule


module counter (out, enable, clk, reset);

  output [7:0] out;
  input enable, clk, reset;
  reg [7:0] out;

  always @(posedge clk)
  if (reset)
  begin
    out <= 8'b0 ;
  end
  else if (enable)
  begin
    out <= out + 1;
  end


endmodule
