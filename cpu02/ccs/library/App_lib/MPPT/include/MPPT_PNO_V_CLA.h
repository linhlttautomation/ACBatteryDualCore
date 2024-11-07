#ifndef MPPT_PNO_V_CLA_H_
#define MPPT_PNO_V_CLA_H_

//*********** Structure Definition ********//
typedef struct {
	float32  Ipv;
	float32  Vpv;
	float32  DeltaPmin;
    float32  DeltaVmin;
	float32  MaxVolt;
	float32  MinVolt;
	float32  Stepsize;
	float32  VmppOut;
	float32  DeltaP;
    float32  DeltaV;
    float32  VpvOld;
	float32  PanelPower;
	float32  PanelPower_Prev;
	int mppt_first;
	int mppt_enable;
} MPPT_PNO_V_CLA;

typedef MPPT_PNO_V_CLA *mppt_pno_v_handle;

//*********** Function Declarations *******//
void MPPT_PNO_V_CLA_FUNC(MPPT_PNO_V_CLA *v);

//*********** Structure Init Macro *******//
#define MPPT_PNO_V_CLA_init(v)	\
	v.Ipv=0;					\
	v.Vpv=0;					\
	v.DeltaPmin=(0.00001);		\
    v.DeltaVmin=(0.00001);      \
	v.MaxVolt=(0.9);			\
	v.MinVolt=0;				\
	v.Stepsize=(0.002);			\
	v.VmppOut=0;				\
	v.DeltaP=0;					\
    v.DeltaV=0;                 \
    v.VpvOld=0;                 \
	v.PanelPower=0;				\
	v.PanelPower_Prev=0;		\
	v.mppt_enable=1;			\
	v.mppt_first=1;				\

//*********** Macro Definition ***********//
#define MPPT_PNO_V_CLA_MACRO(v)						\
if (v.mppt_enable==1)								\
{													\
	if (v.mppt_first == 1)							\
	{												\
		v.VmppOut= v.Vpv - (0.02);					\
		v.mppt_first=0;								\
		v.PanelPower_Prev=v.PanelPower;				\
	}												\
	else											\
	{												\
		v.PanelPower= (v.Vpv*v.Ipv);				\
		v.DeltaP=v.PanelPower-v.PanelPower_Prev;	\
		if (v.DeltaP > v.DeltaPmin)					\
		{											\
			v.VmppOut=v.Vpv+v.Stepsize;				\
		}											\
		else										\
		{											\
			if (v.DeltaP < -v.DeltaPmin)			\
			{										\
				v.Stepsize=-v.Stepsize;				\
				v.VmppOut=v.Vpv+v.Stepsize;			\
			}										\
		}											\
		v.PanelPower_Prev = v.PanelPower;			\
	}												\
	if(v.VmppOut< v.MinVolt) v.VmppOut = v.MinVolt;	\
	if(v.VmppOut> v.MaxVolt) v.VmppOut= v.MaxVolt;	\
}	

//*********** Macro Definition ***********//
#define MPPT_PNO_V_CLA_MACRO_V2(v)                  \
if (v.mppt_enable==1)                               \
{                                                   \
    if (v.mppt_first == 1)                          \
    {                                               \
        v.VmppOut= v.Vpv - (0.02);                  \
        v.mppt_first=0;                             \
        v.PanelPower_Prev=v.PanelPower;             \
        v.VpvOld = v.Vpv;                           \
    }                                               \
    else                                            \
    {                                               \
        v.PanelPower= (v.Vpv*v.Ipv);                \
        v.DeltaP=v.PanelPower-v.PanelPower_Prev;    \
        v.DeltaV=v.Vpv-v.VpvOld;                    \
        if (v.DeltaP > v.DeltaPmin)                 \
        {                                           \
            if(v.DeltaV > v.DeltaVmin)              \
            {                                       \
                v.VmppOut=v.Vpv + v.Stepsize;        \
            }                                       \
            else                                    \
            {                                       \
                if(v.DeltaV < -v.DeltaVmin)         \
                {                                   \
                    v.VmppOut=v.Vpv - v.Stepsize;     \
                }                                   \
            }                                       \
        }                                           \
        else                                        \
        {                                           \
            if (v.DeltaP < -v.DeltaPmin)             \
            {                                       \
                if(v.DeltaV > v.DeltaVmin)          \
                {                                   \
                    v.VmppOut=v.Vpv - v.Stepsize;     \
                }                                   \
                else                                \
                {                                   \
                    if(v.DeltaV < -v.DeltaVmin)     \
                    {                               \
                        v.VmppOut=v.Vpv + v.Stepsize; \
                    }                               \
                }                                   \
            }                                       \
        }                                           \
        v.PanelPower_Prev = v.PanelPower;           \
        v.VpvOld = v.Vpv;                           \
    }                                               \
    if(v.VmppOut< v.MinVolt) v.VmppOut = v.MinVolt; \
    if(v.VmppOut> v.MaxVolt) v.VmppOut= v.MaxVolt;  \
}

//*********** Macro Definition ***********//
#define MPPT_PNO_V_CLA_MACRO_V3(v)                          \
if (v.mppt_enable==1)                                       \
{                                                           \
    if (v.mppt_first == 1)                                  \
    {                                                       \
        v.VmppOut= v.Vpv - (0.02);                          \
        v.mppt_first=0;                                     \
        v.PanelPower_Prev=v.PanelPower;                     \
        v.VpvOld = v.Vpv;                                   \
    }                                                       \
    else                                                    \
    {                                                       \
        v.PanelPower= (v.Vpv*v.Ipv);                        \
        v.DeltaP=v.PanelPower-v.PanelPower_Prev;            \
        v.DeltaV=v.Vpv-v.VpvOld;                            \
        if (v.DeltaP > v.DeltaPmin)                         \
        {                                                   \
            if(v.DeltaV > v.DeltaVmin)                      \
            {                                               \
                v.VmppOut=v.VmppOut + v.Stepsize;           \
            }                                               \
            else                                            \
            {                                               \
                if(v.DeltaV < -v.DeltaVmin)                 \
                {                                           \
                    v.VmppOut=v.VmppOut - v.Stepsize;       \
                }                                           \
            }                                               \
        }                                                   \
        else                                                \
        {                                                   \
            if (v.DeltaP < -v.DeltaPmin)                    \
            {                                               \
                if(v.DeltaV > v.DeltaVmin)                  \
                {                                           \
                    v.VmppOut=v.VmppOut - v.Stepsize;       \
                }                                           \
                else                                        \
                {                                           \
                    if(v.DeltaV < -v.DeltaVmin)             \
                    {                                       \
                        v.VmppOut=v.VmppOut + v.Stepsize;   \
                    }                                       \
                }                                           \
            }                                               \
        }                                                   \
        v.PanelPower_Prev = v.PanelPower;                   \
        v.VpvOld = v.Vpv;                                   \
    }                                                       \
    if(v.VmppOut< v.MinVolt) v.VmppOut = v.MinVolt;         \
    if(v.VmppOut> v.MaxVolt) v.VmppOut= v.MaxVolt;          \
}

#endif /*MPPT_PNO_V_CLA_H_*/
