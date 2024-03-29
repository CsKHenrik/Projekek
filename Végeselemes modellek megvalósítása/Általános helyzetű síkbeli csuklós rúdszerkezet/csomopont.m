classdef csomopont
   properties
      %koordinatak
      x double
      y double
      %terhelesek
      Fx double
      Fy double
      %kenyszerek
      kenyszerX logical
      kenyszerY logical
   end
   methods
       %konstruktor
       function obj = csomopont(x,y,Fx,Fy,kenyszerX,kenyszerY)
           obj.x=x;
           obj.y=y;
           obj.Fx=Fx;
           obj.Fy=Fy;
           obj.kenyszerX=kenyszerX;
           obj.kenyszerY=kenyszerY;
       end
       %egyenloseg operator a kereseshez
       function r = eq(obj1,obj2)
           r = (obj1.x==obj2.x) & (obj1.y==obj2.y);
       end
   end
end