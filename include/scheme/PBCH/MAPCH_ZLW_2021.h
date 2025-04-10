#ifndef MAPCH_ZLW_2021_H
#define MAPCH_ZLW_2021_H

#include "../../utils/Hash.h"
#include "../../base/PbcScheme.h"
#include "../../base/GmpElements.h"
#include "../../utils/TypeConverter.h"

#include "../../ABE/MA_ABE.h"
#include "../CH/CH_ET_BC_CDK_2017.h"

class MAPCH_ZLW_2021_pp{
    private:
        CH_ET_BC_CDK_2017_pp pp_CH;
        MA_ABE_gpk gpk_MA_ABE;
    public:
        CH_ET_BC_CDK_2017_pp &get_pp_CH(){
            return this->pp_CH;
        }
        MA_ABE_gpk &get_gpk_MA_ABE(){
            return gpk_MA_ABE;
        }
};

class MAPCH_ZLW_2021_mhk{
    private:
        CH_ET_BC_CDK_2017_pk hk;
        MA_ABE_pkTheta pkj;
        MA_ABE_gpk gpk_ABE;
    public:
        CH_ET_BC_CDK_2017_pk &get_hk(){
            return this->hk;
        }
        MA_ABE_pkTheta &get_pkj(){
            return this->pkj;
        }
        MA_ABE_gpk &get_gpk_ABE(){
            return this->gpk_ABE;
        }
};
class MAPCH_ZLW_2021_mtk{
    private:
        CH_ET_BC_CDK_2017_sk tk;
        MA_ABE_skTheta skj;
    public:
        CH_ET_BC_CDK_2017_sk &get_tk(){
            return this->tk;
        }
        MA_ABE_skTheta &get_skj(){
            return this->skj;
        }
};

class MAPCH_ZLW_2021_mski{
    private:
        CH_ET_BC_CDK_2017_sk tk;
        MA_ABE_skgidA KiGid;
    public:
        CH_ET_BC_CDK_2017_sk &get_tk(){
            return this->tk;
        }
        MA_ABE_skgidA &get_KiGid(){
            return this->KiGid;
        }
};

class MAPCH_ZLW_2021_h{
    private:
        CH_ET_BC_CDK_2017_h h;
    public:
        CH_ET_BC_CDK_2017_h &get_h(){
            return this->h;
        }
};

class MAPCH_ZLW_2021_r{
    private:
        CH_ET_BC_CDK_2017_r r;
        MA_ABE_ciphertext c;
    public:
        CH_ET_BC_CDK_2017_r &get_r(){
            return this->r;
        }
        MA_ABE_ciphertext &get_c(){
            return this->c;
        }
};

class MAPCH_ZLW_2021: public PbcScheme{
    protected:
        MA_ABE ma_abe;
        CH_ET_BC_CDK_2017 ch_et;

        element_t tmp_G, tmp_GT, tmp_Zn, tmp_Zn_2;

    public:
        MAPCH_ZLW_2021(int curve);

        void GlobalSetup(CH_ET_BC_CDK_2017_pk &pkCH, CH_ET_BC_CDK_2017_sk &skCH, MAPCH_ZLW_2021_pp &pp, int k);

        void AuthSetUp(MAPCH_ZLW_2021_mhk &mhk, MAPCH_ZLW_2021_mtk &mtk, std::string A, CH_ET_BC_CDK_2017_pk &pkCH, CH_ET_BC_CDK_2017_sk &skCH, MAPCH_ZLW_2021_pp &pp);

        void KeyGen(MAPCH_ZLW_2021_mski &msk, MAPCH_ZLW_2021_mtk &mtk, MAPCH_ZLW_2021_mhk &mhk, std::string A, std::string GID);

        void Hash(MAPCH_ZLW_2021_h &h, MAPCH_ZLW_2021_r &r, std::string m, MAPCH_ZLW_2021_pp &pp, std::vector<MAPCH_ZLW_2021_mhk *> &mhks, Element_t_matrix *MSP, std::string policy);

        bool Check(MAPCH_ZLW_2021_h &h, MAPCH_ZLW_2021_r &r, std::string m, std::vector<MAPCH_ZLW_2021_mhk *> &mhks);

        void Adapt(MAPCH_ZLW_2021_r &r_p, std::string m_p, MAPCH_ZLW_2021_h &h, MAPCH_ZLW_2021_r &r, std::string m, std::vector<MAPCH_ZLW_2021_mhk *> &mhks, std::vector<MAPCH_ZLW_2021_mski *> &msks, Element_t_matrix *MSP);

        bool Verify(MAPCH_ZLW_2021_h &h, MAPCH_ZLW_2021_r &r, std::string m, std::vector<MAPCH_ZLW_2021_mhk *> &mhks);

        Element_t_matrix* ComputeMSP(const std::string &policy_str);
        
        ~MAPCH_ZLW_2021();
};


#endif //MAPCH_ZLW_2021_H