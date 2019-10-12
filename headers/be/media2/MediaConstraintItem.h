/***************************************************************************
//
//	File:			media2/MediaConstraintItem.h
//
//	Description:	Relates a key to a value in one of these ways:
//					"equal to"
//					"less than"
//					"multiple of"
//					"power of"
//					or the inverse of any of these.
//
//	Copyright 2001, Be Incorporated, All Rights Reserved.
//
***************************************************************************/

#ifndef _MEDIA2_MEDIACONSTRAINTITEM_H_
#define _MEDIA2_MEDIACONSTRAINTITEM_H_

#include <support2/String.h>
#include <support2/Value.h>

namespace B {
namespace Media2 {

using namespace Support2;

enum constraint_relation_t
{
	B_EQ				= 0,
	B_EQUALS			= B_EQ,	
	B_ONE_OF			= B_EQ,

	B_LT,
	B_LESS_THAN			= B_LT,
	
	B_MULTIPLE_OF,
	B_POWER_OF,
	B_NAME_DEPENDENT,				// internal use only !!!
	B_INVERT			= 0x100,

	B_NE				= B_INVERT,
	B_NOT_EQUAL			= B_NE,
	B_NOT_ANY_OF		= B_INVERT,
	
	B_GE,
	B_GREATER_OR_EQUAL	= B_GE,
	
	B_NOT_MULTIPLE_OF,
	B_NOT_POWER_OF
};

enum constraint_scope_t
{
	B_MANDATORY = 0,
	B_OPTIONAL
};
	
class BMediaConstraintItem
{	
public:		
	enum simplify_result_t
	{
		B_KEEP_FIRST,
		B_KEEP_SECOND,
		B_KEEP_BOTH,
		B_FALSE,
		B_TRUE,
		B_NONE
	};

								BMediaConstraintItem();
		
								BMediaConstraintItem(
									const BValue &key,
									constraint_relation_t rel,
									const BValue &value,
									bool optional);
									
								BMediaConstraintItem(
									const BValue & archive);

			void				PrintToStream(ITextOutput::sptr_ref io) const;
		
			bool				operator==(const BMediaConstraintItem &S) const;
				
			BValue				Key() const;
			constraint_relation_t			Relation() const;
			BValue 				Value() const;
			
			BValue				AsValue() const;
			inline  bool				IsOptional() const { return mOptional; }
		
private:
	friend class BMediaConstraintAlternative;

			bool				mIsValid;
			BValue				mKey;
			constraint_relation_t			mRel;
			BValue				mValue;
			bool				mOptional;
			
	inline	constraint_relation_t			BaseRelation() const { return constraint_relation_t(mRel & ~B_INVERT); }
	inline	bool				IsInverted() const { return mRel & B_INVERT; }
	
		static simplify_result_t Simplify (bool conjunctive,
											BMediaConstraintItem &S1,
											BMediaConstraintItem &S2);

		static simplify_result_t SimplifyLTONE_OF (bool conjunctive,
											BMediaConstraintItem &S1,
											BMediaConstraintItem &S2);

		static simplify_result_t SimplifyLTLT (bool conjunctive,
											BMediaConstraintItem &S1,
											BMediaConstraintItem &S2);

		static simplify_result_t SimplifyONE_OFMULTIPLE_OF (bool conjunctive,
											BMediaConstraintItem &S1,
											BMediaConstraintItem &S2);

		static simplify_result_t SimplifyMULTIPLE_OFMULTIPLE_OF (bool conjunctive,
											BMediaConstraintItem &S1,
											BMediaConstraintItem &S2);

		static simplify_result_t SimplifyONE_OFONE_OF (bool conjunctive,
											BMediaConstraintItem &S1,
											BMediaConstraintItem &S2);

		static BValue &RemoveCriteria(BValue &set,
										bool (*meets_criteria)(const BValue &value, const BValue &param),
										const BValue &param);

		static bool GreaterOrEqualThan(const BValue &value, const BValue &param);
		static bool LessThan(const BValue &value, const BValue &param);

		static bool IsNotMultipleOf(const BValue &value, const BValue &param);
		static bool IsMultipleOf(const BValue &value, const BValue &param);

		static int64 GetValueAsInt64(const BValue &value);
		static BValue Intersection(const BValue &n1, const BValue &n2);
};

}; }; // namespace B::Media2

#endif // _MEDIA2_MEDIACONSTRAINTITEM_H_
