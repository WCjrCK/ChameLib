#include "scheme/PBCH/RPCH_XNM_2021.h"
#include "CommonTest.h"

struct TestParams{
	int curve;
    bool swap;
    int leafNodeSize;
    int k;
};

class RPCH_XNM_2021_Test : public BaseTest<TestParams>{
    protected:
        void SetUp() override {
            BaseTest::SetUp();

            std::string testName = ::testing::UnitTest::GetInstance()->current_test_info()->name();
            std::string curveName = Curve::curve_names[GetParam().curve];
            fprintf(out, "%s %s swap: %d leafNodeSize: %d k: %d\n", testName.c_str(), curveName.c_str(), GetParam().swap, GetParam().leafNodeSize, GetParam().k);
            if(visiable)printf("%s %s swap: %d leafNodeSize: %d k: %d\n", testName.c_str(), curveName.c_str(), GetParam().swap, GetParam().leafNodeSize, GetParam().k);
        }
};

std::vector<TestParams> generateTestParams() {
    int curves[] = {
        Curve::A,
        Curve::A1,
        Curve::D_159, Curve::D_201, Curve::D_224, Curve::D_105171_196_185, Curve::D_277699_175_167, Curve::D_278027_190_181,
        Curve::E,
        Curve::F, Curve::SM9,
        Curve::G_149
    };

    bool swaps[] = {false, true};

    int leadNodeSizes[] = {8, 16, 32, 64};

    int ks[] = {128, 256, 512};

    std::vector<TestParams> test_params;

    for (int curve : curves) {
        for (bool swap : swaps) {
            for (int leafNodeSize : leadNodeSizes) {
                for (int k : ks) {
                    test_params.push_back({curve, swap, leafNodeSize, k});
                }
            }
        }
    }

    return test_params;
}

const std::vector<TestParams> test_values = generateTestParams();

INSTANTIATE_TEST_CASE_P(
	PBCH_Test,
    RPCH_XNM_2021_Test,
	testing::ValuesIn(test_values)
);

TEST_P(RPCH_XNM_2021_Test, Test){
    for(int i = 0; UpdateProcBar(i, repeat), i < repeat; i++){

        RPCH_XNM_2021 ch(GetParam().curve, GetParam().swap);

        std::vector<std::string> attr_list = {"ONE","TWO","THREE"};
        const int SIZE_OF_ATTR = attr_list.size();  // S
        const std::string POLICY = "(ONE&THREE)&(TWO|FOUR)";
        const int SIZE_OF_POLICY = 4;
        // compute policy matrix
        Policy_resolution pr;
        Policy_generation pg;
        std::vector<std::string>* postfix_expression = pr.infixToPostfix(POLICY);
        if(visiable){
            printf("postfix_expression of Policy: ");
            for(int i = 0;i < postfix_expression->size();i++){
                printf("%s ", postfix_expression->at(i).c_str());
            }
            printf("\n");
        }
        Binary_tree_policy* binary_tree_expression = pr.postfixToBinaryTree(postfix_expression, ch.GetZrElement());
        pg.generatePolicyInMatrixForm(binary_tree_expression);
        Element_t_matrix* MSP = pg.getPolicyInMatrixFormFromTree(binary_tree_expression);
        if(visiable){
            printf("Policy Matrix:\n");
            MSP->printMatrix();
        }

        element_s *id_1 = ch.GetZrElement();
        element_s *id_2 = ch.GetZrElement();
        element_s *id_3 = ch.GetZrElement();
        
        // T1 < T3 < T_present < T2
        const time_t T_present = TimeUtils::TimeCast(2025, 2, 1, 0, 0, 0);  // present time
        const time_t re_time_1 = TimeUtils::TimeCast(2025, 1, 1, 0, 0, 0);
        const time_t re_time_2 = TimeUtils::TimeCast(2025, 2, 31, 0, 0, 0);
        const time_t re_time_3 = TimeUtils::TimeCast(2025, 1, 2, 0, 0, 0);

        RPCH_XNM_2021_pp ppRPCH;
        RPCH_XNM_2021_sk skRPCH;
        RPCH_XNM_2021_pk pkRPCH;

        RPCH_XNM_2021_skid skidRPCH_1;
        RPCH_XNM_2021_skid skidRPCH_2;
        RPCH_XNM_2021_skid skidRPCH_3;

        RPCH_XNM_2021_dkidt dkidtRPCH_1;
        RPCH_XNM_2021_dkidt dkidtRPCH_2;
        RPCH_XNM_2021_dkidt dkidtRPCH_3;

        RPCH_XNM_2022_kut kut;
        RPCH_XNM_2021_h h;
        RPCH_XNM_2021_r r,r_p;

        RPCH_XNM_2021_RevokedPresonList rl;
        RPCH_XNM_2022_Binary_tree st;
        
        std::string m = "message to hash";
        std::string m_p = "message to adapt";

        this->start("SetUp");
        ch.SetUp(ppRPCH, skRPCH, pkRPCH, rl, st, GetParam().k, GetParam().leafNodeSize);
        this->end("SetUp");

        this->start("KeyGen");
        ch.KeyGen(skidRPCH_1, pkRPCH, skRPCH, st, attr_list, id_1, re_time_1);
        this->end("KeyGen");
        ch.KeyGen(skidRPCH_2, pkRPCH, skRPCH, st, attr_list, id_2, re_time_2);
        ch.KeyGen(skidRPCH_3, pkRPCH, skRPCH, st, attr_list, id_3, re_time_3);

        this->start("Rev");
        ch.Rev(rl, id_1, re_time_1);
        this->end("Rev");
        ch.Rev(rl, id_2, re_time_2);
        ch.Rev(rl, id_3, re_time_3);

        this->start("Hash");
        ch.Hash(h, r, m, pkRPCH, MSP, T_present, ppRPCH);
        this->end("Hash");
        if(visiable){    
            h.get_h().print();
            r.get_rCHET().print();
        }

        this->start("Check");
        bool check = ch.Check(h, r, m, pkRPCH);
        this->end("Check");
        ASSERT_TRUE(check);

        this->start("KUpt");
        ch.KUpt(kut, pkRPCH, st, rl, T_present);
        this->end("KUpt");
        if(visiable){
            printf("size of kut.ku_theta: %ld\n", kut.get_kut().get_ku_theta().size());
        }
    
        try{
            ch.DKGen(dkidtRPCH_1, pkRPCH, skidRPCH_1, kut);
        }catch(const std::runtime_error& e){
            if(visiable) printf("%s\n", e.what());
        }
        try{
            ch.DKGen(dkidtRPCH_3, pkRPCH, skidRPCH_3, kut);
        }catch(const std::runtime_error& e){
            if(visiable) printf("%s\n", e.what());
        }
        this->start("DKGen");
        ch.DKGen(dkidtRPCH_2, pkRPCH, skidRPCH_2, kut);
        this->end("DKGen");

        this->start("Adapt");
        ch.Adapt(r_p, m_p, h, r, m, pkRPCH, dkidtRPCH_2, MSP);
        this->end("Adapt");
        if(visiable){
            r_p.get_rCHET().print();
        }

        this->start("Verify");
        bool verify = ch.Verify(h, r_p, m_p, pkRPCH);
        this->end("Verify");
        ASSERT_TRUE(verify);
    }
    average();
}

int main(int argc, char **argv) 
{
    ::testing::GTEST_FLAG(catch_exceptions) = false;
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}