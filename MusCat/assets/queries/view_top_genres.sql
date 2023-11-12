WITH song_in_interval AS (
SELECT *
FROM song
	WHERE  release_date > '2000-07-29' AND release_date < '2006-08-04'
ORDER BY song_id
)
, genre_countsongs AS(
SELECT genre_id, count(song_in_interval.song_id) as countsongs
FROM song_in_interval
GROUP BY genre_id
	ORDER BY countsongs DESC
)
, genre_countartists AS(
SELECT genre_id, count(DISTINCT artist_song.artist_id) as countartists
FROM song_in_interval
	INNER JOIN artist_song ON song_in_interval.song_id = artist_song.song_id
GROUP BY genre_id
	ORDER BY countartists DESC
)

, artist_most_songs AS (
    SELECT
        song_in_interval.genre_id,
        artist_song.artist_id,
        RANK() OVER(PARTITION BY song_in_interval.genre_id ORDER BY COUNT(song_in_interval.song_id) DESC) AS artist_rank
    FROM
        song_in_interval
    INNER JOIN
        artist_song  ON song_in_interval.song_id = artist_song.song_id
    GROUP BY
        artist_song.artist_id, song_in_interval.genre_id
)
, results AS(

SELECT
    max(genre.name) AS genre,
    max(genre_countsongs.countsongs) AS song_count,
    max(genre_countartists.countartists) AS artist_count,
    min(artist.title) AS most_popular_artist
FROM
    genre_countsongs
JOIN genre_countartists ON genre_countsongs.genre_id = genre_countartists.genre_id
JOIN artist_most_songs ON genre_countsongs.genre_id = artist_most_songs.genre_id AND artist_most_songs.artist_rank = 1
JOIN artist ON artist_most_songs.artist_id = artist.artist_id
JOIN genre ON genre_countsongs.genre_id = genre.genre_id
	GROUP BY genre_countsongs.genre_id
	ORDER BY genre_countsongs.genre_id
)

SELECT * FROM results


