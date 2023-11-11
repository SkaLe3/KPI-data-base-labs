 -- Artist Table with numbered artists from 1 to count
WITH artist_number AS(
SELECT 
    ROW_NUMBER() OVER (ORDER BY artist.artist_id) AS number,
    artist.artist_id
FROM 
    artist
)
-- Artist Table with id and random number in range 1 - count artist
, pick_artist AS(
SELECT  number, floor(random() * (SELECT count(*) FROM artist) + 1) as rnd
	FROM generate_series(1, 100) as number
)

-- Artist Table with id and artist_id
, ready_artist AS(
SELECT pick_artist.number AS id, artist_number.artist_id
	FROM pick_artist
	LEFT JOIN artist_number ON pick_artist.rnd = artist_number.number
	ORDER BY id
)

 -- Person Table with numbered persons from 1 to count
, person_number AS(
SELECT 
    ROW_NUMBER() OVER (ORDER BY person.person_id) AS number,
    person.person_id
FROM 
    person
)
-- Person Table with id and random number in range 1 - count person
, pick_person AS(
SELECT  number, floor(random() * (SELECT count(*) FROM person) + 1) as rnd
	FROM generate_series(1, 100) as number
)

-- Person Table with id and person_id
, ready_person AS(
SELECT pick_person.number AS id, person_number.person_id
	FROM pick_person
	LEFT JOIN person_number ON pick_person.rnd = person_number.number
	ORDER BY id
)

, results AS(
SELECT ready_artist.artist_id, ready_person.person_id
	FROM ready_artist
	INNER JOIN ready_person ON ready_artist.id = ready_person.id
	ORDER BY artist_id, person_id
)
	
SELECT DISTINCT * FROM results
	