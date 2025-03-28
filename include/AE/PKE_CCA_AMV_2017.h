#ifndef CHAMELIB_PKE_CCA_AMV_2017_H
#define CHAMELIB_PKE_CCA_AMV_2017_H

#include "../base/PbcScheme.h"
#include "../base/PbcElements.h"
#include "../utils/Hash.h"
#include "../utils/TypeConverter.h"


class PKE_CCA_AMV_2017_pk:public PbcElements{};

class PKE_CCA_AMV_2017_sk:public PbcElements{};

class PKE_CCA_AMV_2017_c:public PbcElements{};

class PKE_CCA_AMV_2017: public PbcScheme{
    private:
        element_t tmp_G, tmp_G_2, tmp_G_3, tmp_G_4, tmp_Zn, tmp_Zn_2;

    public:
        PKE_CCA_AMV_2017(){}
        PKE_CCA_AMV_2017(int curve, int group);

        void init(element_t _G, element_t _Zn);

        void Setup(PKE_CCA_AMV_2017_pk &pk, PKE_CCA_AMV_2017_sk &sk, PKE_CCA_AMV_2017_c &c);

        void KeyGen(PKE_CCA_AMV_2017_pk &pk, PKE_CCA_AMV_2017_sk &sk);

        void Encrypt(PKE_CCA_AMV_2017_c &c, element_t m, PKE_CCA_AMV_2017_pk &pk);

        void Decrypt(element_t m, PKE_CCA_AMV_2017_c &c, PKE_CCA_AMV_2017_sk &sk);

        ~PKE_CCA_AMV_2017();

        enum {
            g1,
            g2,
            y1,
            y2,
            y3
        };

        enum {
            x11,
            x12,
            x13,
            x21,
            x22,
            x23
        };

        enum {
            rho,
            c1,
            c2,
            c3,
            c3_,
            c4
        };
};

#endif  // CHAMELIB_PKE_CCA_AMV_2017_H