/*
// <legal>CERT Rosecheckers// 
// Copyright 2022 Carnegie Mellon University.// 
// NO WARRANTY. THIS CARNEGIE MELLON UNIVERSITY AND SOFTWARE ENGINEERING INSTITUTE MATERIAL IS FURNISHED ON AN 'AS-IS' BASIS. CARNEGIE MELLON UNIVERSITY MAKES NO WARRANTIES OF ANY KIND, EITHER EXPRESSED OR IMPLIED, AS TO ANY MATTER INCLUDING, BUT NOT LIMITED TO, WARRANTY OF FITNESS FOR PURPOSE OR MERCHANTABILITY, EXCLUSIVITY, OR RESULTS OBTAINED FROM USE OF THE MATERIAL. CARNEGIE MELLON UNIVERSITY DOES NOT MAKE ANY WARRANTY OF ANY KIND WITH RESPECT TO FREEDOM FROM PATENT, TRADEMARK, OR COPYRIGHT INFRINGEMENT.// 
// Released under a BSD (SEI)-style license, please see license.txt or contact permission@sei.cmu.edu for full terms.// 
// [DISTRIBUTION STATEMENT A] This material has been approved for public release and unlimited distribution.  Please see Copyright notice for non-US Government use and distribution.// 
// CERT(R) is registered in the U.S. Patent and Trademark Office by Carnegie Mellon University.// 
// This Software includes and/or makes use of Additional Software Dependencies, each subject to its own license.// 
// DM21-0505</legal>// 
 */

/*
 * \file type.h
 */

#ifndef TYPE_H
#define TYPE_H

//
// Type is a thin layer over SgType, intended to
//	a) simplify use of SgType, and
//	b) provide additional functionality to ask questions about and manipulate types
//
// This layer ordinarily strips away initial typedefs from a type so that, for instance,
//
//	typedef int INT;
//
// both int and INT will return true for isInt(), but INT will return true for isTypedef() as well.
//

#include "rose.h"

class Type {
  public:
	explicit Type( const SgType * );

	bool operator==(const Type& that) const;

	// We'll let the compiler write these for us:
	// ~Type();
	// Type( const Type & );
	// Type &operator =( const Type & );

	std::string mangledName() const;

	Type dereference() const; // will deref pointer, reference, or array, else same type
	Type stripInitialTypedefs() const;
	Type stripInitialPointers() const; // strip sequence of typedefs and pointers, but leave trailing typedefs
	Type stripInitialReferences() const;
	Type stripInitialPointersAndReferences() const;
	Type stripInitialArrays() const;
	Type stripInitialPointersAndArrays() const;
	Type stripInitialPointersReferencesAndArrays() const;
	Type stripTypedefsAndModifiers() const;
	Type baseType() const;

	bool isTypedef() const;
	bool isEllipsis() const;
	bool isBool() const;
	bool isPlainChar() const;
	bool isSignedChar() const;
	bool isUnsignedChar() const;
	bool isAnyChar() const;
	bool isPlainShort() const;
	bool isSignedShort() const;
	bool isUnsignedShort() const;
	bool isShort() const;
	bool isPlainInt() const;
	bool isSignedInt() const;
	bool isUnsignedInt() const;
	bool isInt() const;
	bool isPlainLong() const;
	bool isSignedLong() const;
	bool isUnsignedLong() const;
	bool isLong() const;
	bool isLongLong() const;
	bool isUnsignedLongLong() const;
	// no signed long long?
	bool isFloat() const;
	bool isDouble() const;
	bool isLongDouble() const;
	bool isWchar() const;

	bool isSize_t() const;

	//
	// These follow the structure of the TR1 <type_traits> library
	//
	// primary type categories, 11.3
	bool isVoid() const;
	bool isIntegral() const;
	bool isFloatingPoint() const;
	bool isArray() const;
	bool isPointer() const;
	bool isReference() const;
	bool isMemberObjectPointer() const;
	bool isMemberFunctionPointer() const;
	bool isEnum() const;
	const SgEnumDeclaration *getEnumDeclaration() const;
	bool isUnion() const;
	bool isClass() const; // Note: may be struct but not union!
	const SgClassDeclaration *getClassDeclaration() const;
	const SgClassDefinition *getClassDefinition() const;
	bool isFunction() const;

	// composite type categories, 11.4
	bool isArithmetic() const;
	bool isFundamental() const; //XXX what is the status of bool?
	bool isObject() const;
	bool isScalar() const;
	bool isCompound() const;
	bool isMemberPointer() const;

	// type properties, 11.5
	bool isConst() const;
	bool isVolatile() const;
	bool isPOD() const;
	bool isEmpty() const;
	bool isPolymorphic() const;
	bool isAbstract() const;
	bool hasTrivialConstructor() const;
	bool hasTrivialCopy() const;
	bool hasTrivialAssign() const;
	bool hasTrivialDestructor() const;
	bool hasNothrowConstuctor() const;
	bool hasNothrowCopy() const;
	bool hasNothrowAssign() const;
	bool hasVirtualDestructor() const;
	bool isSigned() const;
	bool isUnsigned() const;
	size_t rank() const; // array dimension, 0 if not array
	size_t extent( unsigned n ) const; // bound of nth array modifier

	// type relationsips, 11.6
	bool isSame( Type ) const;
	bool isConvertible( Type ) const;
	bool isBaseOf( Type ) const;

	// type transformations, 11.7
	Type removeConst() const;
	Type removeVolatile() const;
	Type removeCV() const;
	Type addConst() const;
	Type addVolatile() const;
	Type addCV() const;
	Type removeReference() const;
	Type addReference() const;
	Type removePointer() const;
	Type addPointer() const;
	Type removeExtent () const;
	Type removeAllExtents() const; // same as stripInitialArrays

	// alignment, 11.8
	//size_t alignmentOf() const;
	//Type alignedStorage( ) const;

  private:
	const SgType *orig_; // SgType used to initialize
	const SgType *t_; // orig_ stripped of initial sequence of typedefs (note uses of orig_ vs. t_ in implementations)
	bool isConst_;
	bool isVolatile_;
};

#endif
