#include "ChameLib.h"
#include "CommonTest.h"

struct TestParams{
	int curve;
    bool swap;
};

std::ostream& operator<<(std::ostream& os, const TestParams& params) {
    return os << "curve=" << Curve::curve_names[params.curve] << " swap=" << params.swap;
}

class IB_CH_KEF_CZS_2014_Test : public BaseTest<TestParams>{
    protected:
        void SetUp() override {
            BaseTest::SetUp();

            std::string testName = ::testing::UnitTest::GetInstance()->current_test_info()->name();
            std::string curveName = Curve::curve_names[GetParam().curve];
            fprintf(out, "%s %s swap:%d\n", testName.c_str(), curveName.c_str(), GetParam().swap);
            if(visiable)printf("%s %s swap:%d\n", testName.c_str(), curveName.c_str(), GetParam().swap);
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

    std::vector<TestParams> test_params;

    for (int curve : curves) {
        for (bool swap : swaps) {
            test_params.push_back({curve, swap});
        }
    }

    return test_params;
}

const std::vector<TestParams> test_values = generateTestParams();

int op_cnt[][diff_max_len] = {
    {
        1, 0, 0, 1, 
        0, 0, 0, 0, 
        0, 0, 0, 0, 
        1, 0, 0, 0, 
        0
    }, //0, setup

    {
        0, 0, 0, 0, 
        0, 1, 0, 0, 
        0, 0, 0, 0, 
        0, 1, 0, 0, 
        0
    }, //1, Extract
    
    {
        0, 0, 0, 1, 
        1, 1, 0, 0, 
        0, 0, 0, 0, 
        3, 0, 0, 0, 
        1
    }, //2, hash

    {
        0, 0, 0, 0, 
        1, 0, 0, 0, 
        0, 0, 0, 0, 
        1, 0, 0, 0, 
        1
    }, //3, check

    {
        0, 0, 0, 0, 
        1, 0, 0, 0, 
        0, 0, 1, 0, 
        1, 0, 1, 0, 
        2
    }, //4, adapt
};

int op_cnt_swap[][diff_max_len] = {
    {
        0, 1, 0, 1, 
        0, 0, 0, 0, 
        0, 0, 0, 0, 
        0, 1, 0, 0, 
        0
    }, //0, setup

    {
        0, 0, 0, 0, 
        1, 0, 0, 0, 
        0, 0, 0, 0, 
        1, 0, 0, 0, 
        0
    }, //1, Extract
    
    {
        0, 0, 0, 1, 
        1, 1, 0, 0, 
        0, 0, 0, 0, 
        0, 3, 0, 0, 
        1
    }, //2, hash

    {
        0, 0, 0, 0, 
        0, 1, 0, 0, 
        0, 0, 0, 0, 
        0, 1, 0, 0, 
        1
    }, //3, check

    {
        0, 0, 0, 0, 
        0, 1, 0, 0, 
        0, 0, 1, 0, 
        0, 1, 1, 0, 
        2
    }, //4, adapt
};

INSTANTIATE_TEST_CASE_P(
	IBCH_Test,
    IB_CH_KEF_CZS_2014_Test,
	testing::ValuesIn(test_values)
);

TEST_P(IB_CH_KEF_CZS_2014_Test, Test){
    IB_CH_KEF_CZS_2014 ch(GetParam().curve, GetParam().swap);

    IB_CH_KEF_CZS_2014_pp pp[repeat];
    IB_CH_KEF_CZS_2014_msk msk[repeat];
    IB_CH_KEF_CZS_2014_td td[repeat];
    IB_CH_KEF_CZS_2014_h h[repeat];
    IB_CH_KEF_CZS_2014_r r[repeat], r_p[repeat];

    std::string ID = "identity string ID";
    std::string L = "customized identity L";

    element_s *m[repeat], *m_p[repeat];
    for (int i = 0; i < repeat; i++) {
        m[i] = ch.GetZrElement();
        m_p[i] = ch.GetZrElement();
    }

    this->start("SetUp");
    for (int i = 0; i < repeat; i++) ch.SetUp(pp[i], msk[i]);
    this->end("SetUp");

    this->start("Extract");
    for (int i = 0; i < repeat; i++) ch.Extract(td[i], ID, msk[i]);
    this->end("Extract");

    this->start("Hash");
    for (int i = 0; i < repeat; i++) ch.Hash(h[i], r[i], ID, L, m[i], pp[i]);
    this->end("Hash");

    bool check_result[repeat];
    this->start("Check");
    for (int i = 0; i < repeat; i++) check_result[i] = ch.Check(h[i], r[i], L, m[i], td[i]);
    this->end("Check");
    for (int i = 0; i < repeat; i++) ASSERT_TRUE(check_result[i]);

    this->start("Adapt");
    for (int i = 0; i < repeat; i++) ch.Adapt(r_p[i], m_p[i], h[i], r[i], L, m[i], td[i]);
    this->end("Adapt");

    bool verify_result[repeat];
    this->start("Verify");
    for (int i = 0; i < repeat; i++) verify_result[i] = ch.Verify(h[i], r_p[i], L, m_p[i], td[i]);
    this->end("Verify");
    for (int i = 0; i < repeat; i++) ASSERT_TRUE(verify_result[i]);

    average();

    if(!GetParam().swap){
        EXPECT_TRUE(check_time(GetParam().curve, op_cnt[0], "SetUp"));
        EXPECT_TRUE(check_time(GetParam().curve, op_cnt[1], "Extract"));
        EXPECT_TRUE(check_time(GetParam().curve, op_cnt[2], "Hash"));
        EXPECT_TRUE(check_time(GetParam().curve, op_cnt[3], "Check"));
        EXPECT_TRUE(check_time(GetParam().curve, op_cnt[4], "Adapt"));
        EXPECT_TRUE(check_time(GetParam().curve, op_cnt[3], "Verify"));
    }else{
        EXPECT_TRUE(check_time(GetParam().curve, op_cnt_swap[0], "SetUp"));
        EXPECT_TRUE(check_time(GetParam().curve, op_cnt_swap[1], "Extract"));
        EXPECT_TRUE(check_time(GetParam().curve, op_cnt_swap[2], "Hash"));
        EXPECT_TRUE(check_time(GetParam().curve, op_cnt_swap[3], "Check"));
        EXPECT_TRUE(check_time(GetParam().curve, op_cnt_swap[4], "Adapt"));
        EXPECT_TRUE(check_time(GetParam().curve, op_cnt_swap[3], "Verify"));
    }

    // free
    for (int i = 0; i < repeat; i++) {
        element_clear(m[i]);
        element_clear(m_p[i]);
        delete m[i];
        delete m_p[i];
    }
}

int main(int argc, char **argv){
    ParseCommandLineArgs(argc, argv);

	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}



