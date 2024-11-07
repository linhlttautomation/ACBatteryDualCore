/**
 * Ham thuc hien tinh toan bo dieu khien PID.
 * Ham duoc thuc hien trong de thuc hien bo dieu khien PID tinh toan voi so thuc, dau phay dong.
 * Co the ung dung su dung trong tinh toan dieu khien trong nhan CLA su dung trinh bien dich C-Compiler.
 * Xay dung tren CCS V5.3, Codegen 6.1.0. Thang 02-2013
 * Mr. LuanLT (luanlt@viettel.com.vn)
 * Phien ban 1.0 - CNTL_2P2Z_CLA_C.h
 */

#ifndef CNTL_2P2Z_CLA_C_H_
#define CNTL_2P2Z_CLA_C_H_

/**
 * Cau truc bien bo dieu khien PID so.
 */
typedef struct {
	// Cac tham so dau vao
	float Ref;
	float Fdbk;
	// Cac he so dieu khien
	float Kp;
	float Ki;
	float Kd;

	float a2;
	float a1;
	float b0;
	float b1;
	float b2;
	// Gioi han bao hoa dau ra
	float Max;
	float DutyMax;
	float IMin;
	float Min;
	// Cac tham so su dung ben trong ham
	float Errn;
	float Errn_1;
	float Errn_2;
	// Bien dau ra
	float Out;
	float Out_1;
	float Out_2;
} CNTL_2P2Z_CLA_C_Vars;

typedef struct {
	float In;
	float Out;
	float Max;
	float Min;
} LIMIT_VALUES;

/**
 * Tinh toan bo dieu khien PID so su dung cac he so va cac tham bien truyen vao.
 * Bo dieu khien PID duoc xay dung tu cong thuc ham truyen trong mien thoi gian su dung
 * Phuong phap sai phan hoa
 */

#define CNTL_2P2Z_CLA_C(v)																	\
		v.Out = v.a1*v.Out_1 + v.a2*v.Out_2 + v.b0*v.Errn + v.b1*v.Errn_1 + v.b2*v.Errn_2;	\
		v.Errn_2 = v.Errn_1;								\
		v.Errn_1 = v.Errn;									\
		v.Out =	__mmaxf32(__mminf32(v.Out, v.Max),v.IMin);	\
		v.Out_2 = v.Out_1;									\
		v.Out_1 = v.Out;									\
		v.Out = __mmaxf32(v.Out,v.Min)
/*
 * Khoi tao cac tham so ban dau cua bo dieu khien PID so.
 */
#define CNTL_2P2Z_CLA_C_INIT(v)				\
		/* Khoi tao bien */					\
		v.Ref = 0;							\
		v.Fdbk = 0;							\
		v.Errn = 0;							\
		v.Errn_1 = 0;						\
		v.Errn_2 = 0;						\
		v.Out = 0;							\
		v.Out_1 = 0;						\
		v.Out_2 = 0

#endif /* CNTL_2P2Z_CLA_C_H_ */

#define LIMIT(v)	v.Out =	__mmaxf32(__mminf32(v.In, v.Max),v.Min)
