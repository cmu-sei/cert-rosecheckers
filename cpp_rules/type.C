/*
 *
 * Copyright (c) 2007 Carnegie Mellon University.
 * All rights reserved.

 * Permission to use this software and its documentation for any purpose is hereby granted,
 * provided that the above copyright notice appear and that both that copyright notice and
 * this permission notice appear in supporting documentation, and that the name of CMU not
 * be used in advertising or publicity pertaining to distribution of the software without
 * specific, written prior permission.
 *
 * CMU DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING ALL IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS, IN NO EVENT SHALL CMU BE LIABLE FOR ANY SPECIAL, INDIRECT OR
 * CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,
 * WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, RISING OUT OF OR IN
 * CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */


#include "type.h"
#include "utilities.h"

Type::Type( const SgType *t )
	: orig_(t), t_(t), isConst_(false), isVolatile_(false) {
	while( true ) {
		t = t->stripType( SgType::STRIP_TYPEDEF_TYPE );
		if( const SgModifierType *mt = isSgModifierType( t ) ) {
			const SgTypeModifier &m = mt->get_typeModifier();
			const SgConstVolatileModifier &cv = m.get_constVolatileModifier();
			isConst_ |= cv.isConst();
			isVolatile_ |= cv.isVolatile();
			t = mt->get_base_type();
		}
		if( t_ == t )
			break;
		else
			t_ = t;
	}
}

bool Type::operator==(const Type& that) const {
  // prob not the best way to check type equality...but what is???
  return mangledName() == that.mangledName();
}

#if 0
Type::operator bool Type::*() const { // tests if type is valid (non-null, basically)
  //  return  (bool Type::*) orig_;
  std::cerr << "not implemented: Type::operator bool Type::*()" << std::endl;
  return 0;
}
#endif

std::string Type::mangledName() const {
	return t_->get_mangled().getString();
}

bool Type::isTypedef() const {
	return isSgTypedefType(orig_) != 0;
}

bool Type::isVoid() const {
	return isSgTypeVoid( t_ ) != 0;
}

bool Type::isEllipsis() const {
	return isSgTypeEllipse( t_ ) != 0;
}
	
bool Type::isBool() const {
	return isSgTypeBool( t_ ) != 0;
}
	
bool Type::isPlainChar() const {
	return isSgTypeChar( t_ ) != 0;
}
	
bool Type::isSignedChar() const {
	return isSgTypeSignedChar( t_ ) != 0;
}
	
bool Type::isUnsignedChar() const {
	return isSgTypeUnsignedChar( t_ ) != 0;
}
	
bool Type::isAnyChar() const {
	return isPlainChar() || isUnsignedChar() || isSignedChar();
}
	
bool Type::isPlainShort() const {
	return isSgTypeShort( t_ ) != 0;
}
	
bool Type::isSignedShort() const {
	return isSgTypeSignedShort( t_ ) != 0;
}
	
bool Type::isUnsignedShort() const {
	return isSgTypeUnsignedShort( t_ ) != 0;
}
	
bool Type::isShort() const {
	return isPlainShort() || isSignedShort();
}
	
bool Type::isPlainInt() const {
	return isSgTypeInt( t_ ) != 0;
}
	
bool Type::isSignedInt() const {
	return isSgTypeSignedInt( t_ ) != 0;
}
	
bool Type::isUnsignedInt() const {
	return isSgTypeUnsignedInt( t_ ) != 0;
}
	
bool Type::isInt() const {
	return isPlainInt() || isSignedInt();
}
	
bool Type::isPlainLong() const {
	return isSgTypeLong( t_ ) != 0;
}
	
bool Type::isSignedLong() const {
	return isSgTypeSignedLong( t_ ) != 0;
}
	
bool Type::isUnsignedLong() const {
	return isSgTypeUnsignedLong( t_ ) != 0;
}
	
bool Type::isLong() const {
	return isPlainLong() || isSignedLong();
}
	
bool Type::isLongLong() const {
	return isSgTypeLongLong( t_ ) != 0;
}
	
