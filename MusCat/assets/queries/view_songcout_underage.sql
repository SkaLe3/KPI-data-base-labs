WITH person_under_age AS (
SELECT 
	person_id, 
	name, 
	(DATE_PART('year', age(current_date, birth_date))) AS age 
FROM person
	WHERE  (DATE_PART('year', age(current_date, birth_date))) < 30
ORDER BY age
)

, artists_of_persons AS(
SELECT artist_person.artist_id, artist_person.person_id
FROM artist_person
	INNER JOIN person_under_age ON person_under_age.person_id = artist_person.person_id
)
, artist_song_of_persons AS( 
SELECT DISTINCT artists_of_persons.artist_id, artist_song.song_id FROM artist_song 
	 INNER JOIN artists_of_persons ON artist_song.artist_id = artists_of_persons.artist_id
	INNER JOIN song ON artist_song.song_id = song.song_id
WHERE song.genre_id = 1
)
, artist_countsongs AS(
SELECT artist_id, count(song_id) AS countsongs
FROM artist_song_of_persons
GROUP BY artist_id
	ORDER BY artist_id
)
, artist_countalbums AS(
SELECT artist_id, count(album_id) AS countalbums
FROM artist_song_of_persons
	INNER JOIN song ON artist_song_of_persons.song_id = song.song_id
GROUP BY artist_id
	ORDER BY artist_id
)
, person_count AS(
SELECT 
	artists_of_persons.person_id, 
	sum(artist_countsongs.countsongs) as countsong,
	sum(artist_countalbums.countalbums) as countalbum
FROM artists_of_persons
	INNER JOIN artist_countsongs ON artists_of_persons.artist_id = artist_countsongs.artist_id
	INNER JOIN artist_countalbums ON artists_of_persons.artist_id = artist_countalbums.artist_id
GROUP BY artists_of_persons.person_id
)
, results AS(
SELECT person.name, age, countsong, countalbum, 'Rock' as genre
FROM person_count
	INNER JOIN person ON person_count.person_id = person.person_id
	INNER JOIN person_under_age ON person_count.person_id = person_under_age.person_id
WHERE countsong > 2
	ORDER BY countsong DESC
)

SELECT * FROM results