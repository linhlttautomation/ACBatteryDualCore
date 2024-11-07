// Code cu
//#ifndef SVM_H_
//#define SVM_H_
//
//typedef struct 	{ float  Ualpha; 			// Input: reference alpha-axis phase voltage
//				  float  Ubeta;			// Input: reference beta-axis phase voltage
//				  float  Udc;				// Input: reference Udc voltage
//				  float  Ta;				// Output: reference phase-a switching function
//				  float  Tb;				// Output: reference phase-b switching function
//				  float  Tc;				// Output: reference phase-c switching function
//				  float  Va;
//				  float  Vb;
//				  float  Vc;
//				  float  d1;
//				  float  d2;
//				  float  d0;
//				  float  temp_sv1;
//				  float  temp_sv2;
//				  float  inver_Udc;
//				  Uint16 Sector;
//				} SVGENDQ_CLA;
//
//#define SVGEN_CLA_MACRO(v)											                      	\
//	v.Sector = 0;																	       	\
//	v.temp_sv1=0.5*v.Ualpha;	            											  	\
//	v.temp_sv2=0.8660254037844386*v.Ubeta;	 /* 0.8660254037844386 = sqrt(3)/2*/		 	\
//	/* Inverse clarke transformation - ma tran clarke*/						               	\
//	v.Va = v.Ualpha;														               	\
//	v.Vb = -v.temp_sv1 + v.temp_sv2;											           	\
//	v.Vc = -v.temp_sv1 - v.temp_sv2;											           	\
//	/* 60 degree Sector determination - xac dinh sector*/								   	\
//	/* SAI ngay ngat dau tien da chon 2 phai chon 1*/									   	\
//	if(v.Va >= v.Vb)                                                                        \
//		{	                                                                               	\
//			if(v.Vb >= v.Vc) v.Sector = 1;                                                  \
//			else if(v.Vc >= v.Va) v.Sector = 5;                                             \
//				else v.Sector = 6;                                                         	\
//		}                                                                                  	\
//	else if(v.Vc > v.Vb) v.Sector = 4;                                                      \
//		else if (v.Va > v.Vc) v.Sector = 2;                                                 \
//			else v.Sector = 3;                                                              \
//																							\
//	v.inver_Udc = (Us_max*1.0/Udc_max)*(1.0/v.Udc);											\
//	/*	v.Udc gia tri NGHICH DAO da dc chuan hoa pu ???	*/								   	\
//	/* 1.732050807568877 = sqrt(3)*/                                                       	\
//	/* 0.8660254038 = sqrt(3)/2*/                                                           \
//																							\
//switch(v.Sector)															                \
//{																		                   	\
//	case 0:                                                                                	\
//	/*Tinh toan thoi gian thuc hien cac vector chuan*/                                     	\
//	/*Tinh toan thoi gian dong cat - calcalate upper*/                                     	\
//	v.Ta = 0.0;												                       			\
//	v.Tb = 0.0;												                       			\
//	v.Tc = 0.0;												                      			\
//	break;																                   	\
//	case 1:   /*Sector 1: d1=Z and d2=Y (abc ---> Tb,Ta,Tc)*/			                   	\
//	/*Tinh toan thoi gian thuc hien cac vector chuan*/                                     	\
//	v.d1 = (1.50*v.Ualpha + (-0.8660254038)*v.Ubeta)*v.inver_Udc;							\
//	v.d2 = (1.732050807568877*v.Ubeta)*v.inver_Udc;										  	\
//	v.d0 =  1.0 - v.d1- v.d2;														       	\
//	/*Tinh toan thoi gian dong cat - calcalate upper*/							           	\
//	v.Ta = 	v.d0/2.0 + v.d1 + v.d2;			                                \
//	v.Tb = 	v.d0/2.0 + v.d2;			                                 \
//	v.Tc = 	v.d0/2.0;			                          \
//	break;																                   	\
//	case 2:	  /* Sector 2: d1=Y and d2=-X (abc ---> Ta,Tc,Tb)*/ 		                   	\
//	/*Tinh toan thoi gian thuc hien cac vector chuan*/                                     	\
//	v.d1 = (-1.50*v.Ualpha + (0.8660254038)*v.Ubeta)*v.inver_Udc;							\
//	v.d2 = (1.50*v.Ualpha + (0.8660254038)*v.Ubeta)*v.inver_Udc;							\
//	v.d0 =  1.0 - v.d1 - v.d2;														        \
//	/*Tinh toan thoi gian dong cat - calcalate upper*/			 		                   	\
//	v.Ta = 	v.d0/2.0 + v.d2;						                \
//	v.Tb = 	v.d0/2.0 + v.d2 + v.d1;			        		                \
//	v.Tc = 	v.d0/2.0;			       		                \
//	break;																                   	\
//	case 3:	  /* Sector 3: d1=-Z and d2=X (abc ---> Ta,Tb,Tc)*/			                   	\
//	/*Tinh toan thoi gian thuc hien cac vector chuan*/                                     	\
//	v.d1 = (1.732050807568877*v.Ubeta)*v.inver_Udc;								   			\
//    v.d2 = (-1.50*v.Ualpha + (-0.8660254038)*v.Ubeta)*v.inver_Udc;					   		\
//    v.d0 =  1.0 - v.d1 - v.d2;																\
//	/*Tinh toan thoi gian dong cat - calcalate upper*/					                   	\
//	v.Ta = 	v.d0/2.0;							                    \
//	v.Tb = 	v.d0/2.0 + v.d1 + v.d2;						                    \
//	v.Tc = 	v.d0/2.0 + v.d2;						                    \
//	break;															                       	\
//	case 4:	  /* Sector 4: d1=-X and d2=Z (abc ---> Tc,Tb,Ta)*/		                       	\
//    v.d1 = (-1.732050807568877*v.Ubeta)*v.inver_Udc;								    	\
//    v.d2 = (-1.50*v.Ualpha + (0.8660254038)*v.Ubeta)*v.inver_Udc;						   	\
//    v.d0 =  1.0 - v.d1 - v.d2;																\
//	/*Tinh toan thoi gian dong cat - calcalate upper*/					                   	\
//	v.Ta = v.d0/2.0;							                    \
//	v.Tb = v.d0/2.0 + v.d2;								                    \
//	v.Tc = v.d0/2.0 + v.d2 + v.d1;						                   	\
//	break;																                   	\
//	case 5:	  /* Sector 5: d1=X and d2=-Y (abc ---> Tb,Tc,Ta)*/			                   	\
//	/*Tinh toan thoi gian thuc hien cac vector chuan*/                                     	\
//	v.d1 = (-1.50*v.Ualpha + (-0.8660254038)*v.Ubeta)*v.inver_Udc;						   	\
//	v.d2 = (1.50*v.Ualpha + (-0.8660254038)*v.Ubeta)*v.inver_Udc;						    \
//	v.d0 =  1.0 - v.d1 - v.d2;														    	\
//	/*Tinh toan thoi gian dong cat - calcalate upper*/				                       	\
//	v.Ta = v.d0/2.0 + v.d2;				                               	\
//	v.Tb = v.d0/2.0;				                                	\
//	v.Tc = v.d0/2.0 + v.d2 + v.d1;				                           	\
//	break;																                   	\
//	case 6:	  /* Sector 6: d1=-Y and d2=-Z (abc ---> Tc,Ta,Tb)*/		                   	\
//	/*Tinh toan thoi gian thuc hien cac vector chuan*/                                     	\
//	v.d1 = (1.50*v.Ualpha + (0.866025403)*v.Ubeta)*v.inver_Udc;							   	\
//	v.d2 = (-1.732050807568877*v.Ubeta)*v.inver_Udc;								       	\
//	v.d0 =  1.0 - v.d1 - v.d2;															   	\
//	/*Tinh toan thoi gian dong cat - calcalate upper*/									   	\
//	v.Ta = v.d0/2.0 + v.d1 + v.d2;							                   	\
//	v.Tb = v.d0/2.0;							                   	\
//	v.Tc = v.d0/2.0 + v.d2;						                   	\
//	break;		 																		   	\
//}																						   	\
///* Limit v.Ta,v.Tb,v.Tc 0:1*/															   	\
//if (v.Ta<0.0) v.Ta = 0.0; else if(v.Ta>0.95) v.Ta = 0.98;									   	\
//if (v.Tb<0.0) v.Tb = 0.0; else if(v.Tb>0.95) v.Tb = 0.98;									   	\
//if (v.Tc<0.0) v.Tc = 0.0; else if(v.Tc>0.95) v.Tc = 0.98;
//
//#define SVGEN_CLA_INT(v)	\
//		v.Ualpha = 0;		\
//		v.Ubeta = 0;		\
//		v.Udc = 0;			\
//		v.Ta = 0;			\
//		v.Tb = 0;			\
//		v.Tc = 0;			\
//		v.Va = 0;			\
//		v.Vb = 0;			\
//		v.Vc = 0;			\
//		v.d1 = 0;			\
//		v.d2 = 0;			\
//		v.d0 = 0;			\
//		v.temp_sv1 =0;		\
//		v.temp_sv2 =0;		\
//		v.inver_Udc = 0;
//
//#endif /* SVM_H_ */

