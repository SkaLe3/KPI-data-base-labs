#pragma once
#include <orm/tiny/model.hpp>
#include <orm/tiny/relations/pivot.hpp>
#include "Models/AlbumModel.h"
#include "Models/GenreModel.h"
#include "Models/ArtistModel.h"

namespace Models {

	using Orm::Tiny::Model;
	using Orm::Tiny::Relations::Pivot;

	class Album;
	class Genre;
	class Artist;

	class Song final : public Model<Song, Album, Genre, Artist, Pivot>
	{
		friend Model;
		using Model::Model;

	public:
		std::unique_ptr<BelongsTo<Song, Album>>
			album()
		{
			return belongsTo<Album>();
		}

		std::unique_ptr<BelongsTo<Song, Genre>>
			genre()
		{
			return belongsTo<Genre>();
		}

		std::unique_ptr<BelongsToMany<Song, Artist>>
			artists()
		{
			return belongsToMany<Artist>("artist_song");
		}
	private:
		QString u_table{ "song" };
		QString u_primaryKey{ "song_id" };
		bool u_timestamps = false;

	private:
		QHash<QString, RelationVisitor> u_relations
		{
			{"album", [](auto& v) { v(&Song::album);     }},
			{"genre", [](auto& v) { v(&Song::genre);     }},
			{"artists", [](auto& v) { v(&Song::artists); }},
		};
	};
}