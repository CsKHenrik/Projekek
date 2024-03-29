classdef elem
   properties
      csuklo1 csomopont
      csuklo2 csomopont
      A double
      E double
      L
      ca
      sa
      K
   end
   methods
       %konstruktor
       function obj = elem(csomopont1,csomopont2,A,E)
            obj.csuklo1 = csomopont1;
            obj.csuklo2 = csomopont2;
            obj.A = A;
            obj.E = E;
            %elemi mennyisegek
            obj.L = sqrt((obj.csuklo2.x-obj.csuklo1.x)^2+(obj.csuklo2.y-obj.csuklo1.y)^2);
            obj.ca = (obj.csuklo2.x-obj.csuklo1.x)/obj.L;
            obj.sa = (obj.csuklo2.y-obj.csuklo1.y)/obj.L;
            obj.K=[obj.ca^2 obj.ca*obj.sa -obj.ca^2 -obj.ca*obj.sa;
                   obj.ca*obj.sa obj.sa^2 -obj.ca*obj.sa -obj.sa^2;
                   -obj.ca^2 -obj.ca*obj.sa obj.ca^2 obj.ca*obj.sa;
                   -obj.ca*obj.sa -obj.sa^2 obj.ca*obj.sa obj.sa^2]*obj.A*obj.E/obj.L;
       end
   end
end