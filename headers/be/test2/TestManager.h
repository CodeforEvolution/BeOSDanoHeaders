/***************************************************************************
//
//	File:			test2/TestManager.h
//
//	Description:	Helper class for executing and managing tests.
//
//	Copyright 2001, Be Incorporated, All Rights Reserved.
//
***************************************************************************/

#ifndef	_TEST2_TESTMANAGER_H
#define	_TEST2_TESTMANAGER_H

#include <test2/ITestSuite.h>

#include <support2/IByteStream.h>
#include <support2/IContext.h>
#include <support2/ITeam.h>
#include <support2/String.h>
#include <support2/Value.h>

namespace B { namespace Render2 { class BStaticPixmap; } }

namespace B {
namespace Test2 {

using namespace B::Support2;
using namespace B::Render2;

/*-------------------------------------------------------------*/
/*------- BTestManager Implementation -------------------------*/

enum {
	B_RUN_TESTS_LOCALLY		= 0x00000001
};

enum {
	B_DIFF_EXPECT_ONLY			= 0x00000001,	// Return only expected differences.
	B_DIFF_ACTUAL_ONLY			= 0x00000002,	// Return only actual differences.
	B_DIFF_PIXMAPS				= 0x00000004	// Perform graphical diff if pixmap data.
};

class BTestManager
{
public:
								BTestManager(	IContext::sptr_ref context,
												uint32 flags = 0);
	virtual						~BTestManager();
	
			//!	Return a BValue containing all available tests.
			/*!	The returned value is structured as:
					{
						Vendor ->
						{
							Package ->
							{
								Component ->
								{
									0 -> TestItemName1 ->
									{
										0 -> PresetName1,
										...
									},
									...
								},
								...
							},
							...
						},
						...
					}
			*/
			BValue				AvailTests() const;
	
			// ------------------------------------------------------------
			
			//!	Turn on test validation, reporting errors if a test's
			//!	results don't match the results of another test.
			void				EnableValidation();
			//!	Turn off test validation, not comparing results between
			//!	different tests.
			void				DisableValidation();
			//!	Is the manager currently performing validation?
			bool				IsValidating() const;
			
			//!	Set up manager to start recording test results into the given
			//!	byte stream.
			status_t			StartRecording(IByteOutput::sptr_ref dest);
			//!	End recording, writing results out to the stream previously
			//!	passed in to StartRecording().
			status_t			FinishRecording();
			//!	Is the manager currently recording its test results?
			bool				IsRecording() const;
			
			//!	Set up manager to start comparing test results with those
			//!	previously recorded into the given byte stream.
			status_t			StartRegressing(IByteInput::sptr_ref prev);
			//!	Stop regressing with data previously retrieved by
			//!	StartRegressing().
			status_t			FinishRegressing();
			//!	Is the manager currently regressing with historic test
			//!	results?
			bool				IsRegressing() const;
			
			//!	Set location to log test results (defaults is stdout).
			void				SetLog(IByteOutput::sptr_ref log);
			void				ResetLog();
			ITextOutput::sptr	Log() const;
			
			//!	Select the tests to run.
			/*!	By default, this is all tests, i.e. AvailTests().  The given
				value is structured the same as AvailTests(), except that
				you can supply a terminal BValue::wild at any branch to include
				all available tests from that point down.
				\sa AvailTests()
			*/
			void				SelectTests(const BValue& which);
			
			//!	Return currently selected tests.
			BValue				SelectedTests() const;
			
			//!	Run the selected batch of tests, either recording or regressing
			//!	its results.
			/*!	\sa SelectTests() */
			BValue				RunSelectedTests(bigtime_t timePerTest = -1, uint32 flags = 0);
	
			//!	Execute the next test in the selected set.
			/*!	The BValue returned contains a set of mappings containing
				the test results.
					next_Vend:   The vendor the executed test suite is in.
					next_Pack:   The package the executed test suite is in.
					next_Comp:   The component of the executed test suite.
					next_Suite:  The name of the test suite that was run.
					next_Item:   The name of the test item that was run.
					next_Expect: The results that were expected, if regressing.
					next_Actual: The results returned by this run of the test.
					next_Diff:   Differences between the above two, if any.
					next_Fail:   Boolean indicating whether the test failed.
				
				When the last test has executed, BValue::undefined is returned,
				and afterwards tests will restart at the beginning.
				
				The \a flags parameter is passed on to the flags of Diff() when
				performing regression or validation.
			*/
			BValue				RunNextTest(bigtime_t availTime = -1, uint32 flags = 0);
	static	value_csml			next_Vend;
	static	value_csml			next_Pack;
	static	value_csml			next_Comp;
	static	value_csml			next_Suite;
	static	value_csml			next_Item;
	static	value_csml			next_Expect;
	static	value_csml			next_Actual;
	static	value_csml			next_Diff;
	static	value_csml			next_Fail;
	
			//!	Instantiate the next test to run.  Updates current test
			//!	state.
			/*!	\sa CurrentSuiteName()
				\sa CurrentTestSuite()
				\sa CurrentItemName()
				\sa CurrentTestItem()
				\sa CurrentPreset()
				\sa CurrentParameters()
				\sa CurrentParameterMods()
			*/
			ITestItem::sptr		InstantiateNextTest(status_t* outError = NULL);
			
