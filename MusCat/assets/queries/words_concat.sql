CREATE OR REPLACE FUNCTION generate_random_sequence(max_number INT)
RETURNS TABLE (amount_of_words INT, word_id INT) AS $$
BEGIN
    FOR i IN 1..max_number LOOP
        FOR j IN 1..floor(random() * 5) + 1 LOOP
            amount_of_words := i;
			word_id:= floor(random() * (select count(*) from words)::int + 1);
            RETURN NEXT;
        END LOOP;
    END LOOP;
END;
$$ LANGUAGE plpgsql;

WITH tab AS(
SELECT * FROM generate_random_sequence(100)),

-- Join tables to replace word id with actual word
new_words AS(
SELECT tab.amount_of_words, words.data
	FROM tab
	INNER JOIN words ON tab.word_id = words.words_id
)
-- Concatenate words into titles
, only_titles AS(
SELECT amount_of_words AS title_id, string_agg(data, ' ' order by random()) AS title
	FROM new_words
	GROUP BY amount_of_words) 

-- Label Table with id and random number in range 1 - count label
, pick_label AS(
SELECT  number, floor(random() * (SELECT count(*) FROM label) + 1) as rnd
	FROM generate_series(1, 100) as number
)
 -- Label Table with numbered labels from 1 to count
, label_number AS(
SELECT 
    ROW_NUMBER() OVER (ORDER BY label.label_id) AS number,
    label.label_id
FROM 
    label
)
-- Label Table with id and label_id
, picked_label AS(
SELECT pick_label.number AS id, label_number.label_id
	FROM pick_label
	LEFT JOIN label_number ON pick_label.rnd = label_number.number
)

-- Label Table with nulls
, ready_label AS(
SELECT id,
  CASE
    WHEN (id + label_id) % 2 = 0 THEN NULL
    ELSE label_id
  END AS label_id
FROM picked_label
)

-- Genre Table with id and random number in range 1 - count genre
, pick_genre AS(
SELECT  number, floor(random() * (SELECT count(*) FROM genre) + 1) as rnd
	FROM generate_series(1, 100) as number
)
 -- Genre Table with numbered genres from 1 to count
, genre_number AS(
SELECT 
    ROW_NUMBER() OVER (ORDER BY genre.genre_id) AS number,
    genre.genre_id
FROM 
    genre
)
-- Genre Table with id and genre_id
, ready_genre AS(
SELECT pick_genre.number AS id, genre_number.genre_id
	FROM pick_genre
	LEFT JOIN genre_number ON pick_genre.rnd = genre_number.number
)

-- Album Table with id and random number in range 1 - count album
, pick_album AS(
SELECT  number, floor(random() * (SELECT count(*) FROM album) + 1) as rnd
	FROM generate_series(1, 100) as number
)
 -- Album Table with numbered albums from 1 to count
, album_number AS(
SELECT 
    ROW_NUMBER() OVER (ORDER BY album.album_id) AS number,
    album.album_id
FROM 
    album
)
-- Album Table with id and album_id
, picked_album AS(
SELECT pick_album.number AS id, album_number.album_id
	FROM pick_album
	LEFT JOIN album_number ON pick_album.rnd = album_number.number
)

-- Album Table with nulls
, ready_album AS(
SELECT id,
  CASE
    WHEN (id + album_id) % 5 = 0 THEN NULL
    ELSE album_id
  END AS album_id
FROM picked_album
)

, results AS
(
SELECT
only_titles.title, 
date_trunc('day', current_date - (random() * 365* 40 +10 || 'days')::interval)::date AS date,
ready_label.label_id,
ready_genre.genre_id,
ready_album.album_id

FROM only_titles
	INNER JOIN ready_label ON only_titles.title_id = ready_label.id
	INNER JOIN ready_genre ON only_titles.title_id = ready_genre.id
	INNER JOIN ready_album ON only_titles.title_id = ready_album.id)
	
SELECT * from results
 

 
 
 
 
 
 
 
 
 
 
 
 
 