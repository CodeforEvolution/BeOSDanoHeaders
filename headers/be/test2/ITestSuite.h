/***************************************************************************
//
//	File:			test2/ITestSuite.h
//
//	Description:	Interfaces implemented by a test component.
//
//	Copyright 2001, Be Incorporated, All Rights Reserved.
//
***************************************************************************/

#ifndef	_TEST2_TESTSUITE_INTERFACE_H
#define	_TEST2_TESTSUITE_INTERFACE_H

#include <support2/IInterface.h>

#include <support2/ITextStream.h>
#include <support2/Value.h>

namespace B {
namespace Test2 {

using namespace B::Support2;

/*-------------------------------------------------------------*/
/*------- ITestContext Interface ------------------------------*/

class ITestContext : public IInterface
{
public:

	B_DECLARE_META_INTERFACE(TestContext);

			//!	Return the name of the suite that is currently running.
	virtual	BValue			Suite() const = 0;
	static	value_csml		key_Suite;
	
			//! Return the name of the item that is currently running.
	virtual	BValue			Item() const = 0;
	static	value_csml		key_Item;
};

/*-------------------------------------------------------------*/
/*------- ITestItem Interface ---------------------------------*/

enum {
	B_REPLACE_TEST_PARAMETERS	= 0x00000001
};

class ITestItem : public IInterface
{
public:

	B_DECLARE_META_INTERFACE(TestItem);

			//!	Execute this test and return its results.
			/*!	The \a availTime parameter all for performance testing.
				If it is > 0, then it is the desired length of time that
				the test should run for; if < 0, it is the exact number of
				times the test should execute.
				
				The returned value has mappings for information about the
				test.  "Value" contains the test's return value (which can
				be any arbitrary mapping), "Time" is the total amount of
				time the test spent running (which may be larger or smaller
				than the requested availTime), and "Count" is the total
				number of times it was executed.  The time for a single
				operation is thus Time/Count.
			*/
	virtual	BValue			RunTest(ITextOutput::sptr_ref log,
									bigtime_t availTime = -1,
									const BValue& opts = BValue::undefined) = 0;
	static	value_csml		key_RunTest;
	static	value_csml		runopt_Profile;
	static	value_csml		run_Value;
	static	value_csml		run_Time;
	static	value_csml		run_Count;
	
			//!	Retrieve metadata about this test's parameters.
			/*!	The returned value contains all of the parameters
				available in this test mapping to the following
				metadata:
					describe_Message: English message about the parameter.
					describe_Default: Default value of parameter.
					describe_Min: Minimum value for parameter.
					describe_Max: Maximum value for parameter.
					describe_Options: Set of choices for parameter.
			*/
	virtual	BValue			DescribeParameters() const = 0;
	static	value_clrg		key_DescribeParameters;
	static	value_csml		describe_Message;
	static	value_csml		describe_Default;
	static	value_csml		describe_Min;
	static	value_csml		describe_Max;
	static	value_csml		describe_Options;
	
			//!	Update the parameters this test is using.
	virtual	status_t		SetParameters(const BValue& params, uint32 flags = 0) = 0;
	static	value_clrg		key_SetParameters;
	
			//!	Retrieve the parameters this test is using.
	virtual	BValue			QueryParameters(const BValue& which = BValue::wild) const = 0;
	static	value_clrg		key_QueryParameters;
	
			//!	Return the number of preset parameter lists that this
			//!	test contains.
	virtual	int32			CountPresets() const = 0;
	static	value_clrg		key_CountPresets;
	
			//!	Return set of all presets this test provides.
			/*!	The returned value is a set of mappings of the
				form { preset_index -> preset_name }.
			*/
	virtual	BValue			Presets() const = 0;
	static	value_csml		key_Presets;
	
			//!	Return the parameters for a particular preset.
			/*!	The returned value contains the following keys:
					preset_Params: (value) The preset's parameters.
					preset_Name: (string) Descriptive name for this preset.
					preset_Match: (int32) The index of a preset that
						the test results from this one should match.
					preset_MatchError: (int32) Number of pixels that are
						allowed to differ with match for validation.
					preset_Regress: (bool) Should this preset be
						included in regression data?  True if not set.
			*/
	virtual	BValue			RetrievePreset(int32 which) const = 0;
	static	value_clrg		key_RetrievePreset;
	static	value_csml		preset_Params;
	static	value_csml		preset_Name;
	static	value_csml		preset_Match;
	static	value_clrg		preset_MatchError;
	static	value_csml		preset_Regress;
};

/*-------------------------------------------------------------*/
/*------- ITestSuite Interface --------------------------------*/

class ITestSuite : public IInterface
{
public:

	B_DECLARE_META_INTERFACE(TestSuite);

			//!	Return a particular test in the suite.
			/*!	A new ITestItem for \a name is returned, initialized
				with the given \a params.
			*/
	virtual	ITestItem::sptr	GetTest(const char* name,
									ITestContext::sptr_ref context,
									const BValue& params = BValue::undefined) = 0;
	static	value_csml		key_GetTest;
	
			//!	Return set of all tests this suite provides.
	virtual	BValue			QueryTests(const BValue& which = BValue::wild) const = 0;
	static	value_clrg		key_QueryTests;
	
			//!	Return a summary of all tests in the suite and their presets.
			/*!	The returned value contains mappings of the form:
				{
					test_index -> test_name ->
					{
						preset_index -> preset_name,
						...
					},
					...
			*/
	virtual	BValue			TestsAndPresets() const = 0;
	static	value_clrg		key_TestsAndPresets;
};

} } // namespace B::Test2

#endif	// _TEST2_TESTSUITE_INTERFACE_H
