
#include "config.h"

#ifdef HAVE_STD_IOSTREAM
#include <iostream>
#else
#include <iostream.h>
#endif
#include <memory>

#include "zipfile.h"

using namespace fcol ;

using std::cerr ;
using std::cout ;
using std::endl ;
using std::auto_ptr ;

int main() {
  try {
  
    cout << "Instantiating a ZipFile" << endl ;
    ZipFile zf( "test.zip" ) ;
    
    cout << "list length : " << zf.size() << endl ;
    
    vector< ConstEntryPointer > entries ;
    entries = zf.entries() ;
    
    
    vector< ConstEntryPointer >::iterator it ;
    for( it = entries.begin() ; it != entries.end() ; it++)
      cout << *(*it) << endl ;
    
    ConstEntryPointer ent = zf.getEntry( "file2.txt", FileCollection::IGNORE ) ;
    if ( ent != 0 ) {
      auto_ptr< istream > is( zf.getInputStream( ent ) ) ;
      const int buflen = 20 ;
      char buf[ buflen ] ;
      
      cout << "Contents of entry, " << ent->getName() << " :" << endl ;
      
      while ( *is && ! is->eof() ) {
	is->read( buf, buflen - 1 ) ;
	buf[ is->gcount() ] = '\0' ;
	cout << buf ;
      }
    }
    cout << "end of main()" << endl ;
    
    return 0 ;
  }
  catch( exception &excp ) {
    cerr << "Exception caught in main() :" << endl ;
    cerr << excp.what() << endl ;
  }
}

/** \file
    \anchor example_zip_anchor
    source code to a small program that demonstrates the central elements
    of Zipios++.
*/

/*
  Zipios++ - a small C++ library that provides easy access to .zip files.
  Copyright (C) 2000  Thomas Søndergaard
  
  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.
  
  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.
  
  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/