//Code moi

#ifndef SVM_H_
#define SVM_H_

typedef struct  { float  Ualpha;            // Input: reference alpha-axis phase voldage
                  float  Ubeta;         // Input: reference beda-axis phase voldage
                  float  Udc;               // Input: reference Udc voldage
                  float  da;                // Output: reference phase-a swidching function
                  float  db;                // Output: reference phase-b swidching function
                  float  dc;                // Output: reference phase-c swidching function
                  float  dn;                // Output: reference phase-n swidching function
                  float  Va;
                  float  Vb;
                  float  Vc;
                  float  T1;
                  float  T2;
                  float  T0;
                  float  temp_sv1;
                  float  temp_sv2;
                  float  inver_Udc;
                  Uint16 Sector;
                } SVGENDQ_CLA;

#define SVGEN_CLA_MACRO(v)                                                                  \
    v.Sector = 0;                                                                           \
    v.temp_sv1=0.5*v.Ualpha;                                                                \
    v.temp_sv2=0.8660254037844386*v.Ubeta;   /* 0.8660254037844386 = sqrt(3)/2*/            \
    /* Inverse clarke transformation - ma tran clarke*/                                     \
    v.Va = v.Ualpha;                                                                        \
    v.Vb = -v.temp_sv1 + v.temp_sv2;                                                        \
    v.Vc = -v.temp_sv1 - v.temp_sv2;                                                        \
    /* 60 degree Sector determination - xac dinh sector*/                                   \
    /* SAI ngay ngat dau tien da chon 2 phai chon 1*/                                       \
    if(v.Va >= v.Vb)                                                                        \
        {                                                                                   \
            if(v.Vb >= v.Vc) v.Sector = 1;                                                  \
            else if(v.Vc >= v.Va) v.Sector = 5;                                             \
                else v.Sector = 6;                                                          \
        }                                                                                   \
    else if(v.Vc > v.Vb) v.Sector = 4;                                                      \
        else if (v.Va > v.Vc) v.Sector = 2;                                                 \
            else v.Sector = 3;                                                              \
                                                                                            \
    v.inver_Udc = (Us_max*1.0/Udc_max)*(1.0/v.Udc);                                         \
    /*  v.Udc gia tri NGHICH DAO da dc chuan hoa pu ??? */                                  \
    /* 1.732050807568877 = sqrt(3)*/                                                        \
    /* 0.8660254038 = sqrt(3)/2*/                                                           \
                                                                                            \
