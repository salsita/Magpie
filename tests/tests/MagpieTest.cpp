#include "stdafx.h"
#include "Magpie_i.h"
#include "Magpie_i.c"

//============================================================================
// Fixture
//============================================================================

class MagpieTest :
  public ::testing::Test
{
};

//============================================================================
// TESTS
//============================================================================

//----------------------------------------------------------------------------
// NULL pointer checks
TEST_F(MagpieTest, CreateInstance)
{
/*
  CoInitialize(NULL);
  CComPtr<IMagpieApplication> application;
  HRESULT hr = application.CoCreateInstance(CLSID_MagpieApplication);
  ASSERT_EQ(S_OK, hr);
  CoUninitialize();
*/
}

