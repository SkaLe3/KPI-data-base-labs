#pragma once
#include <orm/tiny/model.hpp>
#include <orm/tiny/relations/pivot.hpp>
#include "Models/ArtistModel.h"

namespace Models {

	using Orm::Tiny::Model;
	using Orm::Tiny::Relations::Pivot;

	class Artist;

	class Person final : public Model<Person, Artist, Pivot>
	{
		friend Model;
		using Model::Model;

	public:
		std::unique_ptr<BelongsToMany<Person, Artist>>
			artists()
		{
			return belongsToMany<Artist>("artist_person");
		}

	private:
		QString u_table{ "person" };
		QString u_primaryKey{ "person_id" };
		bool u_timestamps = false;

	private:
		QHash<QString, RelationVisitor> u_relations
		{
			{"artists", [](auto& v) { v(&Person::artists); }},
		};
	};
}