			//!	Return the name of the currently executing vendor.
			/*!	\sa InstantiateNextTest() */
			BValue				CurrentVendorName() const;
			//!	Return the name of the currently executing package.
			/*!	\sa InstantiateNextTest() */
			BValue				CurrentPackageName() const;
			//!	Return the name of the currently executing component.
			/*!	\sa InstantiateNextTest() */
			BValue				CurrentComponentName() const;
			//!	Return the name of the currently executing test suite id.
			/*!	\sa InstantiateNextTest() */
			BValue				CurrentSuiteName() const;
			//!	Return the ITestSuite that is currently being executed.
			/*!	\sa InstantiateNextTest() */
			ITestSuite::sptr	CurrentTestSuite() const;
			//!	Return the index of the currently executing test item.
			/*!	\sa InstantiateNextTest() */
			BValue				CurrentItemIndex() const;
			//!	Return the name of the currently executing test item.
			/*!	\sa InstantiateNextTest() */
			BValue				CurrentItemName() const;
			//!	Return the ITestItem that is being run in the current suite.
			/*!	\sa InstantiateNextTest() */
			ITestItem::sptr		CurrentTestItem() const;
			//!	Return the preset that is being run in the current test item.
			/*!	\sa InstantiateNextTest() */
			BValue				CurrentPresetIndex() const;
			//!	Return name of the current preset being used.
			/*!	\sa InstantiateNextTest() */
			BValue				CurrentPresetName() const;
			//!	Return preset index that the current preset should match.
			/*!	\sa InstantiateNextTest() */
			int32				CurrentPresetMatch() const;
			//!	Return results that the current preset should match.
			/*!	\sa InstantiateNextTest() */
			BValue				CurrentPresetMatchData() const;
			//!	Return the parameters that are being run in the current test item.
			/*!	\sa InstantiateNextTest() \sa CurrentParameterMods() */
			BValue				CurrentParameters() const;
			//!	Return modifications of the current test parameters from the
			//!	test item's base paramaters.
			/*!	\sa InstantiateNextTest() \sa CurrentParameters() */
			BValue				CurrentParameterMods() const;
			
			//!	Retrieve the data created by the test when run with the given
			//!	preset.
			BValue				PresetData(int32 index = -1) const;
			
			//!	Set the data created by the test when run with the current
			//!	preset.
			status_t			SetPresetData(	const BValue& data,
												int32 index = -1);
			
			// ------------------------------------------------------------
			
			//!	Store the given test results, if the manager is currently
			//!	recording.
			/*!	\note The \a params should be only the \e modifications
				from the test's base parameters.
				\sa StartRecording() \sa Mods()
			*/
			status_t			RecordResult(	const BValue& suiteName,
												const BValue& itemName,
												const BValue& params,
												const BValue& result);
												
			//!	Retrieve the given test results, if the manager is currently
			//!	regressing.
			/*!	\note The \a params should be only the \e modifications
				from the test's base parameters.
				\sa StartRegressing() \sa Mods()
			*/
			BValue				RegressionData(	const BValue& suiteName,
												const BValue& itemName,
												const BValue& params);
	
			//!	Return the modifications to go from \a base to \a current.
			/*!	The result satisfies current = base.Overlay(Mods(base, current)),
				where the result of Mods() is only the set of values that need
				to be changed.  Note that this does -not- work if there are values
				in \a base that are not in \a current.
			*/
	static	BValue				Mods(	const BValue& base,
										const BValue& current);
	
			//!	Return the differences between \a expect and \a actual.
			/*!	The returned value contains all of the values that are
				different between the two.  If \a flags is B_DIFF_EXPECT_ONLY,
				then only the values in \a expect that are not in \a actual are
				returned.  If \a flags is B_DIFF_ACTUAL_ONLY, then only the
				values in \a actual that are not in \a expect are returned.
				If \a error supplied and B_DIFF_PIXMAPS is set, then \a error
				will be set to the number of pixels that are different.
			*/
	static	BValue				Diff(	const BValue& expect,
										const BValue& actual,
										uint32 flags = 0,
										int32* error = 0);
	
	static	atom_ptr<BStaticPixmap>	Diff(	const atom_ptr<BStaticPixmap>& expect,
											const atom_ptr<BStaticPixmap>& actual,
											int32* error = 0);
					
			ITestSuite::sptr	InstantiateSuite(const BValue& suiteName,
												 status_t* outError = NULL);
			
private:
								BTestManager(const BTestManager& o);
			
			IContext::sptr		m_context;
			BValue				m_availTests;
			uint32				m_flags;
			BValue				m_teamEnv;
			ITeam::sptr			m_testTeam;
			
			// High-level testing state.
			bool				m_validating;
			char				_reserved_0[3];
			IByteOutput::sptr	m_recordStream;
			BValue				m_recordData;
			BValue				m_regressData;
			IByteOutput::sptr	m_rawLog;
			ITextOutput::sptr	m_log;
			BValue				m_rawSelectedTests;
			BValue				m_selectedTests;
			
			// Information about the currently executing test.
			void*				m_vendorCookie;
			BValue				m_vendorName;
			BValue				m_packages;
			void*				m_packageCookie;
			BValue				m_packageName;
			BValue				m_components;
			void*				m_componentCookie;
			BValue				m_componentName;
			ITestSuite::sptr	m_testSuite;
			BValue				m_items;
			void*				m_itemCookie;
			BValue				m_itemIndex;
			BValue				m_itemName;
			ITestItem::sptr		m_testItem;
			BValue				m_presets;
			void*				m_presetCookie;
			BValue				m_presetIndex;
			BValue				m_presetName;
			
			int32				m_presetMatch;
			int32				m_presetMatchError;
			bool				m_presetRegress;
			uint8				_pad[3];
			BValue				m_baseParams;
			BValue				m_params;
			BValue				m_presetData;
};

/*-------------------------------------------------------------*/

} } // namespace B::Test2

#endif /* _TEST2_TESTMANAGER_H */
