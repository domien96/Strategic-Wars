#ifndef TAGS_H
#define TAGS_H

#include <set>
#include "Component.h"

class Tags
{
public:

	/* Make a Tags object based on a given tag */
	Tags(Component::Tag tag) {
		v.insert(tag);
	}

	/* Add a new tag to the Tags object with a builder pattern style */
	Tags And(Component::Tag tag)
	{
		v.insert(tag);
		return *this;
	}

	/* Convert the Tags object to a set of Tags */
	std::set<Component::Tag> List()
	{
		return v;
	}

private:
	std::set<Component::Tag>v;

};

#endif