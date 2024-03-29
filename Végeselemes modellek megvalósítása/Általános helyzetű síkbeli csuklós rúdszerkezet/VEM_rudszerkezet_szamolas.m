clear
close all
clc

%ADATOK:
a=2.5;
b=1.9;
c=6;
d=40/1000;
f=130*1000;
E=170e9;
nu=0.3;

%SZAMOLAS:
%Keresztmetszet
A=((1.3*d)^2-d^2)*pi/4;

%Csomopontok megadasa (koordinatak,terhelesek,kenyszerek)
Csomopontok(1) = csomopont(0,b,0,0,false,false);
Csomopontok(2) = csomopont(c,b,0,-f,true,false);
Csomopontok(3) = csomopont(a,b/2,-f,0,false,false);
Csomopontok(4) = csomopont(0,0,0,3*f,true,false);
Csomopontok(5) = csomopont(c,0,0,0,true,true);

cspontSzam = size(Csomopontok);
cspontSzam = cspontSzam(2);

%Elemek megadasa - elemi mennyisegek szamitasa az osztaly konstruktoraban
Elemek(1) = elem(Csomopontok(1),Csomopontok(2),A,E);
Elemek(2) = elem(Csomopontok(1),Csomopontok(3),A,E);
Elemek(3) = elem(Csomopontok(3),Csomopontok(2),A,E);
Elemek(4) = elem(Csomopontok(1),Csomopontok(4),A,E);
Elemek(5) = elem(Csomopontok(4),Csomopontok(3),A,E);
Elemek(6) = elem(Csomopontok(3),Csomopontok(5),A,E);
Elemek(7) = elem(Csomopontok(4),Csomopontok(5),A,E);

elemSzam = size(Elemek);
elemSzam = elemSzam(2);

%K globalis merevsegi matrix eloallitasa
K = zeros(cspontSzam*2,cspontSzam*2);
for n = 1:elemSzam
    for k = 1:cspontSzam
        if (Csomopontok(k)==Elemek(n).csuklo1)
            row=k;
        end
    end
    for k = 1:cspontSzam
        if (Csomopontok(k)==Elemek(n).csuklo2)
            col=k;
        end
    end
    K([2*row-1 2*row],[2*row-1 2*row]) = K([2*row-1 2*row],[2*row-1 2*row]) + Elemek(n).K([1 2],[1 2]);
    K([2*row-1 2*row],[2*col-1 2*col]) = K([2*row-1 2*row],[2*col-1 2*col]) + Elemek(n).K([1 2],[3 4]);
    K([2*col-1 2*col],[2*row-1 2*row]) = K([2*col-1 2*col],[2*row-1 2*row]) + Elemek(n).K([3 4],[1 2]);
    K([2*col-1 2*col],[2*col-1 2*col]) = K([2*col-1 2*col],[2*col-1 2*col]) + Elemek(n).K([3 4],[3 4]);
end

%Kondenzalt valtozok eloallitasa
Fhullam=zeros(cspontSzam*2,1);
Khullam=K;
seged=eye(cspontSzam*2);
for k = cspontSzam:-1:1
    Fhullam(2*k-1)=Csomopontok(k).Fx;
    Fhullam(2*k)=Csomopontok(k).Fy;
    if(Csomopontok(k).kenyszerY)
        Khullam(2*k,:)=[];
        Khullam(:,2*k)=[];
        seged(:,2*k)=[];
        Fhullam(2*k,:)=[];
    end
    if(Csomopontok(k).kenyszerX)
        Khullam(2*k-1,:)=[];
        Khullam(:,2*k-1)=[];
        seged(:,2*k-1)=[];
        Fhullam(2*k-1,:)=[];
    end
end

%Szerkezeti egyenlet megoldasa
Uhullam=Khullam^-1*Fhullam;
U=seged*Uhullam;
F=K*U;

