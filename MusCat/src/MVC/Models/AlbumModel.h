#pragma once
#include <orm/tiny/model.hpp>
#include "Models/SongModel.h"

namespace Models {
	
	using Orm::Tiny::Model;

	class Song;

	class Album final : public Model<Album, Song>
	{
		friend Model;
		using Model::Model;

	public:
		std::unique_ptr<HasMany<Album, Song>>
			songs()
		{
			return hasMany<Song>();
		}
	private:
		QString u_table{ "album" };
		QString u_primaryKey{ "album_id" };
		bool u_timestamps = false;

	private:
		QHash<QString, RelationVisitor> u_relations
		{
			{"songs", [](auto& v) { v(&Album::songs); }},
		};
	};
}