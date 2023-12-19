#pragma once
#include <orm/tiny/model.hpp>
#include <orm/tiny/relations/pivot.hpp>
#include "Models/LabelModel.h"
#include "Models/PersonModel.h"
#include "Models/SongModel.h"

namespace Models {

	using Orm::Tiny::Model;
	using Orm::Tiny::Relations::Pivot;

	class Label;
	class Person;
	class Song;

	class Artist final : public Model<Artist, Label, Person, Song, Pivot>
	{
		friend Model;
		using Model::Model;

	public:
		std::unique_ptr<BelongsTo<Artist, Label>>
			label()
		{
			return belongsTo<Label>();
		}

		std::unique_ptr<BelongsToMany<Artist, Person>>
			persons()
		{
			return belongsToMany<Person>("artist_person", "artist_id", "person_id");
		}

		std::unique_ptr<BelongsToMany<Artist, Song>>
			songs()
		{
			return belongsToMany<Song>("artist_song", "artist_id", "song_id");
		}

	private:
		QString u_table{ "artist" };
		QString u_primaryKey{ "artist_id" };
		bool u_timestamps = false;

	private:
		QHash<QString, RelationVisitor> u_relations
		{
			{"label", [](auto& v) { v(&Artist::label);   }},
			{"persons", [](auto& v) { v(&Artist::persons); }},
			{"songs", [](auto& v) { v(&Artist::songs);   }},
		};
	};
}