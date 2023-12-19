#pragma once
#include <orm/tiny/model.hpp>
#include "Models/AlbumModel.h"
#include "Models/GenreModel.h"
#include "Models/LabelModel.h"
#include "Models/SongModel.h"

namespace Models {

	using Orm::Tiny::Model;

	class Song;

	class Genre final : public Model<Genre, Song>
	{
		friend Model;
		using Model::Model;

	public:
		std::unique_ptr<HasMany<Genre, Song>>
			songs()
		{
			return hasMany<Song>();
		}
	private:
		QString u_table{ "genre" };
		QString u_primaryKey{ "genre_id" };
		bool u_timestamps = false;

	private:
		QHash<QString, RelationVisitor> u_relations
		{
			{"songs", [](auto& v) { v(&Genre::songs); }},
		};
	};
}