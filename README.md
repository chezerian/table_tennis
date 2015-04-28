# table_tennis
Library for Table Tennis Arduino scorer

						B1	B2
						B3	B4
			
			L1	L2		U1	U2		R1	R2
			L3	L4		U3	U4		R3	R4
	
						F1	F2
						F3	F4
				
						D1	D2
						D3	D4
				
RIGHT 	(R1,R2,R3,R4) (D2,F2,T2,B2) (D4,F4,T4,B4)
LEFT 	(L1,L2,L3,L4) (B3,T3,F3,D3) (B1,T1,F1,D1)

UP		(U1,U2,U3,R4) (B3,R1,F2,L4) (B4,R3,F1,L2)
DOWN	(D1,D2,D3,D4) (B1,R2,F4,L3) (B2,R4,F3,L1)

FRONT	(F1,F2,F3,F4) (U3,R3,D2,L3) (U4,R4,D1,L1)
BACK	(B1,B2,B3,B4) (U1,R1,D4,L1) (U2,R2,D3,L2)