bool Type::isUnsignedLongLong() const {
	return isSgTypeUnsignedLongLong( t_ ) != 0;
}
	
bool Type::isFloat() const {
	return isSgTypeFloat( t_ ) != 0;
}
	
bool Type::isDouble() const {
	return isSgTypeDouble( t_ ) != 0;
}
	
bool Type::isLongDouble() const {
	return isSgTypeLongDouble( t_ ) != 0;
}
	
bool Type::isWchar() const {
	return isSgTypeWchar( t_ ) != 0;
}

bool Type::isSize_t() const {
	return isUnsignedInt(); //XXX not correct!  fix...
}

bool Type::isArray() const {
	return isSgArrayType(t_) != 0;
}

bool Type::isPointer() const {
	return (isSgPointerType(t_) != 0)
		&& !isMemberPointer(); //XXX ROSE identifies ptr to member as ptr!
}

bool Type::isReference() const {
	return isSgReferenceType(t_) != 0;
}

bool Type::isMemberPointer() const {
	return isSgPointerMemberType( t_ ) != 0; //XXX make sure a member pointer is not also considered a pointer!
}

bool Type::isMemberObjectPointer() const {
	const SgPointerMemberType *mp = isSgPointerMemberType( t_ );
	if( !mp )
		return false;
	Type base(mp->get_base_type());
	return !base.isFunction();
}

bool Type::isMemberFunctionPointer() const {
	const SgPointerMemberType *mp = isSgPointerMemberType( t_ );
	if( !mp )
		return false;
	Type base(mp->get_base_type());
	return base.isFunction();
}

bool Type::isConst() const {
	return isConst_;
}

bool Type::isVolatile() const {
	return isVolatile_;
}

bool Type::isPOD() const {
	if( const SgClassDefinition *cdef = getClassDefinition() )
		// This seems to cover POD-unions, too.
		return isPODClass( cdef );
	else
		return isScalar();
}

bool Type::isPolymorphic() const {
	if( const SgClassDefinition *cdef = getClassDefinition() )
		return hasVirtualFunction( cdef );
	else
		return false;
}

bool Type::hasTrivialConstructor() const {
	if( const SgClassDefinition *cdef = getClassDefinition() )
		return hasTrivialCtor( cdef );
	else
		return false;
}

bool Type::hasTrivialCopy() const {
	if( const SgClassDefinition *cdef = getClassDefinition() )
		return hasTrivialCopyCtor( cdef );
	else
		return false;
}

bool Type::hasTrivialAssign() const {
	if( const SgClassDefinition *cdef = getClassDefinition() )
		return hasTrivialCopyAssignment( cdef );
	else
		return false;
}

bool Type::hasTrivialDestructor() const {
	if( const SgClassDefinition *cdef = getClassDefinition() )
		return hasTrivialDtor( cdef );
	else
		return false;
}

bool Type::isEnum() const {
	return isSgEnumType(t_) != 0;
}

const SgEnumDeclaration *Type::getEnumDeclaration() const {
	if( const SgEnumType *enumt = isSgEnumType( t_ ) )
		return isSgEnumDeclaration( enumt->get_declaration() );
	return 0;
}

bool Type::isUnion() const {
	return false; //XXXXXXXXXXXXX???
}

bool Type::isClass() const {
	return isSgClassType(t_) != 0;
}

const SgClassDeclaration *Type::getClassDeclaration() const {
	if( const SgClassType *ct = isSgClassType(t_) ) {
		if( const SgDeclarationStatement *dec = ct->get_declaration() ) {
			if( const SgClassDeclaration *cdec = isSgClassDeclaration( dec ) ) {
				//std::cout << "SgClassDeclaration: it's a declaration at line " << cdec->get_file_info()->get_line() << std::endl;
				return cdec;
			}
		}
	}
	return 0;
}

