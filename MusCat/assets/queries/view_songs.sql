WITH new_song AS(
SELECT song_id, song.title, genre.name as genre, song.release_date, album.title AS album, label.name AS label
FROM song
	INNER JOIN genre ON song.genre_id = genre.genre_id
	LEFT JOIN album ON song.album_id = album.album_id
	LEFT JOIN label ON song.label_id = label.label_id
)
, results AS(
SELECT 
	new_song.title,
	new_song.genre,
	STRING_AGG(artist.title, ', ') AS artists,
	new_song.release_date,
	new_song.album,
	new_song.label
FROM new_song
	INNER JOIN artist_song ON new_song.song_id = artist_song.song_id
	INNER JOIN artist ON artist_song.artist_id = artist.artist_id
GROUP BY new_song.song_id, new_song.title, new_song.genre, new_song.release_date, new_song.album, new_song.label
)


SELECT * FROM results