#define BOOST_TEST_MODULE Tidop evaluation test
#include <boost/test/unit_test.hpp>

#include <tidop/featmatch/evaluation.h>

using namespace tl;


BOOST_AUTO_TEST_SUITE(CurvesTestSuite)

struct CurvesTest
{
  CurvesTest()
  { }
    
  ~CurvesTest()
  { 
    delete mROCCurve;
    delete mDETCurve;
  }

  void setup()
  {
    data.push_back(std::make_pair(-0.037, 0));
    data.push_back(std::make_pair(0.288, 0));
    data.push_back(std::make_pair(-1.649, 0));
    data.push_back(std::make_pair(-0.074, 0));
    data.push_back(std::make_pair(0.833, 0));
    data.push_back(std::make_pair(-2.019, 0));
    data.push_back(std::make_pair(0.976, 0));
    data.push_back(std::make_pair(0.561, 0));
    data.push_back(std::make_pair(0.494, 0));
    data.push_back(std::make_pair(-1.699, 0));
    data.push_back(std::make_pair(0.981, 0));
    data.push_back(std::make_pair(0.808, 0));
    data.push_back(std::make_pair(0.044, 0));
    data.push_back(std::make_pair(-1.107, 0));
    data.push_back(std::make_pair(2.162, 0));
    data.push_back(std::make_pair(0.090, 0));
    data.push_back(std::make_pair(0.304, 0));
    data.push_back(std::make_pair(0.153, 0));
    data.push_back(std::make_pair(-0.234, 0));
    data.push_back(std::make_pair(-0.681, 0));
    data.push_back(std::make_pair(-0.332, 0));
    data.push_back(std::make_pair(-1.554, 0));
    data.push_back(std::make_pair(1.644, 0));
    data.push_back(std::make_pair(0.482, 0));
    data.push_back(std::make_pair(0.501, 0));
    data.push_back(std::make_pair(0.457, 0));
    data.push_back(std::make_pair(-0.155, 0));
    data.push_back(std::make_pair(-1.101, 0));
    data.push_back(std::make_pair(1.116, 0));
    data.push_back(std::make_pair(0.002, 0));
    data.push_back(std::make_pair(0.658, 0));
    data.push_back(std::make_pair(-1.167, 0));
    data.push_back(std::make_pair(1.277, 0));
    data.push_back(std::make_pair(1.059, 0));
    data.push_back(std::make_pair(-1.658, 0));
    data.push_back(std::make_pair(2.614, 0));
    data.push_back(std::make_pair(0.695, 0));
    data.push_back(std::make_pair(-0.272, 0));
    data.push_back(std::make_pair(-0.124, 0));
    data.push_back(std::make_pair(-0.886, 0));
    data.push_back(std::make_pair(1.769, 0));
    data.push_back(std::make_pair(0.131, 0));
    data.push_back(std::make_pair(-2.201, 0));
    data.push_back(std::make_pair(-0.740, 0));
    data.push_back(std::make_pair(1.513, 0));
    data.push_back(std::make_pair(-1.201, 0));
    data.push_back(std::make_pair(-1.687, 0));
    data.push_back(std::make_pair(-0.442, 0));
    data.push_back(std::make_pair(0.522, 0));
    data.push_back(std::make_pair(0.694, 0));
    data.push_back(std::make_pair(-0.696, 0));
    data.push_back(std::make_pair(0.796, 0));
    data.push_back(std::make_pair(-1.267, 0));
    data.push_back(std::make_pair(1.973, 0));
    data.push_back(std::make_pair(1.750, 0));
    data.push_back(std::make_pair(2.056, 0));
    data.push_back(std::make_pair(1.537, 0));
    data.push_back(std::make_pair(-1.399, 0));
    data.push_back(std::make_pair(1.302, 0));
    data.push_back(std::make_pair(0.055, 0));
    data.push_back(std::make_pair(0.656, 0));
    data.push_back(std::make_pair(-0.211, 0));
    data.push_back(std::make_pair(0.335, 0));
    data.push_back(std::make_pair(1.078, 0));
    data.push_back(std::make_pair(-0.234, 0));
    data.push_back(std::make_pair(-0.460, 0));
    data.push_back(std::make_pair(-0.680, 0));
    data.push_back(std::make_pair(-0.006, 0));
    data.push_back(std::make_pair(0.835, 1));
    data.push_back(std::make_pair(-0.334, 1));
    data.push_back(std::make_pair(1.172, 1));
    data.push_back(std::make_pair(2.962, 1));
    data.push_back(std::make_pair(1.001, 1));
    data.push_back(std::make_pair(1.402, 1));
    data.push_back(std::make_pair(0.829, 1));
    data.push_back(std::make_pair(1.593, 1));
    data.push_back(std::make_pair(2.861, 1));
    data.push_back(std::make_pair(-0.653, 1));
    data.push_back(std::make_pair(2.118, 1));
    data.push_back(std::make_pair(0.361, 1));
    data.push_back(std::make_pair(0.971, 1));
    data.push_back(std::make_pair(1.812, 1));
    data.push_back(std::make_pair(1.293, 1));
    data.push_back(std::make_pair(1.534, 1));
    data.push_back(std::make_pair(3.000, 1));
    data.push_back(std::make_pair(0.365, 1));
    data.push_back(std::make_pair(0.633, 1));
    data.push_back(std::make_pair(2.002, 1));
    data.push_back(std::make_pair(0.054, 1));
    data.push_back(std::make_pair(4.353, 1));
    data.push_back(std::make_pair(1.568, 1));
    data.push_back(std::make_pair(1.163, 1));
    data.push_back(std::make_pair(-0.777, 1));
    data.push_back(std::make_pair(0.547, 1));
    data.push_back(std::make_pair(-0.095, 1));
    data.push_back(std::make_pair(0.830, 1));
    data.push_back(std::make_pair(0.905, 1));
    data.push_back(std::make_pair(1.297, 1));
    data.push_back(std::make_pair(1.780, 1));
    data.push_back(std::make_pair(2.602, 1));
    data.push_back(std::make_pair(2.699, 1));
    data.push_back(std::make_pair(1.807, 1));
    data.push_back(std::make_pair(2.367, 1));
    data.push_back(std::make_pair(0.052, 1));
    data.push_back(std::make_pair(4.195, 1));
    data.push_back(std::make_pair(-0.659, 1));
    data.push_back(std::make_pair(3.030, 1));
    data.push_back(std::make_pair(0.850, 1));
    data.push_back(std::make_pair(2.062, 1));
    data.push_back(std::make_pair(1.366, 1));
    data.push_back(std::make_pair(4.850, 1));
    data.push_back(std::make_pair(2.779, 1));
    data.push_back(std::make_pair(2.282, 1));
    data.push_back(std::make_pair(0.586, 1));
    data.push_back(std::make_pair(0.976, 1));
    data.push_back(std::make_pair(0.501, 1));
    data.push_back(std::make_pair(0.620, 1));
    data.push_back(std::make_pair(1.287, 1));
    data.push_back(std::make_pair(1.457, 1));
    data.push_back(std::make_pair(1.374, 1));
    data.push_back(std::make_pair(1.015, 1));
    data.push_back(std::make_pair(1.116, 1));
    data.push_back(std::make_pair(0.321, 1));
    data.push_back(std::make_pair(1.784, 1));
    data.push_back(std::make_pair(4.680, 1));
    data.push_back(std::make_pair(0.747, 1));
    data.push_back(std::make_pair(0.948, 1));
    data.push_back(std::make_pair(1.508, 1));
    data.push_back(std::make_pair(0.378, 1));
    data.push_back(std::make_pair(1.250, 1));
    data.push_back(std::make_pair(0.225, 1));
    data.push_back(std::make_pair(1.373, 1));
    data.push_back(std::make_pair(-0.869, 1));
    data.push_back(std::make_pair(0.817, 1));
    data.push_back(std::make_pair(1.541, 1));
    data.push_back(std::make_pair(1.123, 1));
    data.push_back(std::make_pair(0.907, 1));
    data.push_back(std::make_pair(0.210, 1));
    data.push_back(std::make_pair(1.472, 1));
    data.push_back(std::make_pair(-0.099, 1));
    data.push_back(std::make_pair(2.951, 1));
    data.push_back(std::make_pair(1.254, 1));
    data.push_back(std::make_pair(0.789, 1));
    data.push_back(std::make_pair(0.882, 1));
    data.push_back(std::make_pair(0.554, 1));
    data.push_back(std::make_pair(0.560, 1));
    data.push_back(std::make_pair(1.273, 1));
    data.push_back(std::make_pair(-0.207, 1));

    mROCCurve = new ROCCurve<double>(data);
    mDETCurve = new DETCurve<double>(data);

    mROCCurve->compute(10);
    mDETCurve->compute(10);
  }

