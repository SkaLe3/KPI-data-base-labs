
WITH rnd_numbers AS(
SELECT 
floor(random()* (SELECT count(*) FROM names)+1) as rnd_name,
floor(random()* (SELECT count(*) FROM surnames)+1) as rnd_surname
FROM generate_series(1, 100)
)

, results AS(
SELECT
	names.data || ' ' || surnames.data,
	date_trunc('day', current_date - (random() * 365* 40 +365*18 || 'days')::interval)::date AS date
FROM rnd_numbers
	INNER JOIN names ON rnd_name = names.names_id
	INNER JOIN surnames ON rnd_surname = surnames.surnames_id
)

SELECT * FROM results