//XXX problem?  definition of class is retroactive to earlier forward declarations.
//XXX compare line numbers?
const SgClassDefinition *Type::getClassDefinition() const {
	//std::cout << "getClassDefinition: "; //XXX
	if( const SgClassDeclaration *cdec = getClassDeclaration() ) {
		//std::cout << "it's a class declaration at line " << cdec->get_file_info()->get_line(); //XXX
		if( const SgDeclarationStatement *defdec = cdec->get_definingDeclaration() ) {
			//std::cout << " and it has a defining declaration at line " << defdec->get_file_info()->get_line(); //XXX
			if( const SgClassDeclaration *defcdec = isSgClassDeclaration( defdec ) ) {
				//std::cout << " and that's a class definition at line " << defcdec->get_file_info()->get_line(); //XXX
				//std::cout << std::endl; //XXX
				return cdec->get_definition();
			}
		}
	}
	//std::cout << std::endl; //XXX
	return 0;
}


bool Type::isFunction() const {
	return isSgFunctionType(t_) != 0;
}

bool Type::isSigned() const { //XXX what about plain char?
	return isSignedChar() || isShort() || isInt() || isLong() || isLongLong();
}

bool Type::isUnsigned() const { //XXX what about plain char?
	return isUnsignedChar() || isUnsignedShort() || isUnsignedInt() || isUnsignedLong() || isUnsignedLongLong();
}

bool Type::isIntegral() const {
	return isAnyChar() || isShort() || isUnsignedShort() || isInt()
		|| isUnsignedInt() || isLong() || isUnsignedLong() || isLongLong() || isUnsignedLongLong();
}

bool Type::isFloatingPoint() const {
	return isFloat() || isDouble() || isLongDouble();
}

bool Type::isArithmetic() const {
	return isIntegral() || isFloatingPoint();
}

bool Type::isFundamental() const {
	return isArithmetic() || isVoid();
}

bool Type::isObject() const {
	return !isFunction() && !isReference() && !isVoid();
}

bool Type::isScalar() const {
	return isArithmetic() || isEnum() || isPointer() || isMemberPointer();
}

bool Type::isCompound() const {
	return !isFundamental();
}

Type Type::dereference() const { // will deref pointer, reference, or array, else same type
	SgType *t = const_cast<SgType *>(t_); // SgType::dereference should be const
	return Type(t->dereference()); //XXX dereference may infinite loop for pointer to member!
}

Type Type::stripInitialTypedefs() const {
	return Type(orig_->stripType( SgType::STRIP_TYPEDEF_TYPE ));
}

Type Type::stripTypedefsAndModifiers() const {
	return Type( orig_->stripTypedefsAndModifiers() );
}

Type Type::baseType() const {
	return Type( orig_->findBaseType() ); // same as striptype()
}

Type Type::stripInitialPointers() const {
	if( isPointer() )
		return dereference().stripInitialPointers();
	else
		return *this;
}

Type Type::stripInitialReferences() const {
	if( isReference() )
		return dereference().stripInitialReferences();
	else
		return *this;
}

Type Type::stripInitialArrays() const {
	if( isArray() )
		return dereference().stripInitialArrays();
	else
		return *this;
}

Type Type::stripInitialPointersAndReferences() const {
	if( isReference() || isPointer() )
		return dereference().stripInitialPointersAndReferences();
	else
		return *this;
}

Type Type::stripInitialPointersAndArrays() const {
	if( isReference() || isArray() )
		return dereference().stripInitialPointersAndArrays();
	else
		return *this;
}

Type Type::stripInitialPointersReferencesAndArrays() const {
	if( isPointer() || isReference() || isArray() )
		return dereference().stripInitialPointersReferencesAndArrays();
	else
		return *this;
}

Type Type::removeReference() const {
	return dereference();
}

Type Type::removePointer() const {
	return dereference();
}

size_t Type::rank() const {
	size_t n = 0;
	Type tmp( *this );
	while( tmp.isArray() ) {
		tmp = tmp.dereference();
		++n;
	}
	return n;
}

Type Type::removeExtent () const {
	if( isArray() )
		return dereference();
	else
		return *this;
}

Type Type::removeAllExtents () const {
	return stripInitialArrays();
}