  void teardown()
  {
  }

  std::vector<std::pair<double, int>> data;
  ROCCurve<double> *mROCCurve;
  DETCurve<double> *mDETCurve;
};

BOOST_FIXTURE_TEST_CASE(roc, CurvesTest)
{
  BOOST_CHECK_EQUAL(0.970221, mROCCurve->auc());
}

BOOST_FIXTURE_TEST_CASE(det, CurvesTest)
{
  BOOST_CHECK_EQUAL(0.970221, mDETCurve->auc());
}

BOOST_FIXTURE_TEST_CASE(positives, CurvesTest)
{
  BOOST_CHECK_EQUAL(80, mROCCurve->positives());
}

BOOST_FIXTURE_TEST_CASE(negatives, CurvesTest)
{
  BOOST_CHECK_EQUAL(68, mROCCurve->negatives());
}

BOOST_FIXTURE_TEST_CASE(truePositives, CurvesTest)
{
  double threshold = this->data.back().first;
  BOOST_CHECK_EQUAL(6, mROCCurve->truePositives(threshold));
}

BOOST_FIXTURE_TEST_CASE(falsePositives, CurvesTest)
{
  double threshold = this->data.back().first;
  BOOST_CHECK_EQUAL(25, mROCCurve->falsePositives(threshold));
}

