/***************************************************************************
//
//	File:			test2/TestSuite.h
//
//	Description:	Standard test component base classes.
//
//	Copyright 2001, Be Incorporated, All Rights Reserved.
//
***************************************************************************/

#ifndef	_TEST2_TESTSUITE_H
#define	_TEST2_TESTSUITE_H

#include <test2/ITestSuite.h>
#include <support2/Binder.h>
#include <support2/Profile.h>
#include <kernel/OS.h>

namespace B {
namespace Test2 {

class BTestSuite;

/*-------------------------------------------------------------*/
/*------- LTestContext Local Proxy ----------------------------*/

class LTestContext : public LInterface<ITestContext>
{
protected:
	inline						LTestContext(IContext::sptr_ref context) : LInterface<ITestContext>(context) { }
	inline virtual				~LTestContext() { }
	
	virtual	status_t			HandleEffect(const BValue &in, const BValue &inBindings, const BValue &outBindings, BValue *out);

private:
								LTestContext(const LTestContext& o);
};

/*-------------------------------------------------------------*/
/*------- LTestItem Local Proxy -------------------------------*/

class LTestItem : public LInterface<ITestItem>
{
protected:
	inline						LTestItem(IContext::sptr_ref context) : LInterface<ITestItem>(context) { }
	inline virtual				~LTestItem() { }
	
	virtual	status_t			HandleEffect(const BValue &in, const BValue &inBindings, const BValue &outBindings, BValue *out);

private:
								LTestItem(const LTestItem& o);
};

/*-------------------------------------------------------------*/
/*------- LTestSuite Local Proxy ------------------------------*/

class LTestSuite : public LInterface<ITestSuite>
{
protected:
	inline						LTestSuite(IContext::sptr_ref context) : LInterface<ITestSuite>(context) { }
	inline virtual				~LTestSuite() { }
	
	virtual	status_t			HandleEffect(const BValue &in, const BValue &inBindings, const BValue &outBindings, BValue *out);

private:
								LTestSuite(const LTestSuite& o);
};

/*-------------------------------------------------------------*/
/*------- BTestSuite Implementation ---------------------------*/

class BTestSuite : public LTestSuite
{
public:
	B_STANDARD_ATOM_TYPEDEFS(BTestSuite)
	
	typedef	ITestItem::sptr		(*test_instantiate_func)(	BTestSuite::sptr_ref suite,
															ITestContext::sptr_ref testContext,
															const BValue& params);
	
	struct test_item {
		const char*				name;
		const char*				description;
		test_instantiate_func	instantiate;
	};
			
								BTestSuite(	IContext::sptr_ref context,
											const test_item* items);

	virtual	ITestItem::sptr		GetTest(const char* name,
										ITestContext::sptr_ref context,
										const BValue& params = BValue::undefined);
	virtual	BValue				QueryTests(const BValue& which = BValue::wild) const;
	virtual	BValue				TestsAndPresets() const;
	
protected:
	virtual						~BTestSuite();

private:
								BTestSuite(const BTestSuite& o);
			
			const test_item*	m_items;
};

/*-------------------------------------------------------------*/
/*------- BTestItem Implementation ----------------------------*/

class BTestItem : public LTestItem
{
public:
	B_STANDARD_ATOM_TYPEDEFS(BTestItem)
	
	//! Entry for a single preset parameter value.  An array of
	//!	these it terminated by a NULL key.
	struct preset_value {
		const void*		raw_key;			//!< BValue of parameter key.
		const BValue&	key() const;		//!< convenience for retrieving key
		const void*		raw_value;			//!< BValue of parameter value.
		const BValue&	value() const;		//!< convenience for retrieving value
	};
	
	//!	Information about a single preset in this test.  An array
	//!	of these is terminated by a NULL 'values'.
	struct preset_item {
		size_t				struct_size;	//!< Always sizeof(preset_item).
		preset_item*		next();			//!< Retrieve next preset_item in array.
		const preset_item*	next() const;	//!< Retrieve next preset_item in array.
		
