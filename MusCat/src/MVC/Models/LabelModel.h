#pragma once
#include <orm/tiny/model.hpp>
#include "Models/ArtistModel.h"
#include "Models/SongModel.h"

namespace Models {

	using Orm::Tiny::Model;

	class Artist;
	class Song;

	class Label final : public Model<Label, Artist, Song>
	{
		friend Model;
		using Model::Model;

	public:
		std::unique_ptr<HasMany<Label, Artist>>
			artists()
		{
			return hasMany<Artist>();
		}
		std::unique_ptr<HasMany<Label, Song>>
			songs()
		{
			return hasMany<Song>();
		}

	private:
		QString u_table{ "label" };
		QString u_primaryKey{ "label_id" };
		bool u_timestamps = false;

	private:
		QHash<QString, RelationVisitor> u_relations
		{
			{"artists", [](auto& v) { v(&Label::artists); }},
			{"songs",   [](auto& v) { v(&Label::songs);   }},
		};
	};
}