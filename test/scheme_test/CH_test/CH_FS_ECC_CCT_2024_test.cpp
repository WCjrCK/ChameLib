#include "scheme/CH/CH_FS_ECC_CCT_2024.h"
#include "CommonTest.h"

struct TestParams{
	int curve;
    int group;
};

class CH_FS_ECC_CCT_2024_Test : public BaseTest<TestParams>{
    protected:
        void SetUp() override {
            BaseTest::SetUp();

            std::string testName = ::testing::UnitTest::GetInstance()->current_test_info()->name();
            std::string curveName = Curve::curve_names[GetParam().curve];
            std::string groupName;
            if (GetParam().group == Group::G1) {
                groupName = "G1";
            } else if (GetParam().group == Group::G2) {
                groupName = "G2";
            } else if (GetParam().group == Group::GT) {
                groupName = "GT";
            } else {
                groupName = "UNKNOWN";
            }
            fprintf(out, "%s %s %s\n", testName.c_str(), curveName.c_str(), groupName.c_str());
            if(visiable)printf("%s %s %s\n", testName.c_str(), curveName.c_str(), groupName.c_str());
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

    int groups[] = {Group::G1, Group::G2, Group::GT};

    std::vector<TestParams> test_params;

    for (int curve : curves) {
        for (int group : groups) {
            test_params.push_back({curve, group});
        }
    }

    return test_params;
}

const std::vector<TestParams> test_values = generateTestParams();

INSTANTIATE_TEST_CASE_P(
	CH_Test,
    CH_FS_ECC_CCT_2024_Test,
	testing::ValuesIn(test_values)
);

int op_cnt_G1G2[][diff_max_len] = {
    {
        1, 0, 0, 0, 
        0, 0, 0, 0, 
        0, 0, 0, 0, 
        0, 0, 0, 0, 
        0
    }, //0, setup

    {
        0, 0, 0, 1, 
        0, 0, 0, 0, 
        0, 0, 0, 0, 
        1, 0, 0, 0, 
        0
    }, //1, keygen
    
    {
        0, 0, 0, 3, 
        1, 0, 0, 2, 
        2, 0, 0, 1, 
        4, 0, 0, 0, 
        0
    }, //2, hash

    {
        0, 0, 0, 0, 
        1, 0, 0, 2, 
        3, 0, 0, 0, 
        4, 0, 0, 0, 
        0
    }, //3, check

    {
        0, 0, 0, 2, 
        2, 0, 0, 4, 
        5, 0, 0, 1, 
        7, 0, 0, 0, 
        0
    }, //4, adapt
};

int op_cnt_GT[][diff_max_len] = {
    {
        0, 0, 1, 0, 
        0, 0, 0, 0, 
        0, 0, 0, 0, 
        0, 0, 0, 0, 
        0
    }, //0, setup

    {
        0, 0, 0, 1, 
        0, 0, 0, 0, 
        0, 0, 0, 0, 
        0, 0, 1, 0, 
        0
    }, //1, keygen
    
    {
        0, 0, 0, 3, 
        0, 0, 1, 2, 
        0, 0, 2, 1, 
        0, 0, 4, 0, 
        0
    }, //2, hash

    {
        0, 0, 0, 0, 
        0, 0, 1, 2, 
        0, 0, 3, 0, 
        0, 0, 4, 0, 
        0
    }, //3, check

    {
        0, 0, 0, 2, 
        0, 0, 2, 4, 
        0, 0, 5, 1, 
        0, 0, 7, 0, 
        0
    }, //4, adapt
};
TEST_P(CH_FS_ECC_CCT_2024_Test, Test){
    for(int i = 0; UpdateProcBar(i, repeat), i < repeat; i++){

        CH_FS_ECC_CCT_2024 ch(GetParam().curve, GetParam().group);

        CH_FS_ECC_CCT_2024_pp pp;
        CH_FS_ECC_CCT_2024_pk pk;
        CH_FS_ECC_CCT_2024_sk sk;
        CH_FS_ECC_CCT_2024_r r,r_p;
        
        element_s* m = ch.GetZrElement();
        element_s* m_p = ch.GetZrElement();

        element_s* h = ch.GetG1Element();

        this->start("SetUp");
        ch.SetUp(pp);
        this->end("SetUp");

        this->start("KeyGen");
        ch.KeyGen(pk, sk, pp);
        this->end("KeyGen");

        this->start("Hash");
        ch.Hash(h, r, pk, m, pp);
        this->end("Hash");
        if(visiable){
            r.print(CH_FS_ECC_CCT_2024::c1);
            r.print(CH_FS_ECC_CCT_2024::z1);
            r.print(CH_FS_ECC_CCT_2024::z2);
            Logger::PrintPbc("h", h);
        }

        this->start("Check");
        bool check_result = ch.Check(pk, m, h, r, pp);
        this->end("Check");
        ASSERT_TRUE(check_result);

        this->start("Adapt");
        ch.Adapt(r_p, pk, sk, m, m_p, h, r, pp);
        this->end("Adapt");

        this->start("Verify");
        bool verify_result = ch.Verify(pk, m_p, h, r_p, pp);
        this->end("Verify");
        ASSERT_TRUE(verify_result);
    }

    average();

    if(GetParam().group == Group::G1 || GetParam().group == Group::G2){
        EXPECT_TRUE(check_time(GetParam().curve, op_cnt_G1G2[0], "SetUp"));
        EXPECT_TRUE(check_time(GetParam().curve, op_cnt_G1G2[1], "KeyGen"));
        EXPECT_TRUE(check_time(GetParam().curve, op_cnt_G1G2[2], "Hash"));
        EXPECT_TRUE(check_time(GetParam().curve, op_cnt_G1G2[3], "Check"));
        EXPECT_TRUE(check_time(GetParam().curve, op_cnt_G1G2[4], "Adapt"));
        EXPECT_TRUE(check_time(GetParam().curve, op_cnt_G1G2[3], "Verify"));
    } else if(GetParam().group == Group::GT){
        EXPECT_TRUE(check_time(GetParam().curve, op_cnt_GT[0], "SetUp"));
        EXPECT_TRUE(check_time(GetParam().curve, op_cnt_GT[1], "KeyGen"));
        EXPECT_TRUE(check_time(GetParam().curve, op_cnt_GT[2], "Hash"));
        EXPECT_TRUE(check_time(GetParam().curve, op_cnt_GT[3], "Check"));
        EXPECT_TRUE(check_time(GetParam().curve, op_cnt_GT[4], "Adapt"));
        EXPECT_TRUE(check_time(GetParam().curve, op_cnt_GT[3], "Verify"));
    }
}

int main(int argc, char **argv) 
{
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}