		const preset_value* values;			//!< Values in this preset.
		const char*			name;			//!< A descriptive name for this preset.
		int32				match;			//!< Other preset this should match, or -1.
		int32				match_error;	//!< Maximum match error allowed.
		bool				regress;		//!< Include in regression?
	};

								BTestItem(	BTestSuite::sptr_ref suite,
											ITestContext::sptr_ref testContext,
											const BValue& params = BValue::undefined,
											const preset_item* presets = NULL);

			BTestSuite::sptr	TestSuite() const;
			ITestContext::sptr	TestContext() const;
	
			// ---------------------------------------------------------------
			
			// These are convenience functions for a standard implementation of
			// RunTest() that takes care of timing and such.  You can implement
			// these, or just implement all of RunTest() yourself.
			
			struct TestState {
					ITextOutput::sptr_ref	log;
					BValue					opts;
					const BString			profile;
					
					int32					availIter;
					int32					totalIter;
					bigtime_t				availTime;
					bigtime_t				startTime;
					bigtime_t				endTime;
					
					inline void				Start()
					{
						if (profile_str) {
							start_profile();
						}
						startTime = system_time();
					}
					
					inline bool				IsFinished() const
					{
						return ((endTime-startTime) >= availTime || totalIter >= availIter);
					}
					inline bool				Iterate()
					{
						endTime = system_time();
						totalIter++;
						return !IsFinished();
					}
					
					inline void				Stop()
					{
						if (profile_str) {
							stop_profile(profile_str);
						}
					}
					
					TestState(ITextOutput::sptr_ref _log, const BValue& _opts);
					virtual ~TestState();
				
				private:
					char*					profile_str;
			};
	virtual	TestState*			StartTest(	ITextOutput::sptr_ref log,
											const BValue& opts);
	virtual	BValue				ExecTest(TestState& s);
	
			//!	Return value of a test parameter.
			const BValue		ParamFor(const BValue& key) const;
			
			// ---------------------------------------------------------------
			
			//!	Standard implementation of RunTest(), which calls StartTest()
			//!	and ExecTest().
	virtual	BValue				RunTest(ITextOutput::sptr_ref log,
										bigtime_t availTime = -1,
										const BValue& opts = BValue::undefined);
	
			//!	Standard implementation of parameter handling.
	virtual	BValue				DescribeParameters() const;
	virtual	status_t			SetParameters(const BValue& params, uint32 flags = 0);
	virtual	BValue				QueryParameters(const BValue& which = BValue::wild) const;
	
			//!	Standard implementation of preset handling, based on
			//!	preset_item array passed to constructor.
	virtual	int32				CountPresets() const;
	virtual	BValue				Presets() const;
	virtual	BValue				RetrievePreset(int32 which) const;
	
protected:
	virtual						~BTestItem();

private:
								BTestItem(const BTestItem& o);
			
			BTestSuite::sptr	m_suite;
			ITestContext::sptr	m_testContext;
			
			const preset_item*	m_presets;
			int32				m_presetCount;
			
			BValue				m_params;
};

/*-------------------------------------------------------------*/
/*---- No user serviceable parts after this -------------------*/

inline const BValue& BTestItem::preset_value::key() const
{
	return *reinterpret_cast<const BValue*>(raw_key);
}

inline const BValue& BTestItem::preset_value::value() const
{
	return *reinterpret_cast<const BValue*>(raw_value);
}

inline BTestItem::preset_item* BTestItem::preset_item::next()
{
	return reinterpret_cast<preset_item*>(reinterpret_cast<uint8*>(this) + struct_size);
}

inline const BTestItem::preset_item* BTestItem::preset_item::next() const
{
	return reinterpret_cast<const preset_item*>(reinterpret_cast<const uint8*>(this) + struct_size);
}

} } // namespace B::Test2

#endif /* _TEST2_TESTSUITE_H */
