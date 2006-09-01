/***************************************************************************
 *   Copyright (C) 2006 by Dominik Seichter                                *
 *   domseichter@web.de                                                    *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU Library General Public License as       *
 *   published by the Free Software Foundation; either version 2 of the    *
 *   License, or (at your option) any later version.                       *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU Library General Public     *
 *   License along with this program; if not, write to the                 *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include "PdfContents.h"

#include "PdfName.h"

namespace PoDoFo {

PdfContents::PdfContents( PdfVecObjects* pParent )
: mContObj( pParent->CreateObject() )
{
	mContObj->Stream();	// this will force it to be a Stream instead of just a Dictionary
}

PdfContents::PdfContents( PdfObject* inObj )
: mContObj( inObj )
{
	if ( mContObj->GetDataType() == ePdfDataType_Reference ) {
		mContObj = inObj->GetParent()->GetObject( inObj->GetReference() );
	} else if ( mContObj->GetDataType() == ePdfDataType_Dictionary ) {
		mContObj->Stream();	// should make it a valid stream..
	}
}

PdfObject* PdfContents::ContentsForAppending() const
{
	if ( mContObj->GetDataType() == ePdfDataType_Stream || 
		 mContObj->GetDataType() == ePdfDataType_Dictionary ) {
		return mContObj;	// just return the stream itself
	} else if ( mContObj->GetDataType() == ePdfDataType_Array ) {
		/*
			Create a new stream, add it to the array, return it
		*/
		PdfObject*	newStm = mContObj->GetParent()->CreateObject();
		newStm->Stream();
		PdfReference	pdfr( newStm->ObjectNumber(), newStm->GenerationNumber() );

		PdfArray&	cArr = mContObj->GetArray();
		cArr.push_back( pdfr );
		return newStm;
	} else {
		RAISE_ERROR( ePdfError_InvalidDataType );
	}
}

};

