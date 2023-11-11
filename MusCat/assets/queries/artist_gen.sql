
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

, results AS(
SELECT 
	only_titles.title,
	NULL AS desciption,
	date_trunc('day', current_date - (random() * 365* 40 +10 || 'days')::interval)::date AS founded_date,
	NULL AS closed_date,
	ready_label.label_id
FROM only_titles
	INNER JOIN ready_label ON only_titles.title_id = ready_label.id
)

SELECT * FROM results




