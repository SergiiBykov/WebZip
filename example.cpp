#include <iostream>
#include <fstream>
#include <stdio.h>
#include <chrono>

// Zip library header
#include "zip.h"

// Emscripten headers
#include <emscripten.h>
#include <emscripten/bind.h>
#include <emscripten/val.h>


using namespace std;
using namespace emscripten;

class MyClass
{
public:
	MyClass()
	{
		
	}
		
	val myFunction(std::string buffer, string entryFileName )
	{
		const auto start{std::chrono::steady_clock::now()};
		
      std::string zipFilePath( "/temp.zip" );
      //create a zip archive
      struct zip_t* zip = zip_open( zipFilePath.c_str(), ZIP_DEFAULT_COMPRESSION_LEVEL, 'w' );
      
      char* archiveBuffer = NULL;
      int archiveBufferLength = 0;

      zip_entry_open( zip, entryFileName.c_str() );
      {
         cout << "addEntry " << entryFileName << " Size: " << buffer.length() << " B" << endl;
         //write the buffer to the zip entry
         zip_entry_write( zip, buffer.c_str(), buffer.length() );
      }
      //close the zip entry
      zip_entry_close( zip );
      
      //close the zip archive
      zip_close( zip );
      
      //check if zip archive exists
      ifstream f( zipFilePath );
      if( f.good() )
      {
         std::ifstream t;
         //open the archive
         t.open( zipFilePath );
         //go to the end
         t.seekg( 0, std::ios::end );
         //report location (this is the length)
         archiveBufferLength = t.tellg();
         //go back to the beginning
         t.seekg( 0, std::ios::beg );
         //allocate memory for a buffer of appropriate dimension
         archiveBuffer = new char[ archiveBufferLength ];
         //read the whole file into the buffer
         t.read( archiveBuffer, archiveBufferLength );
         //close the archive
         t.close();
         cout << "Archive successfully read. Size: " << archiveBufferLength << " B" << endl;
      }
      else
      {
         return val();
      }

      //remove the zip file
      if( remove( zipFilePath.c_str() ) == 0 )
      {
         cout << "Archive successfully deleted" << endl;
      }
      else
      {
         return val();
      }

	const auto end{std::chrono::steady_clock::now()};
    const std::chrono::duration<double> elapsed_seconds{end - start};

int aa = elapsed_seconds.count();

	cout << "Time: " << aa<<  endl;

      //return fileContent;
      return val( typed_memory_view( archiveBufferLength, archiveBuffer ) );
		
	}
	
};

// Binding code
EMSCRIPTEN_BINDINGS(zip_bindings) {
  class_<MyClass>("MyClass")
    .constructor<>()
    .function("myFunction", &MyClass::myFunction)
    ;
}

int main(){}