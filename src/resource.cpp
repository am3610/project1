#include "resource.h"

using namespace std;

resource::resource(string title){
	this->title = title;
	this->borrowerID = -1;		// -1 is init value representing a status 
								// in which a resource is not borrowed.
}

book::book(string title) : resource(title){
	
}