BOOST_FIXTURE_TEST_CASE(trueNegatives, CurvesTest)
{
  double threshold = this->data.back().first;
  BOOST_CHECK_EQUAL(43, mROCCurve->trueNegatives(threshold));
}

BOOST_FIXTURE_TEST_CASE(falseNegatives, CurvesTest)
{
  double threshold = this->data.back().first;
  BOOST_CHECK_EQUAL(74, mROCCurve->falseNegatives(threshold));
}

BOOST_FIXTURE_TEST_CASE(accuracy, CurvesTest)
{
  double threshold = this->data.back().first;
  BOOST_CHECK_CLOSE(0.3311, mROCCurve->accuracy(threshold), 0.1);
  BOOST_CHECK_CLOSE(0.3311, Curve<double>::accuracy(mROCCurve->truePositives(threshold), mROCCurve->trueNegatives(threshold), mROCCurve->positives(), mROCCurve->negatives()), 0.1);
}

BOOST_FIXTURE_TEST_CASE(positivePredictiveValue, CurvesTest)
{
  double threshold = this->data.back().first;
  BOOST_CHECK_EQUAL(0.1935, mROCCurve->positivePredictiveValue(threshold));
  BOOST_CHECK_EQUAL(0.1935, Curve<double>::positivePredictiveValue(mROCCurve->truePositives(threshold), mROCCurve->positives()));
}

BOOST_FIXTURE_TEST_CASE(negativePredictiveValue, CurvesTest)
{
  double threshold = this->data.back().first;
  BOOST_CHECK_CLOSE(0.3675, mROCCurve->negativePredictiveValue(threshold), 0.1);
  BOOST_CHECK_CLOSE(0.3675, Curve<double>::negativePredictiveValue(mROCCurve->falseNegatives(threshold), mROCCurve->negatives()), 0.1);
}

BOOST_FIXTURE_TEST_CASE(truePositiveRate, CurvesTest)
{
  double threshold = this->data.back().first;
  BOOST_CHECK_EQUAL(0.0750, mROCCurve->truePositiveRate(threshold));
  BOOST_CHECK_EQUAL(0.0750, Curve<double>::truePositiveRate(mROCCurve->truePositives(threshold), mROCCurve->positives()));
}

BOOST_FIXTURE_TEST_CASE(falsePositiveRate, CurvesTest)
{
  double threshold = this->data.back().first;
  BOOST_CHECK_CLOSE(0.3676, mROCCurve->falsePositiveRate(threshold), 0.1);
  BOOST_CHECK_CLOSE(0.3676, Curve<double>::falsePositiveRate(mROCCurve->falsePositives(threshold), mROCCurve->negatives()), 0.1);
}

BOOST_FIXTURE_TEST_CASE(trueNegativeRate, CurvesTest)
{
  double threshold = this->data.back().first;
  BOOST_CHECK_EQUAL(0.6324, mROCCurve->trueNegativeRate(threshold));
  BOOST_CHECK_EQUAL(0.6324, Curve<double>::trueNegativeRate(mROCCurve->trueNegatives(threshold), mROCCurve->negatives()));
}

BOOST_FIXTURE_TEST_CASE(falseNegativeRate, CurvesTest)
{
  double threshold = this->data.back().first;
  BOOST_CHECK_EQUAL(0.9250, mROCCurve->falseNegativeRate(threshold));
  BOOST_CHECK_EQUAL(0.9250, Curve<double>::falseNegativeRate(mROCCurve->falseNegatives(threshold), mROCCurve->positives()));
}

BOOST_AUTO_TEST_SUITE_END()