switch(v.Sector)                                                                            \
{                                                                                           \
    case 0:                                                                                 \
    /*Tinh toan thoi gian thuc hien cac vector chuan*/                                      \
    /*Tinh toan thoi gian dong cat - calcalate upper*/                                      \
    v.da = 0.0;                                                                             \
    v.db = 0.0;                                                                             \
    v.dc = 0.0;                                                                             \
    break;                                                                                  \
    case 1:   /*Sector 1: T1=Z and T2=Y (abc ---> db,da,dc)*/                               \
    /*Tinh toan thoi gian thuc hien cac vector chuan*/                                      \
    v.T1 = (1.50*v.Ualpha + (-0.8660254038)*v.Ubeta)*v.inver_Udc;                           \
    v.T2 = (1.732050807568877*v.Ubeta)*v.inver_Udc;                                         \
    v.T0 =  1.0 - v.T1- v.T2;                                                               \
    /*Tinh toan thoi gian dong cat - calcalate upper*/                                      \
    v.da =  v.T0/2.0 + v.T1 + v.T2;                                                         \
    v.db =  v.T0/2.0 + v.T2;                                                                \
    v.dc =  v.T0/2.0;                                                                       \
    v.dn = (v.da + v.db +v.dc)/3.0;                                                         \
    break;                                                                                  \
    case 2:   /* Sector 2: T1=Y and T2=-X (abc ---> da,dc,db)*/                             \
    /*Tinh toan thoi gian thuc hien cac vector chuan*/                                      \
    v.T1 = (-1.50*v.Ualpha + (0.8660254038)*v.Ubeta)*v.inver_Udc;                           \
    v.T2 = (1.50*v.Ualpha + (0.8660254038)*v.Ubeta)*v.inver_Udc;                            \
    v.T0 =  1.0 - v.T1 - v.T2;                                                              \
    /*Tinh toan thoi gian dong cat - calcalate upper*/                                      \
    v.da =  v.T0/2.0 + v.T2;                                                                \
    v.db =  v.T0/2.0 + v.T2 + v.T1;                                                         \
    v.dc =  v.T0/2.0;                                                                       \
    v.dn = (v.da + v.db +v.dc)/3.0;                                                         \
    break;                                                                                  \
    case 3:   /* Sector 3: T1=-Z and T2=X (abc ---> da,db,dc)*/                             \
    /*Tinh toan thoi gian thuc hien cac vector chuan*/                                      \
    v.T1 = (1.732050807568877*v.Ubeta)*v.inver_Udc;                                         \
    v.T2 = (-1.50*v.Ualpha + (-0.8660254038)*v.Ubeta)*v.inver_Udc;                          \
    v.T0 =  1.0 - v.T1 - v.T2;                                                              \
    /*Tinh toan thoi gian dong cat - calcalate upper*/                                      \
    v.da =  v.T0/2.0;                                                                       \
    v.db =  v.T0/2.0 + v.T1 + v.T2;                                                         \
    v.dc =  v.T0/2.0 + v.T2;                                                                \
    v.dn = (v.da + v.db +v.dc)/3.0;                                                         \
    break;                                                                                  \
    case 4:   /* Sector 4: T1=-X and T2=Z (abc ---> dc,db,da)*/                             \
    v.T1 = (-1.732050807568877*v.Ubeta)*v.inver_Udc;                                        \
    v.T2 = (-1.50*v.Ualpha + (0.8660254038)*v.Ubeta)*v.inver_Udc;                           \
    v.T0 =  1.0 - v.T1 - v.T2;                                                              \
    /*Tinh toan thoi gian dong cat - calcalate upper*/                                      \
    v.da = v.T0/2.0;                                                                        \
    v.db = v.T0/2.0 + v.T2;                                                                 \
    v.dc = v.T0/2.0 + v.T2 + v.T1;                                                          \
    v.dn = (v.da + v.db +v.dc)/3.0;                                                         \
    break;                                                                                  \
    case 5:   /* Sector 5: T1=X and T2=-Y (abc ---> db,dc,da)*/                             \
    /*Tinh toan thoi gian thuc hien cac vector chuan*/                                      \
    v.T1 = (-1.50*v.Ualpha + (-0.8660254038)*v.Ubeta)*v.inver_Udc;                          \
    v.T2 = (1.50*v.Ualpha + (-0.8660254038)*v.Ubeta)*v.inver_Udc;                           \
    v.T0 =  1.0 - v.T1 - v.T2;                                                              \
    /*Tinh toan thoi gian dong cat - calcalate upper*/                                      \
    v.da = v.T0/2.0 + v.T2;                                                                 \
    v.db = v.T0/2.0;                                                                        \
    v.dc = v.T0/2.0 + v.T2 + v.T1;                                                          \
    v.dn = (v.da + v.db +v.dc)/3.0;                                                         \
    break;                                                                                  \
    case 6:   /* Sector 6: T1=-Y and T2=-Z (abc ---> dc,da,db)*/                            \
    /*Tinh toan thoi gian thuc hien cac vector chuan*/                                      \
    v.T1 = (1.50*v.Ualpha + (0.866025403)*v.Ubeta)*v.inver_Udc;                             \
    v.T2 = (-1.732050807568877*v.Ubeta)*v.inver_Udc;                                        \
    v.T0 =  1.0 - v.T1 - v.T2;                                                              \
    /*Tinh toan thoi gian dong cat - calcalate upper*/                                      \
    v.da = v.T0/2.0 + v.T1 + v.T2;                                                          \
    v.db = v.T0/2.0;                                                                        \
    v.dc = v.T0/2.0 + v.T2;                                                                 \
    v.dn = (v.da + v.db +v.dc)/3.0;                                                         \
    break;                                                                                  \
}                                                                                           \
/* Limit v.da,v.db,v.dc 0:1*/                                                               \
if (v.da<0.0) v.da = 0.0; else if(v.da>0.95) v.da = 0.98;                                   \
if (v.db<0.0) v.db = 0.0; else if(v.db>0.95) v.db = 0.98;                                   \
if (v.dc<0.0) v.dc = 0.0; else if(v.dc>0.95) v.dc = 0.98;

#define SVGEN_CLA_INT(v)    \
        v.Ualpha = 0;       \
        v.Ubeta = 0;        \
        v.Udc = 0;          \
        v.da = 0;           \
        v.db = 0;           \
        v.dn = 0;           \
        v.dc = 0;           \
        v.Va = 0;           \
        v.Vb = 0;           \
        v.Vc = 0;           \
        v.T1 = 0;           \
        v.T2 = 0;           \
        v.T0 = 0;           \
        v.temp_sv1 =0;      \
        v.temp_sv2 =0;      \
        v.inver_Udc = 0;

#endif /* SVM_H_ */
