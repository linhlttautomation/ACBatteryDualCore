#ifndef MPPT_PNO_D_CLA_H_
#define MPPT_PNO_D_CLA_H_

//*********** Structure Definition ********//
typedef struct {
    float32  Ipv;
    float32  Vpv;
    float32  VpvOld;
    float32  DeltaPmin;
    float32  DeltaVmin;
    float32  MaxDuty;
    float32  MinDuty;
    float32  DutyStart;
    float32  DutyOut;
    float32  Stepsize;
    float32  DeltaP;
    float32  DeltaV;
    float32  PanelPower;
    float32  PanelPower_Prev;
    int mppt_first;
    int mppt_enable;
} MPPT_PNO_D_CLA;

typedef MPPT_PNO_D_CLA *mppt_pno_d_handle;

//*********** Function Declarations *******//
void MPPT_PNO_D_CLA_FUNC(MPPT_PNO_D_CLA *v);

//*********** Structure Init Macro *******//
#define MPPT_PNO_D_CLA_init(v)  \
    v.Ipv=0;                    \
    v.Vpv=0;                    \
    v.VpvOld=0;                 \
    v.DeltaPmin=(0.00001);      \
    v.DeltaVmin=(0.00001);      \
    v.MaxDuty=(0.9);            \
    v.MinDuty=0;                \
    v.Stepsize=(0.002);         \
    v.DutyStart;                \
    v.DutyOut=0;                \
    v.DeltaP=0;                 \
    v.DeltaV=0;                 \
    v.PanelPower=0;             \
    v.PanelPower_Prev=0;        \
    v.mppt_enable=1;            \
    v.mppt_first=1;             \

//*********** Macro Definition ***********//
#define MPPT_PNO_D_CLA_MACRO(v)                     \
if (v.mppt_enable==1)                               \
{                                                   \
    if (v.mppt_first == 1)                          \
    {                                               \
        v.DutyOut= v.DutyStart;                     \
        v.mppt_first=0;                             \
        v.PanelPower_Prev=v.PanelPower;             \
    }                                               \
    else                                            \
    {                                               \
        v.PanelPower= (v.Vpv*v.Ipv);                \
        v.DeltaP=v.PanelPower-v.PanelPower_Prev;    \
        if (v.DeltaP > v.DeltaPmin)                 \
        {                                           \
            v.DutyOut=v.DutyOut+v.Stepsize;         \
        }                                           \
        else                                        \
        {                                           \
            if (v.DeltaP < -v.DeltaPmin)            \
            {                                       \
                v.Stepsize=-v.Stepsize;             \
                v.DutyOut=v.DutyOut+v.Stepsize;     \
            }                                       \
        }                                           \
        v.PanelPower_Prev = v.PanelPower;           \
    }                                               \
    if(v.DutyOut< v.MinDuty) v.DutyOut = v.MinDuty; \
    if(v.DutyOut> v.MaxDuty) v.DutyOut= v.MaxDuty;  \
}

//*********** Macro Definition ***********//
#define MPPT_PNO_D_CLA_MACRO_V2(v)                          \
if (v.mppt_enable==1)                                       \
{                                                           \
    if (v.mppt_first == 1)                                  \
    {                                                       \
        v.DutyOut = v.DutyStart;                            \
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
                v.DutyOut=v.DutyOut + v.Stepsize;           \
            }                                               \
            else                                            \
            {                                               \
                if(v.DeltaV < -v.DeltaVmin)                 \
                {                                           \
                    v.DutyOut=v.DutyOut - v.Stepsize;       \
                }                                           \
            }                                               \
        }                                                   \
        else                                                \
        {                                                   \
            if (v.DeltaP < -v.DeltaPmin)                    \
            {                                               \
                if(v.DeltaV > v.DeltaVmin)                  \
                {                                           \
                    v.DutyOut= v.DutyOut - v.Stepsize;      \
                }                                           \
                else                                        \
                {                                           \
                    if(v.DeltaV < -v.DeltaVmin)             \
                    {                                       \
                        v.DutyOut=v.DutyOut + v.Stepsize;   \
                    }                                       \
                }                                           \
            }                                               \
        }                                                   \
        v.PanelPower_Prev = v.PanelPower;                   \
        v.VpvOld = v.Vpv;                                   \
    }                                                       \
    if(v.DutyOut< v.MinDuty) v.DutyOut = v.MinDuty;         \
    if(v.DutyOut> v.MaxDuty) v.DutyOut = v.MaxDuty;         \
}


#endif /*MPPT_PNO_D_CLA_H_*/
