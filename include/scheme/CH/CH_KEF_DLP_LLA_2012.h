#ifndef CH_KEF_DLP_LLA_2012_H
#define CH_KEF_DLP_LLA_2012_H

#include <stdexcept>
#include "../../base/PbcElements.h"
#include "../../base/PbcScheme.h"
#include "../../utils/Hash.h"

class CH_KEF_DLP_LLA_2012_pp : public PbcElements{
    public:
        CH_KEF_DLP_LLA_2012_pp(){
            init(1);
        }
};
class CH_KEF_DLP_LLA_2012_pk : public PbcElements{
    public:
        CH_KEF_DLP_LLA_2012_pk(){
            init(1);
        }
};
class CH_KEF_DLP_LLA_2012_sk : public PbcElements{
    public:
        CH_KEF_DLP_LLA_2012_sk(){
            init(3);
        }
};
class CH_KEF_DLP_LLA_2012_label : public PbcElements{
    public:
        CH_KEF_DLP_LLA_2012_label(){
            init(2);
        }
};

class CH_KEF_DLP_LLA_2012: public PbcScheme{
    private:
        element_t y;
        element_t y1;
        element_t t;

        element_t tmp_G, tmp_G_2, tmp_Zn, tmp_Zn_2, tmp_Zn_3;

        void H1(element_t res, element_t m1, element_t m2, element_t m3);
        void H2(element_t res, element_t m);
   

    public:
        CH_KEF_DLP_LLA_2012(int curve, int group);

        void LabelManager(CH_KEF_DLP_LLA_2012_label &label, element_t y1, element_t w1, element_t t);

        void SetUp(CH_KEF_DLP_LLA_2012_pp &pp);

        void KeyGen(CH_KEF_DLP_LLA_2012_sk &sk, CH_KEF_DLP_LLA_2012_pk &pk, CH_KEF_DLP_LLA_2012_label &label, CH_KEF_DLP_LLA_2012_pp &pp);

        void Hash(element_t h, element_t r, CH_KEF_DLP_LLA_2012_pk &pk, element_t m, CH_KEF_DLP_LLA_2012_label &label, CH_KEF_DLP_LLA_2012_pp &pp);

        bool Check(element_t m, element_t r, CH_KEF_DLP_LLA_2012_pk &pk, CH_KEF_DLP_LLA_2012_label &label, element_t h, CH_KEF_DLP_LLA_2012_pp &pp);

        void UForge(element_t r_p, CH_KEF_DLP_LLA_2012_sk &sk, CH_KEF_DLP_LLA_2012_pk &pk, CH_KEF_DLP_LLA_2012_label &label, element_t h, element_t m, element_t m_p, element_t r, CH_KEF_DLP_LLA_2012_pp &pp);
        void IForge(element_t r_pp, CH_KEF_DLP_LLA_2012_label &label, element_t m, element_t m_p, element_t r, element_t r_p, element_t m_pp);

        bool Verify(element_t m_p, element_t r_p, CH_KEF_DLP_LLA_2012_pk &pk, CH_KEF_DLP_LLA_2012_label &label, element_t h, CH_KEF_DLP_LLA_2012_pp &pp);

        ~CH_KEF_DLP_LLA_2012();

        enum{
            g
        };

        enum {
            alpha,
            x1,
            x2
        };

        enum {
            y2
        };

        enum {
            L,
            R
        };
};


#endif //CH_KEF_DLP_LLA_2012_H